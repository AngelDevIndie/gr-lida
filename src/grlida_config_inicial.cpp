/*
 *
 * GR-lida by Monthy
 *
 * This file is part of GR-lida is a Frontend for DOSBox, ScummVM and VDMSound
 * Copyright (C) 2006  Pedro A. Garcia Rosado Aka Monthy
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 *
**/

#include "grlida_config_inicial.h"

frmConfigInicial::frmConfigInicial(QDialog *parent, Qt::WFlags flags)
    : QDialog( parent, flags )
{
	ui.setupUi(this);

	connect( ui.btnOk     , SIGNAL( clicked() ), this, SLOT( on_btnOk()     ) );
	connect( ui.btnDirDbx , SIGNAL( clicked() ), this, SLOT( on_btnDirDbx() ) );
	connect( ui.btnDirSvm , SIGNAL( clicked() ), this, SLOT( on_btnDirSvm() ) );
	connect( ui.cbxIdioma, SIGNAL( activated(const QString &) ), this, SLOT( on_setLanguage(const QString &) ) );

	stHomeDir     = QDir::homePath()+"/.gr-lida/";		// directorio de trabajo del GR-lida
//	stHomeDir     = QDir::currentPath()+"/";	// directorio de trabajo del GR-lida
//	stHomeDir     = "./";								// directorio de trabajo del GR-lida

	QSettings settings( stHomeDir+"GR-lida.conf", QSettings::IniFormat ); 
	settings.beginGroup("OpcGeneral");
	ui.chkConfini_1->setChecked(settings.value("DOSBoxDisp"    , "false").toBool()   );
	ui.btnDirDbx->setEnabled( settings.value("DOSBoxDisp"      , "false").toBool()   );
	ui.txtDirDbx->setEnabled( settings.value("DOSBoxDisp"      , "false").toBool()   );
	ui.txtDirDbx->setText( settings.value("DirDOSBox"          , ""     ).toString() );
	ui.chkConfini_2->setChecked(settings.value("ScummVMDisp"   , "false").toBool()   );
	ui.btnDirSvm->setEnabled( settings.value("ScummVMDisp"     , "false").toBool()   );
	ui.txtDirSvm->setEnabled( settings.value("ScummVMDisp"     , "false").toBool()   );
	ui.txtDirSvm->setText( settings.value("DirScummVM"         , ""     ).toString() );
	ui.chkConfini_3->setChecked( settings.value("VDMSoundDisp" , "false").toBool()   );
	IdiomaSelect = settings.value("IdiomaSelect", "es_ES" ).toString();
	IdiomaExterno = settings.value("IdiomaExterno" , "false").toBool();

	settings.endGroup();
	
	if(IdiomaExterno)
		fGrl.CargarIdiomasCombo( stHomeDir + "idiomas/", ui.cbxIdioma );	
	else
		fGrl.CargarIdiomasCombo( ":/idiomas/", ui.cbxIdioma );

	ui.cbxIdioma->setCurrentIndex( ui.cbxIdioma->findText(IdiomaSelect, Qt::MatchContains) ); //
	on_setLanguage( ui.cbxIdioma->currentText() );
	
	#ifdef Q_OS_WIN32
		ui.chkConfini_3->setEnabled(true);
	#else
		ui.chkConfini_3->setEnabled(false);
	#endif
	
// centra la aplicacion en el escritorio
	QDesktopWidget *desktop = qApp->desktop();
	const QRect rect = desktop->availableGeometry( desktop->primaryScreen() );
	int left = ( rect.width() - width() ) / 2;
	int top = ( rect.height() - height() ) / 2;
	setGeometry( left, top, width(), height() );
}

frmConfigInicial::~frmConfigInicial(){}

void frmConfigInicial::on_setLanguage(const QString txt_locale)
{
	QStringList parts = txt_locale.split(" - ");

	if(IdiomaExterno)
		translator.load( stHomeDir + "idiomas/gr-lida_" + parts.value(1)+".qm" );
	else
		translator.load(":/idiomas/gr-lida_" + parts.value(1)+".qm" );
	
	qApp->installTranslator(&translator);

	IdiomaSelect = parts.value(1);
	ui.retranslateUi(this);
}

void frmConfigInicial::closeEvent( QCloseEvent *e )
{
	on_btnOk();
	e->accept();
}

void frmConfigInicial::on_btnOk(){
	QSettings settings( stHomeDir + "GR-lida.conf", QSettings::IniFormat ); 

	QString stDirDbx, stDirSvm;
	if(ui.chkConfini_1->isChecked())
		stDirDbx = ui.txtDirDbx->text();
	else
		stDirDbx = "";

	if(ui.chkConfini_2->isChecked())
		stDirSvm = ui.txtDirSvm->text();
	else
		stDirSvm = "";

	settings.beginGroup("OpcGeneral");
	settings.setValue("DOSBoxDisp"   , ui.chkConfini_1->isChecked() );
	settings.setValue("ScummVMDisp"  , ui.chkConfini_2->isChecked() );
	settings.setValue("VDMSoundDisp" , ui.chkConfini_3->isChecked() );
	settings.setValue("Primeravez"   , ui.chkConfini_4->isChecked() );
	settings.setValue("DirDOSBox"    , stDirDbx						);
	settings.setValue("DirScummVM"   , stDirSvm						);
	settings.setValue("IdiomaSelect" , IdiomaSelect );
	settings.endGroup();
	
	on_setLanguage( ui.cbxIdioma->currentText() );
	
	QDialog::accept();
}

void frmConfigInicial::on_btnDirDbx()
{
	ui.txtDirDbx->setText( fGrl.VentanaAbrirArchivos( tr("Selecciona el Ejecutable del DOSBox"), stHomeDir, ui.txtDirDbx->text(), tr("Todos los archivo (*)"), 0, false) );
}

void frmConfigInicial::on_btnDirSvm()
{
	ui.txtDirSvm->setText( fGrl.VentanaAbrirArchivos( tr("Selecciona el Ejecutable del ScummVM"), stHomeDir, ui.txtDirSvm->text(), tr("Todos los archivo (*)"), 0, false) );
}
