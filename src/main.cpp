/**
 * This file is a part of Qtpfsgui package.
 * ---------------------------------------------------------------------- 
 * Copyright (C) 2006,2007 Giuseppe Rota
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * ---------------------------------------------------------------------- 
 *
 * @author Giuseppe Rota <grota@users.sourceforge.net>
 */

#include <QApplication>
#include <QLocale>
#include <QTranslator>
// #include <QDir>
// #include <QDebug>
#include "maingui_impl.h"

int main( int argc, char ** argv )
{
	QApplication a( argc, argv );
// 	qDebug() << "QDir::currentPath()=" << QDir::currentPath();
// 	qDebug() << "QCoreApplication::applicationDirPath()=" << QCoreApplication::applicationDirPath();
	QString locale = QLocale::system().name();
	QTranslator translator;
	translator.load(QString("lang_") + locale, I18NDIR);
	qDebug( I18NDIR );
	a.installTranslator(&translator);
	MainGui w;
	a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
	w.show();
	return a.exec();
}
