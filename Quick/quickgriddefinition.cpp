#include "quickgriddefinition.h"

QuickGridDefinition::QuickGridDefinition() :
    _rowSize(0.0f),
    _columnSize(0.0f),
    _invRowSize(0.0f),
    _invColumnSize(0.0f)
{
}

void QuickGridDefinition::addRowDefinition(qreal weight, qint32 row)
{
    if(row < 0 || row >= rowCount())
    {
        _rows << QuickGridCell(weight, _rowSize);
    }
    else
    {
        _rows.insert(row, QuickGridCell(weight, _rows[row]._offset));

        for(row = row + 1; row < rowCount(); row++)
        {
            _rows[row]._offset += weight;
        }
    }

    _rowSize += weight;
    _invRowSize = 1.0f / _rowSize;
}

void QuickGridDefinition::addColumnDefinition(qreal weight, qint32 column)
{
    if(column < 0 || column >= columnCount())
    {
        _columns << QuickGridCell(weight, _columnSize);
    }
    else
    {
        _columns.insert(column, QuickGridCell(weight, _columns[column]._offset));

        for(column = column + 1; column < columnCount(); column++)
        {
            _columns[column]._offset += weight;
        }
    }

    _columnSize += weight;
    _invColumnSize = 1.0f / _columnSize;
}

void QuickGridDefinition::removeRowDefinition(qint32 row)
{
    if(rowCount() == 0)
    {
        return;
    }

    qreal
            weight;

    if(row < 0 || row >= rowCount())
    {
        weight = _rows[rowCount() - 1]._weight;

        _rows.removeLast();
    }
    else
    {
        weight = _rows[row]._weight;

        _rows.removeAt(row);

        for( ; row < rowCount(); row++)
        {
            _rows[row]._offset -= weight;
        }
    }

    _rowSize -= weight;
    _invRowSize = 1.0f / _rowSize;
}

void QuickGridDefinition::removeColumnDefinition(qint32 column)
{
    if(columnCount() == 0)
    {
        return;
    }

    qreal
            weight;

    if(column < 0 || column >= columnCount())
    {
        weight = _columns[columnCount() - 1]._weight;

        _columns.removeLast();
    }
    else
    {
        weight = _columns[column]._weight;

        _columns.removeAt(column);

        for( ; column < columnCount(); column++)
        {
            _columns[column]._offset -= weight;
        }
    }

    _columnSize -= weight;
    _invColumnSize = 1.0f / _columnSize;
}

qint32 QuickGridDefinition::rowCount() const
{
    return _rows.size();
}

qint32 QuickGridDefinition::columnCount() const
{
    return _columns.size();
}

qreal QuickGridDefinition::rowOffset(qint32 index)
{
    return _rows[index]._offset * _invRowSize;
}

qreal QuickGridDefinition::columnOffset(qint32 index)
{
    return _columns[index]._offset * _invColumnSize;
}

qreal QuickGridDefinition::rowWeight(qint32 index)
{
    return _rows[index]._weight * _invRowSize;
}

qreal QuickGridDefinition::columnWeight(qint32 index)
{
    return _columns[index]._weight * _invColumnSize;
}

QPointF QuickGridDefinition::cellPoint(QRectF rect, qint32 row, qint32 column)
{
    return QPointF(columnOffset(column) * rect.width() + rect.x(), rowOffset(row) * rect.height() + rect.y());
}

QSizeF QuickGridDefinition::cellSize(QRectF rect, qint32 row, qint32 column, qint32 rowSpan, qint32 columnSpan)
{
    QSizeF
        size;

    for(qint32 end = qMin(row + rowSpan, rowCount()); row < end; row++)
    {
        size.rheight() += rowWeight(row) * rect.height();
    }

    for(qint32 end = qMin(column + columnSpan, columnCount()); column < end; column++)
    {
        size.rwidth() += columnWeight(column) * rect.width();
    }

    return size;
}

QRectF QuickGridDefinition::cellRect(QRectF rect, qint32 row, qint32 column, qint32 rowSpan, qint32 columnSpan)
{
    return QRectF(cellPoint(rect, row, column), cellSize(rect, row, column, rowSpan, columnSpan));
}

void QuickGridDefinition::calculateBounds(qint32 &row, qint32 &column, qint32 &rowSpan, qint32 &columnSpan)
{
    qint32
        rowSize = rowCount(),
        columnSize = columnCount();

    row = qBound(0, row, rowSize - 1);
    column = qBound(0, column, columnSize - 1);

    if(rowSpan <= 0)
    {
        rowSpan = rowSize - row + rowSpan;
    }

    if(columnSpan <= 0)
    {
        columnSpan = columnSize - column + columnSpan;
    }

    rowSpan = qBound(1, rowSpan, rowSize - row);
    columnSpan = qBound(1, columnSpan, columnSize - column);
}
