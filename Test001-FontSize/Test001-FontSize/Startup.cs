using System;
using System.Windows.Forms;

namespace Test001
{
    internal sealed class Startup
    {
        [STAThread]
        public static void Main(string[] args)
        {
            Application.EnableVisualStyles();
            Application.Run(new MainForm());
        }
    }
}