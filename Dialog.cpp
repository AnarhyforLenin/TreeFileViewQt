#include "Dialog.h"
#include "ui_Dialog.h"

Dialog::Dialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

dialog::~Dialog(){
    delete ui;
}
