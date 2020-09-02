#include "film.h"
#include <QDebug>

Film::Film(const QVector <QVariant> &data, Film *parent)
{
    //this->films = films;
    // this->year = year;
    this->parent = parent;
    columnFields = data;
}

Film::~Film()
{
    qDeleteAll(children);
}

void Film::appendChild(Film * childParam)
{
    children.append(childParam);
}

Film *Film::child(int row)
{
    return children.value(row);
}

int Film::childCount() const
{
    return children.count();
}

QVariant Film::data(int column) const
{
    return columnFields.value(column);
}

//Returns index of this film in the list of children of its parents

int Film::row() const
{
    if(parent){
        parent->children.indexOf(const_cast<Film*>(this));
    }
    return  0;
}

Film *Film::parentFilm()
{
    return parent;
}

void Film::showInfo()
{
   qDebug() << "Person : " << columnFields.at(0).toString() << "(" << childCount() << " children)";

   QStringList columns;
   foreach (QVariant column, columnFields) {
       columns.append(column.toString());
   }
   qDebug() << " Columns : " << columns;
   foreach (Film * child, children) {
       child->showInfo();
   }
}


int Film::columnCount() const
{
    return columnFields.count();
}

bool Film::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > children.size())
        return false;

    for( int row = 0; row < count ; row ++){
        QVector<QVariant> data(columns);
        Film * film = new Film(data,this);
        children.insert(position,film);
    }
    return  true;
}

bool Film::insertColumns(int position, int columns)
{

    if (position < 0 || position > columnFields.size())
        return false;
    for (int column = 0; column < columns; ++column){
        columnFields.insert(position, QVariant());
    }

    foreach (Film * film, children) {
        film->insertColumns(position,columns);
    }
    return true;
}

bool Film::removeChildren(int position, int count)
{
    if (position < 0 || position + count > children.size())
        return false;
    for(int row = 0 ; row < count ; row++){
        Film * child = children.takeAt(position);
        delete child;
    }

    return true;
}

bool Film::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > columnFields.size())
        return false;

    for (int column = 0; column < columns; ++column)
        columnFields.remove(position);
    foreach (Film * film, children) {
        film->removeColumns(position,columns);
    }
    return true;
}

int Film::childNumber() const
{
    if (parent)
        return parent->children.indexOf(const_cast<Film*>(this));

    return 0;
}

bool Film::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= columnFields.size())
        return false;
    columnFields[column] = value;

    return  true;
}


