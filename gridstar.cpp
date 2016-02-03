#include "gridstar.h"

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
    calculateSpan(row, column, rowSpan, columnSpan);

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

void GridStarLayout::calculateSpan(int &row, int &column, int &rowSpan, int &columnSpan)
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
