#include "gridstar.h"

GridDefinition::GridDefinition() :
    _rowSize(0.0f),
    _columnSize(0.0f),
    _invRowSize(0.0f),
    _invColumnSize(0.0f)
{
}

void GridDefinition::addRowDefinition(float size)
{
    _rows << GridCell(size, _rowSize);

    _rowSize += size;
    _invRowSize = 1.0f / _rowSize;
}

void GridDefinition::addColumnDefinition(float size)
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

float GridDefinition::rowOffset(int index)
{
    return _rows[index]._offset * _invRowSize;
}

float GridDefinition::columnOffset(int index)
{
    return _columns[index]._offset * _invColumnSize;
}

float GridDefinition::rowWeight(int index)
{
    return _rows[index]._weight * _invRowSize;
}

float GridDefinition::columnWeight(int index)
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

GridStarLayout::GridStarLayout(GridDefinition &gridDefinition, QWidget *parent) :
    QLayout(parent),
    _gridDefinition(gridDefinition)
{
}

GridStarLayout::~GridStarLayout()
{
    QLayoutItem
        *item;

    while((item = takeAt(0)))
    {
        delete item;
    }
}

void GridStarLayout::addItem(QLayoutItem *item)
{
    _items << new GridItem(item, 0, 0, 1, 1);
}

void GridStarLayout::addWidget(QWidget *widget, int row, int column, int rowSpan, int columnSpan)
{
    applyBounds(row, column, rowSpan, columnSpan);

    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    _items << new GridItem(new QWidgetItem(widget), row, column, rowSpan, columnSpan);
}

QWidget *GridStarLayout::addLayout(QLayout *layout, int row, int column, int rowSpan, int columnSpan)
{
    applyBounds(row, column, rowSpan, columnSpan);

    QWidget *widget = new QWidget();

    _items << new GridItem(new QWidgetItem(widget), row, column, rowSpan, columnSpan);

    widget->setLayout(layout);

    return widget;
}

void GridStarLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);

    for(GridItem *item : _items)
    {
        item->setGeometry(_gridDefinition.cellRect(rect, item->_row, item->_column, item->_rowSpan, item->_columnSpan));
    }
}

QSize GridStarLayout::sizeHint() const
{
    QSize
        size = QSize(100, 100);

    for(GridItem *item : _items)
    {
        size = size.expandedTo(item->sizeHint());
    }

    return size;
}

QSize GridStarLayout::minimumSize() const
{
    QSize
        size = QSize(0, 0);

    for(GridItem *item : _items)
    {
        size = size.expandedTo(item->minimumSize());
    }

    return size;
}

QLayoutItem *GridStarLayout::itemAt(int index) const
{
    if(index < 0 || index >= _items.size())
    {
        return 0;
    }

    return _items[index]->_item;
}

QLayoutItem *GridStarLayout::takeAt(int index)
{
    if(index < 0 || index >= _items.size())
    {
        return 0;
    }

    GridItem
        *wrapper = _items.takeAt(index);
    QLayoutItem
        *item = wrapper->_item;

    delete wrapper;

    return item;
}

int GridStarLayout::count() const
{
    return _items.size();
}

void GridStarLayout::applyBounds(int &row, int &column, int &rowSpan, int &columnSpan)
{
    int
        rowCount = _gridDefinition.rowCount(),
        columnCount = _gridDefinition.columnCount();

    row = qBound(0, row, rowCount - 1);
    column = qBound(0, column, columnCount - 1);

    if(rowSpan <= 0)
    {
        rowSpan = rowCount - row + rowSpan;
    }

    if(columnSpan <= 0)
    {
        columnSpan = columnCount - column + columnSpan;
    }

    rowSpan = qBound(1, rowSpan, rowCount - row);
    columnSpan = qBound(1, columnSpan, columnCount - column);
}
