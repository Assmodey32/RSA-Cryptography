#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FrontendRSA.h"

class FrontendRSA : public QMainWindow
{
    Q_OBJECT

public:
    FrontendRSA(QWidget *parent = Q_NULLPTR);

private:
    Ui::FrontendRSAClass ui;
};
