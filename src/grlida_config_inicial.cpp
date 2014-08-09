/*
 *
 * GR-lida by Monthy
 *
 * This file is part of GR-lida is a Frontend for DOSBox, ScummVM and VDMSound
 * Copyright (C) 2006-2014 Pedro A. Garcia Rosado Aka Monthy
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 *
**/

#include "grlida_config_inicial.h"
#include "ui_config_inicial.h"

frmConfigInicial::frmConfigInicial(stGrlCfg m_cfg, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::frmConfigInicial)
{
	ui->setupUi(this);
	fGrl = new Funciones;

	grlCfg = m_cfg;

	grlDir.Home    = fGrl->GRlidaHomePath();
	grlDir.Idiomas = grlDir.Home +"idiomas/";

	fGrl->setTheme(grlCfg.NameDirTheme);
	fGrl->setIdioma(grlCfg.IdiomaSelect);

	cargarConfig();
	setTheme();

// centra la aplicacion en el escritorio
	this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(), qApp->desktop()->availableGeometry()));
}

frmConfigInicial::~frmConfigInicial()
{
	delete fGrl;
	delete ui;
}

void frmConfigInicial::cargarConfig()
{
	fGrl->cargarIdiomasComboBox(ui->cbxIdioma, ":/idiomas/");
	fGrl->cargarIdiomasComboBox(ui->cbxIdioma, grlDir.Idiomas, false);

	int idx = ui->cbxIdioma->findData( grlCfg.IdiomaSelect +".qm", Qt::UserRole, Qt::MatchEndsWith);
	ui->cbxIdioma->setCurrentIndex( idx );
	emit on_cbxIdioma_activated( idx );

	ui->chkDOSBoxDisp->setChecked( grlCfg.DOSBoxDisp );
	ui->txtDbxPath->setText( grlCfg.DirDOSBox );
	ui->chkScummVMDisp->setChecked( grlCfg.ScummVMDisp );
	ui->txtSvmPath->setText( grlCfg.DirScummVM );

	ui->chkVDMSoundDisp->setChecked( grlCfg.VDMSoundDisp );
	ui->chkShowNext->setChecked( grlCfg.Primeravez );

	#ifdef Q_OS_WIN32
		ui->chkVDMSoundDisp->setEnabled(true);
	#else
		ui->chkVDMSoundDisp->setEnabled(false);
	#endif
}

void frmConfigInicial::setTheme()
{
	ui->btnOk->setIcon( QIcon(fGrl->Theme() +"img16/aplicar.png") );
	ui->btnDbxPath->setIcon( QIcon(fGrl->Theme() +"img16/carpeta_1.png") );
	ui->btnDbxPath_find->setIcon( QIcon(fGrl->Theme() +"img16/zoom.png") );
	ui->btnDbxPath_clear->setIcon( QIcon(fGrl->Theme() +"img16/limpiar.png") );
	ui->btnSvmPath->setIcon( QIcon(fGrl->Theme() +"img16/carpeta_1.png") );
	ui->btnSvmPath_find->setIcon( QIcon(fGrl->Theme() +"img16/zoom.png") );
	ui->btnSvmPath_clear->setIcon( QIcon(fGrl->Theme() +"img16/limpiar.png") );

	if( grlCfg.font_usar )
		this->setStyleSheet(fGrl->myStyleSheet() +"*{font-family:\""+ grlCfg.font_family +"\";font-size:"+ grlCfg.font_size +"pt;}");
	else
		this->setStyleSheet(fGrl->myStyleSheet());

	this->setStyle(QStyleFactory::create(grlCfg.Style));

	if( grlCfg.StylePalette )
		this->setPalette(QApplication::style()->standardPalette());
	else
		this->setPalette(QApplication::palette());
}

void frmConfigInicial::on_cbxIdioma_activated(int index)
{
	if( index > -1 )
	{
		QString idioma = ui->cbxIdioma->itemData(index).toString();
		translator.load( idioma );

		idioma.remove(grlDir.Idiomas +"gr-lida_");
		idioma.remove(":/idiomas/gr-lida_");
		idioma.remove(".qm");

		grlCfg.IdiomaSelect = idioma;
	} else {
		grlCfg.IdiomaSelect = "es_ES";
		translator.load(":/idiomas/gr-lida_"+ grlCfg.IdiomaSelect +".qm");
	}

	fGrl->setIdioma(grlCfg.IdiomaSelect);

	qApp->installTranslator(&translator);
	ui->retranslateUi(this);
}

