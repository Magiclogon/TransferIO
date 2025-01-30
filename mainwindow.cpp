#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QPushButton>
#include <QLabel>
#include <QFileInfo>
#include <QFileDialog>
#include <QDebug>
#include <QToolButton>
#include <QMessageBox>
#include <QSpacerItem>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QScrollArea>
#include <QWidget>
#include <QSignalMapper>

bool existsInList(const QString &filePath, const QList<ServerFile> &serverFiles) {
    for (const ServerFile &file : serverFiles) {
        if (file.filePath == filePath) {
            return true;
        }
    }
    return false;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Apply a global stylesheet for better appearance
    this->setStyleSheet(R"(
        QMainWindow {
            background-color: #121212;
        }
        QDialog {
            background-color: #121212;
        }
        QGroupBox {
            border: 1px solid #444;
            border-radius: 5px;
            margin-top: 10px;
            padding-top: 15px;
            font-weight: bold;
            color: #ffffff;
            background-color: #1e1e1e;
        }
        QPushButton {
            background-color: #414141;
            color: #ffffff;
        }

        QPushButton::Hover {
            background-color: #2f2f2f;
            color: #ffffff;
        }
        QPushButton:pressed, QToolButton:pressed {
            background-color: #004899;
        }
        QTableView {
            background-color: #1E1E1E;
            border: 1px solid #333;
            border-radius: 3px;
            gridline-color: #333;
            color: #ffffff;
            selection-background-color: #565758;
            selection-color: #ffffff;
        }
        QHeaderView::section {
            background-color: #333;
            color: white;
            padding: 5px;
            border: none;
        }
        QLabel {
            color: #ffffff;
        }
        QSplitter {
            background-color: #2e2e2e;
            border: 1px solid #444;
        }
        QSplitter::handle {
            background-color: #555;
            width: 5px;
        }
        QSplitter::handle:hover {
            background-color: #777;
        }
    )");

    // Adding the menu bar
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    // Add the file menu.
    QMenu *fileMenu = menuBar->addMenu("&File");
    QAction *addServerAction = new QAction("Add Server", this);
    fileMenu->addAction(addServerAction);
    connect(addServerAction, &QAction::triggered, this, &MainWindow::addServer);

    // Add the servers menu.
    QMenu *serversMenu = menuBar->addMenu("&Servers");
    QAction *startAllServersAction = new QAction("Start All Servers", this);
    serversMenu->addAction(startAllServersAction);
    connect(startAllServersAction, &QAction::triggered, this, &MainWindow::startAllServers);

    QAction *stopAllServersAction = new QAction("Stop All Servers", this);
    serversMenu->addAction(stopAllServersAction);
    connect(stopAllServersAction, &QAction::triggered, this, &MainWindow::stopAllServers);

    // Add the view menu
    QMenu *viewMenu = menuBar->addMenu("&View");
    QAction *toggleServersListAction = new QAction("Toggle Servers List");
    toggleServersListAction->setCheckable(true);
    toggleServersListAction->setChecked(true);
    viewMenu->addAction(toggleServersListAction);
    connect(toggleServersListAction, &QAction::triggered, this, [this, toggleServersListAction]() {
        toggleServersListAction->setChecked(toggleServersListAction->isChecked() ? true : false);
        toggleServersList();
    });

    QAction *toggleFilesListAction = new QAction("Toggle Files List");
    toggleFilesListAction->setCheckable(true);
    toggleFilesListAction->setChecked(true);
    viewMenu->addAction(toggleFilesListAction);
    connect(toggleFilesListAction, &QAction::triggered, this, [this, toggleFilesListAction]() {
        toggleFilesListAction->setChecked(toggleFilesListAction->isChecked() ? true : false);
        toggleFilesList();
    });

    // Add the help menu
    QMenu *helpMenu = menuBar->addMenu("&Help");
    QAction *aboutAction = new QAction("About", this);
    helpMenu->addAction(aboutAction);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);

    // Content.

    QSpacerItem *spacerH = new QSpacerItem(10, 2, QSizePolicy::Expanding, QSizePolicy::Maximum);

    // Main layout
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Left section: Server List
    serverGroup = new QGroupBox("Servers", this);
    QVBoxLayout *serverLayout = new QVBoxLayout(serverGroup);

    // Use QTableView for the server list
    serverTable = new QTableView(this);
    serverModel = new QStandardItemModel(this);
    serverModel->setHorizontalHeaderLabels({"Server Name", "Port", "Status", "Actions"});
    serverTable->setModel(serverModel);
    serverTable->horizontalHeader()->setStretchLastSection(true);
    serverTable->verticalHeader()->hide();
    serverTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    serverTable->setSelectionMode(QAbstractItemView::SingleSelection);

    QHBoxLayout *serverBtnLayout = new QHBoxLayout(serverGroup);
    QPushButton *addServerButton = new QPushButton("Add Server", this);

    serverBtnLayout->addItem(spacerH);
    serverBtnLayout->addWidget(addServerButton);

    serverLayout->addWidget(serverTable);
    serverLayout->addLayout(serverBtnLayout);

    // Right section: ServerFile List
    // Inside your MainWindow constructor, update the file section to include a QTableView:
    fileGroup = new QGroupBox("Files", this);
    QVBoxLayout *fileLayout = new QVBoxLayout(fileGroup);

    // Create a QTableView for the file list
    fileTable = new QTableView(this);
    fileModel = new QStandardItemModel(this);
    fileModel->setHorizontalHeaderLabels({"File Name", "Size", "Number of downloads", "Action"});
    fileTable->setModel(fileModel);
    fileTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    fileTable->verticalHeader()->hide();
    fileTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    fileTable->setSelectionMode(QAbstractItemView::SingleSelection);

    // Button to add files
    QHBoxLayout *fileBtnLayout = new QHBoxLayout(fileGroup);
    QPushButton *addFileButton = new QPushButton("Add File", this);
    QPushButton *refreshFilesButton = new QPushButton("Refresh", this);

    fileBtnLayout->addItem(spacerH);
    fileBtnLayout->addWidget(refreshFilesButton);
    fileBtnLayout->addWidget(addFileButton);

    // Add the QTableView and buttons to the layout
    fileLayout->addWidget(fileTable);
    fileLayout->addLayout(fileBtnLayout);

    // Add the file section to the splitter
    splitter->addWidget(serverGroup);
    splitter->addWidget(fileGroup);
    splitter->setStretchFactor(0, 26);
    splitter->setStretchFactor(1, 45);

    // Add the main layout to the central widget
    mainLayout->addWidget(splitter);
    setCentralWidget(centralWidget);


    // Connections
    connect(addServerButton, &QPushButton::clicked, this, &MainWindow::addServer);
    connect(serverTable->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::loadFiles);
    connect(addFileButton, &QPushButton::clicked, this, &MainWindow::addFile);
    connect(refreshFilesButton, &QPushButton::clicked, this, &MainWindow::refreshFiles);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::addServer() {

    AddServerDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        QString serverName = dialog.serverName();
        quint16 serverPort = dialog.serverPort();

        if (servers.contains(serverPort)) {
            QMessageBox::warning(this, "Port in use", "The selected port is already in use.");
            return;
        }

        // Creating the structure and adding it to the map.
        FileServer *server = new FileServer(this);
        QList<ServerFile> *serverFiles = new QList<ServerFile>();

        ServerInfo info;
        info.fileServer = server;
        info.serverName = serverName;
        info.serverPort = serverPort;
        info.serverFiles = serverFiles;

        servers.insert(serverPort, info);

        // Add server to the table
        QList<QStandardItem*> rowItems;
        rowItems << new QStandardItem(serverName) << new QStandardItem(QString::number(serverPort));

        // Add action buttons and status
        QLabel *serverStatusLabel = new QLabel("Stopped..", this);

        QSpacerItem *spacerH2 = new QSpacerItem(20, 2, QSizePolicy::Expanding, QSizePolicy::Maximum);

        QWidget *actionWidget = new QWidget(this);
        QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);
        QToolButton *runBtn = new QToolButton(this);
        runBtn->setIcon(QIcon(":/assets/play-button.png"));
        runBtn->setStyleSheet("background: transparent,; border: none;");
        QToolButton *stopBtn = new QToolButton(this);
        stopBtn->setIcon(QIcon(":/assets/stop.png"));
        stopBtn->setEnabled(false);
        stopBtn->setStyleSheet("background: transparent,; border: none;");
        QToolButton *removeBtn = new QToolButton(this);
        removeBtn->setIcon(QIcon(":/assets/delete.png"));
        removeBtn->setStyleSheet("background: transparent,; border: none;");

        actionLayout->addItem(spacerH2);
        actionLayout->addWidget(runBtn);
        actionLayout->addWidget(stopBtn);
        actionLayout->addWidget(removeBtn);
        actionLayout->setContentsMargins(0, 0, 0, 0);
        actionWidget->setLayout(actionLayout);

        serverModel->appendRow(rowItems);
        serverTable->setIndexWidget(serverModel->index(serverModel->rowCount() - 1, 2), serverStatusLabel);
        serverTable->setIndexWidget(serverModel->index(serverModel->rowCount() - 1, 3), actionWidget);

        connect(server, &FileServer::runningStatusChanged, this, [this, serverStatusLabel, runBtn, stopBtn](bool isRunning) {
            updateServerStatus(isRunning, serverStatusLabel, runBtn, stopBtn);
        });

        connect(runBtn, &QToolButton::clicked, this, [this, info, runBtn, stopBtn, serverStatusLabel]() {
            info.fileServer->startServer(info.serverPort, info.serverFiles);
            qDebug() << "STARTED " << info.serverName << " ON PORT " << info.serverPort;
        });

        connect(stopBtn, &QToolButton::clicked, this, [this, info, runBtn, stopBtn, serverStatusLabel]() {
            info.fileServer->closeServer();
            qDebug() << "STOPPED " << info.serverName << " ON PORT " << info.serverPort;
        });

        connect(removeBtn, &QToolButton::clicked, this, [this, info, rowItems]() {
            removeServer(info.serverPort);
            serverModel->removeRow(serverModel->indexFromItem(rowItems[0]).row());
        });
    }
}

