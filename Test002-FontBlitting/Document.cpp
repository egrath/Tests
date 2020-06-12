#include "Document.h"

void Document::drawRulers(QPainter &painter)
{
    qDebug() << "Document::drawRulers";

    QPen rulerPen(QColor(64,64,64));
    painter.setPen(rulerPen);

    QBrush rulerBrush(QColor(222,222,222));
    painter.fillRect(0,0,m_RulerWidth-1,this->height(),rulerBrush);
    painter.drawLine(m_RulerWidth-1,0,m_RulerWidth-1,this->height());

    painter.fillRect(m_RulerWidth,0,this->width(),m_RulerHeight-1,rulerBrush);
    painter.drawLine(m_RulerWidth,m_RulerHeight-1,this->width(),m_RulerHeight-1);
}

void Document::drawLine(Line &line, QPainter &painter)
{
    int yDraw = 0;

    // Draw the Above Baseline rows
    int rowsAboveBaseline = line.getMaxRowsAbove();
    if(rowsAboveBaseline>0)
    {
        int heightAbove = rowsAboveBaseline * m_LineRowHeight;
        QBrush lineBackgroundAbove(QColor(0,255,0,32));
        painter.fillRect(0,yDraw,this->width(),heightAbove,lineBackgroundAbove);
        yDraw+=rowsAboveBaseline*m_LineRowHeight;
    }

    // Draw the Baseline rows
    QBrush lineBackgroundNormal(QColor(255,0,0,32));
    painter.fillRect(0,yDraw,this->width(),m_LineRowHeight*4,lineBackgroundNormal);
    yDraw+=m_LineRowHeight*4;

    // Draw the below Baseline rows
    int rowsBelowBaseline = line.getMaxRowsBelow();
    if(rowsBelowBaseline>0)
    {
        QBrush lineBackgroundBelow(QColor(0,0,255,32));
        painter.fillRect(0,yDraw,this->width(),rowsBelowBaseline*m_LineRowHeight,lineBackgroundBelow);
    }
    yDraw+=rowsBelowBaseline*m_LineRowHeight;

    QPen lineSeparatorPen = QPen(QColor(64,64,64));
    lineSeparatorPen.setStyle(Qt::DashLine);
    painter.setPen(lineSeparatorPen);
    painter.drawLine(0,yDraw,this->width(),yDraw);

    // Draw Glyphs
    int glyphWidth = m_FontCache->getGlyphSize().width();
    yDraw = rowsAboveBaseline*m_LineRowHeight;
    QList<Cell>::iterator iter;
    for(iter=line.getCells().begin();iter!=line.getCells().end();iter++)
    {
        painter.drawImage(
            (*iter).getLocation().x() * glyphWidth,
            yDraw + (*iter).getLocation().y()*m_LineRowHeight*(-1),
            m_FontCache->getGlyphImage((*iter).getGlyph())
         );
    }
}

void Document::drawCursor(QPainter &painter)
{
    QPen cursorPen(QColor(0,0,255));

    painter.setPen(cursorPen);

    int yPos = calculateLinePosition(m_Cursor.getLine());
    Line *currentLine = m_Lines.at(m_Cursor.getLine());
    yPos += currentLine->getMaxRowsAbove() * m_LineRowHeight;
    yPos += m_Cursor.getRow() * m_LineRowHeight * (-1);

    int cursorWidth = m_FontCache->getGlyphSize().width();
    int cursorHeight = m_LineRowHeight * 4;
    int xPos = cursorWidth * m_Cursor.getColumn();

    painter.drawRect(xPos,yPos,cursorWidth,cursorHeight);
}

// Calculate the y-Start of a given line index
int Document::calculateLinePosition(int line) const
{
    if(line>m_Lines.count())
        return -1;

    int yPos = 0;
    for(int i=0;i<line;i++)
    {
        yPos+=calculateLineHeight(*(m_Lines.at(i)));
        qDebug() << "ypos at" << i << yPos;
    }

    return yPos;
}

// Calculate the complete height in pixels of a given line
int Document::calculateLineHeight(const Line &line) const
{
    int lineHeight = m_LineRowHeight * 4;

    lineHeight += line.getMaxRowsAbove() * m_LineRowHeight;
    lineHeight += line.getMaxRowsBelow() * m_LineRowHeight;

    return lineHeight;
}

// Calculate the entire document height
int Document::calculateDocumentHeight() const
{
    int height = 0;
    QList<Line *>::const_iterator iter;
    for(iter=m_Lines.constBegin();iter!=m_Lines.constEnd();iter++)
    {
        height+=calculateLineHeight(**iter);
    }

    return height;
}

void Document::insertGlyphAtCursor(const QString &glyph)
{
    Cell c = Cell(glyph,QPoint(m_Cursor.getColumn(),m_Cursor.getRow()));

    Line *l = m_Lines.at(m_Cursor.getLine());
    l->insertSingleCell(c);

    OnCellInserted(*l,c);
}

void Document::OnCellInserted(const Line &line, const Cell &cell)
{
    setMinimumHeight(calculateDocumentHeight());
}

void Document::OnLineInserted(const Line &line)
{
    setMinimumHeight(calculateDocumentHeight());
}

