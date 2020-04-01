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
        private WorkbookCanvas m_Canvas;

        public MainForm()
        {
            InitializeComponent();

            panelContent.Size = new Size(this.ClientSize.Width, this.ClientSize.Height - menuStrip1.Size.Height);

            m_Canvas = new WorkbookCanvas();                        
            panelContent.Controls.Add(m_Canvas);
        }
    }
}
