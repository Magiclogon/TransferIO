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

    // Main layout
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Left section: Server List
    QGroupBox *serverGroup = new QGroupBox("Servers", this);
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

    QPushButton *addServerButton = new QPushButton("Add Server", this);

    serverLayout->addWidget(serverTable);
    serverLayout->addWidget(addServerButton);

    // Right section: File List
    // Inside your MainWindow constructor, update the file section to include a QTableView:
    QGroupBox *fileGroup = new QGroupBox("Files", this);
    QVBoxLayout *fileLayout = new QVBoxLayout(fileGroup);

    // Create a QTableView for the file list
    fileTable = new QTableView(this);
    fileModel = new QStandardItemModel(this);
    fileModel->setHorizontalHeaderLabels({"File Name", "Size", "Status", "Actions"});
    fileTable->setModel(fileModel);
    fileTable->horizontalHeader()->setStretchLastSection(true);
    fileTable->verticalHeader()->hide();
    fileTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    fileTable->setSelectionMode(QAbstractItemView::SingleSelection);

    // Buttons to add/remove files
    QPushButton *addFileButton = new QPushButton("Add File", this);
    QPushButton *removeFileButton = new QPushButton("Remove File", this);

    // Add the QTableView and buttons to the layout
    fileLayout->addWidget(fileTable);
    fileLayout->addWidget(addFileButton);
    fileLayout->addWidget(removeFileButton);

    // Add the file section to the splitter
    splitter->addWidget(serverGroup);
    splitter->addWidget(fileGroup);
    splitter->setStretchFactor(0, 17);
    splitter->setStretchFactor(1, 45);

    // Add the main layout to the central widget
    mainLayout->addWidget(splitter);
    setCentralWidget(centralWidget);


    // Connections
    connect(addServerButton, &QPushButton::clicked, this, &MainWindow::addServer);
    connect(serverTable->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::loadFiles);
    connect(addFileButton, &QPushButton::clicked, this, &MainWindow::addFile);
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
        QStringList *serverFiles = new QStringList({});

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
        QToolButton *stopBtn = new QToolButton(this);
        stopBtn->setIcon(QIcon(":/assets/stop.png"));
        stopBtn->setEnabled(false);
        QToolButton *removeBtn = new QToolButton(this);
        removeBtn->setIcon(QIcon(":/assets/delete.png"));

        actionLayout->addItem(spacerH2);
        actionLayout->addWidget(runBtn);
        actionLayout->addWidget(stopBtn);
        actionLayout->addWidget(removeBtn);
        actionLayout->setContentsMargins(0, 0, 0, 0);
        actionWidget->setLayout(actionLayout);

        serverModel->appendRow(rowItems);
        serverTable->setIndexWidget(serverModel->index(serverModel->rowCount() - 1, 2), serverStatusLabel);
        serverTable->setIndexWidget(serverModel->index(serverModel->rowCount() - 1, 3), actionWidget);

        connect(runBtn, &QToolButton::clicked, this, [this, info, runBtn, stopBtn, serverStatusLabel]() {
            info.fileServer->startServer(info.serverPort, *(info.serverFiles));
            qDebug() << "STARTED " << info.serverName << " ON PORT " << info.serverPort;
            runBtn->setEnabled(false);
            stopBtn->setEnabled(true);
            qDebug() << info.fileServer->getIsRunning();
            if(info.fileServer->getIsRunning()) {
                serverStatusLabel->setText("Running..");
            }
        });

        connect(stopBtn, &QToolButton::clicked, this, [this, info, runBtn, stopBtn, serverStatusLabel]() {
            info.fileServer->closeServer();
            qDebug() << "STOPPED " << info.serverName << " ON PORT " << info.serverPort;
            runBtn->setEnabled(true);
            stopBtn->setEnabled(false);
            qDebug() << info.fileServer->getIsRunning();
            if(!info.fileServer->getIsRunning()) {
                serverStatusLabel->setText("Stopped..");
            }
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
                if (!info.serverFiles->contains(fileToAdd)) {
                    info.serverFiles->append(fileToAdd);

                    // Add the file to the table model
                    QList<QStandardItem*> rowItems;
                    rowItems << new QStandardItem(QFileInfo(fileToAdd).fileName())
                             << new QStandardItem(QString::number(QFileInfo(fileToAdd).size()))
                             << new QStandardItem("Ready")
                             << new QStandardItem("Remove");

                    // Append to the fileModel
                    fileModel->appendRow(rowItems);
                }
            }

            // Reload the file list in the QTableView
            loadFiles(currentIndex, QModelIndex());
        }
    }
}

void MainWindow::removeFile() {
    // Placeholder for remove file functionality
}

void MainWindow::loadFiles(const QModelIndex &current, const QModelIndex &previous) {
    Q_UNUSED(previous);

    // Clear the file table
    fileModel->removeRows(0, fileModel->rowCount());

    if (current.isValid()) {
        quint16 port = serverModel->item(current.row(), 1)->text().toUInt();

        if (servers.contains(port)) {
            ServerInfo info = servers[port];

            // Add the files from the server to the fileModel
            for (const QString &filePath : *(info.serverFiles)) {
                QFileInfo fileInfo(filePath);
                QList<QStandardItem*> rowItems;
                rowItems << new QStandardItem(fileInfo.fileName())
                         << new QStandardItem(QString::number(fileInfo.size() / 1024) + "kB")
                         << new QStandardItem("Ready")
                         << new QStandardItem("Remove");

                fileModel->appendRow(rowItems);
            }
        }
    }
}
