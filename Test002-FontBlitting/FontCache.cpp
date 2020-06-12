#include "FontCache.h"

// PRIVATE
void FontCache::renderGlyph(const QString &glyph)
{
    qDebug() << "FontCache: Rendering Glyph" << glyph << "to cache";

    QImage *glyphImage = new QImage(m_GlyphSize.width(),m_GlyphSize.height(),QImage::Format_ARGB32_Premultiplied);
    glyphImage->fill(Qt::transparent);

    QPainter painter(glyphImage);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::black);
    painter.setFont(m_Font);

    painter.drawText(m_FontMetrics.leftBearing('X'),m_FontMetrics.ascent(),glyph);

    m_Cache.insert(glyph,glyphImage);
}

// PUBLIC
FontCache::FontCache(const QString &fontName, const int &fontSize)
{
    qDebug() << "FontCache: ctor()";

    m_Font = QFont(fontName,fontSize);
    m_FontMetrics = QFontMetrics(m_Font);

    m_GlyphSize = QSize(
        m_FontMetrics.horizontalAdvance('X') + m_FontMetrics.leftBearing('X') + m_FontMetrics.rightBearing('X'),
        m_FontMetrics.ascent() + m_FontMetrics.descent()
    );    
}

FontCache::~FontCache()
{
    QMap<QString,QImage *>::const_iterator iter;

    for(iter=m_Cache.constBegin();iter!=m_Cache.constEnd();iter++)
        delete *iter;
}

QImage & FontCache::getGlyphImage(const QString &glyph)
{
    if(!m_Cache.contains(glyph))
        renderGlyph(glyph);

    return *(m_Cache[glyph]);
}

QSize & FontCache::getGlyphSize()
{
    return m_GlyphSize;
}
