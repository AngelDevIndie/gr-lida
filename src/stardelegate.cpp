#include "stardelegate.h"

StarDelegate::StarDelegate(QString theme, QString home_dir, int column, QWidget *parent)
	: QItemDelegate(parent)
{
	stHomeDir  = home_dir;
	stTheme    = theme;
	num_column = column;//26;
	col_Icono  = 0;
	star_on.load( stTheme+"images/star_on.png" );
	star_off.load(stTheme+"images/star_off.png");
}

StarDelegate::~StarDelegate(){}

void StarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
		const QModelIndex &index) const
{
	if (index.column() == num_column)
	{
		const QAbstractItemModel *model = index.model();
//		QPalette::ColorGroup cg = (option.state & QStyle::State_Enabled) ?
//			(option.state & QStyle::State_Active) ? QPalette::Normal : QPalette::Inactive : QPalette::Disabled;

		if (option.state & QStyle::State_Selected)
			painter->fillRect(option.rect, option.palette.highlight() );

		if( model->data(index, Qt::DisplayRole).toString()!="")
		{
			int rating = model->data(index, Qt::DisplayRole).toInt();
			int width  = star_on.width();
			int height = star_on.height();
			int x = option.rect.x();
			int y = option.rect.y() + (option.rect.height() / 2) - (height / 2);
			for (int i = 0; i < 5; ++i)
			{
				if(i < rating)
					painter->drawPixmap(x, y, star_on );
				else
					painter->drawPixmap(x, y, star_off );
				x += width;
			}
		}
		drawFocus(painter, option, option.rect.adjusted(0, 0, 0, 0));
	} else {
		QItemDelegate::paint(painter, option, index);
	}
}

QSize StarDelegate::sizeHint(const QStyleOptionViewItem &option,
		const QModelIndex &index) const
{
	if (index.column() == num_column)
		return QSize(5 * star_on.width(), star_on.height()) + QSize(1, 1);

	return QItemDelegate::sizeHint(option, index) + QSize(1, 1); // since we draw the grid ourselves
}

bool StarDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
		const QStyleOptionViewItem &option, const QModelIndex &index)
{
	if (index.column() != num_column)
		return QItemDelegate::editorEvent(event, model, option, index);

	if (event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
		if (model->data(index, Qt::DisplayRole).toString()!="")
		{
			int stars = qBound(0, int(0.7 + qreal(mouseEvent->pos().x() - option.rect.x()) / star_on.width()), 5);

			model->setData(index, QVariant(stars));

			QString id_grl = model->data(index.sibling(index.row(),0), Qt::DisplayRole).toString();
			sql->ItemActualizaDatosRating( QVariant(stars).toString(), id_grl );

			QString stThumbs = sql->ItemIDIndex("dbgrl", id_grl, "thumbs");
			QHash<QString, QVariant> LwDat;
			LwDat.clear();
			LwDat["Dat_destino"]        = stHomeDir + "thumbs_list/";
			LwDat["Dat_titulo"]         = sql->ItemIDIndex("dbgrl", id_grl, "titulo");
			LwDat["Dat_tipo_emu"]       = sql->ItemIDIndex("dbgrl", id_grl, "tipo_emu");
			LwDat["Dat_rating"]         = sql->ItemIDIndex("dbgrl", id_grl, "rating");
			LwDat["Dat_rating_visible"] = true;

			if( stThumbs != "" )
			{
				LwDat["Dat_img_src"]   = stHomeDir + "thumbs/" + stThumbs;
				LwDat["Dat_img_cover"] = stThumbs;
			} else {
				LwDat["Dat_img_src"]   = stTheme+"images/juego_sin_imagen.png";
				LwDat["Dat_img_cover"] = "ImgTemp";
			}

			fGrl.CrearCoverList(LwDat, fGrl.CargarListWidgetIconConf());
		}
		return false; //so that the selection can change
	}
	return true;
}
