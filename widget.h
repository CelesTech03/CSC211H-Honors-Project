#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "film.h"


namespace Ui { class Widget; }


class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    //Functions implemented from widget.ui in order to make widget buttons functionable
    void on_addRowButton_clicked();

    void on_removeRowButton_clicked();

    void on_addColumnButton_clicked();

    void on_removeColumnButton_clicked();

    void on_addChildButton_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
