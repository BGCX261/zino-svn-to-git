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
#ifndef ZINO_H
#define ZINO_H

//QT Includes
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
//Local Includes
#include <ui_zinobase.h>
#include <questionwidget.h>

/**
@author Tim Sutton
*/
class Zino : public QMainWindow, private Ui::ZinoBase
{
  Q_OBJECT
  public:
    Zino(QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~Zino();
    public slots:
      void on_pbnFilter_clicked();
    void on_toolCancelAdd_clicked();
    void on_toolAdd_clicked();
    void on_cboSortOrder_currentIndexChanged(int theIndex);
    private slots:
      void print();
    void quit();
    void closeEvent(QCloseEvent *thepEvent);
    void resizeEvent ( QResizeEvent * event );
    void remove();
    void insert();
    void add();
    void about();
  private:
    void clearFilter();
    void readSettings();
    void writeSettings();
    void hideNotice();
    void showNotice(QString theNotice, QSqlError theError);
    void showNotice(QString theNotice);
    void showNotice(); 
    // properties
    QSqlDatabase mDb;
    QSqlTableModel  *mpTableModel;
    QuestionWidget * mpQuestion;
    QString mLang1Name, mLang2Name;
    QSqlQuery * mpQuery;
};

#endif
