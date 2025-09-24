using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Management;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RecordData
{
    public partial class frmMain : Form
    {
        public delegate void UpdateTextCallback(string text);
        enum CaptureType
        {
            Accelerometer, Magnetometer
        }
        private CaptureType CurrentType = CaptureType.Accelerometer;
        private bool CaptureFlag = false;
        private XYZ Totals = new XYZ();
        private int CurrentAvgs = 25;
        private List<int> CaptureValueY = new List<int>();
        private List<int> CaptureValueZ = new List<int>();
        private string LastLine = string.Empty;
        private List<string> Lines = new List<string>();

        public frmMain()
        {
            InitializeComponent();
        }

        static public List<string> myGetSerialPortNames()
        {
            List<string> rc = new List<string>();
            try
            {
                using (ManagementObjectSearcher ManObjSearch = new ManagementObjectSearcher("root\\CIMV2", "SELECT * FROM Win32_PnPEntity WHERE Caption LIKE '%(COM%'"))
                {
                    ManagementObjectCollection ManObjReturn = ManObjSearch.Get();
                    foreach (ManagementObject ManObj in ManObjReturn)
                    {
                        string desc = ManObj["Caption"].ToString();
                        {
                            rc.Add(desc);
                        }
                    }
                }
            }
            catch { }
            return rc;
        }

        private void btnRefresh_Click(object sender, EventArgs e)
        {
            cboPort.DataSource = myGetSerialPortNames();
        }

        private void frmMain_Load(object sender, EventArgs e)
        {
            // refresh list of COM ports
            btnRefresh_Click(null, null);
            cboCaptureType.SelectedIndex = 0;
        }

        private void EnableAll(bool en)
        {
            btnOpen.Enabled = !en;
            btnClose.Enabled = en;
            btnRefresh.Enabled = !en;
            cboPort.Enabled = !en;
            btnDelete.Enabled = en;
            btnCapture.Enabled = en;
            cboCaptureType.Enabled = !en;
            btnDelete.Enabled = en;
        }
        private void btnOpen_Click(object sender, EventArgs e)
        {
            string portName = cboPort.Text.Substring(cboPort.Text.IndexOf("(COM") + 1);
            portName = portName.Substring(0, portName.IndexOf(')'));
            try
            {
                serialPort.PortName = portName;
                serialPort.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            if (serialPort.IsOpen)
            {
                EnableAll(true);
            }

        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                try
                {
                    serialPort.Close();
                }
                catch { }
            }
            EnableAll(false);
        }

        private void ScrollToEnd(TextBox txt)
        {
            txt.SelectionStart = txt.Text.Length;
            txt.ScrollToCaret();
            txt.Refresh();
        }

        private void UpdateOutput(string text)
        {
            // save the lines in an array for capturing
            foreach (char c in text)
            {
                if (c == '\r')
                {
                    Lines.Add(LastLine);
                    LastLine = string.Empty;
                } 
                else if (c != '\n')
                {
                    LastLine += c;
                }
                
            }
            txtOutput.AppendText(text);
            ScrollToEnd(txtOutput);
        }

        private void serialPort_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            string s = serialPort.ReadExisting();

            txtOutput.Invoke(new UpdateTextCallback(this.UpdateOutput), s);
            if (CaptureFlag)
            {
                if (Lines.Count > 0)
                {
                    if (CurrentType == CaptureType.Accelerometer)
                    {
                        foreach(string str in Lines)
                        {
                            if (str.StartsWith("a:\t"))
                            {
                                Totals.Add(str);
                                if (Totals.Count >= CurrentAvgs)     // capture X readings for the average
                                {
                                    txtReadings.AppendText(Totals.Average());
                                    Totals.Clear();
                                    CaptureFlag = false;
                                    btnCapture.Enabled = true;
                                    break;
                                }
                            }
                        }
                    }
                    Lines.Clear();
                }
            }
        }

        private void btnCapture_Click(object sender, EventArgs e)
        {
            CurrentType = (CaptureType)cboCaptureType.SelectedIndex;
            if (CurrentType == CaptureType.Accelerometer)
            {
                CurrentAvgs = atoi(txtAvgs.Text);
                if (CurrentAvgs <= 0 || CurrentAvgs > 99)
                    CurrentAvgs = 25;
            }
            Totals.Clear();
            Lines.Clear();
            CaptureFlag = true;

            btnCapture.Enabled = false;
        }

        /// <summary>
        /// Helper function convert string to int
        /// </summary>
        /// <param name="s">string to convert</param>
        /// <returns>integer value else zero if error</returns>
        public static int atoi(string s)
        {
            int i = 0;
            int.TryParse(s, out i);
            return i;
        }


        public class XYZ
        {
            public XYZ()
            {
                Clear();
            }

            public void Clear()
            {
                X = 0;
                Y = 0;
                Z = 0;
                Count = 0;
            }

            public int Count { get; set; }

            public void Add(string data)
            {
                string[] strs = data.Split('\t');
                if (strs.Length == 4)
                {
                    X += atoi(strs[1]);
                    Y += atoi(strs[2]);
                    Z += atoi(strs[3]);
                }
                Count++;
            }
            public int X { get; set; }
            public int Y { get; set; }
            public int Z { get; set; }

            public string Average()
            {
                return string.Format("{0}\t{1}\t{2}\r\n", X / Count, Y / Count, Z / Count);
            }
        }

        private void btnSaveReadings_Click(object sender, EventArgs e)
        {
            DateTime now = DateTime.Now;
            string fileName = string.Format(@"{0}_{1}_{2}.txt", (CaptureType)cboCaptureType.SelectedIndex == CaptureType.Accelerometer ? "Accel" : "Mag", now.ToString("yyyyMMdd"), now.ToString("HHmmss"));
            bool fileErr = false;
            try
            {
                if (txtReadings.Lines.Length == 0)
                    MessageBox.Show("No data to be saved");
                else
                    using (StreamWriter sw = new StreamWriter(fileName))
                    {
                        foreach(string line in txtReadings.Lines)
                            sw.WriteLine(line);
                    }
            }
            catch(Exception x)
            {
                MessageBox.Show(x.Message);
                fileErr = true;
            }
            if (!fileErr)
                MessageBox.Show(string.Format("Readings saved to {0}", fileName));
        }
    }
}
