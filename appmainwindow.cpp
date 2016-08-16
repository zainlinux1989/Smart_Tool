#include "appmainwindow.h"
#include "ui_appmainwindow.h"

AppMainWindow::AppMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AppMainWindow)
{
    ui->setupUi(this);

    AppMainWindow::setWindowTitle("Smart Tool");

    ui->App_tabWidget->setCurrentIndex(0);

    AppMainWindow::setFixedSize(w.width()*0.5,w.height()*0.3);      // resize window height wrt desktop height 0.3 is factor

    ui->centralWidget->setFixedWidth(AppMainWindow::width());       // fix central widget size

    connect(ui->App_tabWidget,SIGNAL(currentChanged(int)),this,SLOT(changeTabWidgetSize()));

 // File verification tab start-----------------------------------------------

    ui->cal_checksum_label->setText(ui->checksum_comboBox->currentText()+":");

    ui->org_checksum_lineEdit->setPlaceholderText("Enter "+ui->checksum_comboBox->currentText()+" hash code");

    ui->org_checksum_lineEdit->setToolTip("Enter "+ui->checksum_comboBox->currentText()+" hash code");

    ui->cal_checksum_label->setMinimumWidth(ui->checksum_comboBox->width());

    ui->org_checksum_label->setMinimumWidth(ui->checksum_comboBox->width());

    ui->cal_checksum_pushButton->setEnabled(false);

    ui->verify_checksum_pushButton->setEnabled(false);

 // File verification tab end-----------------------------------------------

 // Find duplicate file tab start-----------------------------------------------

    ui->scan_files_pushButton->setEnabled(false);                     // by default scan button disable

    ui->stop_scan_pushButton->setEnabled(false);                      // by default stop button disable

    ui->next_pushButton->setEnabled(false);                           // by default next button disable

    connect(this,SIGNAL(signalEnabledScanButton(bool)),ui->scan_files_pushButton,SLOT(setEnabled(bool)));   // monitor enabledScan() signal

    ui->filetype_selection_stackedWidget->setCurrentIndex(0);

    ui->all_type_radioButton->setChecked(true);

    ui->progress_file_label->setText("....");

    ui->duplicate_files_treeWidget->setColumnCount(4);      // set four columes, 1st for checkable button with file name, 2nd for file group, 3rd for file path, 4th for file size.

    ui->duplicate_files_treeWidget->setColumnWidth(0, 200);  // set file name column width

    ui->duplicate_files_treeWidget->setColumnWidth(1, 60);  // set file group column width

    ui->duplicate_files_treeWidget->setColumnWidth(2, 600);  // set file path column width

    ui->delete_file_pushButton->setEnabled(false);          // at start delete button disable

    ui->find_duplicate_files_stackedWidget->setCurrentIndex(0); // set first page as index of find duplicate files

    connect(ui->duplicate_files_treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(enable_Delete_File_Button(QTreeWidgetItem*,int)));    // enable delete button if any item selected in item list

    // Find duplicate file tab end-----------------------------------------------

}



AppMainWindow::~AppMainWindow()
{
    delete ui;
}


