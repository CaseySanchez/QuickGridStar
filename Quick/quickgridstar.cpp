#include "quickgridstar.h"

QuickRowDefinition::QuickRowDefinition(QQuickItem *parent) :
    QQuickItem(parent),
    _weight(1.0f)
{
}

QuickColumnDefinition::QuickColumnDefinition(QQuickItem *parent) :
    QQuickItem(parent),
    _weight(1.0f)
{
}

QuickGridStarAttached::QuickGridStarAttached(QObject *object) :
    QObject(object),
    _ignore(false),
    _dirty(true),
    _row(0),
    _column(0),
    _rowSpan(1),
    _columnSpan(1),
    _rowSpanActual(1),
    _columnSpanActual(1)
{
}

void QuickGridStarAttached::setRow(qint32 row)
{
    _row = row;

    _dirty = true;
}

void QuickGridStarAttached::setColumn(qint32 column)
{
    _column = column;

    _dirty = true;
}

void QuickGridStarAttached::setRowSpan(qint32 rowSpan)
{
    _rowSpan = rowSpan;

    _dirty = true;
}

void QuickGridStarAttached::setColumnSpan(qint32 columnSpan)
{
    _columnSpan = columnSpan;

    _dirty = true;
}

qint32 QuickGridStarAttached::getRow()
{
    return _row;
}

qint32 QuickGridStarAttached::getColumn()
{
    return _column;
}

qint32 QuickGridStarAttached::getRowSpan()
{
    return _rowSpan;
}

qint32 QuickGridStarAttached::getColumnSpan()
{
    return _columnSpan;
}

QuickGridStar::QuickGridStar(QQuickItem *parent) :
    QQuickItem(parent)
{
}

QuickGridStar::~QuickGridStar()
{
}

qint32 QuickGridStar::rowCount()
{
    return _gridDefinition.rowCount();
}

qint32 QuickGridStar::columnCount()
{
    return _gridDefinition.columnCount();
}

QVariant QuickGridStar::itemsAt(qint32 row, qint32 column)
{
    QVariantList
            quickItems;

    for(QQuickItem *item : _items)
    {
        QuickGridStarAttached
                *attached = qobject_cast<QuickGridStarAttached *>(qmlAttachedPropertiesObject<QuickGridStar>(item));

        if(attached->_row == row && attached->_column == column)
        {
            quickItems << QVariant::fromValue(item);
        }
    }

    return quickItems;
}

void QuickGridStar::addItem(QObject *object, qint32 row, qint32 column, qint32 rowSpan, qint32 columnSpan)
{
    removeItem(object);

    object->setParent(this);

    QuickGridStarAttached
            *attached = qobject_cast<QuickGridStarAttached *>(qmlAttachedPropertiesObject<QuickGridStar>(object));

    attached->_row = row;
    attached->_column = column;
    attached->_rowSpan = rowSpan;
    attached->_columnSpan = columnSpan;

    QQuickItem
            *item = qobject_cast<QQuickItem *>(object);

    _items << item;
}

bool QuickGridStar::removeItem(QObject *object)
{
    return _items.removeOne(qobject_cast<QQuickItem *>(object));
}

void QuickGridStar::addRowDefinition(qreal weight, qint32 row)
{
    for(QQuickItem *item : _items)
    {
        QuickGridStarAttached
            *attached = qobject_cast<QuickGridStarAttached *>(qmlAttachedPropertiesObject<QuickGridStar>(item));

        if(attached->_row >= row)
        {
            attached->_row++;
        }

        attached->_dirty = true;
    }

    _gridDefinition.addRowDefinition(weight, row);
}

void QuickGridStar::addColumnDefinition(qreal weight, qint32 column)
{
    for(QQuickItem *item : _items)
    {
        QuickGridStarAttached
            *attached = qobject_cast<QuickGridStarAttached *>(qmlAttachedPropertiesObject<QuickGridStar>(item));

        if(attached->_column >= column)
        {
            attached->_column++;
        }

        attached->_dirty = true;
    }

    _gridDefinition.addColumnDefinition(weight, column);
}

