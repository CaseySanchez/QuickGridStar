#include "qquickgridstar.h"

QQuickRowDefinition::QQuickRowDefinition(QQuickItem *parent) :
    QQuickItem(parent),
    m_weight(1.0)
{
}

QQuickColumnDefinition::QQuickColumnDefinition(QQuickItem *parent) :
    QQuickItem(parent),
    m_weight(1.0)
{
}

QQuickGridStarAttached::QQuickGridStarAttached(QObject *object) :
    QObject(object),
    m_dirty(true),
    m_ignore(false),
    m_row(0),
    m_column(0),
    m_rowSpan(1),
    m_columnSpan(1),
    m_rowSpanActual(1),
    m_columnSpanActual(1)
{
}

bool QQuickGridStarAttached::getIgnore()
{
    return m_ignore;
}

qint32 QQuickGridStarAttached::getRow()
{
    return m_row;
}

qint32 QQuickGridStarAttached::getColumn()
{
    return m_column;
}

qint32 QQuickGridStarAttached::getRowSpan()
{
    return m_rowSpan;
}

qint32 QQuickGridStarAttached::getColumnSpan()
{
    return m_columnSpan;
}

void QQuickGridStarAttached::setIgnore(bool ignore)
{
    m_ignore = ignore;

    m_dirty = true;
}

void QQuickGridStarAttached::setRow(qint32 row)
{
    m_row = row;

    m_dirty = true;
}

void QQuickGridStarAttached::setColumn(qint32 column)
{
    m_column = column;

    m_dirty = true;
}

void QQuickGridStarAttached::setRowSpan(qint32 rowSpan)
{
    m_rowSpan = rowSpan;

    m_dirty = true;
}

void QQuickGridStarAttached::setColumnSpan(qint32 columnSpan)
{
    m_columnSpan = columnSpan;

    m_dirty = true;
}

QQuickGridStar::QQuickGridStar(QQuickItem *parent) :
    QQuickItem(parent)
{
}

QQuickGridStar::~QQuickGridStar()
{
}

bool QQuickGridStar::validRow(qint32 row)
{
    return row >= 0 && row < rowCount();
}

bool QQuickGridStar::validColumn(qint32 column)
{
    return column >= 0 && column < rowCount();
}

qint32 QQuickGridStar::rowCount()
{
    return m_gridDefinition.rowCount();
}

qint32 QQuickGridStar::columnCount()
{
    return m_gridDefinition.columnCount();
}

QVariant QQuickGridStar::itemsAt(qint32 row, qint32 column)
{
    QVariantList
            quickItems;

    if(validRow(row) && validColumn(column))
    {
        for(QQuickItem *item : m_items)
        {
            QQuickGridStarAttached
                *attached = qobject_cast<QQuickGridStarAttached *>(qmlAttachedPropertiesObject<QQuickGridStar>(item));

            if(attached->m_row == row && attached->m_column == column)
            {
                quickItems << QVariant::fromValue(item);
            }
        }
    }

    return quickItems;
}

void QQuickGridStar::addRowDefinition(qreal weight, qint32 row)
{
    if(validRow(row))
    {
        for(QQuickItem *item : m_items)
        {
            QQuickGridStarAttached
                *attached = qobject_cast<QQuickGridStarAttached *>(qmlAttachedPropertiesObject<QQuickGridStar>(item));

            if(attached->m_row >= row)
            {
                attached->m_row++;
            }

            attached->m_dirty = true;
        }

        m_gridDefinition.addRowDefinition(weight, row);

        polish();
    }
}

void QQuickGridStar::addColumnDefinition(qreal weight, qint32 column)
{
    if(validColumn(column))
    {
        for(QQuickItem *item : m_items)
        {
            QQuickGridStarAttached
                *attached = qobject_cast<QQuickGridStarAttached *>(qmlAttachedPropertiesObject<QQuickGridStar>(item));

            if(attached->m_column >= column)
            {
                attached->m_column++;
            }

            attached->m_dirty = true;
        }

        m_gridDefinition.addColumnDefinition(weight, column);

        polish();
    }
}

void QQuickGridStar::removeRowDefinition(qint32 row)
{
    if(validRow(row))
    {
        for(QQuickItem *item : m_items)
        {
            QQuickGridStarAttached
                *attached = qobject_cast<QQuickGridStarAttached *>(qmlAttachedPropertiesObject<QQuickGridStar>(item));

            if(attached->m_row >= row)
            {
                attached->m_row--;
            }

            attached->m_dirty = true;
        }

        m_gridDefinition.removeRowDefinition(row);

        polish();
    }
}

