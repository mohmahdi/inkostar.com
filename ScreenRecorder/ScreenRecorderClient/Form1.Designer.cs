namespace ScreenRecorderClient
{
    partial class Form1
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
            this.cmdStart = new System.Windows.Forms.Button();
            this.cmdStop = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // cmdStart
            // 
            this.cmdStart.Location = new System.Drawing.Point(402, 12);
            this.cmdStart.Name = "cmdStart";
            this.cmdStart.Size = new System.Drawing.Size(75, 23);
            this.cmdStart.TabIndex = 0;
            this.cmdStart.Text = "Start";
            this.cmdStart.UseVisualStyleBackColor = true;
            this.cmdStart.Click += new System.EventHandler(this.cmdStart_Click);
            // 
            // cmdStop
            // 
            this.cmdStop.Location = new System.Drawing.Point(12, 12);
            this.cmdStop.Name = "cmdStop";
            this.cmdStop.Size = new System.Drawing.Size(75, 23);
            this.cmdStop.TabIndex = 1;
            this.cmdStop.Text = "Stop";
            this.cmdStop.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(489, 171);
            this.Controls.Add(this.cmdStop);
            this.Controls.Add(this.cmdStart);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button cmdStart;
        private System.Windows.Forms.Button cmdStop;
    }
}

