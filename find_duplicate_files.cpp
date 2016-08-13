#include "find_duplicate_files.h"

findDuplicateFiles::findDuplicateFiles()
{
    //nonDuplicateFileCount=0;
    //duplicateFileCount=0;
    stopProgram=false;
}

void findDuplicateFiles::setFileAttribute(const QDir &path, const QStringList &fileTypes, Ui_AppMainWindow *ui) // ui contain all ui components reference
{
    this->path=path;
    this->fileType=fileTypes;
    this->ui=ui;
}


void findDuplicateFiles::run()  // overriding Qthread run()
{
     findDuplicate(path,fileType,ui);
     exec();
}

void findDuplicateFiles::stop() // overriding Qthread stop()
{
    stopProgram=true;
}

void findDuplicateFiles::findDuplicate(const QDir &dir, const QStringList &fileTypes,Ui_AppMainWindow *ui)      // ui contain all ui components reference
{
    QDirIterator fileIterator(dir.path(),fileTypes,QDir::Files,QDirIterator::Subdirectories);    // QDir::Files is QDir filter
                                                                                           // QDir::Files for List files.


                                                                                           //QDirIterator::Subdirectories
                                                                                          //  List entries inside all subdirectories as well.

    duplicateFilesRecord.clear();       // clear duplicate record

    nonDuplicateFilesRecord.clear();    // clear non duplicate record

    myQFileInfo singleFileRecord;            // save record of single file
    file_verifier fileCheckSum;            // hold file checksum
    QString fileCheckSumString;            // save file check sum in array
    int fileSize;                      // save file size
    QString fileChecksum_plus_fileSize="";     // hash code key, key = checksum+size;

    QPixmap icon;         // for save "progress_degree_0.png" to "progress_degree_330.png"
    int fileCount=0;      // for rotate progress

    int fileGroupNumber=0;  // duplicate files group number

    stopProgram=false;      // by default stop program flag is false

    while(fileIterator.hasNext())       // check is current directory contain subdirectory or files
    {
          singleFileRecord.setFile(fileIterator.next());                           // save single file information in singleFileRecord

          if(stopProgram)   // check process end condition
          {
              break;
          }

          if(singleFileRecord.isFile())
          {


              fileCheckSum.setFileName(singleFileRecord.absoluteFilePath(),0);         // set file name with path, 0 for MD4SUM algorithum
              fileCheckSumString=fileCheckSum.getCheckSum();          // get file checksum and save fileCheckSumString
              fileSize=singleFileRecord.size();           // get file size and save in fileSize;

              fileChecksum_plus_fileSize=fileCheckSumString+" "+QString::number(fileSize);       // Hash key file checksum+ file size, QString::number convert int into QString

              // detect file is duplicate or not duplicate, if not duplicate the add in "nonDuplicateFilesRecord" QHash as key. Qhash also contain non duplicate keys.
              if(!nonDuplicateFilesRecord.contains(fileChecksum_plus_fileSize))
              {

                  nonDuplicateFilesRecord.insert(fileChecksum_plus_fileSize,singleFileRecord);        // add non duplicate file checksum+size as hash key, non duplicate file record QFileInfo as value in HASH record;

                  //nonDuplicateFileCount++;
              }
              else    // add in duplicateFilesRecord and duplicateFilesStackWidget if duplicate
              {
                  if(duplicateFilesRecord.count(fileChecksum_plus_fileSize)==0)   // this condition detect is first duplicate file detect, add one copy extra in list as 1st original copy, its takes from non duplicate record
                  {
                      fileGroupNumber++;    // allocate group no. for certain duplicate file

                      if(nonDuplicateFilesRecord.contains(fileChecksum_plus_fileSize))  // if nonDuplicate contain filechecksum + size
                      {
                            myQFileInfo tempFile=nonDuplicateFilesRecord.value(fileChecksum_plus_fileSize);  // create new temp QfileInfo variable and copy value from nonDuplicate
                            tempFile.setGroupNo(fileGroupNumber);       // set Group number

                            nonDuplicateFilesRecord.insert(fileChecksum_plus_fileSize,tempFile);  // then replace nonDuplicate record with temp( change file group number)

                      }
                      setDuplicateFilesRecordOnTree(nonDuplicateFilesRecord.value(fileChecksum_plus_fileSize).completeBaseName(),nonDuplicateFilesRecord.value(fileChecksum_plus_fileSize).getGroupNo(),nonDuplicateFilesRecord.value(fileChecksum_plus_fileSize).absoluteFilePath(),QString::number(nonDuplicateFilesRecord.value(fileChecksum_plus_fileSize).size()/1024)+" Kbytes",0); // file name, file group , path, size, Qtree entery text color( 0 | 1 ) 0 for main enrty, 1 for sub entry
                  }

                  //if(duplicateFilesRecord.contains(fileChecksum_plus_fileSize))
                  //{
                      singleFileRecord.setGroupNo(nonDuplicateFilesRecord.value(fileChecksum_plus_fileSize).getGroupNo());  // get group number from nonDuplicate record and set group number of singleFileRecord
                      setDuplicateFilesRecordOnTree(singleFileRecord.completeBaseName(),singleFileRecord.getGroupNo(),singleFileRecord.absoluteFilePath(),QString::number(singleFileRecord.size()/1024)+" Kbytes",1); // file name, file group, path, size, Qtree entry text color
                  //}
                    duplicateFilesRecord.append(fileChecksum_plus_fileSize);           // add duplicate file record fileCheckSum + fileSize in QList;
                  //duplicateFileCount++;
              }

              //ui->progress_file_label->repaint();     // refresh "progressFileLabel" text
              ui->progress_file_label->setText(singleFileRecord.absoluteFilePath());               // set current scanning text ProgressBar text


              switch (fileCount)            // load next degree image after 15 count in "progressIcon"
              {
              case 0:
                  icon.load((":/images/progress_degree_0.png"));
                  //ui->progress_icon->repaint();
                  ui->progress_icon->setPixmap(icon);
                  break;
              case 10:
                  icon.load((":/images/progress_degree_30.png"));
                  //ui->progress_icon->repaint();
                  ui->progress_icon->setPixmap(icon);
                  break;
              case 20:
                  icon.load((":/images/progress_degree_60.png"));
                  //ui->progress_icon->repaint();
                  ui->progress_icon->setPixmap(icon);
                  break;
              case 30:
                  icon.load((":/images/progress_degree_90.png"));
                  //ui->progress_icon->repaint();
                  ui->progress_icon->setPixmap(icon);
                  break;
              case 40:
                  icon.load((":/images/progress_degree_120.png"));
                  //ui->progress_icon->repaint();
                  ui->progress_icon->setPixmap(icon);
                  break;
              case 50:
                  icon.load((":/images/progress_degree_150.png"));
                  //ui->progress_icon->repaint();
                  ui->progress_icon->setPixmap(icon);
                  break;
              case 60:
                  icon.load((":/images/progress_degree_180.png"));
                  //ui->progress_icon->repaint();
                  ui->progress_icon->setPixmap(icon);
                  break;
              case 70:
                  icon.load((":/images/progress_degree_210.png"));
                  //ui->progress_icon->repaint();
                  ui->progress_icon->setPixmap(icon);
                  break;
              case 80:
                  icon.load((":/images/progress_degree_240.png"));
                  //ui->progress_icon->repaint();
                  ui->progress_icon->setPixmap(icon);
                  break;
              case 90:
                  icon.load((":/images/progress_degree_270.png"));
                  //ui->progress_icon->repaint();
                  ui->progress_icon->setPixmap(icon);
                  break;
              case 100:
                  icon.load((":/images/progress_degree_300.png"));
                 // ui->progress_icon->repaint();
                  ui->progress_icon->setPixmap(icon);
                  break;
              case 110:
                  icon.load((":/images/progress_degree_330.png"));
                  //ui->progress_icon->repaint();
                  ui->progress_icon->setPixmap(icon);
                  break;

              }


              fileCount++;

              if(fileCount>120)     // reset "fileCount" after 120
                  fileCount=0;

          }

    }

    nonDuplicateFilesRecord.clear();            // clear all item of non Duplicate file record
    if(stopProgram)
    {
         ui->progress_file_label->setText("User Intrrupt progress. Click next to delete duplicate files");

    }
    else
    {
        ui->progress_file_label->setText("Finish. Click next to delete duplicate files");
    }
    ui->duplicate_files_treeWidget->sortItems(1,Qt::AscendingOrder);    // sort file in files groups assending order
    ui->next_pushButton->setEnabled(true);   // enable next button after processing

}

