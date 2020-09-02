#include "widget.h"
#include "ui_widget.h"
#include "model.h"
#include <QTextStream>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    Model * model = new Model(this);

    ui->treeView->setModel(model);

    //Makes widget more visually pleasing
    ui->treeView->setAlternatingRowColors(true);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_addRowButton_clicked()
{
    //Gets a hold of the current index and model.
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->treeView->model();


    //Inserts the item past the current item.
    if (!model->insertRow(index.row()+1, index.parent()))
        return;

    //Loops through the columns setting the data to [Empty Cell] for the new inserted row.
    for (int column = 0; column < model->columnCount(index.parent()); ++column) {
        QModelIndex child = model->index(index.row()+1, column, index.parent());
        model->setData(child, QVariant("[Empty Cell]"), Qt::EditRole);
    }
}

void Widget::on_removeRowButton_clicked()
{
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->treeView->model();


    model->removeRows(index.row(),1,index.parent());
}

void Widget::on_addColumnButton_clicked()
{
    QAbstractItemModel *model = ui->treeView->model();
    int column = ui->treeView->selectionModel()->currentIndex().column();

    // Inserts a column in the parent item.
    bool changed = model->insertColumns(column+1,1);
    if (changed)
        model->setHeaderData(column + 1, Qt::Horizontal,
                             QVariant("[No header]"), Qt::EditRole);

}

void Widget::on_removeColumnButton_clicked()
{
    QAbstractItemModel *model = ui->treeView->model();
    int column = ui->treeView->selectionModel()->currentIndex().column();
    model->removeColumn(column);
}

void Widget::on_addChildButton_clicked()
{
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->treeView->model();

    //If there are zero columns under index, functions inserts a new column under it
    //If that fails, we get out .This will be the case when the tree is completely empty
    if (model->columnCount(index) == 0) {
        if (!model->insertColumn(0, index))
            return;
    }

    //Inserts new row
    if (!model->insertRow(0, index))
        return;

    //Loops around in each column of the new child setting all the
    for (int column = 0; column < model->columnCount(index); ++column) {

    //The new added child is always at row 0
        QModelIndex child = model->index(0, column, index);
        model->setData(child, QVariant("[Empty Cell]"), Qt::EditRole);
    }

    //Sets the newly added item as the current item
    ui->treeView->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                                    QItemSelectionModel::ClearAndSelect);
}
