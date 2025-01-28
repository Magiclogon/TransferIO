#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QMap>
#include <QListWidgetItem>
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

private slots:
    void addServer();
    void removeServer(quint16 port);
    void addFile();
    void removeFile();
    void loadFiles(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::MainWindow *ui;

    struct ServerInfo {
        FileServer *fileServer;
        QString serverName;
        quint16 serverPort;
        QStringList *serverFiles;
    };

    QMap<quint16, ServerInfo> servers;
    QListWidget *serverList;
    QListWidget *fileList;
};

#endif // MAINWINDOW_H