void findDuplicateFiles::setDuplicateFilesRecordOnTree(const QString& fileName,int groupNo, const QString& filePath, const QString& fileSize, int entryColor)
{
    QTreeWidgetItem *fileTreeItem=new QTreeWidgetItem(ui->duplicate_files_treeWidget);

   if(entryColor==0)        // font setting for main files
   {
       // 1st column for checkable button with filename
       // for adding checkable button in item list
       //fileTreeItem->setFlags(fileTreeItem->flags() | Qt::ItemIsUserCheckable);
       fileTreeItem->setCheckState(0,Qt::Unchecked);

       // set style of 1st column
       fileTreeItem->setForeground(0, QBrush(QColor(Qt::white)));   // set font color white
       fileTreeItem->setBackgroundColor(0,QColor(132, 191, 79));    // set background rgb=(132, 191, 79)
       fileTreeItem->setToolTip(0,"Delete with care!\n"+fileName);           // set tool tip
       fileTreeItem->setText(0,fileName);     // set file name in 1st column of tree widget


       // 2nd column for file group
       fileTreeItem->setForeground(1, QBrush(QColor(Qt::white)));   // set font color white
       fileTreeItem->setBackgroundColor(1,QColor(132, 191, 79));    // set background rgb=(132, 191, 79)
       fileTreeItem->setFont(1,QFont("System", 10, QFont::Bold));   // set font style, system font prefered
       fileTreeItem->setToolTip(1,"Delete with care!\nGroup "+QString::number(groupNo)+" contain same files");           // set tool tip
       fileTreeItem->setData(1,Qt::DisplayRole,groupNo);        // set file group in 2nd column of tree widget, during sorting show data like 1,2,3.. not 1,10,2,3

       // 3rd column for file path
       fileTreeItem->setForeground(2, QBrush(QColor(Qt::white)));   // set font color white
       fileTreeItem->setBackgroundColor(2,QColor(132, 191, 79));    // set background rgb=(132, 191, 79)
       fileTreeItem->setFont(2,QFont("System", 10, QFont::Bold));   // set font style, system font prefered
       fileTreeItem->setToolTip(2,"Delete with care!\n"+filePath);           // set tool tip
       fileTreeItem->setText(2,filePath);     // set file path in 3rd column of tree widget

       // 4th column for file size
       fileTreeItem->setForeground(3, QBrush(QColor(Qt::white)));   // set font color white
       fileTreeItem->setBackgroundColor(3,QColor(132, 191, 79));    // set background rgb=(132, 191, 79)
       fileTreeItem->setFont(3,QFont("System", 10, QFont::Bold));   // set font style, system font prefered
       fileTreeItem->setToolTip(3,"Delete with care!\n"+fileSize);           // set tool tip
       fileTreeItem->setText(3,fileSize);     // set file size in 4th colum of tree widget

       //fileTreeItem->setFlags(Qt::ItemIsSelectable);        // disable selectable property for main file


   }
   else if(entryColor==1)   // font setting for duplicate files
   {
       // 1st column for checkable button with filename
       // for adding checkable button in item list
       //fileTreeItem->setFlags(fileTreeItem->flags() | Qt::ItemIsUserCheckable);
       fileTreeItem->setCheckState(0,Qt::Unchecked);
       fileTreeItem->setText(0,fileName);     // set file name in 1st column of tree widget
       fileTreeItem->setToolTip(0,fileName);           // set tool tip

       // 2nd column for file group
       fileTreeItem->setForeground(1, QBrush(QColor(Qt::black)));   // set font color black
       fileTreeItem->setBackgroundColor(1,"white");                 // set background color white
       fileTreeItem->setData(1,Qt::DisplayRole,groupNo);         // set file group in 2nd column of tree widget, during sorting show data like 1,2,3.. not 1,10,2,3
       fileTreeItem->setToolTip(1,"Group "+QString::number(groupNo)+" contain same files");           // set tool tip

       // 3rd column for file path
       fileTreeItem->setForeground(2, QBrush(QColor(Qt::black)));   // set font color black
       fileTreeItem->setBackgroundColor(2,"white");                 // set background color white
       fileTreeItem->setText(2,filePath);     // set file path in 3rd column of tree widget
       fileTreeItem->setToolTip(2,filePath);           // set tool tip

       // 4th column for file size
       fileTreeItem->setForeground(3, QBrush(QColor(Qt::black)));   // set font color black
       fileTreeItem->setBackgroundColor(3,"white");                 // set background color white
       fileTreeItem->setText(3,fileSize);     // set file size in 4th colum of tree widget
       fileTreeItem->setToolTip(3,fileSize);           // set tool tip

   }


}



