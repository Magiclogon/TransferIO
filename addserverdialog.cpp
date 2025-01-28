#include "addserverdialog.h"

AddServerDialog::AddServerDialog(QWidget *parent) : QDialog(parent) {

    setWindowTitle("Server properties");

    QFormLayout *formLayout = new QFormLayout(this);

    nameEdit = new QLineEdit(this);
    portEdit = new QLineEdit(this);

    portEdit->setValidator(new QIntValidator(1024, 65535, this));

    formLayout->addRow("Server name:", nameEdit);
    formLayout->addRow("Server Port:", portEdit);

    QPushButton *createServerBtn = new QPushButton("Create", this);
    QPushButton *cancelServerBtn = new QPushButton("Cancel", this);

    connect(createServerBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelServerBtn, &QPushButton::clicked, this, &QDialog::reject);

    QHBoxLayout *btnLayout = new QHBoxLayout(this);
    btnLayout->addWidget(createServerBtn);
    btnLayout->addWidget(cancelServerBtn);

    formLayout->addRow(btnLayout);

    setLayout(formLayout);
}

QString AddServerDialog::serverName() const {
    return nameEdit->text();
}

quint16 AddServerDialog::serverPort() const {
    return portEdit->text().toUInt();
}
