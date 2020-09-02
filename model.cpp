#include "model.h"
#include <QTextStream>
#include <QFile>
#include <QDebug>


Model::Model(QObject * parent) : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    rootData << "Films" << "Year";
    rootFilm = new Film(rootData);
    readFile();
}

Model::~Model()
{
    //Memory management
    delete rootFilm;
}

QModelIndex Model::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();
    Film * parentFilm;

    if(!parent.isValid()){
        parentFilm = rootFilm;
    }else{
        parentFilm = static_cast<Film*>(parent.internalPointer());
    }

    Film * childFilm = parentFilm->child(row);
    if(childFilm)
        return createIndex(row,column,childFilm);
    return QModelIndex();
}

QModelIndex Model::parent(const QModelIndex &child) const
{
    if(!child.isValid())
    return QModelIndex();

    Film * childFilm = static_cast<Film*>(child.internalPointer());
    Film * parentFilm = childFilm->parentFilm();
    if(parentFilm == rootFilm)
        return QModelIndex();
    return createIndex(parentFilm->row(),0,parentFilm);
}

int Model::rowCount(const QModelIndex &parent) const
{
    Film * parentFilm;
    //Only first columns can have children
    if(parent.column() > 0)
        return 0;
    if(!parent.isValid()){
        parentFilm = rootFilm;
    }else{
        parentFilm = static_cast<Film*>(parent.internalPointer());
    }
    return parentFilm->childCount();
}

int Model::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rootFilm->columnCount();
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if ((role == Qt::DisplayRole) ||(role == Qt::EditRole )){
        Film *person = static_cast<Film*>(index.internalPointer());
        return person->data(index.column());
    }
    return QVariant();
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && (role == Qt::DisplayRole)){
        return rootFilm->data(section);
    }
    return QVariant();
}

Qt::ItemFlags Model::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return nullptr;
    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

Film *Model::getFilmFromIndex(const QModelIndex &index) const
{
    if (index.isValid()) {
        Film *person = static_cast<Film*>(index.internalPointer());
        if (person)
            return person;
    }
    return rootFilm;
}

void Model::readFile()
{
    QString filename = ":/data/films.txt";
    QFile inputFile(filename);
    if(inputFile.open(QIODevice::ReadOnly)){
        int lastIndentation = 0;
        Film * lastParent = rootFilm;
        Film * lastFilm = nullptr;

        QTextStream in(&inputFile);

        while (!in.atEnd()) {

            QString line = in.readLine();

            int currentIndentation = line.count("\t");

            qDebug() << "Read line : " << line << " tab count : " << QString::number(currentIndentation);

            QVector<QVariant> infoList = getFilmsAndYear(line);


            int diffIndent = currentIndentation - lastIndentation;

            if(diffIndent == 0){
                //First Level Person
                Film * film = new Film(infoList,lastParent);
                lastParent->appendChild(film);
                lastFilm = film;

            }else if(diffIndent > 0){
                //Move the parent down
                lastParent = lastFilm;
                Film * film = new Film(infoList,lastParent);
                lastParent->appendChild(film);
                lastFilm = film;
            }else{
                //Move up the parent chain looking the  correct parent
                int iterations = - diffIndent;
                for(int i = 0; i < iterations ; i++){
                    lastParent = lastParent->parentFilm();
                }
                Film * film = new Film(infoList,lastParent);
                lastParent->appendChild(film);
                lastFilm = film;
            }
            lastIndentation = currentIndentation;
        }
        inputFile.close();
    }
}

QVector<QVariant> Model::getFilmsAndYear(QString txtString)
{
    //Gets rid of spaces in txt file
    QString cleanedUpStr = txtString.trimmed();
    QStringList split = cleanedUpStr.split("(");
    split[1].chop(1);

    QVector<QVariant> data;
    data << split[0] << split[1];
    return data;
}

bool Model::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;
    Film *person = getFilmFromIndex(index);
    bool result = person->setData(index.column(),value);

    if(result)
        emit dataChanged(index,index,{role});

    return result;
}

bool Model::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootFilm->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

bool Model::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootFilm->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool Model::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootFilm->removeColumns(position, columns);
    endRemoveColumns();

    if (rootFilm->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool Model::insertRows(int position, int rows, const QModelIndex &parent)
{
    Film *personParent = getFilmFromIndex(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);

    //The third parameter is the number of columns of data each person item is going to contain
    success = personParent->insertChildren(position, rows, rootFilm->columnCount());
    endInsertRows();

    return success;

}

bool Model::removeRows(int position, int rows, const QModelIndex &parent)
{
    Film *parentPerson = getFilmFromIndex(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentPerson->removeChildren(position, rows);
    endRemoveRows();

    return success;

}







