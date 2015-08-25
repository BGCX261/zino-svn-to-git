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
#include "firstrun.h"
#include <QSettings>
#include <QLineEdit>

	FirstRun::FirstRun(QWidget* parent, Qt::WFlags fl)
: QDialog (parent,fl), FirstRunBase()
{
	//required by Qt4 to initialise the ui
	setupUi(this);
	readSettings();
}

FirstRun::~FirstRun()
{
	writeSettings();
}

void FirstRun::readSettings()
{
	QSettings mySettings;
	QPoint pos = mySettings.value("firstrun/pos", QPoint(200, 200)).toPoint();
	QSize size = mySettings.value("firstrun/size", QSize(400, 400)).toSize();
	resize(size);
	move(pos);
	leNewLanguage->setText(mySettings.value("zino/lang1","").toString());
	leMotherToungue->setText(mySettings.value("zino/lang2","").toString());
}

void FirstRun::writeSettings()
{
	QSettings mySettings;
	mySettings.setValue("firstrun/pos", pos());
	mySettings.setValue("firstrun/size", size());
	mySettings.setValue("zino/lang1",leNewLanguage->text());
	mySettings.setValue("zino/lang2",leMotherToungue->text());
}

void FirstRun::on_pbnOk_clicked()
{

	accept();
}
