#ifndef DOCUMENTRENDERER_H
#define DOCUMENTRENDERER_H

#include <QWidget>
#include <QAbstractScrollArea>
#include <QScrollBar>
#include <QElapsedTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QFont>
#include <QFontMetrics>
#include <QVector>

#include <QDebug>

#include "FontCache.h"
#include "Cell.h"
#include "Line.h"
#include "Cursor.h"

class Document : public QWidget
{
private:
    const int m_RulerWidth = 20;
    const int m_RulerHeight = 20;

    FontCache *m_FontCache=nullptr;
    QList<Line *> m_Lines;
    int m_LineRowHeight;
    int m_DocumentWidth;
    Cursor m_Cursor;

    void drawRulers(QPainter &painter);
    void drawLine(Line &line, QPainter &painter);
    void drawCursor(QPainter &painter);

    int calculateLinePosition(int line) const;
    int calculateLineHeight(const Line &line) const;
    int calculateDocumentHeight() const;

    void insertGlyphAtCursor(const QString &glyph);

private slots:
    void OnCellInserted(const Line &line, const Cell &cell);
    void OnLineInserted(const Line &line);

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void keyPressEvent(QKeyEvent *event) override;

public:
    Document(QWidget *parent=nullptr);
    ~Document();
};

#endif
