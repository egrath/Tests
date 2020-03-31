using System;
using System.Drawing;

namespace Test001
{
    internal sealed class Workbook
    {
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

        public Point CursorPosition
        {
            get
            {
                return m_CursorPosition;
            }

            set
            {
                m_CursorPosition = value;
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