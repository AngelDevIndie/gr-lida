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

#include "grlida_addedit_montajes.h"

frmAddEditMontajes::frmAddEditMontajes(QHash<QString, QString> Montajes, QDialog *parent, Qt::WFlags flags)
    : QDialog( parent, flags )
{
	ui.setupUi(this);

	connect( ui.btnOk      , SIGNAL( clicked() ), this, SLOT( on_btnOk() ) );
	connect( ui.btnDirFile , SIGNAL( clicked() ), this, SLOT( on_DirFile() ) );

	stHomeDir = fGrl.GRlidaHomePath();		// directorio de trabajo del GR-lida

	ui.lb_Montaje_info->setText( "<b>-aspi</b> -- "+ tr("Fuerza el uso de la capa aspi. Sólo válido si montas un CD-ROM bajo los sistemas Windows con un ASPI-Layer.")+"<br>"+
		"<b>-ioctl</b> -- "+ tr("Fuerza el uso de los comandos ioctl. Sólo válido si montar un CD-ROM bajo un sistema operativo de Windows que lo soporten (Win2000/XP/NT).")+"<br>"+
		"<b>-noioctl</b> -- "+ tr("Fuerza el uso de las SDL para el CD-ROM. Válido para todos los sistemas.")+"<br>"+
		tr("Procura no montar la Raiz del sistema operativo: ejemplo en windows seria")+" <b style=\"color:#FF0000;\">C:\\</b> "+ tr("y en linux seria directamente")+ " <b style=\"color:#FF0000;\">/</b> ");

	ui.txtMontaje_path->setText( Montajes["path"] );
	ui.txtMontaje_label->setText( Montajes["label"] );
	ui.cbxMontaje_type_drive->setCurrentIndex( ui.cbxMontaje_type_drive->findText( Montajes["tipo_as"] ) );
	ui.cbxMontaje_letter->setCurrentIndex( ui.cbxMontaje_letter->findText( Montajes["letter"] ) );
	ui.cbxMontaje_cdrom->setCurrentIndex( ui.cbxMontaje_cdrom->findText( Montajes["indx_cd"] ) );
	ui.txtMontaje_opt_mount->setText( Montajes["opt_mount"] );
	ui.cbxMontaje_mode_cdrom->setCurrentIndex( ui.cbxMontaje_mode_cdrom->findText( Montajes["io_ctrl"], Qt::MatchContains ) );

// centra la ventana en el escritorio
	QDesktopWidget *desktop = qApp->desktop();
	const QRect rect = desktop->availableGeometry( desktop->primaryScreen() );
	int left = ( rect.width() - width() ) / 2;
	int top = ( rect.height() - height() ) / 2;
	setGeometry( left, top, width(), height() );
}

frmAddEditMontajes::~frmAddEditMontajes(){}
		
void frmAddEditMontajes::on_btnOk()
{
	DatosMontaje.clear();
	DatosMontaje["path"]  = ui.txtMontaje_path->text();						// directorio o iso
	DatosMontaje["label"] = ui.txtMontaje_label->text();					// etiqueta

	if ( ui.cbxMontaje_type_drive->currentText()!="" )
		DatosMontaje["tipo_as"] = ui.cbxMontaje_type_drive->currentText();	// tipo de montaje
	else DatosMontaje["tipo_as"] = "";

	if ( ui.cbxMontaje_letter->currentText()!="" )
		DatosMontaje["letter"] = ui.cbxMontaje_letter->currentText();		// letra de montaje
	else DatosMontaje["letter"] = "";

	if ( ui.cbxMontaje_cdrom->currentText()!="" )
		DatosMontaje["indx_cd"] = ui.cbxMontaje_cdrom->currentText();		// index de la unidad de cd-rom
	else DatosMontaje["indx_cd"] = "";

	DatosMontaje["opt_mount"] = ui.txtMontaje_opt_mount->text();			// opciones del cd-rom

	if ( ui.cbxMontaje_mode_cdrom->currentText()!="" )
		DatosMontaje["io_ctrl"] = ui.cbxMontaje_mode_cdrom->currentText();	// cd/dvd windows, w9x, linux
	else DatosMontaje["io_ctrl"] = "-noioctl";								// Forces use of the SDL

	QDialog::accept();
}

void frmAddEditMontajes::on_DirFile()
{
	int tipomontaje = ui.cbxMontaje_type_drive->currentIndex();
	if((tipomontaje==0)||(tipomontaje==1)||(tipomontaje==2))
		ui.txtMontaje_path->setText( fGrl.VentanaDirectorios( tr("Seleccionar un directorio."), stHomeDir, ui.txtMontaje_path->text() ));
	else
		ui.txtMontaje_path->setText( fGrl.VentanaAbrirArchivos( tr("Selecciona un archivo"), ui.txtMontaje_path->text(), ui.txtMontaje_path->text(), tr("Todos los archivo") + " (*)", 0, false) );
}
