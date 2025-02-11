#include "fileserver.h"

#include <QTcpSocket>
#include <QStringList>
#include <QFileInfo>
#include <QDebug>
#include <QThreadPool>
#include <QRunnable>

/*
class FileTransferTask : public QRunnable {
public:
    FileTransferTask(QTcpSocket *socket, const QString &filePath, const QString &fileName, const qint64 fileSize)
        : socket(socket), filePath(filePath), fileName(fileName), fileSize(fileSize) {
        // Ensure the socket is not deleted while the task is running
        socket->setParent(nullptr);
    }

    void run() override {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QString header = "HTTP/1.1 200 OK\r\n";
            header += "Content-Disposition: attachment; filename=\"" + fileName + "\"\r\n\r\n";
            socket->write(header.toUtf8());

            QByteArray buffer;
            while (!file.atEnd() && socket->state() == QAbstractSocket::ConnectedState) {

                buffer = file.read(CHUNK_SIZE);
                socket->write(buffer);
                socket->waitForBytesWritten();

            }
            file.close();
        } else {
            QString header = "HTTP/1.1 404 Not Found\r\n\r\nFile not found.";
            socket->write(header.toUtf8());
        }

        // Clean up the socket
        socket->disconnectFromHost();
        socket->deleteLater();
    }

private:
    QTcpSocket *socket;
    QString filePath;
    QString fileName;
    qint64 fileSize;
};

*/

FileServer::FileServer(QObject *parent) : QTcpServer(parent) {
    QThreadPool::globalInstance()->setMaxThreadCount(10); // Adjust thread pool size
    isRunning = false;
}

void FileServer::startServer(quint16 port, QList<ServerFile> *files) {
    if (!this->listen(QHostAddress::Any, port)) {
        qDebug() << "Couldn't start server.";
    } else {
        qDebug() << "Server started on port:" << port;
        isRunning = true;
        emit runningStatusChanged(isRunning);
        sharedFiles = files;
    }
}

void FileServer::closeServer() {
    this->close();
    isRunning = false;
    emit runningStatusChanged(isRunning);
}

void FileServer::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    QString clientIp = socket->peerAddress().toString();
    qDebug() << clientIp;

    if (clientIp.startsWith("::ffff:")) {
        clientIp = clientIp.mid(7);
    }

    if(!authorizedIps.contains(clientIp) && !authorizedIps.isEmpty()) {
        handleNotAuthorizedRequest(socket);
        return;
    }

    connect(socket, &QTcpSocket::readyRead, this, [this, socket]() {
        handleRequest(socket);
    });

    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

void FileServer::handleRequest(QTcpSocket *socket) {
    QString request = socket->readAll();

    // If user requests a file
    if (request.startsWith("GET /file/")) {
        QString fileName = request.split(' ')[1].mid(6);

        // Converting for files names containing spaces like "photo 1.png" that will be "photo%201.png" in the GET request.
        fileName = QUrl::fromPercentEncoding(fileName.toUtf8());

        QString filePath = findFilePath(fileName, *sharedFiles);

        QFileInfo fileInfo(filePath);
        qint64 fileSize = fileInfo.size();

        // Increasing the download count.
        increaseDownloadCount(filePath, sharedFiles);

        // Openning the file and sending it.
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QString header = "HTTP/1.1 200 OK\r\n";
            header += "Content-Disposition: attachment; filename=\"" + fileName + "\"\r\n\r\n";
            socket->write(header.toUtf8());

            QByteArray buffer;
            isTransferring = true;
            while (!file.atEnd() && socket->state() == QAbstractSocket::ConnectedState) {

                buffer = file.read(CHUNK_SIZE);
                socket->write(buffer);
                socket->waitForBytesWritten();

            }
            isTransferring = false;
            file.close();
        }
        // Can't find the file.
        else {
            QString response = "HTTP/1.1 404 Not Found\r\n\r\n";

            QFile notFoundPage(":/assets/pages/not_found.html");

            // Openning the html page.
            if(notFoundPage.open(QIODevice::ReadOnly)) {
                QTextStream in(&notFoundPage);
                QString pageContent = in.readAll();
                response += pageContent;
            }
            else {
                response += "File not found. Contact your administrator.";
            }

            socket->write(response.toUtf8());
        }

        // Clean up the socket
        socket->disconnectFromHost();
        socket->deleteLater();
    }

    // If user requests home page.
    else if (request.startsWith("GET")) {

        QList<ServerFile> listFiles;
        if(request.startsWith("GET /search?query=")) {
            QString filter = request.split(' ')[1].mid(14);
            filter = QUrl::fromPercentEncoding(filter.toUtf8());
            qDebug() << filter;

            listFiles = filterSharedFiles(filter, *sharedFiles);
        }
        else {
            listFiles = *sharedFiles;
        }

        QString response = constructHomePage(listFiles);

        socket->write(response.toUtf8());
        socket->disconnectFromHost();
    }
}