void frmConfigInicial::on_btnDbxPath_clicked()
{
	QString archivo = fGrl->ventanaAbrirArchivos( tr("Selecciona el ejecutable del DOSBox"), grlCfg.Dbx_path, "", "DOSBox (dosbox.exe dosbox);;"+ tr("Todos los archivo") +" (*)");

	if( !archivo.isEmpty() )
	{
		stFileInfo f_info = fGrl->getInfoFile( archivo );
		if( f_info.Exists )
		{
			ui->txtDbxPath->setText( fGrl->setDirRelative(archivo) );
			grlCfg.Dbx_path = fGrl->setDirRelative(f_info.Path);
		}
	}

}

void frmConfigInicial::on_btnDbxPath_find_clicked()
{
#ifdef Q_OS_WIN32
	QStringList lista_drivers;
	QFileInfoList list_drives = QDir::drives();
	lista_drivers.clear();
	for (int i = 0; i < list_drives.size(); ++i)
		lista_drivers.insert(i, QDir::toNativeSeparators(list_drives.at(i).absoluteFilePath()));

	bool ok = false;
	QString letra_drive = QInputDialog::getItem(this, tr("Buscar") +" DOSBox", tr("Selecciona la letra de la unidad:"), lista_drivers, 0, false, &ok);
	if( ok && !letra_drive.isEmpty() )
		ui->txtDbxPath->setText( fGrl->setDirRelative( fGrl->getFindFile(letra_drive, "dosbox.exe") ) );
#else
	#ifdef Q_OS_MAC
		ui->txtDbxPath->setText( fGrl->getFindFile("/", "dosbox") );
	#else
		ui->txtDbxPath->setText( fGrl->getFindFile("/usr/bin/", "dosbox") );
	#endif
#endif
}

void frmConfigInicial::on_btnDbxPath_clear_clicked()
{
	ui->txtDbxPath->clear();
}

void frmConfigInicial::on_btnSvmPath_clicked()
{
	QString archivo = fGrl->ventanaAbrirArchivos( tr("Selecciona el ejecutable del ScummVM"), grlCfg.Svm_path, "", "ScummVM (scummvm.exe scummvm);;"+ tr("Todos los archivo") +" (*)");

	if( !archivo.isEmpty() )
	{
		stFileInfo f_info = fGrl->getInfoFile( archivo );
		if( f_info.Exists )
		{
			ui->txtSvmPath->setText( fGrl->setDirRelative(archivo) );
			grlCfg.Svm_path = fGrl->setDirRelative(f_info.Path);
		}
	}
}

void frmConfigInicial::on_btnSvmPath_find_clicked()
{
#ifdef Q_OS_WIN32
	QStringList lista_drivers;
	QFileInfoList list_drives = QDir::drives();
	lista_drivers.clear();
	for (int i = 0; i < list_drives.size(); ++i)
		lista_drivers.insert(i, QDir::toNativeSeparators(list_drives.at(i).absoluteFilePath()));

	bool ok = false;
	QString letra_drive = QInputDialog::getItem(this, tr("Buscar") +" ScummVM", tr("Selecciona la letra de la unidad:"), lista_drivers, 0, false, &ok);
	if( ok && !letra_drive.isEmpty() )
		ui->txtSvmPath->setText( fGrl->setDirRelative( fGrl->getFindFile(letra_drive, "scummvm.exe") ) );
#else
	#ifdef Q_OS_MAC
		ui->txtSvmPath->setText( fGrl->getFindFile("/", "scummvm") );
	#else
		ui->txtSvmPath->setText( fGrl->getFindFile("/usr/games/", "scummvm") );
	#endif
#endif
}

void frmConfigInicial::on_btnSvmPath_clear_clicked()
{
	ui->txtSvmPath->clear();
}

void frmConfigInicial::on_btnOk_clicked()
{
	grlCfg.DOSBoxDisp   = ui->chkDOSBoxDisp->isChecked();
	grlCfg.DirDOSBox    = ui->txtDbxPath->text();
	grlCfg.ScummVMDisp  = ui->chkScummVMDisp->isChecked();
	grlCfg.DirScummVM   = ui->txtSvmPath->text();
	grlCfg.VDMSoundDisp = ui->chkVDMSoundDisp->isChecked();
	grlCfg.Primeravez   = ui->chkShowNext->isChecked();

	QDialog::accept();
}