void QuickGridStar::removeRowDefinition(qint32 row)
{
    for(QQuickItem *item : _items)
    {
        QuickGridStarAttached
            *attached = qobject_cast<QuickGridStarAttached *>(qmlAttachedPropertiesObject<QuickGridStar>(item));

        if(attached->_row >= row)
        {
            attached->_row--;
        }

        attached->_dirty = true;
    }

    _gridDefinition.removeRowDefinition(row);
}

void QuickGridStar::removeColumnDefinition(qint32 column)
{
    for(QQuickItem *item : _items)
    {
        QuickGridStarAttached
            *attached = qobject_cast<QuickGridStarAttached *>(qmlAttachedPropertiesObject<QuickGridStar>(item));

        if(attached->_column >= column)
        {
            attached->_column--;
        }

        attached->_dirty = true;
    }

    _gridDefinition.removeColumnDefinition(column);
}

void QuickGridStar::componentComplete()
{
    QList<QQuickItem *>
        children = childItems();

    for(QQuickItem *child : children)
    {
        QString
            className(child->metaObject()->className());

        if(!className.compare("QuickRowDefinition"))
        {
            QuickRowDefinition
                *definition = qobject_cast<QuickRowDefinition *>(child);

            _gridDefinition.addRowDefinition(definition->_weight);
        }
        else if(!className.compare("QuickColumnDefinition"))
        {
            QuickColumnDefinition
                *definition = qobject_cast<QuickColumnDefinition *>(child);

            _gridDefinition.addColumnDefinition(definition->_weight);
        }
        else
        {
            QuickGridStarAttached
                *attached = qobject_cast<QuickGridStarAttached *>(qmlAttachedPropertiesObject<QuickGridStar>(child));

            if(!attached->_ignore)
            {
                _items << child;
            }
        }
    }

    QQuickItem::componentComplete();
}

void QuickGridStar::itemChange(ItemChange change, const ItemChangeData &value)
{
    QCoreApplication::postEvent(this, new QEvent(QEvent::LayoutRequest));

    QQuickItem::itemChange(change, value);
}

bool QuickGridStar::event(QEvent *e)
{
    if(e->type() == QEvent::LayoutRequest)
    {
        updateGeometry();
    }

    return QQuickItem::event(e);
}

void QuickGridStar::updateGeometry()
{
    QSizeF
        size(width(), height());

    if(size.isEmpty())
    {
        QObject
            *parentObject = parent();

        if(parentObject->isWindowType())
        {
            QQuickWindow
                *quickWindow = qobject_cast<QQuickWindow *>(parentObject);

            size = QSizeF(quickWindow->width(), quickWindow->height());
        }
        else
        {
            QQuickItem
                *quickItem = qobject_cast<QQuickItem *>(parentObject);

            size = QSizeF(quickItem->width(), quickItem->height());
        }
    }

    QRectF
        rect(QPointF(0.0f, 0.0f), size);

    for(QQuickItem *item : _items)
    {
        QuickGridStarAttached
                *attached = qobject_cast<QuickGridStarAttached *>(qmlAttachedPropertiesObject<QuickGridStar>(item));

        if(attached->_dirty)
        {
            attached->_rowSpanActual = attached->_rowSpan;
            attached->_columnSpanActual = attached->_columnSpan;

            _gridDefinition.calculateBounds(attached->_row, attached->_column, attached->_rowSpanActual, attached->_columnSpanActual);

            attached->_dirty = false;
        }

        QRectF
            cellRect(_gridDefinition.cellRect(rect, attached->_row, attached->_column, attached->_rowSpanActual, attached->_columnSpanActual));

        item->setPosition(cellRect.topLeft());
        item->setWidth(cellRect.width());
        item->setHeight(cellRect.height());
    }
}

QuickGridStarAttached *QuickGridStar::qmlAttachedProperties(QObject *object)
{
    return new QuickGridStarAttached(object);
}

void QuickGridStar::registerTypes()
{
    qmlRegisterType<QuickGridStar>("com.quick.gridStar", 1, 0, "GridStar");
    qmlRegisterType<QuickRowDefinition>("com.quick.gridStar", 1, 0, "RowDefinition");
    qmlRegisterType<QuickColumnDefinition>("com.quick.gridStar", 1, 0, "ColumnDefinition");
}
