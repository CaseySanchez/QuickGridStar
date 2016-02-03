#include "quickgriddefinition.h"

QuickGridDefinition::QuickGridDefinition() :
    _rowSize(0.0f),
    _columnSize(0.0f),
    _invRowSize(0.0f),
    _invColumnSize(0.0f)
{
}

void QuickGridDefinition::addRowDefinition(qreal size)
{
    _rows << QuickGridCell(size, _rowSize);

    _rowSize += size;
    _invRowSize = 1.0f / _rowSize;
}

void QuickGridDefinition::addColumnDefinition(qreal size)
{
    _columns << QuickGridCell(size, _columnSize);

    _columnSize += size;
    _invColumnSize = 1.0f / _columnSize;
}

int QuickGridDefinition::rowCount() const
{
    return _rows.size();
}

int QuickGridDefinition::columnCount() const
{
    return _columns.size();
}

qreal QuickGridDefinition::rowOffset(int index)
{
    return _rows[index]._offset * _invRowSize;
}

qreal QuickGridDefinition::columnOffset(int index)
{
    return _columns[index]._offset * _invColumnSize;
}

qreal QuickGridDefinition::rowWeight(int index)
{
    return _rows[index]._weight * _invRowSize;
}

qreal QuickGridDefinition::columnWeight(int index)
{
    return _columns[index]._weight * _invColumnSize;
}

QPointF QuickGridDefinition::cellPoint(QRectF rect, int row, int column)
{
    return QPointF(columnOffset(column) * rect.width() + rect.x(), rowOffset(row) * rect.height() + rect.y());
}

QSizeF QuickGridDefinition::cellSize(QRectF rect, int row, int column, int rowSpan, int columnSpan)
{
    QSizeF
        size;

    for(int end = qMin(row + rowSpan, rowCount()); row < end; row++)
    {
        size.rheight() += rowWeight(row) * rect.height();
    }

    for(int end = qMin(column + columnSpan, columnCount()); column < end; column++)
    {
        size.rwidth() += columnWeight(column) * rect.width();
    }

    return size;
}

QRectF QuickGridDefinition::cellRect(QRectF rect, int row, int column, int rowSpan, int columnSpan)
{
    return QRectF(cellPoint(rect, row, column), cellSize(rect, row, column, rowSpan, columnSpan));
}

void QuickGridDefinition::calculateSpan(int row, int column, int &rowSpan, int &columnSpan)
{
    int
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
