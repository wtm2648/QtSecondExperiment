#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_About_triggered();

    void on_Find_triggered();

    void on_FindAndReplace_triggered();

    void on_NewNote_triggered();

    void on_SaveNote_triggered();

    void on_OpenNote_triggered();

    void on_SaveNoteAs_triggered();

    void on_TextEdit_textChanged();

    void on_Undo_triggered();

    void on_redo_triggered();

    void on_Cut_triggered();

    void on_Copy_triggered();

    void on_Paste_triggered();

    void on_TextEdit_copyAvailable(bool b);

    void on_TextEdit_redoAvailable(bool b);

    void on_TextEdit_undoAvailable(bool b);

    void on_FontColor_triggered();

    void on_FontBackgroundColor_triggered();

    void on_EditorBackgroundColor_triggered();

    void on_AutoNewLine_triggered();

    void on_FontFamily_triggered();

    void on_ToolBar_triggered();

    void on_StatusBar_triggered();

    void on_SelectAll_triggered();

    void on_Exit_triggered();

    void on_TextEdit_cursorPositionChanged();

    void on_ShowLineCount_triggered(bool checked);

private:
    Ui::MainWindow *ui;

    QLabel StatusCursorLabel;
    QLabel StatusLabel;
    QString FilePath;
    bool textChanged;
    bool userEditConfirmed();
};
#endif // MAINWINDOW_H
