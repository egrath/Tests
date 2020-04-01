using System;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Text;

namespace Test001
{
    internal sealed class WorkbookCanvas : Panel
    {
        private Font m_Font;
        private float m_FontSize = 12.0f;
        private Size m_FontDimension;
        private int m_FontPadding = 4;

        private int m_GridLineStrength = 1;

        private Size m_CursorSize;
        private Pen m_CursorPen;

        private Workbook m_Workbook;

        private PrivateFontCollection m_FontCollection;

        public WorkbookCanvas() : base()
        {
            DoubleBuffered = true;

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

            m_Workbook = new Workbook();
            m_Workbook.CursorPositionChanged += OnWorkbookCursorPositionChanged;

            SetupFont();
            ResizeToFitContent();
        }

        private void OnWorkbookCursorPositionChanged(object sender, Point p)
        {
            ScrollCursorPositionIntoView();
        }

        /// <summary>
        /// Ändert die Größe des Views auf die Workbook Größe
        /// </summary>
        private void ResizeToFitContent()
        {
            Size size = new Size();
            size.Width = (m_FontDimension.Width + m_FontPadding + m_GridLineStrength) * m_Workbook.Size.Width;
            size.Height = (m_FontDimension.Height + m_FontPadding + m_GridLineStrength) * m_Workbook.Size.Height;

            this.Size = size;
        }

        private void SetupFont()
        {
            m_Font = new Font(m_FontCollection.Families[0], m_FontSize, FontStyle.Regular, GraphicsUnit.Point);

            Graphics g = this.CreateGraphics();

            StringFormat format = StringFormat.GenericTypographic;
            format.FormatFlags = StringFormatFlags.MeasureTrailingSpaces & StringFormatFlags.NoClip & StringFormatFlags.FitBlackBox;
            g.TextRenderingHint = TextRenderingHint.AntiAlias;
            SizeF dim = g.MeasureString("A", m_Font, int.MaxValue, format);
            m_FontDimension = new Size((int) dim.Width, (int) dim.Height);

            // Cursor Größe und Pen
            m_CursorSize = new Size(m_FontDimension.Width + m_FontPadding + m_GridLineStrength, m_FontDimension.Height + m_FontPadding + m_GridLineStrength);
            m_CursorPen = new Pen(Brushes.DarkOrange, m_GridLineStrength * 2);

            g.Dispose();
        }

        /// <summary>
        /// Gibt für eine logische Workbook Koordinate die Pixel Koordinate in der View zurück
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <returns></returns>
        private Point CoordinatesForWorkbookPosition(int x, int y)
        {
            Point p = new Point(0, 0);

            p.X = (x * m_GridLineStrength) + (x * m_FontDimension.Width) + (m_FontPadding * x) + m_FontPadding/2;
            p.Y = (y * m_GridLineStrength) + (y * m_FontDimension.Height) + (m_FontPadding * y) + m_FontPadding / 2;

            return p;
        }

        /// <summary>
        /// Gibt für eine Pixel-Koordinate des Views die logische Koordinat eim Workbook zurück
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <returns></returns>
        private Point WorkbookPositionForCoordinates(int x, int y)
        {
            int cellWidth = m_FontDimension.Width + m_FontPadding + m_GridLineStrength;
            int cellHeight = m_FontDimension.Height + m_FontPadding + m_GridLineStrength;

            return new Point(x / cellWidth, y / cellHeight);
        }

        /// <summary>
        /// Gibt die Pixel-Koordinates der aktuellen Cursorposition des Workbooks zurück
        /// </summary>
        /// <returns></returns>
        private Point CoordinatesForCursorPosition()
        {
            Point cursorPosition = CoordinatesForWorkbookPosition(m_Workbook.CursorPosition.X, m_Workbook.CursorPosition.Y);
            cursorPosition.X -= m_FontPadding / 2;
            cursorPosition.Y -= m_FontPadding / 2;

            return cursorPosition;
        }