void MainWindow::removeServer(quint16 port) {
    if (servers.contains(port)) {
        FileServer *server = servers[port].fileServer;
        server->closeServer();
        servers.remove(port);
        qDebug() << "Removed server on port: " << port;
    }
}

void MainWindow::addFile() {
    QModelIndex currentIndex = serverTable->currentIndex();

    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "No server selected", "Please select a server first.");
        return;
    }

    quint16 port = serverModel->item(currentIndex.row(), 1)->text().toUInt();

    if (servers.contains(port)) {
        QStringList filesToAdd = QFileDialog::getOpenFileNames(this, "Select files to add", QString(), "All Files (*)");

        if (!filesToAdd.isEmpty()) {
            ServerInfo &info = servers[port];

            // Update the server files list
            for (const QString &fileToAdd : filesToAdd) {
                if (!existsInList(fileToAdd, *(info.serverFiles))) {
                    QFileInfo fileInfo(fileToAdd);

                    ServerFile fileAdded;
                    fileAdded.filePath = fileToAdd;
                    fileAdded.fileSize = fileInfo.size();
                    fileAdded.numberDownloads = 0;

                    info.serverFiles->append(fileAdded);

                    // Add the file to the table model
                    QList<QStandardItem*> rowItems;
                    rowItems << new QStandardItem(QFileInfo(fileToAdd).fileName())
                             << new QStandardItem(QString::number(QFileInfo(fileToAdd).size()))
                             << new QStandardItem(QString::number(0));

                    // Append to the fileModel
                    fileModel->appendRow(rowItems);
                }
            }
            qDebug() << servers[port].fileServer->getIsRunning();
            // If the server isn't transferring any data, restart it now.
            if(!servers[port].fileServer->getIsTransferring() && servers[port].fileServer->getIsRunning()) {

                servers[port].fileServer->closeServer();
                servers[port].fileServer->startServer(port, servers[port].serverFiles);
            }

            // Reload the file list in the QTableView
            loadFiles(currentIndex, QModelIndex());
        }
    }
}