QString FileServer::findFilePath(QString fileName, QList<ServerFile> sharedFiles) {
    for(ServerFile &file : sharedFiles) {
        if(file.filePath.contains(fileName)) {
            return file.filePath;
        }
    }
    return QString("");
}

void FileServer::increaseDownloadCount(QString filePath, QList<ServerFile> *sharedFiles) {
    for(ServerFile &file : *sharedFiles) {
        if(file.filePath == filePath) {
            file.numberDownloads ++;
            break;
        }
    }
}

void FileServer::handleNotAuthorizedRequest(QTcpSocket *socket) {

    QString response = "HTTP/1.1 403 FORBIDDEN\r\nContent-Type: text/html\r\n\r\n";

    // Openning the forbidden.html page
    QFile file(":/assets/pages/forbidden.html");
    if(file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        QString pageContent = in.readAll();
        response += pageContent;
    } else {
        response += "Not authorized, contact the administrator.";
    }

    socket->write(response.toUtf8());
    socket->waitForBytesWritten();
    socket->disconnectFromHost();
}

QList<ServerFile> FileServer::filterSharedFiles(QString filter, QList<ServerFile> sharedFiles) {
    QList<ServerFile> filteredSharedFiles;
    for(ServerFile file : sharedFiles) {
        QFileInfo fileInfo(file.filePath);
        QString fileName = fileInfo.fileName();

        if(fileName.contains(filter, Qt::CaseInsensitive)) {
            filteredSharedFiles.append(file);
        }
    }

    return filteredSharedFiles;
}

QString FileServer::constructHomePage(QList<ServerFile> sharedFiles) {
    QString response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    QFile file(":/assets/pages/home_page.html");

    if(file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        QString pageContent = in.readAll();
        response += pageContent;
    }
    else {
        response += "<html>"
                    "<body>"
                    "<h1>List of Available Files</h1>"
                    "<table>"
                    "<tr><th>File Name</th><th>Size</th><th>Download</th></tr>";
    }

    for (ServerFile file : sharedFiles) {
        QFileInfo fileInfo(file.filePath);
        QString fileName = fileInfo.fileName();
        qint64 fileSize = file.fileSize / 1024;
        response += QString("<tr><td>%1</td><td>%2</td><td><a href='/file/%3'>Download</a></td></tr>")
                        .arg(fileName)
                        .arg(QString::number(fileSize) + " kB")
                        .arg(fileName);
    }

    response += "</table></body></html>";

    return response;
}

bool FileServer::getIsRunning() {
    return isRunning;
}

bool FileServer::getIsTransferring() {
    return isTransferring;
}

QStringList FileServer::getAuthorizedIps() {
    return authorizedIps;
}

void FileServer::setIsRunning(bool running) {
    isRunning = running;
}

void FileServer::setAuthorizedIps(QStringList authorized) {
    authorizedIps = authorized;
}