        /// <summary>
        /// Scrollt so, dass der aktuelle Cursor sichtbar ist.
        /// </summary>
        private void ScrollCursorPositionIntoView()
        {
            Point cursorPosition = CoordinatesForCursorPosition();
            ScrollableControl parent = Parent as ScrollableControl;

            int horizontalMargin = 4 * m_CursorSize.Width;
            int verticalMargin = 2 * m_CursorSize.Height;

            // Müssen wir nach Rechts scrollen?
            if((cursorPosition.X+horizontalMargin) > (parent.ClientRectangle.Width + parent.HorizontalScroll.Value))
            {
                int newScrollHorizontal = cursorPosition.X + horizontalMargin - parent.ClientRectangle.Width;
                parent.HorizontalScroll.Value = newScrollHorizontal;
            }

            // Müssen wir nach Links scrollen?
            if((cursorPosition.X-horizontalMargin) < parent.HorizontalScroll.Value)
            {
                int newScrollHorizontal = cursorPosition.X - horizontalMargin;
                parent.HorizontalScroll.Value = newScrollHorizontal >= 0 ? newScrollHorizontal : 0;
            }

            // Müssen wir nach unten scrollen?
            if((cursorPosition.Y + verticalMargin) > (parent.ClientRectangle.Height + parent.VerticalScroll.Value))
            {
                int newScrollVertical = cursorPosition.Y + verticalMargin - parent.ClientRectangle.Height;
                parent.VerticalScroll.Value = newScrollVertical;
            }

            // Müssen wir nach oben scrollen?
            if((cursorPosition.Y - verticalMargin) < parent.VerticalScroll.Value)
            {
                int newScrollVertical = cursorPosition.Y - verticalMargin;
                parent.VerticalScroll.Value = newScrollVertical >= 0 ? newScrollVertical : 0;
            }
        }

        protected override void OnMouseClick(MouseEventArgs e)
        {
            base.OnMouseClick(e);

            Point p = WorkbookPositionForCoordinates(e.Location.X, e.Location.Y);

            m_Workbook.CursorPosition = p;

            this.Invalidate();
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);

            Point cursorPosition = m_Workbook.CursorPosition;
            switch(e.KeyCode)
            {
                case Keys.Up:
                    cursorPosition.Y--;
                    break;

                case Keys.Down:
                    cursorPosition.Y++;
                    break;

                case Keys.Left:
                    cursorPosition.X--;
                    break;

                case Keys.Right:
                    cursorPosition.X++;
                    break;

                default:
                    break;
            }

            m_Workbook.CursorPosition = cursorPosition;
            
            this.Invalidate();
        }

        protected override void OnKeyPress(KeyPressEventArgs e)
        {
            base.OnKeyPress(e);

            m_Workbook.SetString(new string(e.KeyChar, 1), m_Workbook.CursorPosition.X, m_Workbook.CursorPosition.Y);
        }

        protected override void OnMouseEnter(EventArgs e)
        {            
            base.OnMouseEnter(e);
            Focus();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            Graphics g = e.Graphics;
            g.FillRectangle(Brushes.White, this.ClientRectangle);

            // Grid zeichnen
            Pen gridPen = new Pen(new SolidBrush(Color.FromArgb(230,230,230)), m_GridLineStrength);
            gridPen.DashPattern = new float[] { m_GridLineStrength * m_FontPadding, m_GridLineStrength * m_FontPadding };
            for(float x = m_FontDimension.Width + m_FontPadding; x <= ClientRectangle.Width; x += m_FontDimension.Width + m_GridLineStrength + m_FontPadding)
                g.DrawLine(gridPen, x, 0, x, ClientRectangle.Height);

            for(float y = m_FontDimension.Height + m_FontPadding; y <= ClientRectangle.Height; y += m_FontDimension.Height + m_GridLineStrength + m_FontPadding)
                g.DrawLine(gridPen, 0, y, ClientRectangle.Width, y);

            // Workbook zeichnen
            StringFormat format = StringFormat.GenericTypographic;
            format.Alignment = StringAlignment.Near;
            format.LineAlignment = StringAlignment.Near;
            format.Trimming = StringTrimming.None;
            format.FormatFlags = StringFormatFlags.NoClip & StringFormatFlags.FitBlackBox;
            g.TextRenderingHint = TextRenderingHint.AntiAlias | TextRenderingHint.ClearTypeGridFit;

            for(int col = 0; col < m_Workbook.Size.Width; col++)
                for(int row = 0; row < m_Workbook.Size.Height; row++)
                    g.DrawString(new string(m_Workbook[col,row], 1), m_Font, Brushes.Black, CoordinatesForWorkbookPosition(col, row), format);

            // Cursor zeichnen
            Point cursorPosition = CoordinatesForCursorPosition();
            g.DrawRectangle(m_CursorPen, new Rectangle(cursorPosition, m_CursorSize));
        }
    }
}