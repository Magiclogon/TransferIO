#ifndef AUTHORIZEIPSDIALOG_H
#define AUTHORIZEIPSDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QListWidget>
#include <QLineEdit>
#include <QToolButton>
#include <QRegularExpression>
#include <QStringList>

#include "fileserver.h"


class AuthorizeIpsDialog : public QDialog
{
public:
    explicit AuthorizeIpsDialog(QWidget *parent = nullptr, QStringList currentIps = QStringList());
    QStringList getAuthorizedIps() const;

private slots:
    void addIp();
    void removeIp();

private:
    QListWidget *ipsList;
    QLineEdit *ipLineEdit;

    QStringList currentIpList;
    bool validateIp(const QString &ip);
};

#endif // AUTHORIZEIPSDIALOG_H
