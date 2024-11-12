#include "replacedialog.h"
#include "ui_replacedialog.h"
#include <QMessageBox>

ReplaceDialog::ReplaceDialog(QWidget *parent, QPlainTextEdit *textedit) :
    QDialog(parent),
    ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
    pTextEdit = textedit;

    ui->rbDown->setChecked(true);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

void ReplaceDialog::on_Cancel_clicked()
{
    accept();
}


void ReplaceDialog::on_SearchNext_clicked()
{
    QString target = ui->SearchText->text();
    if(target == "" || pTextEdit == nullptr){
        return;
    }

    QString text = pTextEdit->toPlainText();
    QTextCursor c = pTextEdit->textCursor();
    int index = -1;

    if(ui->rbDown->isChecked()){
        index = text.indexOf(target, c.position(), ui->CaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
        if(index >= 0){
            c.setPosition(index);
            c.setPosition(index+target.length(), QTextCursor::KeepAnchor);

            pTextEdit->setTextCursor(c);
        }
    }
    else if(ui->rbUp->isChecked()){
        index = text.lastIndexOf(target, c.position()-text.length()-1, ui->CaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
        if(index >= 0){
            c.setPosition(index+target.length());
            c.setPosition(index, QTextCursor::KeepAnchor);

            pTextEdit->setTextCursor(c);
        }
    }

    if(index < 0){
        QMessageBox msg(this);
        msg.setWindowTitle("提示");
        msg.setText(QString("找不到 "+target));
        msg.setWindowFlag(Qt::Drawer);
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
}


void ReplaceDialog::on_Replace_clicked()
{
    QString target = ui->SearchText->text();
    QString to = ui->ReplaceText->text();

    if( pTextEdit != nullptr && target != "" && to!=""){
        QString selText = pTextEdit->textCursor().selectedText();

        if(selText == target){
            pTextEdit->insertPlainText(to);
        }

        on_SearchNext_clicked();
    }
}


void ReplaceDialog::on_ReplaceAll_clicked()
{
    QString target = ui->SearchText->text();
    QString to = ui->ReplaceText->text();

    if( pTextEdit != nullptr && target != "" && to != ""){
        QString text = pTextEdit->toPlainText();
        text.replace(target, to, ui->CaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

        pTextEdit->clear();
        pTextEdit->insertPlainText(text);
    }
}

