#ifndef MODEL_H
#define MODEL_H

#include <QAbstractItemModel>
#include "film.h"

//Treemodel Class
class Model : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);
    ~Model() override;

    //Qt QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;


    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) override;
    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;



private:
    Film * getFilmFromIndex (const QModelIndex &index) const;
    void readFile();
    QVector<QVariant> getFilmsAndYear(QString txtString);
    Film * rootFilm;

};

#endif // MODEL_H
