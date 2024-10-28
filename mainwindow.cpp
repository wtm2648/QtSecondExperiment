#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    StatusLabel.setMaximumWidth(150);
    StatusLabel.setText("Length: " + QString::number(0) + "    lines: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&StatusLabel);

    StatusCursorLabel.setMaximumWidth(150);
    StatusCursorLabel.setText("Ln: " + QString::number(1) + "    Col: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&StatusCursorLabel);

    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("吴富霖"));
    ui->statusbar->addPermanentWidget(author);
}

MainWindow::~MainWindow()
{
    delete ui;
}

