﻿//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.

//Author:Ehsan Tork journalehsan@gmail.com
#include "form1.h"
#include "ui_form1.h"
#include <QtGui>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDataStream>
#include <QTextCodec>
#include <QDir>
#include <QFontDialog>
#include <QShortcut>
#include "about.h"

//QString GlobalFileName;
//QString GlobalFileName;
//QString GlobalFileName;
QString GlobalFileName;


# define utf8(str) ConvertToUTF8(L##str)
const char * ConvertToUTF8(const wchar_t * pStr) {
     static char szBuf[1024];
     WideCharToMultiByte(CP_UTF8, 0, pStr, -1, szBuf, sizeof(szBuf), NULL, NULL);
     return szBuf;
 }

Form1::Form1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form1)
{
    ui->setupUi(this);
    //File Data variable
    QString fileData;
    //setup widgets
    ui->cmbcodec->addItem(utf8("Windows Arabic - نا سالم"));
    ui->cmbcodec->addItem(utf8("UTF-8 - سالم"));
    //set drop on textbox -> false
    ui->textEdit->setAcceptDrops(false);
    ui->btnAbout->setIcon(QIcon::fromTheme("help-about"));
    ui->btnOpen->setIcon(QIcon::fromTheme("document-open"));
    ui->btnSave->setIcon(QIcon::fromTheme("document-save"));
    ui->btnFont->setIcon(QIcon::fromTheme("preferences-desktop-font"));
    //set tooltips
    ui->btnAbout->setToolTip("<p> <b> About... </b></p> Persian Subtitle Fixer <i>0.3-TP1</i>");
    ui->btnOpen->setToolTip("<p> <b> Open Subtitle file </b></p>  <i>Open  Dialog </i>");
    ui->btnSave->setToolTip("<b> <p> Save Fixed Subtitle file </b> </p> <i>Save Dialog</i>");
    ui->btnFont->setToolTip("<p> <b> Change Font </b> </p> Text Preview <i> Customize Font</i>");
    //set true -> drops on form
    setAcceptDrops(true);
    //FileName for Global Usage in app
    QString filePath = GlobalFileName;
    //open file after start if it opened from commandline
    if(QFile::exists(filePath)){
        fileData = openFile(filePath,true);
    }
    else
    {
        ui->textEdit->setPlainText("Drag SRT Subtitle Here"); //write default text for textbox at start when no file open
    }
    //check file dtat after open
    if(filePath != ""){
        ui->textEdit->setPlainText(fileData);
        QFileInfo fileInfo(filePath);
        QString fileName(fileInfo.fileName());
        this->setWindowTitle(fileName);
    }
    //keyboard shortcuts
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(close()));
    new QShortcut(QKeySequence(Qt::Key_Escape), this, SLOT(close()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_O), this, SLOT(on_btnOpen_clicked()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this, SLOT(on_btnSave_clicked()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), this, SLOT(on_btnFont_clicked()));
    //end of shortcuts
}
void setFileName(QString strFileName){
    GlobalFileName = strFileName;
}

Form1::~Form1()
{
    delete ui;
}
void Form1::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}
void Form1::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;

    QString strFileName = urls.first().toLocalFile();
    if (strFileName.isEmpty()){
        this->setWindowTitle("Persian Subtitle Fixer");
        return;
    }
   GlobalFileName = strFileName;
   bool optionOneSelected;
   if (ui->cmbcodec->currentIndex()==0) {
       optionOneSelected = true;}
   else{
       optionOneSelected = false;
   }
   //open file
   QString strPlainData = openFile(strFileName,optionOneSelected);
   ui->textEdit->setPlainText(strPlainData);
}
QString Form1::openFile(QString filePath,bool optionOneSelected){
    QFile flOpenedFileName(filePath);
         if (!flOpenedFileName.open(QIODevice::ReadOnly | QIODevice::Text)){
             return "";
         }
    QTextStream open(&flOpenedFileName);
    if (optionOneSelected == true){
        open.setCodec("windows-1256");
    }
    else{
        open.setCodec("utf-8");
    }
    QString filedata = open.readAll();
    flOpenedFileName.flush();
    flOpenedFileName.close();
    QFileInfo fileInfo(filePath);
    QString fileName(fileInfo.fileName());
    this->setWindowTitle(fileName);
    return filedata;
}
void Form1::on_btnOpen_clicked()
{
    QString filePath = GlobalFileName;
    QString homepath =  QDir::homePath();
        filePath= QFileDialog::getOpenFileName(this,"Open Windows-1256-Arabic SRT File:",homepath,"SRT File(*.srt);;AllFile(*.*)");
    GlobalFileName = filePath;
    //Change Unicode based on combobox
    bool optionOneSelected;
     if (ui->cmbcodec->currentIndex() == 0){
         optionOneSelected = true;
     }
     else{
         optionOneSelected = false;
     }
     if (filePath !="" || QFile::exists(filePath) ){
         QString FileData = openFile(filePath,optionOneSelected);
         //Display datat in Textbox
         ui->textEdit->setPlainText(FileData);
     }
     else{
         this->setWindowTitle("Persian Subtitle Fixer");
     }

}

void Form1::on_btnSave_clicked()
{
    this->setWindowTitle("Persian Subtitle Fixer");
    QString homepath =  GlobalFileName;
    if(GlobalFileName == ""){
        QMessageBox::critical(this,"Error", "Please open file first! \n You can't save befor open a file!");
        on_btnOpen_clicked();
    }
    else{
        QString filename =  QFileDialog::getSaveFileName(this, tr("Save File"),
                                                         homepath,
                                                         tr("Save SRT to UTF-8 (*.srt)"));
        QFile fileForDelete(filename); //check if file exist delete first then save file
        if (fileForDelete.exists())
            fileForDelete.remove();
        fileForDelete.close();
        //savedialog
        QFile filesave(filename);
        if (!filesave.open(QIODevice::ReadWrite | QIODevice::Text)){

        }
        else{
            QTextStream out(&filesave);
           QString text = ui->textEdit->toPlainText();
           out.setCodec("utf-8");
            out << text;
            filesave.close();
            filesave.flush();
        }
    }

}

void Form1::on_btnFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(
                    &ok, QFont("SansSerif", 13), this);

    if (ok)  {
        ui->textEdit->setFont(font);
    } else  {
        // the user canceled the dialog; font is set to the initial
    }
}

void Form1::on_btnAbout_clicked()
{
    about *w = new about;
            w->setAttribute(Qt::WA_DeleteOnClose);
            w->setWindowIcon(QIcon::fromTheme("help-about"));
            w->show();
}
