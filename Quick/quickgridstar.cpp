#include "quickgridstar.h"

QuickGridStarAttached::QuickGridStarAttached(QObject *object) :
    QObject(object),
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
                *def = qobject_cast<QuickRowDefinition *>(child);

            _gridDefinition.addRowDefinition(def->_weight);
        }
        else if(!className.compare("QuickColumnDefinition"))
        {
            QuickColumnDefinition
                *def = qobject_cast<QuickColumnDefinition *>(child);

            _gridDefinition.addColumnDefinition(def->_weight);
        }
        else
        {
            QuickGridStarAttached
                *attached = qobject_cast<QuickGridStarAttached *>(qmlAttachedPropertiesObject<QuickGridStar>(child));

            _gridDefinition.calculateSpan(attached->_row, attached->_column, attached->_rowSpan, attached->_columnSpan);

            _items << new QuickGridItem(child, attached->_row, attached->_column, attached->_rowSpan, attached->_columnSpan);
        }
    }

    QRectF
        rect;

    if(QString(parent()->metaObject()->className()).contains("ApplicationWindow"))
    {
        QQuickWindow
            *p = qobject_cast<QQuickWindow *>(parent());

        rect = QRectF(p->position(), QSizeF(p->width(), p->height()));
    }
    else
    {
        QQuickItem
            *p = qobject_cast<QQuickItem *>(parent());

        rect = QRectF(p->position(), QSizeF(p->width(), p->height()));
    }

    for(QuickGridItem *item : _items)
    {
        QRectF
            cellRect(_gridDefinition.cellRect(rect, item->_row, item->_column, item->_rowSpan, item->_columnSpan));

        item->_item->setPosition(cellRect.topLeft());
        item->_item->setWidth(cellRect.width());
        item->_item->setHeight(cellRect.height());
    }

    QQuickItem::componentComplete();
}

QuickGridStarAttached *QuickGridStar::qmlAttachedProperties(QObject *object)
{
    return new QuickGridStarAttached(object);
}

QuickRowDefinition::QuickRowDefinition(QQuickItem *parent) :
    QQuickItem(parent)
{
}

QuickColumnDefinition::QuickColumnDefinition(QQuickItem *parent) :
    QQuickItem(parent)
{
}
