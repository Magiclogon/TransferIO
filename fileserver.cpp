#include "fileserver.h"

#include <QTcpSocket>
#include <QStringList>
#include <QFileInfo>
#include <QDebug>

FileServer::FileServer(QObject *parent) : QTcpServer(parent) {}

void FileServer::startServer(quint16 port, const QStringList &files) {
    if(!this->listen(QHostAddress::Any, port)) {
        qDebug() << "Couldn't start server";
    }
    else {
        qDebug() << "Server started on port: " << port;
        isRunning = true;
        sharedFiles = files;
    }
}

void FileServer::closeServer() {
    this->close();
    isRunning = false;
}

void FileServer::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, &QTcpSocket::readyRead, this, [this, socket]() {
        handleRequest(socket);
    });
}

void FileServer::handleRequest(QTcpSocket *socket) {
    QString request = socket->readAll();
    qDebug() << request;

    if(request.startsWith("GET /file/")) {
        QString fileName = request.split(' ')[1].mid(6);
        QString filePath = sharedFiles.filter(fileName).first();
        qDebug() << "Filepath: " << fileName;

        QFile file(filePath);
        if(file.open(QIODevice::ReadOnly)) {
            QByteArray fileData = file.readAll();

            QString header = "HTTP/1.1 200 OK\r\nContent-Disposition: attachment; filename=\"" + fileName + "\"\r\n\r\n";
            socket->write(header.toUtf8());
            socket->write(fileData);
        }

        else {
            QString header = "HTTP/1.1 404 Not Found\r\n\r\nFile not found.";
            socket->write(header.toUtf8());
        }

    }

    else if(request.startsWith("GET")) {
        QString response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        response += "<html><body><h1>List of available files</h1><ul>";

        for(QString &filePath : sharedFiles) {
            QFileInfo fileInfo(filePath);
            QString fileName = fileInfo.fileName();
            response += QString("<li><a href='/file/%1'>%2</a></li>").arg(fileName).arg(fileName);
        }

        response += "</ul></body></html>";
        socket->write(response.toUtf8());
    }



    socket->disconnectFromHost();
}

bool FileServer::getIsRunning() {
    return isRunning;
}
















