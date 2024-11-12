#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QColorDialog>
#include <QFontDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    textChanged = false;

    on_NewNote_triggered();

    StatusLabel.setMaximumWidth(200);
    StatusLabel.setText("Length: " + QString::number(0) + "    lines: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&StatusLabel);

    StatusCursorLabel.setMaximumWidth(200);
    StatusCursorLabel.setText("Ln: " + QString::number(1) + "    Col: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&StatusCursorLabel);

    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("吴富霖"));
    ui->statusbar->addPermanentWidget(author);

    ui->Copy->setEnabled(false);
    ui->Cut->setEnabled(false);
    ui->redo->setEnabled(false);
    ui->Undo->setEnabled(false);
    ui->Paste->setEnabled(false);

    QPlainTextEdit::LineWrapMode mode = ui->TextEdit->lineWrapMode();

    if(mode == QPlainTextEdit::NoWrap){
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);

        ui->AutoNewLine->setChecked(true);
    } else {
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);

        ui->AutoNewLine->setChecked(false);
    }

    ui->ToolBar->setChecked(true);

    ui->StatusBar->setChecked(true);

    on_ShowLineCount_triggered(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_About_triggered()
{
    aboutDialog dia;
    dia.exec();
}


void MainWindow::on_Find_triggered()
{
    SearchDialog sd(this, ui->TextEdit);
    sd.exec();
}


void MainWindow::on_FindAndReplace_triggered()
{
    ReplaceDialog rd(this, ui->TextEdit);
    rd.exec();
}

void MainWindow::on_NewNote_triggered()
{
    if(!userEditConfirmed())
        return;
    FilePath = "";
    ui->TextEdit->clear();
    this->setWindowTitle(tr("NewTextFile - Editor"));
    textChanged = false;
}

void MainWindow::on_OpenNote_triggered()
{
    if(!userEditConfirmed())
        return;
    QString filename = QFileDialog::getOpenFileName(this, "打开文件", ".", tr("Text files (*.txt) ;; All (*.*)"));
    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "..", "打开文件失败");
        return;
    }

    FilePath = filename;

    QTextStream in(&file);
    QString text = in.readAll();
    ui->TextEdit->insertPlainText(text);

    file.close();
    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());

    textChanged = false;
}

void MainWindow::on_SaveNote_triggered()
{
    if(FilePath == ""){
        QString filename = QFileDialog::getSaveFileName(this, "保存文件", ".", tr("Text files (*.txt) "));
        QFile file(filename);
        if(!file.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::warning(this,"..","保存文件失败");
            return;
        }
        file.close();
        FilePath = filename;
    }

    QFile file(FilePath);

    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this,"..","保存文件失败");
        return;
    }

    QTextStream out(&file);
    QString text = ui->TextEdit->toPlainText();
    out<<text;
    file.flush();
    file.close();

    textChanged = false;
}

void MainWindow::on_SaveNoteAs_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "保存文件", ".", tr("Text files (*.txt) "));
    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this,"..","保存文件失败");
        return;
    }
    QTextStream out(&file);
    QString text = ui->TextEdit->toPlainText();
    out<<text;
    file.flush();
    file.close();

    this->setWindowTitle(QFileInfo(FilePath).absoluteFilePath());
}


void MainWindow::on_TextEdit_textChanged()
{
    if(!textChanged){
        this->setWindowTitle("*"+this->windowTitle());
        textChanged = true;
    }

    StatusLabel.setText("Length: " + QString::number(ui->TextEdit->toPlainText().length()) + "    lines: " + QString::number(ui->TextEdit->document()->lineCount()));
}

bool MainWindow::userEditConfirmed()
{
    if(textChanged){
        QString path = (FilePath != "") ? FilePath:"无标题.txt";

        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Question);
        msg.setWindowTitle("...");
        msg.setWindowFlag(Qt::Drawer);
        msg.setText(QString("是否将更改保存到\n") + "\"" + path + "\"");
        msg.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        int r=msg.exec();
        switch( r ){
            case QMessageBox::Yes:
                on_SaveNote_triggered();
                break;
            case QMessageBox::No:
                textChanged=false;
                break;
            case QMessageBox::Cancel:
                return false;
        }
    }
    return true;
}

void MainWindow::on_Undo_triggered()
{
    ui->TextEdit->undo();
}


void MainWindow::on_redo_triggered()
{
    ui->TextEdit->redo();
}


void MainWindow::on_Cut_triggered()
{
    ui->TextEdit->cut();
    ui->Paste->setEnabled(true);
}


void MainWindow::on_Copy_triggered()
{
    ui->TextEdit->copy();
    ui->Paste->setEnabled(true);
}


void MainWindow::on_Paste_triggered()
{
    ui->TextEdit->paste();
}


void MainWindow::on_TextEdit_copyAvailable(bool b)
{
    ui->Copy->setEnabled(b);
    ui->Cut->setEnabled(b);
}


void MainWindow::on_TextEdit_redoAvailable(bool b)
{
    ui->redo->setEnabled(b);
}


void MainWindow::on_TextEdit_undoAvailable(bool b)
{
    ui->Undo->setEnabled(b);
}


void MainWindow::on_FontColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "字体颜色");
    if(color.isValid()){
        ui->TextEdit->setStyleSheet(QString("QPlainTextEdit {color: %1}").arg(color.name()));
    }
}


void MainWindow::on_FontBackgroundColor_triggered()
{

}


void MainWindow::on_EditorBackgroundColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "文本框颜色");
    if(color.isValid()){
        ui->TextEdit->setStyleSheet(QString("QPlainTextEdit {background-color: %1}").arg(color.name()));
    }
}


void MainWindow::on_AutoNewLine_triggered()
{
    QPlainTextEdit::LineWrapMode mode = ui->TextEdit->lineWrapMode();

    if(mode == QPlainTextEdit::NoWrap){
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);

        ui->AutoNewLine->setChecked(true);
    } else {
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);

        ui->AutoNewLine->setChecked(false);
    }
}


void MainWindow::on_FontFamily_triggered()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok,this);

    if(ok){
        ui->TextEdit->setFont(font);
    }
}


void MainWindow::on_ToolBar_triggered()
{
    bool visible = ui->toolBar->isVisible();
    ui->toolBar->setVisible(!visible);
    ui->ToolBar->setChecked(!visible);
}


void MainWindow::on_StatusBar_triggered()
{
    bool visible = ui->statusbar->isVisible();
    ui->statusbar->setVisible(!visible);
    ui->StatusBar->setChecked(!visible);
}


void MainWindow::on_SelectAll_triggered()
{
    ui->TextEdit->selectAll();
}


void MainWindow::on_Exit_triggered()
{
    if(userEditConfirmed()){
        exit(0);
    }
    else{

    }
}


void MainWindow::on_TextEdit_cursorPositionChanged()
{
    int col=0;
    int ln=0;
    int flg=-1;
    int pos = ui->TextEdit->textCursor().position();
    QString text = ui->TextEdit->toPlainText();

    for(int i=0;i<pos;i++){
        if(text[i] == '\n'){
            ln++;
            flg=i;
        }
    }
    flg++;
    col = pos - flg;
    StatusCursorLabel.setText("Ln: " + QString::number(ln+1) + "    Col: " + QString::number(col+1));
}


void MainWindow::on_ShowLineCount_triggered(bool checked)
{
    ui->TextEdit->hideLineNumberArea(!checked);
}

