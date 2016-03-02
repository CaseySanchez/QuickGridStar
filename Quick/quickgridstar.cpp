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
    _row(0),
    _column(0),
    _rowSpan(1),
    _columnSpan(1)
{
}

QuickGridStar::QuickGridStar(QQuickItem *parent) :
    QQuickItem(parent)
{
}

QuickGridStar::~QuickGridStar()
{
    for(QuickGridItem *item : _items)
    {
        delete item;
    }
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
                _items << new QuickGridItem(child, attached);
            }
        }
    }

    for(QuickGridItem *item : _items)
    {
        _gridDefinition.calculateBounds(item->_attached->_row, item->_attached->_column, item->_attached->_rowSpan, item->_attached->_columnSpan);
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
        rect(QPointF(0.f, 0.f), size);

    for(QuickGridItem *item : _items)
    {
        QRectF
            cellRect(_gridDefinition.cellRect(rect, item->_attached->_row, item->_attached->_column, item->_attached->_rowSpan, item->_attached->_columnSpan));

        item->_item->setPosition(cellRect.topLeft());
        item->_item->setWidth(cellRect.width());
        item->_item->setHeight(cellRect.height());
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
