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

            _items << new QuickGridItem(child, attached->_row, attached->_column, attached->_rowSpan, attached->_columnSpan);
        }
    }

    for(QuickGridItem *item : _items)
    {
        _gridDefinition.calculateBounds(item->_row, item->_column, item->_rowSpan, item->_columnSpan);
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
    static QQuickWindow
        *win = window();

    QRectF
        rect(QPointF(0.f, 0.f), win == (QQuickWindow *)parent() ? QSizeF(win->width(), win->height()) : QSizeF(width(), height()));

    for(QuickGridItem *item : _items)
    {
        QRectF
            cellRect(_gridDefinition.cellRect(rect, item->_row, item->_column, item->_rowSpan, item->_columnSpan));

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
