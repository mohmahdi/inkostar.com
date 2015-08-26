using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Inkostar.Assemblies;

namespace ScreenRecorderClient
{
    public partial class Form1 : Form
    {
        private ScreenRecorder sr;

        public Form1()
        {                               
            InitializeComponent();
            sr = new ScreenRecorder(@"C:\Development\inkostar\Source\ScreenRecorder\output.wmv");
            cmdStop.Enabled = false;
        }

        private void cmdStart_Click(object sender, EventArgs e)
        {
            cmdStart.Enabled = false;
            cmdStop.Enabled = true;
            sr.Start();
        }

        private void cmdStop_Click(object sender, EventArgs e)
        {
            sr.Shutdown();

            cmdStart.Enabled = true;
            cmdStop.Enabled = false;
        }
    }
}
