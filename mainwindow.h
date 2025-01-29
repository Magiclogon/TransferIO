#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QStandardItemModel>
#include <QScrollArea>
#include <QMap>
#include <QListWidgetItem>
#include <QTableView>
#include "fileserver.h"
#include "addserverdialog.h"

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
    void addFile();
    void removeFile();
    void loadFiles(const QModelIndex &current, const QModelIndex &previous);

private:
    Ui::MainWindow *ui;

    struct ServerInfo {
        FileServer *fileServer;
        QString serverName;
        quint16 serverPort;
        QStringList *serverFiles;
    };

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
};

#endif // MAINWINDOW_H
