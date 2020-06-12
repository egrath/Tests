#ifndef FONTCACHE_H
#define FONTCACHE_H

#include <QDebug>
#include <QFontMetrics>
#include <QFont>
#include <QMap>
#include <QImage>
#include <QPainter>

class FontCache
{
private:
    QFont m_Font;
    QFontMetrics m_FontMetrics = QFontMetrics(QFont());
    QSize m_GlyphSize;    

    QMap<QString,QImage *> m_Cache;

    void renderGlyph(const QString &glyph);

public:
    FontCache(const QString &fontName, const int &fontSize);
    ~FontCache();

    QImage & getGlyphImage(const QString &glyph);
    QSize & getGlyphSize();
};

#endif // FONTCACHE_H
