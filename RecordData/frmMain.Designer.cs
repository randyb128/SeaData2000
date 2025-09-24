namespace RecordData
{
    partial class frmMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmMain));
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.grpSettings = new System.Windows.Forms.GroupBox();
            this.txtAvgs = new System.Windows.Forms.TextBox();
            this.lblAvgs = new System.Windows.Forms.Label();
            this.btnSaveReadings = new System.Windows.Forms.Button();
            this.btnDelete = new System.Windows.Forms.Button();
            this.btnCapture = new System.Windows.Forms.Button();
            this.cboCaptureType = new System.Windows.Forms.ComboBox();
            this.lblCaptureType = new System.Windows.Forms.Label();
            this.btnRefresh = new System.Windows.Forms.Button();
            this.lblPort = new System.Windows.Forms.Label();
            this.btnClose = new System.Windows.Forms.Button();
            this.btnOpen = new System.Windows.Forms.Button();
            this.cboPort = new System.Windows.Forms.ComboBox();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.lblReadings = new System.Windows.Forms.Label();
            this.txtReadings = new System.Windows.Forms.TextBox();
            this.lblOutput = new System.Windows.Forms.Label();
            this.txtOutput = new System.Windows.Forms.TextBox();
            this.serialPort = new System.IO.Ports.SerialPort(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.grpSettings.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.grpSettings);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.splitContainer2);
            this.splitContainer1.Size = new System.Drawing.Size(800, 450);
            this.splitContainer1.SplitterDistance = 143;
            this.splitContainer1.TabIndex = 0;
            // 
            // grpSettings
            // 
            this.grpSettings.Controls.Add(this.txtAvgs);
            this.grpSettings.Controls.Add(this.lblAvgs);
            this.grpSettings.Controls.Add(this.btnSaveReadings);
            this.grpSettings.Controls.Add(this.btnDelete);
            this.grpSettings.Controls.Add(this.btnCapture);
            this.grpSettings.Controls.Add(this.cboCaptureType);
            this.grpSettings.Controls.Add(this.lblCaptureType);
            this.grpSettings.Controls.Add(this.btnRefresh);
            this.grpSettings.Controls.Add(this.lblPort);
            this.grpSettings.Controls.Add(this.btnClose);
            this.grpSettings.Controls.Add(this.btnOpen);
            this.grpSettings.Controls.Add(this.cboPort);
            this.grpSettings.Location = new System.Drawing.Point(15, 12);
            this.grpSettings.Name = "grpSettings";
            this.grpSettings.Size = new System.Drawing.Size(782, 116);
            this.grpSettings.TabIndex = 23;
            this.grpSettings.TabStop = false;
            this.grpSettings.Text = " Settings";
            // 
            // txtAvgs
            // 
            this.txtAvgs.Location = new System.Drawing.Point(351, 80);
            this.txtAvgs.Name = "txtAvgs";
            this.txtAvgs.Size = new System.Drawing.Size(100, 20);
            this.txtAvgs.TabIndex = 18;
            // 
            // lblAvgs
            // 
            this.lblAvgs.AutoSize = true;
            this.lblAvgs.Location = new System.Drawing.Point(293, 83);
            this.lblAvgs.Name = "lblAvgs";
            this.lblAvgs.Size = new System.Drawing.Size(52, 13);
            this.lblAvgs.TabIndex = 17;
            this.lblAvgs.Text = "Averages";
            // 
            // btnSaveReadings
            // 
            this.btnSaveReadings.Location = new System.Drawing.Point(631, 78);
            this.btnSaveReadings.Name = "btnSaveReadings";
            this.btnSaveReadings.Size = new System.Drawing.Size(102, 23);
            this.btnSaveReadings.TabIndex = 16;
            this.btnSaveReadings.Text = "Save Readings";
            this.btnSaveReadings.UseVisualStyleBackColor = true;
            this.btnSaveReadings.Click += new System.EventHandler(this.btnSaveReadings_Click);
            // 
            // btnDelete
            // 
            this.btnDelete.Enabled = false;
            this.btnDelete.Location = new System.Drawing.Point(540, 78);
            this.btnDelete.Name = "btnDelete";
            this.btnDelete.Size = new System.Drawing.Size(85, 23);
            this.btnDelete.TabIndex = 15;
            this.btnDelete.Text = "Remove Last";
            this.btnDelete.UseVisualStyleBackColor = true;
            // 
            // btnCapture
            // 
            this.btnCapture.Enabled = false;
            this.btnCapture.Location = new System.Drawing.Point(459, 78);
            this.btnCapture.Name = "btnCapture";
            this.btnCapture.Size = new System.Drawing.Size(75, 23);
            this.btnCapture.TabIndex = 14;
            this.btnCapture.Text = "Capture";
            this.btnCapture.UseVisualStyleBackColor = true;
            this.btnCapture.Click += new System.EventHandler(this.btnCapture_Click);
            // 
            // cboCaptureType
            // 
            this.cboCaptureType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboCaptureType.FormattingEnabled = true;
            this.cboCaptureType.Items.AddRange(new object[] {
            "Accelerometer",
            "Magnetometer"});
            this.cboCaptureType.Location = new System.Drawing.Point(89, 80);
            this.cboCaptureType.Name = "cboCaptureType";
            this.cboCaptureType.Size = new System.Drawing.Size(196, 21);
            this.cboCaptureType.TabIndex = 13;
            // 
            // lblCaptureType
            // 
            this.lblCaptureType.AutoSize = true;
            this.lblCaptureType.Location = new System.Drawing.Point(12, 83);
            this.lblCaptureType.Name = "lblCaptureType";
            this.lblCaptureType.Size = new System.Drawing.Size(71, 13);
            this.lblCaptureType.TabIndex = 12;
            this.lblCaptureType.Text = "Capture Type";
            // 
            // btnRefresh
            // 
            this.btnRefresh.Location = new System.Drawing.Point(479, 19);
            this.btnRefresh.Name = "btnRefresh";
            this.btnRefresh.Size = new System.Drawing.Size(55, 24);
            this.btnRefresh.TabIndex = 11;
            this.btnRefresh.Text = "Refresh";
            this.btnRefresh.UseVisualStyleBackColor = true;
            this.btnRefresh.Click += new System.EventHandler(this.btnRefresh_Click);
            // 
            // lblPort
            // 
            this.lblPort.AutoSize = true;
            this.lblPort.Location = new System.Drawing.Point(10, 22);
            this.lblPort.Name = "lblPort";
            this.lblPort.Size = new System.Drawing.Size(26, 13);
            this.lblPort.TabIndex = 10;
            this.lblPort.Text = "Port";
            // 
            // btnClose
            // 
            this.btnClose.Enabled = false;
            this.btnClose.Location = new System.Drawing.Point(422, 19);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(51, 23);
            this.btnClose.TabIndex = 9;
            this.btnClose.Text = "Close";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // btnOpen
            // 
            this.btnOpen.Location = new System.Drawing.Point(363, 18);
            this.btnOpen.Name = "btnOpen";
            this.btnOpen.Size = new System.Drawing.Size(53, 23);
            this.btnOpen.TabIndex = 8;
            this.btnOpen.Text = "Open";
            this.btnOpen.UseVisualStyleBackColor = true;
            this.btnOpen.Click += new System.EventHandler(this.btnOpen_Click);
            // 
            // cboPort
            // 
            this.cboPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboPort.FormattingEnabled = true;
            this.cboPort.Location = new System.Drawing.Point(42, 19);
            this.cboPort.Name = "cboPort";
            this.cboPort.Size = new System.Drawing.Size(303, 21);
            this.cboPort.TabIndex = 7;
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.lblReadings);
            this.splitContainer2.Panel1.Controls.Add(this.txtReadings);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.lblOutput);
            this.splitContainer2.Panel2.Controls.Add(this.txtOutput);
            this.splitContainer2.Size = new System.Drawing.Size(800, 303);
            this.splitContainer2.SplitterDistance = 131;
            this.splitContainer2.TabIndex = 0;
            // 
            // lblReadings
            // 
            this.lblReadings.AutoSize = true;
            this.lblReadings.Location = new System.Drawing.Point(12, 8);
            this.lblReadings.Name = "lblReadings";
            this.lblReadings.Size = new System.Drawing.Size(52, 13);
            this.lblReadings.TabIndex = 1;
            this.lblReadings.Text = "Readings";
            // 
            // txtReadings
            // 
            this.txtReadings.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtReadings.Location = new System.Drawing.Point(3, 31);
            this.txtReadings.Multiline = true;
            this.txtReadings.Name = "txtReadings";
            this.txtReadings.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txtReadings.Size = new System.Drawing.Size(797, 100);
            this.txtReadings.TabIndex = 0;
            // 
            // lblOutput
            // 
            this.lblOutput.AutoSize = true;
            this.lblOutput.Location = new System.Drawing.Point(12, 12);
            this.lblOutput.Name = "lblOutput";
            this.lblOutput.Size = new System.Drawing.Size(39, 13);
            this.lblOutput.TabIndex = 2;
            this.lblOutput.Text = "Output";
            // 
            // txtOutput
            // 
            this.txtOutput.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtOutput.Location = new System.Drawing.Point(0, 39);
            this.txtOutput.Multiline = true;
            this.txtOutput.Name = "txtOutput";
            this.txtOutput.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txtOutput.Size = new System.Drawing.Size(800, 129);
            this.txtOutput.TabIndex = 1;
            // 
            // serialPort
            // 
            this.serialPort.BaudRate = 115200;
            this.serialPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort_DataReceived);
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.splitContainer1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "frmMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Record Serial Data";
            this.Load += new System.EventHandler(this.frmMain_Load);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.grpSettings.ResumeLayout(false);
            this.grpSettings.PerformLayout();
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel1.PerformLayout();
            this.splitContainer2.Panel2.ResumeLayout(false);
            this.splitContainer2.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.Label lblReadings;
        private System.Windows.Forms.TextBox txtReadings;
        private System.Windows.Forms.Label lblOutput;
        private System.Windows.Forms.TextBox txtOutput;
        private System.IO.Ports.SerialPort serialPort;
        private System.Windows.Forms.GroupBox grpSettings;
        private System.Windows.Forms.Button btnRefresh;
        private System.Windows.Forms.Label lblPort;
        private System.Windows.Forms.Button btnClose;
        private System.Windows.Forms.Button btnOpen;
        private System.Windows.Forms.ComboBox cboPort;
        private System.Windows.Forms.Button btnDelete;
        private System.Windows.Forms.Button btnCapture;
        private System.Windows.Forms.ComboBox cboCaptureType;
        private System.Windows.Forms.Label lblCaptureType;
        private System.Windows.Forms.Button btnSaveReadings;
        private System.Windows.Forms.TextBox txtAvgs;
        private System.Windows.Forms.Label lblAvgs;
    }
}