void AppMainWindow::changeTabWidgetSize()
{

    if(ui->App_tabWidget->currentIndex()==0)
    {
        AppMainWindow::setFixedHeight(w.height()*0.3);  // resize window height wrt desktop height 0.3 is factor
        AppMainWindow::setFixedWidth(w.width()*0.5);    // resize window width wrt desktop width 0.5 is factor
        ui->centralWidget->setFixedWidth(AppMainWindow::width()*1);   // resize cental widget width wrt main width width 1 is factor
        ui->centralWidget->setFixedHeight(AppMainWindow::height()*0.75);   // resize cental widget height wrt main widget height 0.75 is factor

    }
    else if(ui->App_tabWidget->currentIndex()==1)
    {
        if(ui->find_duplicate_files_stackedWidget->currentIndex()==0)   // if main file duplicate tab
        {
            AppMainWindow::setFixedHeight(w.height()*0.6);  // resize window height wrt desktop height 0.6 is factor
            AppMainWindow::setFixedWidth(w.width()*0.5);    // resize window width wrt desktop width 0.5 is factor
            ui->centralWidget->setFixedWidth(AppMainWindow::width()*1);   // resize cental widget width wrt main width width 1 is factor
            ui->centralWidget->setFixedHeight(AppMainWindow::height()*0.9);     // resize cental widget height wrt main widget height 0.9 is factor

        }

        else if(ui->find_duplicate_files_stackedWidget->currentIndex()==1)  // if duplicate list tab
        {
            AppMainWindow::setFixedHeight(w.height()*0.80);  // resize window height wrt desktop height 0.80 is factor
            AppMainWindow::setFixedWidth(w.width()*0.80);    // resize window width wrt desktop width 0.80 is factor
            ui->centralWidget->setFixedWidth(AppMainWindow::width()*1);   // resize cental widget width wrt main width width 1 is factor
            ui->centralWidget->setFixedHeight(AppMainWindow::height()*0.9);     // resize cental widget height wrt main widget height 0.9 is factor
        }
    }
}




// File verification tab functions START-----------------------------------------------

void AppMainWindow::on_checksum_comboBox_currentIndexChanged(int index)
{
    ui->cal_checksum_label->setText(ui->checksum_comboBox->currentText()+":");

    ui->org_checksum_lineEdit->setPlaceholderText("Enter "+ui->checksum_comboBox->currentText()+" hash code");

    ui->org_checksum_lineEdit->setToolTip("Enter "+ui->checksum_comboBox->currentText()+" hash code");
}

void AppMainWindow::on_file_browse_toolButton_clicked()
{
    fileName=QFileDialog::getOpenFileName(this);
    ui->filepath_lineEdit->setText(fileName);
}

void AppMainWindow::on_cal_checksum_pushButton_clicked()
{
    file_verifier file;
    file.setFileName(fileName,ui->checksum_comboBox->currentIndex());
    ui->cal_checksum_lineEdit->setText(file.getCheckSum());
}

void AppMainWindow::on_cal_checksum_lineEdit_textChanged(const QString &arg1)
{
    if(arg1!="")
        ui->verify_checksum_pushButton->setEnabled(true);
    else
        ui->verify_checksum_pushButton->setEnabled(false);
}

void AppMainWindow::on_filepath_lineEdit_textChanged(const QString &arg1)
{
    QFile f(arg1);
    if(f.open(QFile::ReadOnly))
    {
        f.close();
        ui->cal_checksum_pushButton->setEnabled(true);
    }
    else
    {
        ui->cal_checksum_pushButton->setEnabled(false);
        ui->filepath_lineEdit->setToolTip("Enter correct file path....");
    }
}

void AppMainWindow::on_verify_checksum_pushButton_clicked()
{
    if(ui->org_checksum_lineEdit->text()==ui->cal_checksum_lineEdit->text())
    {
        ui->statusBar->setStyleSheet("color: green");
        ui->statusBar->showMessage(ui->checksum_comboBox->currentText()+" Hash is matched.",2500);
    }
    else
    {
        ui->statusBar->setStyleSheet("color: red");
        ui->statusBar->showMessage(ui->checksum_comboBox->currentText()+" Hash is not matched.",2500);
    }
}

// File verification tab functions END-----------------------------------------------



// Find duplicate file tab functions START-----------------------------------------------

void AppMainWindow::enable_Delete_File_Button(QTreeWidgetItem *item,int n)      // enable and disable delete file button
{
    if(item->checkState(0)==Qt::Checked)               // enable delete button if any filename column is checked
    {
        ui->delete_file_pushButton->setEnabled(true);
        ui->statusBar->showMessage(item->text(0)+" is selected");      // show item name on status bar which select, column 0 is file name column
    }
    else
    {
        ui->delete_file_pushButton->setEnabled(false);
    }
}

