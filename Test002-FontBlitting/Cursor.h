#ifndef CURSOR_H
#define CURSOR_H

class Cursor
{
private:
    int m_Line;
    int m_Row;
    int m_Column;

public:
    Cursor();

    int getLine() const;
    void setLine(const int &line);

    int getRow() const;
    void setRow(const int &row);

    int getColumn() const;
    void setColumn(const int &column);
};

#endif // CURSOR_H
