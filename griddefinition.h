#ifndef GRIDDEFINITION_H
#define GRIDDEFINITION_H

#include <QList>

class GridDefinition
{
    struct GridCell
    {
        qreal
            _weight,
            _offset;

        GridCell(qreal weight, qreal offset) :
            _weight(weight),
            _offset(offset)
        {
        }
    };

    QList<GridCell>
        _rows,
        _columns;

    qreal
        _rowSize,
        _columnSize,
        _invRowSize,
        _invColumnSize;

public:
    GridDefinition();

    void addRowDefinition(qreal size = 1.0f);
    void addColumnDefinition(qreal size = 1.0f);

    int rowCount() const;
    int columnCount() const;

    qreal rowOffset(int index);
    qreal columnOffset(int index);

    qreal rowWeight(int index);
    qreal columnWeight(int index);

    QPoint cellPoint(QRect rect, int row, int column);
    QSize cellSize(QRect rect, int row, int column, int rowSpan, int columnSpan);
    QRect cellRect(QRect rect, int row, int column, int rowSpan = 1, int columnSpan = 1);
};

#endif // GRIDDEFINITION_H