void AppMainWindow::increase_Window_size()  // increase window size when next button clicked
{
    AppMainWindow::setFixedHeight(w.height()*0.80);  // resize window height wrt desktop height 0.80 is factor
    AppMainWindow::setFixedWidth(w.width()*0.80);    // resize window width wrt desktop height 0.80 is factor
    ui->centralWidget->setFixedWidth(AppMainWindow::width()*1);   // resize cental widget width wrt main widget width 1 is factor
    ui->centralWidget->setFixedHeight(AppMainWindow::height()*0.9);     // resize cental widget height wrt main widget height 0.9 is factor
}

void AppMainWindow::decrease_Window_size()  // decrease window size when back button clicked
{
    AppMainWindow::setFixedHeight(w.height()*0.6);  // resize window height wrt desktop height 0.6 is factor
    AppMainWindow::setFixedWidth(w.width()*0.5);    // resize window width wrt desktop width 0.5 is factor
    ui->centralWidget->setFixedWidth(AppMainWindow::width()*1);     // resize cental widget width wrt main widget width 1 is factor
    ui->centralWidget->setFixedHeight(AppMainWindow::height()*0.9);     // resize cental widget height wrt main widget height 0.9 is factor
}

void AppMainWindow::on_directory_scan_pushButton_clicked()       // when select directory for scan
{
    dialog=new QFileDialog(this);                                         // open file dialog to select directory for scanning
    dialog->setFileMode(QFileDialog::Directory);
    dialog->setOption(QFileDialog::ShowDirsOnly);
    dir.setPath(dialog->getExistingDirectory(this,"Select Directory"));
    if(dir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries).count() == 0)       // check selected directory is empty or not
    {
        QMessageBox::information(this,"Directory is empty","Select non empty directory");
        dir.setPath("");
        emit signalEnabledScanButton(false);
    }
    else
    {
        emit signalEnabledScanButton(true);
  }
    delete dialog;
}

void AppMainWindow::on_whole_computer_scan_pushButton_clicked()     // when select whole computer for scan
{
    emit signalEnabledScanButton(true);

    // in the case of linux
    dir.setPath("/");       // set "/" as a whole system path

    // in the case of windows add soon
    // add computer all directory

 }

void AppMainWindow::on_custom_type_radioButton_clicked()    // for set custom file types
{
    ui->filetype_selection_stackedWidget->setCurrentIndex(1);
    fileTypes.clear();
}

void AppMainWindow::on_filetype_back_pushButton_clicked()   // for select all types
{
    ui->filetype_selection_stackedWidget->setCurrentIndex(0);
    ui->all_type_radioButton->setChecked(true);
    fileTypes.clear();
}

void AppMainWindow::on_scan_files_pushButton_clicked() // start scanning
{
    ui->stop_scan_pushButton->setEnabled(true); // enable stop button when scan button clicked

    ui->scan_files_pushButton->setEnabled(false);   // disable scan button when it clicked

    ui->next_pushButton->setEnabled(false);     // disable next button when scan

    ui->duplicate_files_treeWidget->clear();    // clear list

    if(ui->all_type_radioButton->isChecked())   // for select all type of file
    {
        fileTypes.clear();

        fileTypes<<"*";      // * for select all files for scan

    }
    else                                        // for select custom type of file---------- in development phase//
    {
        fileTypes.clear();

        QStringList docFile, musFile,vidFile, picFile;

        docFile.clear();
        musFile.clear();
        vidFile.clear();
        picFile.clear();

        if(ui->document_checkBox->isChecked())
        {
            docFile<<"*.doc"<<"*.docx"<<"*.pdf";         //  and so on add document files extension
        }

        if(ui->music_checkBox->isChecked())
        {
            musFile<<"*.mp3"<<"*.wma";          // and so on add music files extension
        }

        if(ui->video_checkBox->isChecked())
        {
            vidFile<<"*.mp4"<<"*.mkv";          // and so on add video files extension
        }

        if(ui->picture_checkBox->isChecked())
        {
            picFile<<"*.jpeg"<<"*.jpg"<<"*.png";    // and so onadd pictures files extension
        }

        if(!ui->document_checkBox->isChecked()&&!ui->video_checkBox->isChecked()&&!ui->music_checkBox->isChecked()&&!ui->picture_checkBox->isChecked())     // if not file type checked then scan all file using "*"
        {
            fileTypes.clear();
            fileTypes.append("*");
        }

        else
        {
            fileTypes.append(docFile);       // add document file extensions in fileType
            fileTypes.append(musFile);       // add music file extensions in fileType
            fileTypes.append(vidFile);       // add video file extensions in fileType
            fileTypes.append(picFile);       // add picture file extensions in fileType
            // so on add more extensions
        }
    }


    findDuplicate=new findDuplicateFiles();

    findDuplicate->setFileAttribute(dir,fileTypes,ui);//ui->duplicate_files_treeWidget, ui->find_duplicate_files_stackedWidget, ui->progress_file_label,ui->progress_icon,ui->next_pushButton);     // path, files type to scan, list view widget, stackwidget, label of file in progress, progress bar label,progress bar icon, next push button

    findDuplicate->start();      // start duplicate file find thread


}

