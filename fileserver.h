#ifndef FILESERVER_H
#define FILESERVER_H

#include <QTcpServer>
#include <QStringList>

class FileServer : public QTcpServer
{
    Q_OBJECT
    public:
        explicit FileServer(QObject *parent = nullptr);
        void startServer(quint16 port, const QStringList &files);
        QStringList sharedFiles;

    protected:
        void incomingConnection(qintptr socketDescriptor) override;

    private:
        void handleRequest(QTcpSocket *socket);
};

#endif // FILESERVER_H
