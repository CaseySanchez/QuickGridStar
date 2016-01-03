#ifndef GRIDSTAR_H
#define GRIDSTAR_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QList>
#include <QDebug>

class GridDefinition
{
    friend class GridStarLayout;

    struct GridCell
    {
        float _weight;
        float _offset;

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

protected:
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

class GridStarLayout : public QLayout
{
    Q_OBJECT

    struct GridItem
    {
        QLayoutItem
            *_item;
        int
            _row,
            _column,
            _rowSpan,
            _columnSpan;

        GridItem(QLayoutItem *item, int row, int column, int rowSpan, int columnSpan) :
            _item(item),
            _row(row),
            _column(column),
            _rowSpan(rowSpan),
            _columnSpan(columnSpan)
        {
        }

        void setGeometry(const QRect &rect)
        {
            _item->setGeometry(rect);
        }

        QSize sizeHint() const
        {
            return _item->sizeHint();
        }

        QSize minimumSize() const
        {
            return _item->minimumSize();
        }
    };

    QList<GridItem *>
        _items;

    GridDefinition
        _gridDefinition;

public:
    explicit GridStarLayout(GridDefinition &gridDefinition, QWidget *parent = 0);
    ~GridStarLayout();

    void addItem(QLayoutItem *item);
    void addWidget(QWidget *widget, int row, int column, int rowSpan = 1, int columnSpan = 1);
    QWidget *addLayout(QLayout *layout, int row, int column, int rowSpan = 1, int columnSpan = 1);
    void setGeometry(const QRect &rect);
    QSize sizeHint() const;
    QSize minimumSize() const;
    QLayoutItem *itemAt(int index) const;
    QLayoutItem *takeAt(int index);
    int count() const;

private:
    void applyBounds(int &row, int &column, int &rowSpan, int &columnSpan);
};

#endif // GRIDSTAR_H
