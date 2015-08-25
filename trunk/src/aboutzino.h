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
#ifndef ABOUTZINO_H
#define ABOUTZINO_H

//QT Includes
#include <QDialog>
//Local Includes
#include <ui_aboutzinobase.h>


/**
@author Tim Sutton
*/
class AboutZino : public QDialog, private Ui::AboutZinoBase
{
	Q_OBJECT
	public:
		AboutZino(QWidget* parent = 0, Qt::WFlags fl = 0 );
		~AboutZino();
	public slots:
	protected:
		private slots:
	private:
		void readSettings();
		void writeSettings();
		// properties
};

#endif //ABOUTZINO
