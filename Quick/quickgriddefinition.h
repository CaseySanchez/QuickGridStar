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

    void addRowDefinition(qreal weight = 1.0f, qint32 row = -1);
    void addColumnDefinition(qreal weight = 1.0f, qint32 column = -1);

    void removeRowDefinition(qint32 row = -1);
    void removeColumnDefinition(qint32 column = -1);

    qint32 rowCount() const;
    qint32 columnCount() const;

    qreal rowOffset(qint32 index);
    qreal columnOffset(qint32 index);

    qreal rowWeight(qint32 index);
    qreal columnWeight(qint32 index);

    QPointF cellPoint(QRectF rect, qint32 row, qint32 column);
    QSizeF cellSize(QRectF rect, qint32 row, qint32 column, qint32 rowSpan, qint32 columnSpan);
    QRectF cellRect(QRectF rect, qint32 row, qint32 column, qint32 rowSpan = 1, qint32 columnSpan = 1);

    void calculateBounds(qint32 &row, qint32 &column, qint32 &rowSpan, qint32 &columnSpan);
};

#endif // QUICKGRIDDEFINITION_H
