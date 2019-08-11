#ifndef COMBOBOXCOLORPROPERTY_H
#define COMBOBOXCOLORPROPERTY_H

#include "propertyitem.h"
#include "propertybutton.h"
#include "qcolorcombobox.h"

class ComboBoxColorProperty : public PropertyItem
{
public:
	ComboBoxColorProperty(const QString &name, const QColor &color = QColor(), const QColor &defaultColor = QColor());
	ComboBoxColorProperty(PropertyItem *top, const QString &name, const QColor &color = QColor(), const QColor &defaultColor = QColor());

	void setColor(const QColor &color);
	const QColor &getColor() const;

	void setDefaultColor(const QColor &color) { m_defaultColor = color; }
	const QColor &getDefaultColor() const { return m_defaultColor; }

	void setColorsList(const QStringList &colorNames);

	void allowListColorsOnly(bool on);

// reimp
	virtual QVariant getVariantValue() const;
	virtual void validateValue();
	virtual void displayValue();

	virtual QWidget *createEditor() const;
	virtual void valueToEditor();
	virtual void valueFromEditor();

protected:
	mutable QColor m_color;
	mutable QColorComboBox *m_editor;
	QColor m_defaultColor;
	bool m_listColorsOnly;
	QStringList m_listColorNames;
	QList<QColor> m_listColors;

	class ButtonsEditor : public TPropertyButton<QColorComboBox>
	{
	public:
		ButtonsEditor(QColorComboBox *editor, ComboBoxColorProperty *property);
	protected:
		virtual void onButtonClicked();
		virtual void onButtonDefaultClicked();
		ComboBoxColorProperty *m_property;
	};

private:
	void loadColorsList(const QStringList &colorNames);
};

#endif // COMBOBOXCOLORPROPERTY_H
