#ifndef CELL_H
#define CELL_H

#include <QString>
#include <QPoint>

class Cell
{
private:
    QString m_Glyph;
    QPoint m_Location;
    bool m_IsEmpty;

public:
    Cell(const QPoint &location);
    Cell(const QString &glyph, const QPoint &location);

    bool isEmpty() const;

    QString getGlyph() const;
    QPoint getLocation() const;
};

#endif // CELL_H
