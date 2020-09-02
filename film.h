#ifndef FILM_H
#define FILM_H

#include <QString>
#include <QList>
#include <QVariant>

//Class that basically represents the root
class Film
{
public:

    Film(const QVector <QVariant> &data, Film * parent = nullptr);
    ~Film();
    void appendChild(Film * childParam);
    Film * child(int row);
    int childCount() const;
    QVariant data(int column) const;
    int row () const;
    Film * parentFilm();

    //Functions needed to make model editable
    int columnCount() const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    //Return the index of this item in the list of its parent children
    int childNumber() const;
    bool setData(int column, const QVariant &value);

    void showInfo();

private:
    //Allows dynamic resizing.
    QList<Film*> children;
    QVector <QVariant> columnFields; //Films, year and any dynamically added columns
    Film * parent;
};

#endif // FILM_H
