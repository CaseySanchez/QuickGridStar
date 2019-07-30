#include "qquickgridstar.h"

QQuickRowDefinition::QQuickRowDefinition(QQuickItem *parent) :
    QQuickItem(parent),
    m_weight(1.0)
{
}

qreal QQuickRowDefinition::getWeight()
{
    return m_weight;
}

void QQuickRowDefinition::setWeight(qreal weight)
{
    if(weight != m_weight)
    {
        qreal
            oldWeight = m_weight;

        m_weight = weight;

        emit rowChanged(this, oldWeight);
    }
}

QQuickColumnDefinition::QQuickColumnDefinition(QQuickItem *parent) :
    QQuickItem(parent),
    m_weight(1.0)
{
}

qreal QQuickColumnDefinition::getWeight()
{
    return m_weight;
}

void QQuickColumnDefinition::setWeight(qreal weight)
{
    if(weight != m_weight)
    {
        qreal
            oldWeight = m_weight;

        m_weight = weight;

        emit columnChanged(this, oldWeight);
    }
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
    QQuickItem(parent),
    m_rowSize(0.0),
    m_columnSize(0.0),
    m_rowSpacing(0.0),
    m_columnSpacing(0.0)
{
}

qint32 QQuickGridStar::rowCount()
{
    return m_rows.size();
}

qint32 QQuickGridStar::columnCount()
{
    return m_columns.size();
}

qreal QQuickGridStar::rowOffset(qint32 index)
{
    return m_rows[index]->m_offset * m_invRowSize;
}

qreal QQuickGridStar::columnOffset(qint32 index)
{
    return m_columns[index]->m_offset * m_invColumnSize;
}

qreal QQuickGridStar::rowWeight(qint32 index)
{
    return m_rows[index]->m_weight * m_invRowSize;
}

qreal QQuickGridStar::columnWeight(qint32 index)
{
    return m_columns[index]->m_weight * m_invColumnSize;
}

QPointF QQuickGridStar::cellPoint(QRectF rect, qint32 row, qint32 column)
{
    return QPointF(columnOffset(column) * rect.width() + rect.x(), rowOffset(row) * rect.height() + rect.y());
}

QSizeF QQuickGridStar::cellSize(QRectF rect, qint32 row, qint32 column, qint32 rowSpan, qint32 columnSpan)
{
    qreal
        cellHeight = 0.0,
        cellWidth = 0.0;

    for(qint32 end = qMin(row + rowSpan, rowCount()); row < end; row++)
    {
        cellHeight += rowWeight(row);
    }

    for(qint32 end = qMin(column + columnSpan, columnCount()); column < end; column++)
    {
        cellWidth += columnWeight(column);
    }

    return QSizeF(cellWidth * rect.width(), cellHeight * rect.height());
}

QRectF QQuickGridStar::cellRect(QRectF rect, qint32 row, qint32 column, qint32 rowSpan, qint32 columnSpan)
{
    return QRectF(cellPoint(rect, row, column), cellSize(rect, row, column, rowSpan, columnSpan));
}

QVariant QQuickGridStar::itemsAt(qint32 row, qint32 column)
{
    QVariantList
        quickItems;

    if(row >= 0 && row < rowCount() && column >= 0 && column < rowCount())
    {
        for(QQuickItem *item : m_items)
        {
            QQuickGridStarAttached
                *attached = qobject_cast<QQuickGridStarAttached *>(qmlAttachedPropertiesObject<QQuickGridStar>(item));

            if(attached->m_row == row && attached->m_column == column)
            {
                quickItems.push_back(QVariant::fromValue(item));
            }
        }
    }

    return quickItems;
}

QQuickItem *QQuickGridStar::addRow(qreal weight)
{
    auto row = new QQuickRowDefinition(this);

    row->setWeight(weight);

    itemChange(ItemChange::ItemChildAddedChange, ItemChangeData(row));

    return row;
}

QQuickItem *QQuickGridStar::addColumn(qreal weight)
{
    auto column = new QQuickColumnDefinition(this);

    column->setWeight(weight);

    itemChange(ItemChange::ItemChildAddedChange, ItemChangeData(column));

    return column;
}

QQuickItem *QQuickGridStar::getRow(qint32 row)
{
    return m_rows[row];
}

QQuickItem *QQuickGridStar::getColumn(qint32 column)
{
    return m_columns[column];
}

void QQuickGridStar::clearRows()
{
    m_rowSize = 0.0;

    for(auto row : m_rows)
    {
        row->deleteLater();
    }

    m_rows.clear();
}

void QQuickGridStar::clearColumns()
{
    m_columnSize = 0.0;

    for(auto column : m_columns)
    {
        column->deleteLater();
    }

    m_columns.clear();
}

void QQuickGridStar::clearItems()
{
    for(auto item : m_items)
    {
        item->deleteLater();
    }

    m_items.clear();
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

void QQuickGridStar::onRowChanged(QQuickRowDefinition *definition, const qreal &oldWeight)
{
    qreal
        difference = definition->m_weight - oldWeight;

    for(auto it = m_rows.begin(); it != m_rows.end(); it++)
    {
        if((*it) == definition)
        {
            for(it++; it != m_rows.end(); it++)
            {
                (*it)->m_offset += difference;
            }

            break;
        }
    }

    m_rowSize += difference;
    m_invRowSize = 1.0 / m_rowSize;

    polish();
}

void QQuickGridStar::onColumnChanged(QQuickColumnDefinition *definition, const qreal &oldWeight)
{
    qreal
        difference = definition->m_weight - oldWeight;

    for(auto it = m_columns.begin(); it != m_columns.end(); it++)
    {
        if((*it) == definition)
        {
            for(it++; it != m_columns.end(); it++)
            {
                (*it)->m_offset += difference;
            }

            break;
        }
    }

    m_columnSize += difference;
    m_invColumnSize = 1.0 / m_columnSize;

    polish();
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

            definition->m_offset = m_rowSize;

            m_rowSize += definition->m_weight;
            m_invRowSize = 1.0 / m_rowSize;

            m_rows.push_back(definition);

            connect(definition, &QQuickRowDefinition::rowChanged, this, &QQuickGridStar::onRowChanged);
        }
        else if(!className.compare("QQuickColumnDefinition"))
        {
            QQuickColumnDefinition
                *definition = qobject_cast<QQuickColumnDefinition *>(item);

            definition->m_offset = m_columnSize;

            m_columnSize += definition->m_weight;
            m_invColumnSize = 1.0 / m_columnSize;

            m_columns.push_back(definition);

            connect(definition, &QQuickColumnDefinition::columnChanged, this, &QQuickGridStar::onColumnChanged);
        }
        else
        {
            m_items.push_back(item);
        }
    }
    else if(change == ItemChildRemovedChange)
    {
        QQuickItem
            *item = value.item;

        QString
            className(item->metaObject()->className());

        if(!className.compare("QQuickRowDefinition"))
        {
            QQuickRowDefinition
                *definition = qobject_cast<QQuickRowDefinition *>(item);

            for(auto it = m_rows.begin(); it != m_rows.end(); it++)
            {
                if((*it) == definition)
                {
                    for(it = m_rows.erase(it); it != m_rows.end(); it++)
                    {
                        (*it)->m_offset -= definition->m_weight;
                    }

                    break;
                }
            }

            m_rowSize -= definition->m_weight;
            m_invRowSize = 1.0 / m_rowSize;

            disconnect(definition, &QQuickRowDefinition::rowChanged, this, &QQuickGridStar::onRowChanged);
        }
        else if(!className.compare("QQuickColumnDefinition"))
        {
            QQuickColumnDefinition
                *definition = qobject_cast<QQuickColumnDefinition *>(item);

            for(auto it = m_columns.begin(); it != m_columns.end(); it++)
            {
                if((*it) == definition)
                {
                    for(it = m_columns.erase(it); it != m_columns.end(); it++)
                    {
                        (*it)->m_offset -= definition->m_weight;
                    }

                    break;
                }
            }

            m_columnSize -= definition->m_weight;
            m_invColumnSize = 1.0 / m_columnSize;

            disconnect(definition, &QQuickColumnDefinition::columnChanged, this, &QQuickGridStar::onColumnChanged);
        }
        else
        {
            m_items.removeOne(item);
        }
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
                attached->m_row = qBound(0, attached->m_row, rowCount() - 1);
                attached->m_column = qBound(0, attached->m_column, columnCount() - 1);

                attached->m_rowSpanActual = attached->m_rowSpan;
                attached->m_columnSpanActual = attached->m_columnSpan;

                if(attached->m_rowSpanActual <= 0)
                {
                    attached->m_rowSpanActual += rowCount() - attached->m_row;
                }

                if(attached->m_columnSpanActual <= 0)
                {
                    attached->m_columnSpanActual += columnCount() - attached->m_column;
                }

                attached->m_rowSpanActual = qBound(1, attached->m_rowSpanActual, rowCount() - attached->m_row);
                attached->m_columnSpanActual = qBound(1, attached->m_columnSpanActual, columnCount() - attached->m_column);

                attached->m_dirty = false;
            }

            QRectF
                cell = cellRect(rect, attached->m_row, attached->m_column, attached->m_rowSpanActual, attached->m_columnSpanActual);

            item->setPosition(cell.topLeft() + QPointF(m_columnSpacing * qreal(attached->m_column), m_rowSpacing * qreal(attached->m_row)));
            item->setSize(cell.size() + QSizeF(m_columnSpacing * qreal(attached->m_columnSpanActual - 1), m_rowSpacing * qreal(attached->m_rowSpanActual - 1)));

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