void AppMainWindow::on_stop_scan_pushButton_clicked()   // stop scanning
{
    findDuplicate->stop();
}

void AppMainWindow::on_back_main_find_menu_pushButton_clicked()     // back to main file finding gui
{

    ui->find_duplicate_files_stackedWidget->setCurrentIndex(0);   // back to main file find menu

    ui->scan_files_pushButton->setEnabled(false);       // disable scan button after scaning, set default value

    ui->stop_scan_pushButton->setEnabled(false);    // diable stop button after scaning, set default value

    dir.setPath("");            // clear path after scaning, set default value

    fileTypes.clear();          // clear extension filter after scanning, set default value

    ui->statusBar->showMessage(""); // clear status bar message
}

void AppMainWindow::on_delete_file_pushButton_clicked()     // delete duplicate files
{
    int selectButton;   // "Yes" or "No"
    selectButton=QMessageBox::warning(this,"Delete File(s)"," Delete selected files ",QMessageBox::Yes|QMessageBox::No);

    if(selectButton==QMessageBox::Yes)      // if yes button selected then delete selected files
    {
        QTreeWidgetItemIterator it(ui->duplicate_files_treeWidget);

        QStringList deleteFileList;

        deleteFileList.clear();

        while(*it)
        {
            if((*it)->checkState(0)==Qt::Checked)
            {
                deleteFileList.append((*it)->text(2));     // file name with path exsit in column 2 of treewidget add in deleteFileList
            }
            ++it;
        }

        QFileInfo deleteFile;

        for(int i=0;i<deleteFileList.size();i++)
        {
            deleteFile.setFile(deleteFileList.at(i));

            moveToTrashOrRecycleBin(deleteFile);        // move file into trash or recycle bin
        }

        ui->statusBar->setStyleSheet("color:green");
        ui->statusBar->showMessage("Selected file(s) successfully deleted",2500);

        ui->find_duplicate_files_stackedWidget->setCurrentIndex(0);

        ui->scan_files_pushButton->setEnabled(false);                     //  scan button disable

        ui->stop_scan_pushButton->setEnabled(false);                      //  stop button disable

        ui->next_pushButton->setEnabled(false);                           //  next button disable

        decrease_Window_size();

    }
    else if(selectButton==QMessageBox::No)
    {
        ui->statusBar->setStyleSheet("color:red");
        ui->statusBar->showMessage("Unable to delete selected file(s)",2500);
    }
    //    qDebug()<<"Zain Arshad"<<endl;
}

void AppMainWindow::on_next_pushButton_clicked()    // see duplicate file(s) list
{
    ui->find_duplicate_files_stackedWidget->setCurrentIndex(1);

}

