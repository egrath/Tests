#include "Cell.h"

Cell::Cell(const QPoint &location) : m_Location(location), m_IsEmpty(true)
{
}

Cell::Cell(const QString &glyph, const QPoint &location) : m_Glyph(glyph), m_Location(location), m_IsEmpty(false)
{
}

bool Cell::isEmpty() const
{
    return m_IsEmpty;
}

QPoint Cell::getLocation() const
{
    return m_Location;
}

QString Cell::getGlyph() const
{
    return m_Glyph;
}
