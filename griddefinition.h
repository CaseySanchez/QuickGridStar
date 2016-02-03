#ifndef GRIDDEFINITION_H
#define GRIDDEFINITION_H

#include <QList>

class GridDefinition
{
    struct GridCell
    {
        float
            _weight,
            _offset;

        GridCell(float weight, float offset) :
            _weight(weight),
            _offset(offset)
        {
        }
    };

    QList<GridCell>
        _rows,
        _columns;

    float
        _rowSize,
        _columnSize,
        _invRowSize,
        _invColumnSize;

public:
    GridDefinition();

    void addRowDefinition(float size = 1.0f);
    void addColumnDefinition(float size = 1.0f);

    int rowCount() const;
    int columnCount() const;

    float rowOffset(int index);
    float columnOffset(int index);

    float rowWeight(int index);
    float columnWeight(int index);

    QPoint cellPoint(QRect rect, int row, int column);
    QSize cellSize(QRect rect, int row, int column, int rowSpan, int columnSpan);
    QRect cellRect(QRect rect, int row, int column, int rowSpan = 1, int columnSpan = 1);
};

#endif // GRIDDEFINITION_H
