#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent) {

    setWindowTitle("About TransferIO");
    setFixedSize(400, 300);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    QLabel *appName = new QLabel("<h2><b>TransferIO</b></h2>");
    appName->setAlignment(Qt::AlignCenter);

    QLabel *appDescription = new QLabel("TransferIO is a tool designed to simplify sharing files across different devices.");
    appDescription->setWordWrap(true);
    appDescription->setAlignment(Qt::AlignCenter);

    // Link label for GitHub
    QLabel *linkGit = new QLabel("<a href='https://github.com/Magiclogon/TransferIO'><b>Visit GitHub Repository</b></a>");
    linkGit->setOpenExternalLinks(true);
    linkGit->setAlignment(Qt::AlignCenter);

    // Information Labels
    QLabel *developerLabel = new QLabel("<b>Developer:</b> Magiclogon / Walid H.");
    QLabel *versionLabel = new QLabel("<b>Version:</b> 1.0.0");
    QLabel *licenseLabel = new QLabel("<b>License:</b> GNU");
    QLabel *contactLabel = new QLabel("<b>Contact:</b> walidhousni45@gmail.com");

    // Add elements to the form layout
    formLayout->addRow(developerLabel);
    formLayout->addRow(versionLabel);
    formLayout->addRow(licenseLabel);
    formLayout->addRow(contactLabel);

    // Close button
    QPushButton *closeButton = new QPushButton("Close", this);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    // Add widgets to the main layout
    mainLayout->addWidget(appName);
    mainLayout->addWidget(appDescription);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(linkGit);
    mainLayout->addWidget(closeButton, 0, Qt::AlignCenter);
}
