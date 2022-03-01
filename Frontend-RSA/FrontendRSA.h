#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FrontendRSA.h"
#include "../RSA/RSA.h"

class FrontendRSA : public QMainWindow
{
    Q_OBJECT

public:
    FrontendRSA(QWidget *parent = Q_NULLPTR);

private slots:
    void on_pushButtonEncrypt_clicked();

    void on_pushButtonGenerateKeys_clicked();

    void on_pushButtonDecrypt_clicked();


private:

    void popMessageBox();

    Ui::FrontendRSAClass ui;
    RSA rsa;
};
