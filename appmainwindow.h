#ifndef APPMAINWINDOW_H
#define APPMAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QFile>
#include <QtCore>
#include <QDir>
#include <QSysInfo>
#include <QMessageBox>
#include <QProgressBar>
#include <QFileInfo>
#include <QDateTime>
#include "file_verifier.h"
#include "find_duplicate_files.h"

namespace Ui {
class AppMainWindow;
}

class AppMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AppMainWindow(QWidget *parent = 0);
    ~AppMainWindow();


private slots:
    void changeTabWidgetSize();

    void enable_Delete_File_Button(QTreeWidgetItem*,int);

    void increase_Window_size();

    void decrease_Window_size();

    // file verification tab slots-------------------------
    void on_checksum_comboBox_currentIndexChanged(int index);

    void on_file_browse_toolButton_clicked();

    void on_cal_checksum_pushButton_clicked();

    void on_cal_checksum_lineEdit_textChanged(const QString &arg1);

    void on_filepath_lineEdit_textChanged(const QString &arg1);

    void on_verify_checksum_pushButton_clicked();

    // action slots -------------------------------------

    void on_action_Clear_triggered();

    void on_action_Check_triggered();

    void on_action_Verify_triggered();

    void on_action_Exit_triggered();

    void on_action_Delete_files_triggered();

    // file duplicate finder slots ----------------------------

    void on_directory_scan_pushButton_clicked();

    void on_whole_computer_scan_pushButton_clicked();

    void on_action_About_triggered();

    void on_custom_type_radioButton_clicked();

    void on_filetype_back_pushButton_clicked();

    void on_scan_files_pushButton_clicked();

    void on_stop_scan_pushButton_clicked();

    void on_action_Find_Duplicates_triggered();

    void on_back_main_find_menu_pushButton_clicked();

    void on_delete_file_pushButton_clicked();

    void on_next_pushButton_clicked();


signals:
    void signalEnabledScanButton(bool);        // emit enable signal when Whole computer or Directory selected

    void duplicateFileSignal(myQFileInfo);  // emit duplicate file information by find_duplicate_file function

private:

     void moveToTrashOrRecycleBin(QFileInfo FileName);

     Ui::AppMainWindow *ui;
     QDesktopWidget w;         // for getting size of desktop screen
     QString fileName;         // file name with path
     QDir dir;
     QStringList fileTypes;     // file types for scan
     QFileDialog *dialog;
     findDuplicateFiles *findDuplicate;  // thread
};

#endif // APPMAINWINDOW_H
