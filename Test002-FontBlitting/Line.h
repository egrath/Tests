#ifndef LINE_H
#define LINE_H

#include <QObject>
#include <QList>

#include <QDebug>

#include "Cell.h"

class Line : public QObject
{
    Q_OBJECT

private:
    QList<Cell> m_Cells;
    int m_LineWidth;
    int m_MaxRowsAboveInLine;
    int m_MaxRowsBelowInLine;

    void recalculateRowsAboveAndBelow();

public:
    Line(const int width=60);
    virtual ~Line();

    void removeEmptyCells();
    void insertSingleCell(const Cell &cell);
    int getMaxRowsAbove() const;
    int getMaxRowsBelow() const;
    QList<Cell> & getCells();

    int getLineWidth() const;
    void setLineWidth(const int &width);

signals:
    void cellInserted(const Line &line, const Cell &cell);
};

#endif // LINE_H