void AppMainWindow::moveToTrashOrRecycleBin(QFileInfo FileName)       // move delete file into trash or recycle bin
{
    if(QSysInfo::kernelType()=="linux")
    {
        QDateTime currentTime(QDateTime::currentDateTime());    // save System time

        QString trashFilePath=QDir::homePath()+"/.local/share/Trash/files/";    // trash file path contain delete files
        QString trashInfoPath=QDir::homePath()+"/.local/share/Trash/info/";     // trash info path contain delete files information

        // create file format for trash info file----- START
        QFile infoFile(trashInfoPath+FileName.completeBaseName()+"."+FileName.completeSuffix()+".trashinfo");     //filename+extension+.trashinfo //  create file information file in /.local/share/Trash/info/ folder

        infoFile.open(QIODevice::ReadWrite);

        QTextStream stream(&infoFile);         // for write data on open file

        stream<<"[Trash Info]"<<endl;
        stream<<"Path="+QString(QUrl::toPercentEncoding(FileName.absoluteFilePath(),"~_-./"))<<endl;     // convert path string in percentage decoding scheme string
        stream<<"DeletionDate="+currentTime.toString("yyyy-MM-dd")+"T"+currentTime.toString("hh:mm:ss")<<endl;      // get date and time format YYYY-MM-DDThh:mm:ss

        infoFile.close();

        // create info file format of trash file----- END

        QDir file;
        file.rename(FileName.absoluteFilePath(),trashFilePath+FileName.completeBaseName()+"."+FileName.completeSuffix());       // rename(file old path, file trash path)


    }
    else if(QSysInfo::kernelType()=="winnt")
    {

    }
}

// Find duplicate file tab functions END-----------------------------------------------




// Action Slots--------
void AppMainWindow::on_action_Clear_triggered()
{
    if(ui->App_tabWidget->currentIndex()==0)            // if work file verification tab
    {
        ui->filepath_lineEdit->setText("");

        ui->cal_checksum_lineEdit->setText("");

        ui->org_checksum_lineEdit->setText("");

        ui->verify_checksum_pushButton->setEnabled(false);

        ui->cal_checksum_pushButton->setEnabled(false);
    }

    else if(ui->App_tabWidget->currentIndex()==1)       // if work in file finding tab
    {

        ui->scan_files_pushButton->setEnabled(false);       // disable scan button, set default value

        ui->stop_scan_pushButton->setEnabled(false);        // disable stop button, set default value

        ui->next_pushButton->setEnabled(false);             // disable next button, set default value

        dir.setPath("");            // clear path, set default value

        fileTypes.clear();          // clear extension filter, set default value

        ui->duplicate_files_treeWidget->clear();

        ui->progress_file_label->setText("....");

        ui->video_checkBox->setChecked(false);

        ui->picture_checkBox->setChecked(false);

        ui->document_checkBox->setChecked(false);

        ui->music_checkBox->setChecked(false);

        ui->find_duplicate_files_stackedWidget->setCurrentIndex(0);
    }

}

void AppMainWindow::on_action_Find_Duplicates_triggered()
{
//    if(ui->App_tabWidget->currentIndex()==1&&ui->scan_files_pushButton->isEnabled())
//    {
        on_scan_files_pushButton_clicked();
//    }
}

void AppMainWindow::on_action_Check_triggered()
{
    if(ui->App_tabWidget->currentIndex()==0)            // work if file verification tab selected
    {
        on_cal_checksum_pushButton_clicked();
    }
}

void AppMainWindow::on_action_Verify_triggered()
{
    if(ui->App_tabWidget->currentIndex()==0&&ui->verify_checksum_pushButton->isEnabled())            // if work in file verification tab
    {
        on_verify_checksum_pushButton_clicked();
    }
}

void AppMainWindow::on_action_Exit_triggered()
{
    QMessageBox::StandardButton button;

    button=QMessageBox::warning(this,"","Are you sure you want to close prgram",QMessageBox::Yes|QMessageBox::No);

    if(button==QMessageBox::Yes)        // is select " Yes " button
    {
          QApplication::quit();
    }
}

void AppMainWindow::on_action_About_triggered()
{
    QMessageBox::information(this,"About","1.0 release \nDeveloped by: Zain Arshad\nusing Qt(Community)");
}

void AppMainWindow::on_action_Delete_files_triggered()
{
    if(ui->App_tabWidget->currentIndex()==1&&ui->find_duplicate_files_stackedWidget->currentIndex()==1&&ui->delete_file_pushButton->isEnabled())
    {
        on_delete_file_pushButton_clicked();
    }
}


//-----------------------------------------------------------










