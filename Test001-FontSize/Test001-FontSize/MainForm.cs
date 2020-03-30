using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Test001
{
    public partial class MainForm : Form
    {
        private Canvas m_Canvas;

        public MainForm()
        {
            InitializeComponent();

            m_Canvas = new Canvas();
            m_Canvas.Size = panelContent.Size;
            panelContent.Controls.Add(m_Canvas);
        }
    }
}