void Document::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QElapsedTimer timer;
    timer.start();

    QPainter painter(this);            
    painter.fillRect(this->rect(), QBrush(QColor(Qt::white)));

    /*
    drawRulers(painter);
    painter.translate(m_RulerWidth,m_RulerHeight);
    */

    // Draw all the lines
    painter.save();
    QList<Line *>::iterator iter;
    for(iter=m_Lines.begin();iter!=m_Lines.end();iter++)
    {
        drawLine(**iter,painter);
        painter.translate(0,calculateLineHeight(**iter));
    }
    painter.restore();

    drawCursor(painter);

    qDebug() << "Rendering took" << timer.elapsed() << "ms";
}

void Document::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    qDebug() << event->size();
}

void Document::keyPressEvent(QKeyEvent *event)
{
    event->accept();

    qDebug() << "Key event for key" << event;

    // Navigate in the Rows
    if(event->key()==Qt::Key_PageUp && event->modifiers().testFlag(Qt::ControlModifier))
    {
        m_Cursor.setRow(m_Cursor.getRow()+1);

        // If the cursor row is above the current line row, we insert a new line row
        if(m_Cursor.getRow()>m_Lines.at(m_Cursor.getLine())->getMaxRowsAbove())
        {
            Cell empty(QPoint(m_Cursor.getColumn(),m_Cursor.getRow()));

            Line *l = m_Lines.at(m_Cursor.getLine());
            l->insertSingleCell(empty);

            OnCellInserted(*l,empty);
        }
    }

    if(event->key()==Qt::Key_PageDown && event->modifiers().testFlag(Qt::ControlModifier))
    {
        m_Cursor.setRow(m_Cursor.getRow()-1);
        if(m_Cursor.getRow()<(m_Lines.at(m_Cursor.getLine())->getMaxRowsBelow()*(-1)))
        {
            Cell empty(QPoint(m_Cursor.getColumn(),m_Cursor.getRow()));

            Line *l = m_Lines.at(m_Cursor.getLine());
            l->insertSingleCell(empty);

            OnCellInserted(*l,empty);
        }
    }

    // Regular navigation
    if(event->key()==Qt::Key_Down)
    {
        if(m_Cursor.getRow()!=0)
            m_Cursor.setRow(0);

        // Clear empty cells before leaving the line
        m_Lines.at(m_Cursor.getLine())->removeEmptyCells();

        // Now set the new line
        m_Cursor.setLine(m_Cursor.getLine()+1);

        // Do we need to append a new line to the document?
        if(m_Cursor.getLine()>=m_Lines.count())
        {
            Line *l = new Line(m_DocumentWidth);
            m_Lines.append(l);
            OnLineInserted(*l);
        }
    }

    if(event->key()==Qt::Key_Up)
    {
        if(m_Cursor.getRow()!=0)
            m_Cursor.setRow(0);

        // Clear empty cells before leaving the line
        m_Lines.at(m_Cursor.getLine())->removeEmptyCells();

        // Now set the new line
        m_Cursor.setLine(m_Cursor.getLine()-1);

        // Do we need to insert a new line at the beginning of the document?
        if(m_Cursor.getLine()<0)
        {
            Line* l = new Line(m_DocumentWidth);
            m_Lines.insert(0,l);
            m_Cursor.setLine(0);
            OnLineInserted(*l);
        }
    }

    if(event->key()==Qt::Key_Left)
    {
        if(m_Cursor.getRow()!=0)
            m_Cursor.setRow(0);

        if(m_Cursor.getColumn()>0)
            m_Cursor.setColumn(m_Cursor.getColumn()-1);

        m_Lines.at(m_Cursor.getLine())->removeEmptyCells();
    }

    if(event->key()==Qt::Key_Right)
    {
        if(m_Cursor.getRow()!=0)
            m_Cursor.setRow(0);

        if(m_Cursor.getColumn()<(m_DocumentWidth-1))
            m_Cursor.setColumn(m_Cursor.getColumn()+1);

        m_Lines.at(m_Cursor.getLine())->removeEmptyCells();
    }

    // Check for regular text
    if(event->text().length()>=1 && (event->modifiers().testFlag(Qt::NoModifier) || event->modifiers().testFlag(Qt::ShiftModifier)))
    {
        insertGlyphAtCursor(event->text());
        m_Cursor.setColumn(m_Cursor.getColumn()+1);
    }

    repaint();
}

Document::Document(QWidget *parent) : QWidget(parent)
{
    m_FontCache = new FontCache("Liberation Mono", 20);
    m_LineRowHeight = m_FontCache->getGlyphSize().height() / 4;

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    m_DocumentWidth = 144;
    setMinimumWidth(m_DocumentWidth*m_FontCache->getGlyphSize().width());

    Line *firstLine = new Line(m_DocumentWidth);
    m_Lines.append(firstLine);
    OnLineInserted(*firstLine);

}

Document::~Document()
{
    if(m_FontCache!=nullptr)
        delete m_FontCache;

    QList<Line *>::iterator iter;
    for(iter=m_Lines.begin();iter!=m_Lines.end();iter++)
    {
        delete *iter;
    }
}
