#ifndef QQUICKGRIDSTAR_H
#define QQUICKGRIDSTAR_H

#include <QObject>
#include <QList>
#include <QRegularExpression>
#include <QQuickItem>
#include <QQuickWindow>

class QQuickGridStar;

class QQuickRowDefinition : public QQuickItem
{
    friend class QQuickGridStar;

    Q_OBJECT

    Q_PROPERTY(qreal weight READ getWeight WRITE setWeight NOTIFY rowChanged)

public:
    QQuickRowDefinition(QQuickItem *parent = nullptr);

    qreal getWeight();
    void setWeight(qreal weight);

signals:
    void rowChanged(QQuickRowDefinition *definition, const qreal &oldWeight);

protected:
    qreal
        m_weight,
        m_offset;
};

class QQuickColumnDefinition : public QQuickItem
{
    friend class QQuickGridStar;

    Q_OBJECT

    Q_PROPERTY(qreal weight READ getWeight WRITE setWeight NOTIFY columnChanged)

public:
    QQuickColumnDefinition(QQuickItem *parent = nullptr);

    qreal getWeight();
    void setWeight(qreal weight);

signals:
    void columnChanged(QQuickColumnDefinition *definition, const qreal &oldWeight);

protected:
    qreal
        m_weight,
        m_offset;
};

class QQuickGridStarAttached : public QObject
{
    friend class QQuickGridStar;

    Q_OBJECT

    Q_PROPERTY(bool ignore READ getIgnore WRITE setIgnore)
    Q_PROPERTY(qint32 row READ getRow WRITE setRow)
    Q_PROPERTY(qint32 column READ getColumn WRITE setColumn)
    Q_PROPERTY(qint32 rowSpan READ getRowSpan WRITE setRowSpan)
    Q_PROPERTY(qint32 columnSpan READ getColumnSpan WRITE setColumnSpan)

public:
    QQuickGridStarAttached(QObject *object);

    bool getIgnore();
    qint32 getRow();
    qint32 getColumn();
    qint32 getRowSpan();
    qint32 getColumnSpan();

    void setIgnore(bool ignore);
    void setRow(qint32 row);
    void setColumn(qint32 column);
    void setRowSpan(qint32 rowSpan);
    void setColumnSpan(qint32 columnSpan);

signals:
    void layoutChanged();

protected:
    bool
        m_dirty,
        m_ignore;
    qint32
        m_row,
        m_column,
        m_rowSpan,
        m_columnSpan,
        m_rowSpanActual,
        m_columnSpanActual;
};

class QQuickGridStar : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(qreal rowSpacing READ getRowSpacing WRITE setRowSpacing)
    Q_PROPERTY(qreal columnSpacing READ getColumnSpacing WRITE setColumnSpacing)

    QList<QQuickItem *>
        m_items;

    QList<QQuickRowDefinition *>
        m_rows;
    QList<QQuickColumnDefinition *>
        m_columns;

    qreal
        m_rowSize,
        m_columnSize,
        m_invRowSize,
        m_invColumnSize;

    qreal rowOffset(qint32 index);
    qreal columnOffset(qint32 index);

    qreal rowWeight(qint32 index);
    qreal columnWeight(qint32 index);

    QPointF cellPoint(QRectF rect, qint32 row, qint32 column);
    QSizeF cellSize(QRectF rect, qint32 row, qint32 column, qint32 rowSpan, qint32 columnSpan);
    QRectF cellRect(QRectF rect, qint32 row, qint32 column, qint32 rowSpan = 1, qint32 columnSpan = 1);

public:
    QQuickGridStar(QQuickItem *parent = nullptr);

    Q_INVOKABLE qint32 rowCount();
    Q_INVOKABLE qint32 columnCount();

    Q_INVOKABLE QVariant itemsAt(qint32 row, qint32 column);

    Q_INVOKABLE QQuickItem *getRow(qint32 row);
    Q_INVOKABLE QQuickItem *getColumn(qint32 column);

    Q_INVOKABLE QQuickItem *addRow(qreal weight);
    Q_INVOKABLE QQuickItem *addColumn(qreal weight);

    Q_INVOKABLE void clearRows();
    Q_INVOKABLE void clearColumns();
    Q_INVOKABLE void clearItems();

    qreal getRowSpacing();
    qreal getColumnSpacing();

    void setRowSpacing(qreal rowSpacing);
    void setColumnSpacing(qreal columnSpacing);

    static QQuickGridStarAttached *qmlAttachedProperties(QObject *object);

protected slots:
    void onRowChanged(QQuickRowDefinition *definition, const qreal &oldWeight);
    void onColumnChanged(QQuickColumnDefinition *definition, const qreal &oldWeight);

protected:
    void componentComplete() Q_DECL_OVERRIDE;
    void itemChange(ItemChange change, const ItemChangeData &value) Q_DECL_OVERRIDE;
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) Q_DECL_OVERRIDE;
    void updatePolish() Q_DECL_OVERRIDE;

    qreal
        m_rowSpacing,
        m_columnSpacing;
};

QML_DECLARE_TYPEINFO(QQuickGridStar, QML_HAS_ATTACHED_PROPERTIES)
QML_DECLARE_TYPE(QQuickRowDefinition)
QML_DECLARE_TYPE(QQuickColumnDefinition)

void registerQuickGridStarTypes();

#endif // QQUICKGRIDSTAR_H
