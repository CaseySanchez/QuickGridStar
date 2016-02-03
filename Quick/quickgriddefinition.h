#ifndef QUICKGRIDDEFINITION_H
#define QUICKGRIDDEFINITION_H

#include <QObject>
#include <QWidget>
#include <QList>

class QuickGridDefinition
{
    struct QuickGridCell
    {
        float
            _weight,
            _offset;

        QuickGridCell(float weight, float offset) :
            _weight(weight),
            _offset(offset)
        {
        }
    };

    QList<QuickGridCell>
        _rows,
        _columns;

    float
        _rowSize,
        _columnSize,
        _invRowSize,
        _invColumnSize;

public:
    QuickGridDefinition();

    void addRowDefinition(float size = 1.0f);
    void addColumnDefinition(float size = 1.0f);

    int rowCount() const;
    int columnCount() const;

    float rowOffset(int index);
    float columnOffset(int index);

    float rowWeight(int index);
    float columnWeight(int index);

    QPointF cellPoint(QRectF rect, int row, int column);
    QSizeF cellSize(QRectF rect, int row, int column, int rowSpan, int columnSpan);
    QRectF cellRect(QRectF rect, int row, int column, int rowSpan = 1, int columnSpan = 1);
};

#endif // QUICKGRIDDEFINITION_H
