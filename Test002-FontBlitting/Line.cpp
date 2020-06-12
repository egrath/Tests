#include "Line.h"

void Line::recalculateRowsAboveAndBelow()
{
    m_MaxRowsAboveInLine = m_MaxRowsBelowInLine = 0;
    QList<Cell>::const_iterator iter;
    for(iter=m_Cells.constBegin();iter!=m_Cells.constEnd();iter++)
    {
        if((*iter).getLocation().y()>m_MaxRowsAboveInLine)
            m_MaxRowsAboveInLine=(*iter).getLocation().y();

        if((*iter).getLocation().y()<(m_MaxRowsBelowInLine*(-1)))
            m_MaxRowsBelowInLine=(*iter).getLocation().y()*(-1);
    }
}

Line::Line(const int width) : m_LineWidth(width), m_MaxRowsAboveInLine(0), m_MaxRowsBelowInLine(0)
{
}

Line::~Line()
{
}

void Line::removeEmptyCells()
{
    int i=0;
    while(i<m_Cells.count())
    {
        if(m_Cells.at(i).isEmpty())
            m_Cells.removeAt(i);
        else
            i++;
    }
}

void Line::insertSingleCell(const Cell &cell)
{
    m_Cells.append(cell);

    if(cell.getLocation().y()>m_MaxRowsAboveInLine)
        m_MaxRowsAboveInLine=cell.getLocation().y();

    if(cell.getLocation().y()<(m_MaxRowsBelowInLine*(-1)))
        m_MaxRowsBelowInLine=cell.getLocation().y()*(-1);

    // Fire event
    cellInserted(*this, cell);
}

int Line::getMaxRowsAbove() const
{
    return m_MaxRowsAboveInLine;
}

int Line::getMaxRowsBelow() const
{
    return m_MaxRowsBelowInLine;
}

QList<Cell> & Line::getCells()
{
    return m_Cells;
}

int Line::getLineWidth() const
{
    return m_LineWidth;
}


void Line::setLineWidth(const int &width)
{
    m_LineWidth=width;
}
