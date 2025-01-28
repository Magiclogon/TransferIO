#ifndef ADDSERVERDIALOG_H
#define ADDSERVERDIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QIntValidator>
#include <QPushButton>
#include <QMessageBox>

class AddServerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddServerDialog(QWidget *parent = nullptr);
    QString serverName() const;
    quint16 serverPort() const;

private:
    QLineEdit *nameEdit;
    QLineEdit *portEdit;
};

#endif // ADDSERVERDIALOG_H
