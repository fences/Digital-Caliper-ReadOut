using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace INSIZE_Reader
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            comboBox1.Items.Clear();
            string[] ports = SerialPort.GetPortNames();
            foreach (string port in ports)
            {
                comboBox1.Items.Add(port);
            }
        }

        private bool _flag = false;
        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                if (!string.IsNullOrEmpty(comboBox1.Text))
                {
                    _flag ^= true;
                    if (_flag)
                    {
                        serialPort1.PortName = comboBox1.Text;
                        serialPort1.BaudRate = 115200;
                        serialPort1.Open();
                        button1.Text = "Disconnect";
                    }
                    else
                    {
                        if (serialPort1.IsOpen)
                        {
                            button1.Text = "Connect";
                            serialPort1.Close();
                        }
                    }
                }

            }
            catch (Exception ex){ MessageBox.Show(ex.Message); }
          
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                button1.Text = "Connect";
                serialPort1.Close();
            }
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            Invoke(new Action(() => textBox1.Text = serialPort1.ReadLine()));
        }
    }
}
