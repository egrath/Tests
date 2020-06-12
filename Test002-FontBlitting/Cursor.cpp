#include "Cursor.h"

Cursor::Cursor() : m_Line(0), m_Row(0), m_Column(0)
{
}

int Cursor::getLine() const
{
    return m_Line;
}

void Cursor::setLine(const int &line)
{
    m_Line = line;
}

int Cursor::getRow() const
{
    return m_Row;
}

void Cursor::setRow(const int &row)
{
    m_Row = row;
}

int Cursor::getColumn() const
{
    return m_Column;
}

void Cursor::setColumn(const int &column)
{
    m_Column = column;
}
