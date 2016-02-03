#include "griddefinition.h"

GridDefinition::GridDefinition() :
    _rowSize(0.0f),
    _columnSize(0.0f),
    _invRowSize(0.0f),
    _invColumnSize(0.0f)
{
}

void GridDefinition::addRowDefinition(qreal size)
{
    _rows << GridCell(size, _rowSize);

    _rowSize += size;
    _invRowSize = 1.0f / _rowSize;
}

void GridDefinition::addColumnDefinition(qreal size)
{
    _columns << GridCell(size, _columnSize);

    _columnSize += size;
    _invColumnSize = 1.0f / _columnSize;
}

int GridDefinition::rowCount() const
{
    return _rows.size();
}

int GridDefinition::columnCount() const
{
    return _columns.size();
}

qreal GridDefinition::rowOffset(int index)
{
    return _rows[index]._offset * _invRowSize;
}

qreal GridDefinition::columnOffset(int index)
{
    return _columns[index]._offset * _invColumnSize;
}

qreal GridDefinition::rowWeight(int index)
{
    return _rows[index]._weight * _invRowSize;
}

qreal GridDefinition::columnWeight(int index)
{
    return _columns[index]._weight * _invColumnSize;
}

QPoint GridDefinition::cellPoint(QRect rect, int row, int column)
{
    return QPoint(columnOffset(column) * rect.width() + rect.x(), rowOffset(row) * rect.height() + rect.y());
}

QSize GridDefinition::cellSize(QRect rect, int row, int column, int rowSpan, int columnSpan)
{
    QSize
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

QRect GridDefinition::cellRect(QRect rect, int row, int column, int rowSpan, int columnSpan)
{
    return QRect(cellPoint(rect, row, column), cellSize(rect, row, column, rowSpan, columnSpan));
}
