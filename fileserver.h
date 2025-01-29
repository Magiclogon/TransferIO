#ifndef FILESERVER_H
#define FILESERVER_H

#include <QTcpServer>
#include <QStringList>
#include "file.h"

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

    protected:
        void incomingConnection(qintptr socketDescriptor) override;

    private:
        void handleRequest(QTcpSocket *socket);
        bool isRunning;
        QString findFilePath(QString fileName, QList<ServerFile> sharedFiles);
        void increaseDownloadCount(QString filePath, QList<ServerFile> *sharedFiles);
};

#endif // FILESERVER_H
