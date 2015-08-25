/***************************************************************************
 *   Copyright (C) 2005 by Tim Sutton   *
 *   tim@linfiniti.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "zino.h"
#include "questionwidget.h"
#include "aboutzino.h"
#include "firstrun.h"
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QFileDialog>
#include <QSqlError>
#include <QStatusBar>
#include <QLineEdit>
#include <QFile>
#include <QDir>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QCloseEvent>
#include <QTableView>
#include <QSqlIndex>
#include <QHeaderView>

//next includes needed for rand
#include <stdlib.h>
#include <time.h>
  Zino::Zino(QWidget* parent, Qt::WFlags fl)
: QMainWindow(parent,fl)
{
  //required by Qt4 to initialise the ui
  setupUi(this);
  hideNotice();
  connect(actionPrint, SIGNAL(triggered()), this, SLOT(print()));
  connect(actionQuit, SIGNAL(triggered()), this, SLOT(quit()));
  connect(actionNew, SIGNAL(triggered()), this, SLOT(add()));
  connect(actionRemove, SIGNAL(triggered()), this, SLOT(remove()));
  connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
  connect(toolInsert,SIGNAL(clicked()),this,SLOT(insert()));

  mDb = QSqlDatabase::addDatabase("QSQLITE");
  QString myHomeDirPath = QDir::homePath();
  QString myDbFileName = myHomeDirPath + "/zino.db";
  if (!QFile::exists(myDbFileName))
  {
    //copy the template dir over from the qt resource pack
    QFile myFile(":/zinomaster.db");
    myFile.copy(myDbFileName);
  }
  //try our best to set the correct perms
  QFile::setPermissions(myDbFileName,QFile::WriteOwner | QFile::ReadOwner);
  //to do ad some test to ensure the file is writeable now
  mDb.setDatabaseName(myDbFileName);
  bool myFlag = mDb.open();
  if (myFlag) 
  {
    statusBar()->showMessage(tr("Database opened"));
    hideNotice();
  }
  else
  {
    showNotice("Failed to open the zino database - please contact the developers with this problem!");
  }
  mpTableModel = new QSqlTableModel(this,mDb);
  //Doesnt work for inserts when I try to use the view - though they work form sqlite cli
  //mpTableModel->setTable("vw_zino");
  mpTableModel->setTable("zino");
  //next two lines were suggest on the qt mailing list as a fix for update errros in mysql
  //QSqlQuery myQuery("PRAGMA read_uncommitted = 1");
  //myQuery.exec();
  //QSqlQuery myQuery("SELECT * FROM vw_zino");
  //mpTableModel->setQuery(myQuery);  
  qDebug("pkey: " + mpTableModel->primaryKey().name().toLocal8Bit());
  //TODO make user configureable which strategy below we use
  //requires programmtic call to submit all changes
  //mpTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
  //submits changes as each field is altered
  //mpTableModel->setEditStrategy(QSqlTableModel::OnFieldChange);
  //submit change when you leave the current row
  mpTableModel->setEditStrategy(QSqlTableModel::OnRowChange);
  mpTableModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Id"));
  mpTableModel->setHeaderData(1, Qt::Horizontal, QObject::tr("lang1"));
  mpTableModel->setHeaderData(2, Qt::Horizontal, QObject::tr("lang2"));
  mpTableModel->setHeaderData(3, Qt::Horizontal, QObject::tr("score"));
  mpTableModel->setHeaderData(4, Qt::Horizontal, QObject::tr("date"));
  tvDb->setModel(mpTableModel);
  mpTableModel->select();
  //qDebug("rec count: " + QString::number(mpTableModel->rowCount()).toLocal8Bit());

  //format the table a little
  tvDb->horizontalHeader()->hide();
  tvDb->verticalHeader()->hide();
  //first col is the db index id which we need for delete to work
  //but we dont want users to see...
  //so first we make it immune from resize events (interactive means it can only be
  //resized by the user
  tvDb->horizontalHeader()->setResizeMode(0,QHeaderView::Interactive);
  //and then we resize it to width 0
  tvDb->horizontalHeader()->resizeSection(0,0);
  tvDb->horizontalHeader()->setResizeMode(1,QHeaderView::Stretch);
  tvDb->horizontalHeader()->setResizeMode(2,QHeaderView::Stretch);
  tvDb->horizontalHeader()->resizeSection(3,0);
  tvDb->horizontalHeader()->resizeSection(4,0);

  QGridLayout *mypLayout = new QGridLayout(frameQuestion);;
  mpQuestion = new QuestionWidget(frameQuestion);
  mypLayout->addWidget(mpQuestion, 0, 0);
  grpAddEntry->setVisible(false);
  //do this last or risk segfaults!
  readSettings();
}

Zino::~Zino()
{
  //@TODO clean this up ! Mindlessly trying everything I can to release the db...
  delete mpTableModel;
  delete tvDb;
  mDb.close();
  QSqlDatabase::removeDatabase("QSQLITE");
  writeSettings();
}


void Zino::add()
{
  grpFiltering->setVisible(false);
  grpAddEntry->setVisible(true);
  leWord->setFocus();
}
void Zino::on_toolCancelAdd_clicked()
{
  grpAddEntry->setVisible(false);
  grpFiltering->setVisible(true);
  leWord->setText("");
  leTranslation->setText("");
}
void Zino::on_toolAdd_clicked()
{
  grpAddEntry->setVisible(true);
  grpFiltering->setVisible(false);
  clearFilter();
}
void Zino::insert()
{
  //get an empty sqlrecord that we can populate with our data
  //and then insert into the model
  QSqlRecord myRecord = mpTableModel->record();
  myRecord.setValue("lang1",leWord->text());
  myRecord.setValue("lang2",leTranslation->text());
  mpTableModel->database().transaction();
  bool myFlag = mpTableModel->insertRecord(tvDb->currentIndex().row(),myRecord);
  hideNotice();
  if (!myFlag) 
  {
    showNotice("A problem was encountered while adding the record\n", mpTableModel->lastError());
    return;
  }
  //myFlag = mpTableModel->submitAll();
  //if (!myFlag) 
  //{
  //  QString myError = mpTableModel->lastError().text();
  //  showNotice("A problem was encountered while submitting the adding record\n"+myError);
  //  return;
  //}
  myFlag = mpTableModel->database().commit();
  if (!myFlag) 
  {
    QString myError = mpTableModel->lastError().text();
    showNotice("A problem was encountered while committing the transaction when  adding the record\n", mpTableModel->lastError());
    return;
  }
  leWord->setText("");
  leTranslation->setText("");
  leWord->setFocus();
  mpTableModel->select();
  statusBar()->showMessage(tr("Record added"));
  return;
}
void Zino::remove()
{
  hideNotice();
  if (!tvDb->currentIndex().isValid())
  {
    showNotice(tr("To delete an entry you must first click on one of its words" ), mpTableModel->lastError());
  }
  else
  {
    mpTableModel->database().transaction();
    bool myFlag = mpTableModel->removeRows(tvDb->currentIndex().row(),1); 
    myFlag = mpTableModel->database().commit();
    //mpTableModel->submitAll();
    if (myFlag) 
    {
      hideNotice();
      mpTableModel->select();
      statusBar()->showMessage(tr("Entry removed"));
    }
    else
    {
      statusBar()->showMessage(tr("Entry removal failed"));
      showNotice(tr("Entry removal failed"), mpTableModel->lastError());
    }
  }
}
void Zino::readSettings()
{
  QSettings mySettings;
  QPoint pos = mySettings.value("mainwindow/pos", QPoint(200, 200)).toPoint();
  QSize size = mySettings.value("mainwindow/size", QSize(400, 400)).toSize();
  resize(size);
  move(pos);
  //check if either lang1 or lang2 is not in settings, run the first
  //run dialog
  if (mySettings.value("zino/lang1").toString().isEmpty() ||
      mySettings.value("zino/lang2").toString().isEmpty())
  {
    FirstRun myFirstRun;
    myFirstRun.exec();
  }
  mLang1Name = mySettings.value("zino/lang1").toString();
  mLang2Name = mySettings.value("zino/lang2").toString();
  //populate the lang selector combo
  cboSortOrder->clear();
  cboSortOrder->addItem(mLang1Name);
  cboSortOrder->addItem(mLang2Name);
}

void Zino::writeSettings()
{
  QSettings settings;
  settings.setValue("mainwindow/pos", pos());
  settings.setValue("mainwindow/size", size());
}

void Zino::on_cboSortOrder_currentIndexChanged (int theIndex)
{
  if (theIndex < 0) return; //remove unused parameter compiler warning
  //prevent segfault if we are still loading the form and the combo is unpopulated
  if (cboSortOrder->count()<1) return;

  if (cboSortOrder->currentText()==mLang2Name)
  {
    mpTableModel->setSort(2,Qt::AscendingOrder);
    statusBar()->showMessage(tr("Sorted to ") + mLang2Name + " - " + mLang1Name);
    mpTableModel->select();
  }
  else
  {
    mpTableModel->setSort(1,Qt::AscendingOrder);
    statusBar()->showMessage(tr("Sorted to ") + mLang1Name + " - " + mLang2Name);
    mpTableModel->select();
  }
}
void Zino::showNotice()
{
  QSqlError myError = mDb.lastError();
  tbNotice->setPlainText(myError.databaseText() + "\n" + myError.driverText());
  tbNotice->show();
}

void Zino::showNotice(QString theNotice, QSqlError theError)
{
  QString myErrorString = "DatabaseText : " + theError.databaseText() + "\n";
  myErrorString        += "DriverText   : " + theError.driverText() + "\n";
  myErrorString        += "Number       : " + QString::number(theError.number()) + "\n";
  myErrorString        += "ErrorType    : " + QString::number(theError.type()) + "\n";
  tbNotice->setPlainText( myErrorString + "\n" + theNotice);
  grpMessage->show();
}
void Zino::showNotice(QString theNotice)
{
  tbNotice->setPlainText(theNotice);
  grpMessage->show();
}
void Zino::hideNotice()
{
  tbNotice->setPlainText("");
  grpMessage->hide();
}

void Zino::resizeEvent ( QResizeEvent * theEvent )
{
  if (theEvent == NULL) return; //remove unused parameter compiler warning
  tvDb->horizontalHeader()->resizeSection(0,0);
  tvDb->horizontalHeader()->setResizeMode(1,QHeaderView::Stretch);
  tvDb->horizontalHeader()->setResizeMode(2,QHeaderView::Stretch);
  tvDb->horizontalHeader()->resizeSection(3,0);
  tvDb->horizontalHeader()->resizeSection(4,0);
}
void Zino::quit()
{
  close();
}
void Zino::closeEvent(QCloseEvent *thepEvent)
{
  thepEvent->setAccepted(true);
  writeSettings();
}
void Zino::on_pbnFilter_clicked()
{
  if (leFilter->text().isEmpty() || pbnFilter->text()==tr("Clear"))
  {
    clearFilter();
  }
  else
  {
    //set the filter
    QString myFilter = "lang1 like '%" + leFilter->text() + "%' or lang2 like '%" + leFilter->text() + "%'";
    mpTableModel->setFilter(myFilter);
    //showNotice(myFilter);
    hideNotice();
    pbnFilter->setText(tr("Clear"));
  }
}
void Zino::clearFilter()
{
  //clear the filter
  QString myFilter = "";
  mpTableModel->setFilter(myFilter);
  hideNotice();
  pbnFilter->setText(tr("Go"));
}

void Zino::print()
{

}
  
void Zino::about()
{
  AboutZino myAbout;
  myAbout.exec();
}
