#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QStandardItemModel>
#include <QScrollArea>
#include <QGroupBox>
#include <QMap>
#include <QListWidgetItem>
#include <QTableView>
#include "fileserver.h"
#include "addserverdialog.h"
#include "file.h"
#include "aboutdialog.h"
#include "authorizeipsdialog.h"
#include <QNetworkInterface>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE



class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void serverStatusLabel(QLabel *serverStatus);

private slots:
    void addServer();
    void removeServer(quint16 port);
    void addToWhiteList(FileServer *server);
    void removeFileAt(quint16 port, int fileIndex);
    void addFile();
    void refreshFiles();
    void loadFiles(const QModelIndex &current, const QModelIndex &previous);
    void updateServerStatus(bool isRunning, QLabel *statusLabel, QToolButton *runBtn, QToolButton *stopButton);

    // Menu Bar Actions
    void showAboutDialog();
    void startAllServers();
    void stopAllServers();
    void toggleServersList();
    void toggleFilesList();

private:
    Ui::MainWindow *ui;

    struct ServerInfo {
        FileServer *fileServer;
        QString serverName;
        quint16 serverPort;
        QStringList authorizedIps;
        QList<ServerFile> *serverFiles;
    };

    // Groups
    QGroupBox *serverGroup;
    QGroupBox *fileGroup;

    // Variables for servers table view.
    QTableView *serverTable;
    QStandardItemModel *serverModel;
    QScrollArea *fileScrollArea;
    QVBoxLayout *fileContainerLayout;
    QWidget *fileContainer;

    // Variables for files table view.
    QTableView *fileTable;
    QStandardItemModel *fileModel;

    QMap<quint16, ServerInfo> servers;
    QListWidget *serverList;
    QListWidget *fileList;

    // Get the ip address.
    QMap<QString, QString> getIpAddress();

};

#endif // MAINWINDOW_H
