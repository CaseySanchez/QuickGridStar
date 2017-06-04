#ifndef QUICKGRIDSTAR_H
#define QUICKGRIDSTAR_H

#include <QQuickItem>
#include <QQuickWindow>

#include "quickgriddefinition.h"

class QuickGridStar;

class QuickRowDefinition : public QQuickItem
{
    friend class QuickGridStar;

    Q_OBJECT

    Q_PROPERTY(qreal weight MEMBER _weight)

public:
    QuickRowDefinition(QQuickItem *parent = nullptr);

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
    QuickColumnDefinition(QQuickItem *parent = nullptr);

protected:
    qreal
        _weight;
};

class QuickGridStarAttached : public QObject
{
    friend class QuickGridStar;

    Q_OBJECT

    Q_PROPERTY(bool ignore MEMBER _ignore)
    Q_PROPERTY(qint32 row READ getRow WRITE setRow)
    Q_PROPERTY(qint32 column READ getColumn WRITE setColumn)
    Q_PROPERTY(qint32 rowSpan READ getRowSpan WRITE setRowSpan)
    Q_PROPERTY(qint32 columnSpan READ getColumnSpan WRITE setColumnSpan)

public:
    QuickGridStarAttached(QObject *object);

    void setRow(qint32 row);
    void setColumn(qint32 column);
    void setRowSpan(qint32 rowSpan);
    void setColumnSpan(qint32 columnSpan);

    qint32 getRow();
    qint32 getColumn();
    qint32 getRowSpan();
    qint32 getColumnSpan();

protected:
    bool
        _ignore,
        _dirty;
    qint32
        _row,
        _column,
        _rowSpan,
        _columnSpan,
        _rowSpanActual,
        _columnSpanActual;
};

class QuickGridStar : public QQuickItem
{
    Q_OBJECT

    QList<QQuickItem *>
        _items;

    QuickGridDefinition
        _gridDefinition;

public:
    QuickGridStar(QQuickItem *parent = nullptr);
    ~QuickGridStar();

    Q_INVOKABLE qint32 rowCount();
    Q_INVOKABLE qint32 columnCount();

    Q_INVOKABLE QVariant itemsAt(qint32 row, qint32 column);

    Q_INVOKABLE void addItem(QObject *object, qint32 row = 0, qint32 column = 0, qint32 rowSpan = 1, qint32 columnSpan = 1);
    Q_INVOKABLE bool removeItem(QObject *object);

    Q_INVOKABLE void addRowDefinition(qreal weight = 1.0f, qint32 row = -1);
    Q_INVOKABLE void addColumnDefinition(qreal weight = 1.0f, qint32 column = -1);

    Q_INVOKABLE void removeRowDefinition(qint32 row = -1);
    Q_INVOKABLE void removeColumnDefinition(qint32 column = -1);

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
