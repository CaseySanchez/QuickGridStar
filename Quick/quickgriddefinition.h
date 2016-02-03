#ifndef QUICKGRIDDEFINITION_H
#define QUICKGRIDDEFINITION_H

#include <QObject>
#include <QWidget>
#include <QList>

class QuickGridDefinition
{
    struct QuickGridCell
    {
        qreal
            _weight,
            _offset;

        QuickGridCell(qreal weight, qreal offset) :
            _weight(weight),
            _offset(offset)
        {
        }
    };

    QList<QuickGridCell>
        _rows,
        _columns;

    qreal
        _rowSize,
        _columnSize,
        _invRowSize,
        _invColumnSize;

public:
    QuickGridDefinition();

    void addRowDefinition(qreal size = 1.0f);
    void addColumnDefinition(qreal size = 1.0f);

    int rowCount() const;
    int columnCount() const;

    qreal rowOffset(int index);
    qreal columnOffset(int index);

    qreal rowWeight(int index);
    qreal columnWeight(int index);

    QPointF cellPoint(QRectF rect, int row, int column);
    QSizeF cellSize(QRectF rect, int row, int column, int rowSpan, int columnSpan);
    QRectF cellRect(QRectF rect, int row, int column, int rowSpan = 1, int columnSpan = 1);

    void calculateSpan(int row, int column, int &rowSpan, int &columnSpan);
};

#endif // QUICKGRIDDEFINITION_H
