#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QPushButton>
#include <QLabel>
#include <QFileInfo>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Main layout
    QSplitter *splitter = new QSplitter(this);
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Left section: Server List
    QGroupBox *serverGroup = new QGroupBox("Servers", this);
    QVBoxLayout *serverLayout = new QVBoxLayout(serverGroup);

    serverList = new QListWidget(this);
    QPushButton *addServerButton = new QPushButton("Add Server", this);
    QPushButton *removeServerButton = new QPushButton("Remove Server", this);

    serverLayout->addWidget(serverList);
    serverLayout->addWidget(addServerButton);
    serverLayout->addWidget(removeServerButton);

    // Right section: File List
    QGroupBox *fileGroup = new QGroupBox("Files", this);
    QVBoxLayout *fileLayout = new QVBoxLayout(fileGroup);

    fileList = new QListWidget(this);
    QPushButton *addFileButton = new QPushButton("Add File", this);
    QPushButton *removeFileButton = new QPushButton("Remove File", this);

    fileLayout->addWidget(fileList);
    fileLayout->addWidget(addFileButton);
    fileLayout->addWidget(removeFileButton);

    // Add sections to splitter
    splitter->addWidget(serverGroup);
    splitter->addWidget(fileGroup);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);

    mainLayout->addWidget(splitter);
    setCentralWidget(centralWidget);

    // Connections
    connect(addServerButton, &QPushButton::clicked, this, &MainWindow::addServer);
    connect(removeServerButton, &QPushButton::clicked, this, &MainWindow::removeServer);
    connect(serverList, &QListWidget::currentItemChanged, this, &MainWindow::loadFiles);
    connect(addFileButton, &QPushButton::clicked, this, &MainWindow::addFile);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::addServer() {
    AddServerDialog dialog(this);

    if(dialog.exec() == QDialog::Accepted) {
        QString serverName = dialog.serverName();
        quint16 serverPort = dialog.serverPort();

        if(servers.contains(serverPort)) {
            QMessageBox::warning(this, "Port in use", "The selected port is already in use.");
            return;
        }

        // Creating the structure and adding it to the map.
        FileServer *server = new FileServer(this);
        QStringList *serverFiles = new QStringList({});

        ServerInfo info;
        info.fileServer = server;
        info.serverName = serverName;
        info.serverPort = serverPort;
        info.serverFiles = serverFiles;

        servers.insert(serverPort, info);

        // Creating the list item.
        QListWidgetItem *item = new QListWidgetItem(serverList);
        item->setText(QString("%1 - %2").arg(serverName).arg(serverPort));
        QWidget *itemWidget = new QWidget(this);
        QHBoxLayout *itemLayout = new QHBoxLayout(itemWidget);

        QSpacerItem *spacerH3 = new QSpacerItem(10, 2, QSizePolicy::Expanding, QSizePolicy::Maximum);

        QPushButton *runBtn = new QPushButton("Run", this);
        QPushButton *stopBtn = new QPushButton("Stop", this);
        stopBtn->setEnabled(false);
        QPushButton *removeBtn = new QPushButton("Rem", this);

        itemLayout->addItem(spacerH3);
        itemLayout->addWidget(runBtn);
        itemLayout->addWidget(stopBtn);
        itemLayout->addWidget(removeBtn);
        itemLayout->setContentsMargins(2, 2, 2, 2);

        itemWidget->setLayout(itemLayout);
        serverList->setItemWidget(item, itemWidget);

        connect(runBtn, &QPushButton::clicked, this, [this, info, runBtn, stopBtn]() {
            info.fileServer->startServer(info.serverPort, *(info.serverFiles));
            qDebug() << "STARTED " << info.serverName << " ON PORT " << info.serverPort;
            runBtn->setEnabled(false);
            stopBtn->setEnabled(true);
        });

        connect(stopBtn, &QPushButton::clicked, this, [this, info, runBtn, stopBtn]() {
            info.fileServer->close();
            qDebug() << "STOPPED " << info.serverName << " ON PORT " << info.serverPort;
            runBtn->setEnabled(true);
            stopBtn->setEnabled(false);
        });

        connect(removeBtn, &QPushButton::clicked, this, [this, info, item]() {
            removeServer(info.serverPort);
            serverList->takeItem(serverList->row(item));
            delete item;
        });
    }
}

void MainWindow::removeServer(quint16 port) {
    if(servers.contains(port)) {
        FileServer *server = servers[port].fileServer;
        server->close();
        servers.remove(port);
        qDebug() << "Removed server on port: " << port;
    }

}

void MainWindow::addFile() {
    QListWidgetItem *currentItem = serverList->currentItem();

    if(!currentItem) {
        QMessageBox::warning(this, "No server selected", "Please select a server first.");
        return;
    }

    QString serverLabel = currentItem->text();
    QStringList nameParts = serverLabel.split(" - ");

    if(nameParts.size() == 2) {
        quint16 port = nameParts[1].toUInt();

        if(servers.contains(port)) {
            QStringList filesToAdd = QFileDialog::getOpenFileNames(this, "Select files to add", QString(), "All Files (*)");

            if(!filesToAdd.isEmpty()) {
                ServerInfo &info = servers[port];

                for(QString &fileToAdd : filesToAdd) {
                    if(!info.serverFiles->contains(fileToAdd)) {
                        info.serverFiles->append(fileToAdd);
                    }
                }

                info.fileServer->close();
                info.fileServer->startServer(info.serverPort, *(info.serverFiles));
                loadFiles(currentItem, nullptr);
            }
        }
    }
}

void MainWindow::removeFile() {

}


void MainWindow::loadFiles(QListWidgetItem *current, QListWidgetItem *previous) {
    Q_UNUSED(previous);
    fileList->clear();

    if(current) {
        QString serverLabel = current->text();
        QStringList nameParts = serverLabel.split(" - ");

        if(nameParts.size() == 2) {
            quint16 port = nameParts[1].toUInt();

            if(servers.contains(port)) {
                ServerInfo info = servers[port];
                for(QString &filePath : *(info.serverFiles)) {
                    QFileInfo fileInfo(filePath);
                    fileList->addItem(fileInfo.fileName());
                }
            }
        }
    }
}
