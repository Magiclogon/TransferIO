#ifndef FILESERVER_H
#define FILESERVER_H

#include <QTcpServer>
#include <QStringList>
#include "file.h"
#include <QUrl>

#define CHUNK_SIZE 8192

class FileServer : public QTcpServer
{
    Q_OBJECT
    public:
        explicit FileServer(QObject *parent = nullptr);
        void startServer(quint16 port, QList<ServerFile> *files);
        QList<ServerFile> *sharedFiles;
        void closeServer();
        bool getIsRunning();
        bool getIsTransferring();
        QStringList getAuthorizedIps();
        void setIsRunning(bool running);
        void setAuthorizedIps(QStringList authorized);

    signals:
        void runningStatusChanged(bool running);

    protected:
        void incomingConnection(qintptr socketDescriptor) override;

    private:
        bool isRunning;
        bool isTransferring;
        QStringList authorizedIps;

        void handleRequest(QTcpSocket *socket);
        void handleNotAuthorizedRequest(QTcpSocket *socket);
        QString findFilePath(QString fileName, QList<ServerFile> sharedFiles);
        void increaseDownloadCount(QString filePath, QList<ServerFile> *sharedFiles);
};

#endif // FILESERVER_H
