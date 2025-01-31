#include "authorizeipsdialog.h"

AuthorizeIpsDialog::AuthorizeIpsDialog(QWidget *parent, QStringList currentIps) : QDialog(parent){

    currentIpList = currentIps;

    setWindowTitle("Authorized Ips");
    setFixedSize(400, 300);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *label = new QLabel("Manage authorized IP adresses:");
    mainLayout->addWidget(label);

    // Adding the Ips list
    ipsList = new QListWidget(this);
    mainLayout->addWidget(ipsList);
    // Populate it with current authorized IPs.
    for(int i = 0; i < currentIpList.size(); i++) {
        ipsList->addItem(currentIpList.at(i));
    }

    // manage Ip layout
    QHBoxLayout *manageIpLayout = new QHBoxLayout(this);

    ipLineEdit = new QLineEdit(this);
    ipLineEdit->setPlaceholderText("Enter ip adress...");

    QToolButton *addBtn = new QToolButton(this);
    addBtn->setIcon(QIcon(":/assets/plus.png"));
    QToolButton *removeBtn = new QToolButton(this);
    removeBtn->setIcon(QIcon(":/assets/minus.png"));

    manageIpLayout->addWidget(ipLineEdit);
    manageIpLayout->addWidget(addBtn);
    manageIpLayout->addWidget(removeBtn);

    mainLayout->addLayout(manageIpLayout);

    // Add dialog buttons
    QHBoxLayout *dialogBtnsLayout = new QHBoxLayout(this);

    QPushButton *saveBtn = new QPushButton("Save", this);
    QPushButton *cancelBtn = new QPushButton("Cancel", this);

    dialogBtnsLayout->addWidget(saveBtn);
    dialogBtnsLayout->addWidget(cancelBtn);

    mainLayout->addLayout(dialogBtnsLayout);

    // Buttons connection.
    connect(addBtn, &QToolButton::clicked, this, &AuthorizeIpsDialog::addIp);
    connect(removeBtn, &QToolButton::clicked, this, &AuthorizeIpsDialog::removeIp);

    connect(saveBtn, &QPushButton::clicked, this, &AuthorizeIpsDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, this, &AuthorizeIpsDialog::reject);
}

void AuthorizeIpsDialog::addIp() {
    QString ip = ipLineEdit->text().trimmed();

    if(ip.isEmpty()) {
        return;
    }
    qDebug() << "Valid ??? " << validateIp(ip);
    if(!validateIp(ip)) {
        QMessageBox::warning(this, "Warning", "IP provided isn't valid.");
        ipLineEdit->clear();
        return;
    }

    if(!ipsList->findItems(ip, Qt::MatchExactly).isEmpty()) {
        QMessageBox::warning(this, "Warning", "IP already exists in the list.");
        return;
    }

    ipsList->addItem(ip);
    ipLineEdit->clear();
}

void AuthorizeIpsDialog::removeIp() {
    QListWidgetItem *currentItem = ipsList->currentItem();
    if(currentItem) {
        delete currentItem;
    }
}

bool AuthorizeIpsDialog::validateIp(const QString &ip) {
    QStringList octets = ip.split('.');
    if (octets.size() != 4) return false;

    for (const QString &octet : octets) {
        bool ok;
        int value = octet.toInt(&ok);
        if(!ok || (value < 0) || (value > 255)) {
            return false;
        }
    }
    return true;
}

QStringList AuthorizeIpsDialog::getAuthorizedIps() const {
    QStringList ips;
    for(int i = 0; i < ipsList->count(); i++) {
        ips.append(ipsList->item(i)->text());
    }
    return ips;
}