void QQuickGridStar::removeColumnDefinition(qint32 column)
{
    if(validColumn(column))
    {
        for(QQuickItem *item : m_items)
        {
            QQuickGridStarAttached
                *attached = qobject_cast<QQuickGridStarAttached *>(qmlAttachedPropertiesObject<QQuickGridStar>(item));

            if(attached->m_column >= column)
            {
                attached->m_column--;
            }

            attached->m_dirty = true;
        }

        m_gridDefinition.removeColumnDefinition(column);

        polish();
    }
}

qreal QQuickGridStar::getRowSpacing()
{
    return m_rowSpacing;
}

qreal QQuickGridStar::getColumnSpacing()
{
    return m_columnSpacing;
}

void QQuickGridStar::setRowSpacing(qreal rowSpacing)
{
    m_rowSpacing = rowSpacing;
}

void QQuickGridStar::setColumnSpacing(qreal columnSpacing)
{
    m_columnSpacing = columnSpacing;
}

void QQuickGridStar::componentComplete()
{
    QSizeF
        size = QSizeF(width(), height());

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

        setSize(size);
    }

    if(rowCount() == 0)
    {
        addRowDefinition();
    }

    if(columnCount() == 0)
    {
        addColumnDefinition();
    }

    QQuickItem::componentComplete();
}

void QQuickGridStar::itemChange(ItemChange change, const ItemChangeData &value)
{
    if(change == ItemChildAddedChange)
    {
        QQuickItem
            *item = value.item;

        QString
            className(item->metaObject()->className());

        if(!className.compare("QQuickRowDefinition"))
        {
            QQuickRowDefinition
                *definition = qobject_cast<QQuickRowDefinition *>(item);

            m_gridDefinition.addRowDefinition(definition->m_weight);
        }
        else if(!className.compare("QQuickColumnDefinition"))
        {
            QQuickColumnDefinition
                *definition = qobject_cast<QQuickColumnDefinition *>(item);

            m_gridDefinition.addColumnDefinition(definition->m_weight);
        }
        else
        {
            m_items << item;
        }
    }
    else if(change == ItemChildRemovedChange)
    {
        QQuickItem
            *item = value.item;

        m_items.removeOne(item);
    }

    polish();


    QQuickItem::itemChange(change, value);
}

void QQuickGridStar::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    polish();

    QQuickItem::geometryChanged(newGeometry, oldGeometry);
}

void QQuickGridStar::updatePolish()
{
    QSizeF
        size(width() - m_columnSpacing * qreal(columnCount() - 1), height() - m_rowSpacing * qreal(rowCount() - 1));

    QRectF
        rect(QPointF(0.0, 0.0), size);

    for(QQuickItem *item : m_items)
    {
        QQuickGridStarAttached
            *attached = qobject_cast<QQuickGridStarAttached *>(qmlAttachedPropertiesObject<QQuickGridStar>(item));

        if(!attached->m_ignore)
        {
            if(attached->m_dirty)
            {
                attached->m_rowSpanActual = attached->m_rowSpan;
                attached->m_columnSpanActual = attached->m_columnSpan;

                m_gridDefinition.calculateBounds(attached->m_row, attached->m_column, attached->m_rowSpanActual, attached->m_columnSpanActual);

                attached->m_dirty = false;
            }

            QRectF
                cellRect(m_gridDefinition.cellRect(rect, attached->m_row, attached->m_column, attached->m_rowSpanActual, attached->m_columnSpanActual));

            item->setPosition(cellRect.topLeft() + QPointF(m_columnSpacing * qreal(attached->m_column), m_rowSpacing * qreal(attached->m_row)));
            item->setSize(cellRect.size());

            emit attached->layoutChanged();
        }
    }
}

QQuickGridStarAttached *QQuickGridStar::qmlAttachedProperties(QObject *object)
{
    return new QQuickGridStarAttached(object);
}

void registerQuickGridStarTypes()
{
    qmlRegisterType<QQuickGridStar>("QuickGridStar", 1, 0, "GridStar");
    qmlRegisterType<QQuickRowDefinition>("QuickGridStar", 1, 0, "RowDefinition");
    qmlRegisterType<QQuickColumnDefinition>("QuickGridStar", 1, 0, "ColumnDefinition");
}

Q_COREAPP_STARTUP_FUNCTION(registerQuickGridStarTypes)
