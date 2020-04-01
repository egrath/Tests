using System;
using System.Drawing;

namespace Test001
{
    internal sealed class Workbook
    {
        // Events
        public delegate void CursorPositionChangedDelegate(object sender, Point newPosition);
        public event CursorPositionChangedDelegate CursorPositionChanged;

        private char[,] m_Data;

        private Point m_CursorPosition;

        public Workbook()
        {
            m_Data = new char[80, 60];
        }

        public Size Size
        {
            get
            {
                return new Size(m_Data.GetLength(0), m_Data.GetLength(1));
            }
        }

        public void SetString(string text, int x, int y)
        {
            foreach(char c in text)
                m_Data[x++, y] = c;

            CursorPosition = new Point(x, y);
        }

        /// <summary>
        /// Setzen der Cursorposition im Workbook
        /// </summary>
        public Point CursorPosition
        {
            get
            {
                return m_CursorPosition;
            }

            set
            {
                bool changed = false;

                if(value.X >= 0)
                {
                    m_CursorPosition.X = value.X;
                    changed = true;
                }

                if(value.Y >= 0)
                {
                    m_CursorPosition.Y = value.Y;
                    changed = true;
                }

                if(changed)
                {
                    if(CursorPositionChanged != null)
                        CursorPositionChanged(this, m_CursorPosition);
                }
            }
        }

        public char this[int i, int j]
        {
            get
            {
                return m_Data[i,j];
            }

            set
            {
                m_Data[i, j] = value;
            }
        }
    }
}