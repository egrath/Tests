using System;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Text;

namespace Test001
{
    internal sealed class Canvas : Panel
    {
        private Font m_Font;
        private float m_FontSize = 12.0f;
        private PrivateFontCollection m_FontCollection;

        public Canvas() : base()
        {
            // Hinzufügen einer Font aus einer Ressource zu einer PrivateFontCollection
            // und laden der Font in ein Font-Objekt
            m_FontCollection = new PrivateFontCollection();
            unsafe
            {
                fixed (byte* pFontData = Test001.Resources.DejaVuSansMono)
                {
                    m_FontCollection.AddMemoryFont((IntPtr)pFontData, Test001.Resources.DejaVuSansMono.Length);
                }
            }

            foreach(FontFamily familiy in m_FontCollection.Families)
                Console.Out.WriteLine("PFC: [{0}], Regular: {1}, Bold: {2}, Italic: {3}", familiy.Name,
                    familiy.IsStyleAvailable(FontStyle.Regular), familiy.IsStyleAvailable(FontStyle.Bold), familiy.IsStyleAvailable(FontStyle.Italic));

            m_Font = new Font(m_FontCollection.Families[0], m_FontSize, FontStyle.Regular, GraphicsUnit.Point);
        }

        protected override void OnMouseWheel(MouseEventArgs e)
        {
            base.OnMouseWheel(e);

            if(e.Delta > 0)
            {
                m_FontSize += 1.0f;
            }
            else
            {
                m_FontSize -= 1.0f;
            }

            m_Font = new Font(m_FontCollection.Families[0], m_FontSize, FontStyle.Regular, GraphicsUnit.Point);
            this.Invalidate();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            Graphics g = e.Graphics;
            g.FillRectangle(Brushes.White, this.ClientRectangle);

            string demo = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";

            StringFormat format = StringFormat.GenericTypographic;
            format.Alignment = StringAlignment.Near;
            format.LineAlignment = StringAlignment.Near;
            format.Trimming = StringTrimming.None;
            format.FormatFlags = StringFormatFlags.MeasureTrailingSpaces & StringFormatFlags.NoClip & StringFormatFlags.FitBlackBox;
            g.TextRenderingHint = TextRenderingHint.AntiAlias;
            SizeF textSize = g.MeasureString(new string(demo[0],1), m_Font, int.MaxValue,format);

            Console.Out.WriteLine("Glyph size: {0}", textSize);

            // String als einzelnen Glyph zeichnen
            float xStart = 100.0f;
            float padding = 3.0f;
            for(int i=0;i<demo.Length;i++)
            {
                // Zeichenposition errechnet sich aus:
                // Offset 100 + breite eines Glyphs * nummer des aktuell zu zeichnenden Glyphs + padding für linie dazwischen (1)
                float x = xStart + (padding * i) + (textSize.Width * i);
                g.DrawString(new string(demo[i],1), m_Font, Brushes.Black, x, 100,format);
                g.DrawLine(Pens.Red, x - 1, 80, x - 1,100+textSize.Height+20);
            }

            // Höhenlinien zeichnen
            float stringWidth = (demo.Length * textSize.Width) + (demo.Length * padding);
            g.DrawLine(Pens.Blue, 100, 100, stringWidth+100, 100);
            g.DrawLine(Pens.Blue, 100, 100 + textSize.Height, stringWidth + 100, 100 + textSize.Height);
        }
    }
}