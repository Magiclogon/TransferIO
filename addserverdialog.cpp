#include "addserverdialog.h"

AddServerDialog::AddServerDialog(QWidget *parent) : QDialog(parent) {

    this->setStyleSheet(R"(
        QDialog {
            background-color: #121212;
        }

        QLineEdit {
            background-color: #414141;
            color: #ffffff;
            padding: 2px;
            border: 1px;
        }

        QPushButton {
            background-color: #414141;
            color: #ffffff;
        }

        QPushButton::Hover {
            background-color: #2f2f2f;
            color: #ffffff;
        }
    )");

    setWindowTitle("Server properties");

    QFormLayout *formLayout = new QFormLayout(this);

    nameEdit = new QLineEdit(this);
    portEdit = new QLineEdit(this);

    portEdit->setValidator(new QIntValidator(1024, 65535, this));

    formLayout->addRow("Server name:", nameEdit);
    formLayout->addRow("Server Port:", portEdit);

    QPushButton *createServerBtn = new QPushButton("Create", this);
    QPushButton *cancelServerBtn = new QPushButton("Cancel", this);

    connect(createServerBtn, &QPushButton::clicked, this, [this]() {
        if(!nameEdit->text().isEmpty() && !portEdit->text().isEmpty()) {
            accept();
        } else {
            QMessageBox::warning(this, "Missing fields.", "Port or server name aren't given.");
        }
    });
    connect(cancelServerBtn, &QPushButton::clicked, this, &QDialog::reject);

    QHBoxLayout *btnLayout = new QHBoxLayout(this);
    btnLayout->addWidget(cancelServerBtn);
    btnLayout->addWidget(createServerBtn);

    formLayout->addRow(btnLayout);

    setLayout(formLayout);
}

QString AddServerDialog::serverName() const {
    return nameEdit->text();
}

quint16 AddServerDialog::serverPort() const {
    return portEdit->text().toUInt();
}
