/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *mainLayout;
    QWidget *serversWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *serversWidgetLayout;
    QGroupBox *serversGroupBox;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *serversGroupBoxLayout;
    QHBoxLayout *addRemServLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *addServerBtn;
    QPushButton *removeServerBtn;
    QListWidget *serversList;
    QWidget *filesWidget;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *filesWidgetLayout;
    QGroupBox *filesGroupBox;
    QWidget *verticalLayoutWidget_4;
    QVBoxLayout *filesGroupBoxLayout;
    QHBoxLayout *addRemFileLayout;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QListWidget *filesList;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 650);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(-1, -1, 1201, 611));
        mainLayout = new QHBoxLayout(horizontalLayoutWidget);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, 0, 0, 0);
        serversWidget = new QWidget(horizontalLayoutWidget);
        serversWidget->setObjectName("serversWidget");
        verticalLayoutWidget = new QWidget(serversWidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(9, -1, 321, 601));
        serversWidgetLayout = new QVBoxLayout(verticalLayoutWidget);
        serversWidgetLayout->setObjectName("serversWidgetLayout");
        serversWidgetLayout->setContentsMargins(0, 0, 0, 0);
        serversGroupBox = new QGroupBox(verticalLayoutWidget);
        serversGroupBox->setObjectName("serversGroupBox");
        serversGroupBox->setEnabled(true);
        serversGroupBox->setCheckable(false);
        verticalLayoutWidget_2 = new QWidget(serversGroupBox);
        verticalLayoutWidget_2->setObjectName("verticalLayoutWidget_2");
        verticalLayoutWidget_2->setGeometry(QRect(9, 19, 301, 571));
        serversGroupBoxLayout = new QVBoxLayout(verticalLayoutWidget_2);
        serversGroupBoxLayout->setObjectName("serversGroupBoxLayout");
        serversGroupBoxLayout->setContentsMargins(0, 0, 0, 0);
        addRemServLayout = new QHBoxLayout();
        addRemServLayout->setObjectName("addRemServLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        addRemServLayout->addItem(horizontalSpacer);

        addServerBtn = new QPushButton(verticalLayoutWidget_2);
        addServerBtn->setObjectName("addServerBtn");

        addRemServLayout->addWidget(addServerBtn);

        removeServerBtn = new QPushButton(verticalLayoutWidget_2);
        removeServerBtn->setObjectName("removeServerBtn");

        addRemServLayout->addWidget(removeServerBtn);


        serversGroupBoxLayout->addLayout(addRemServLayout);

        serversList = new QListWidget(verticalLayoutWidget_2);
        serversList->setObjectName("serversList");

        serversGroupBoxLayout->addWidget(serversList);


        serversWidgetLayout->addWidget(serversGroupBox);


        mainLayout->addWidget(serversWidget);

        filesWidget = new QWidget(horizontalLayoutWidget);
        filesWidget->setObjectName("filesWidget");
        verticalLayoutWidget_3 = new QWidget(filesWidget);
        verticalLayoutWidget_3->setObjectName("verticalLayoutWidget_3");
        verticalLayoutWidget_3->setGeometry(QRect(10, 0, 851, 601));
        filesWidgetLayout = new QVBoxLayout(verticalLayoutWidget_3);
        filesWidgetLayout->setObjectName("filesWidgetLayout");
        filesWidgetLayout->setContentsMargins(0, 0, 0, 0);
        filesGroupBox = new QGroupBox(verticalLayoutWidget_3);
        filesGroupBox->setObjectName("filesGroupBox");
        verticalLayoutWidget_4 = new QWidget(filesGroupBox);
        verticalLayoutWidget_4->setObjectName("verticalLayoutWidget_4");
        verticalLayoutWidget_4->setGeometry(QRect(9, 19, 831, 571));
        filesGroupBoxLayout = new QVBoxLayout(verticalLayoutWidget_4);
        filesGroupBoxLayout->setObjectName("filesGroupBoxLayout");
        filesGroupBoxLayout->setContentsMargins(0, 0, 0, 0);
        addRemFileLayout = new QHBoxLayout();
        addRemFileLayout->setObjectName("addRemFileLayout");
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        addRemFileLayout->addItem(horizontalSpacer_6);

        pushButton_2 = new QPushButton(verticalLayoutWidget_4);
        pushButton_2->setObjectName("pushButton_2");

        addRemFileLayout->addWidget(pushButton_2);

        pushButton = new QPushButton(verticalLayoutWidget_4);
        pushButton->setObjectName("pushButton");

        addRemFileLayout->addWidget(pushButton);


        filesGroupBoxLayout->addLayout(addRemFileLayout);

        filesList = new QListWidget(verticalLayoutWidget_4);
        filesList->setObjectName("filesList");

        filesGroupBoxLayout->addWidget(filesList);


        filesWidgetLayout->addWidget(filesGroupBox);


        mainLayout->addWidget(filesWidget);

        mainLayout->setStretch(0, 17);
        mainLayout->setStretch(1, 45);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1200, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        serversGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Servers", nullptr));
        addServerBtn->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        removeServerBtn->setText(QCoreApplication::translate("MainWindow", "Remove", nullptr));
        filesGroupBox->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Add File", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Remove File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
