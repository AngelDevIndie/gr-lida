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

#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <QtCore>
#include <QtGui>

class Funciones 
{
public:
	Funciones();
	~Funciones();
	
// Versi�n del GR-lida
	QString stVersionGrl(){ return "0.3.0"; }
// Versiones que soporta
	QString stVersionDbx(){ return "0.72";  }
	QString stVersionSvm(){ return "0.10.0";}
	QString stVersionVdms(){return "2.0.4"; }

// Muestra el Nombre de la plataforma donde esta compilada
	QString get_Plataforma();
// Convierte de N�mero a Texto
	QString	IntToStr(int num);
// Convierte de Texto a N�mero
	int StrToInt(QString text);
// Devuelve la hora y la fecha
	QString HoraFechaActual();
// Elimina un item de un TreeWidget
	void DeleteItemTree( QTreeWidgetItem * item );
//	
	void CargarIdiomasCombo(const QString dirLng, QComboBox *myCombobox);
	void CargarDatosComboBox(QString Archivo, QComboBox *myCombobox,int num_col, bool idioma_svm);
	void CargarIconosComboBox(QString IconDir, QComboBox *myCombobox, QStringList filters);
	void CargarDatosListaSvm(QString Archivo, QTreeWidget *myTreeWidget);
//Carga los Smiles en un TreeWidget
	QHash<QString, QString> Cargar_Smiles(QString Archivo, QTreeWidget *myTreeWidget);
	QHash<QString, QString> Cargar_Smiles(QString Archivo);
	QString ReemplazaTextoSmiles(QString str, QHash<QString, QString> lista);
// Obtiene la direccion y el nombre del archivo atraves de QFileDialog
	QString VentanaAbrirArchivos(const QString caption, const QString dir, const QString tmp_dir, const QString filter, QString *selectedFilter, bool Open_Save);
// Obtiene la direccion de una carpeta atraves de QFileDialog
	QString VentanaDirectorios(const QString caption, const QString dir, const QString tmp_dir);
//
	QStringList CreaConfigMontajes(QTreeWidget *treeWidget, const QHash<QString, QString> datos);

};

#endif /*FUNCIONES_H*/
