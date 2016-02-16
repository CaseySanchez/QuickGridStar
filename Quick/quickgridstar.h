#ifndef QUICKGRIDSTAR_H
#define QUICKGRIDSTAR_H

#include <QQuickItem>
#include <QQuickWindow>
#include <QDebug>

#include "quickgriddefinition.h"

class QuickGridStar;

class QuickRowDefinition : public QQuickItem
{
    friend class QuickGridStar;

    Q_OBJECT

    Q_PROPERTY(qreal weight MEMBER _weight)

public:
    QuickRowDefinition(QQuickItem *parent = 0);

protected:
    qreal
        _weight;
};

class QuickColumnDefinition : public QQuickItem
{
    friend class QuickGridStar;

    Q_OBJECT

    Q_PROPERTY(qreal weight MEMBER _weight)

public:
    QuickColumnDefinition(QQuickItem *parent = 0);

protected:
    qreal
        _weight;
};

class QuickGridStarAttached : public QObject
{
    friend class QuickGridStar;

    Q_OBJECT

    Q_PROPERTY(int row MEMBER _row)
    Q_PROPERTY(int column MEMBER _column)
    Q_PROPERTY(int rowSpan MEMBER _rowSpan)
    Q_PROPERTY(int columnSpan MEMBER _columnSpan)

public:
    QuickGridStarAttached(QObject *object);

protected:
    int
        _row,
        _column,
        _rowSpan,
        _columnSpan;
};

class QuickGridStar : public QQuickItem
{
    Q_OBJECT

    struct QuickGridItem
    {
        QQuickItem
            *_item;
        int
            _row,
            _column,
            _rowSpan,
            _columnSpan;

        QuickGridItem(QQuickItem *item, int row, int column, int rowSpan, int columnSpan) :
            _item(item),
            _row(row),
            _column(column),
            _rowSpan(rowSpan),
            _columnSpan(columnSpan)
        {
        }
    };

    QList<QuickGridItem *>
        _items;

    QuickGridDefinition
        _gridDefinition;

public:
    QuickGridStar(QQuickItem *parent = 0);
    ~QuickGridStar();

    static QuickGridStarAttached *qmlAttachedProperties(QObject *object);

    static void registerTypes();

protected:
    void componentComplete() Q_DECL_OVERRIDE;
    void itemChange(ItemChange change, const ItemChangeData &value) Q_DECL_OVERRIDE;
    bool event(QEvent *e) Q_DECL_OVERRIDE;

private:
    void updateGeometry();
};

QML_DECLARE_TYPEINFO(QuickGridStar, QML_HAS_ATTACHED_PROPERTIES)
QML_DECLARE_TYPE(QuickRowDefinition)
QML_DECLARE_TYPE(QuickColumnDefinition)

#endif // QUICKGRIDSTAR_H