void MainWindow::refreshFiles() {
    QModelIndex currentIndex = serverTable->currentIndex();
    loadFiles(currentIndex, QModelIndex());
}

void MainWindow::removeFileAt(quint16 port, int fileIndex) {
    if(servers.contains(port)) {
        ServerInfo &info = servers[port];

        if(fileIndex >= 0 && fileIndex < info.serverFiles->size()) {
            info.serverFiles->removeAt(fileIndex);
            fileModel->removeRow(fileIndex);

            if(!info.fileServer->getIsTransferring() && info.fileServer->getIsRunning()) {
                info.fileServer->closeServer();
                info.fileServer->startServer(port, info.serverFiles);
            }
        }
    }
}

void MainWindow::loadFiles(const QModelIndex &current, const QModelIndex &previous) {
    Q_UNUSED(previous);

    // Clear the file table
    fileModel->removeRows(0, fileModel->rowCount());

    if (current.isValid()) {
        quint16 port = serverModel->item(current.row(), 1)->text().toUInt();

        if (servers.contains(port)) {
            ServerInfo info = servers[port];

            int i = 0;

            // Add the files from the server to the fileModel
            for (const ServerFile &file : *(info.serverFiles)) {
                QFileInfo fileInfo(file.filePath);
                QList<QStandardItem*> rowItems;
                rowItems << new QStandardItem(fileInfo.fileName())
                         << new QStandardItem(QString::number(fileInfo.size() / 1024) + "kB")
                         << new QStandardItem(QString::number(file.numberDownloads));

                int row = fileModel->rowCount();
                fileModel->appendRow(rowItems);

                QToolButton *removeFileBtn = new QToolButton(this);
                removeFileBtn->setIcon(QIcon(":/assets/delete.png"));
                removeFileBtn->setStyleSheet("background: transparent; border: none;");

                QWidget *actionWidget = new QWidget(this);
                QHBoxLayout *actionLayout = new QHBoxLayout(actionWidget);

                QSpacerItem *spacerH = new QSpacerItem(10, 2, QSizePolicy::Expanding, QSizePolicy::Maximum);
                actionLayout->addItem(spacerH);
                actionLayout->addWidget(removeFileBtn);
                actionWidget->setLayout(actionLayout);

                connect(removeFileBtn, &QToolButton::clicked, this, [this, port, i]() {
                    removeFileAt(port, i);
                });

                i++;

                fileTable->setIndexWidget(fileModel->index(row, 3), actionWidget);

            }
        }
    }
}

void MainWindow::updateServerStatus(bool isRunning, QLabel *statusLabel, QToolButton *runBtn, QToolButton *stopBtn) {
    if (isRunning) {
        statusLabel->setText("Running..");
        runBtn->setEnabled(false);
        stopBtn->setEnabled(true);
    } else {
        statusLabel->setText("Stopped..");
        runBtn->setEnabled(true);
        stopBtn->setEnabled(false);
    }
}

// Menu Bar functions.
void MainWindow::showAboutDialog() {
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void MainWindow::startAllServers() {
    for(auto &server : servers) {
        if(!server.fileServer->getIsRunning()) {
            server.fileServer->startServer(server.serverPort, server.serverFiles);
        }
    }
}

void MainWindow::stopAllServers() {
    for(auto &server : servers) {
        if(server.fileServer->getIsRunning()) {
            server.fileServer->closeServer();
        }
    }
}

void MainWindow::toggleServersList() {
    bool visible = serverGroup->isVisible();
    serverGroup->setVisible(!visible);
}

void MainWindow::toggleFilesList() {
    bool visible = fileGroup->isVisible();
    fileGroup->setVisible(!visible);
}
