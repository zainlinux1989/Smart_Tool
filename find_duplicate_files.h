#include "file_verifier.h"
#include "myqfileinfo.h"
#include "ui_appmainwindow.h"
#include <QDirIterator>
#include <QHash>
#include <QLabel>
#include <QMessageBox>
#include <QProgressBar>
#include <QMainWindow>
#include <QTransform>
#include <QPixmap>
#include <QThread>
#include <QTreeWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QUrl>

#ifndef FINDDUPLICATEFILES_H
#define FINDDUPLICATEFILES_H


class findDuplicateFiles : public QThread
{
public:


    findDuplicateFiles();

    void setFileAttribute(const QDir & path, const QStringList & fileTypes, Ui_AppMainWindow *ui);     // ui contain all ui components reference  Qlistview, QStackedWidget,  prgressbar to see file scanning progress, next push button
                                                                                                                                                              // progressBarLabel is prgressbar label,
    void run();     // call when thread is start

    void stop();    // call for stop  thread


private:

    void findDuplicate(const QDir & path, const QStringList & fileTypes ,Ui_AppMainWindow *ui);    // ui contain all ui components reference prgressIcon to see file scanning progress

    void setDuplicateFilesRecordOnTree(const QString& fileName,int groupNo, const QString& filePath, const QString& fileSize, int entryColor);  // add duplicate items in Qtreewidget list file name, file group number, file path, file size, entry text & background color 0 | 1, 0 for main entry, 1 for sub entry.

    QHash<QString, myQFileInfo> nonDuplicateFilesRecord;     // save record of multiple file with hash code
                                                            // duplicate key not exist in Qhash

    QList<QString> duplicateFilesRecord;     // save duplicate files as list

    //int nonDuplicateFileCount=0;              // save non duplicate file numbers
    //int duplicateFileCount=0;           // save duplicate files numbers
    bool stopProgram;                   // for stop find file duplicate thread

    // for setting attribute for thread
    QDir path;
    QStringList fileType;

    Ui_AppMainWindow *ui;  // ui pointer for access ui components

};

#endif // FINDDUPLICATEFILES_H
