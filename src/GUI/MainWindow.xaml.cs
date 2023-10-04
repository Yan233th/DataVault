using Microsoft.Win32;
using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Runtime.InteropServices;
//using Microsoft.Win32;

namespace GUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow ()
        {
            InitializeComponent ();
        }

        [DllImport ("IO_core.dll")]
        static extern bool DecryptTextAndWriteToFile (string keyStr, string encryptedFilePathStr, string decryptedFilePathStr);
        [DllImport ("IO_core.dll")]
        static extern bool EncryptTextAndWriteToFile (string keyStr, string contentStr, string filePathStr);
        [DllImport ("IO_core.dll")]
        static extern bool EncryptFileAndWriteToFile (string keyStr, string sourcePathStr, string targetPathStr);
        [DllImport ("IO_core.dll")]
        static extern bool DecryptFileAndWriteToFile (string keyStr, string targetPathStr, string sourcePathStr);

        private void ChooseTargetFileButton_Click (object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog ();
            openFileDialog.Title = "Select Target file";
            if (openFileDialog.ShowDialog () == true)
            {
                string selectedFilePath = openFileDialog.FileName;
                TargetFilePathBox.Text = selectedFilePath;
            }
        }

        private void ChooseSourceFileButton_Click (object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog ();
            openFileDialog.Title = "Select Source file";
            if (openFileDialog.ShowDialog () == true)
            {
                string selectedFilePath = openFileDialog.FileName;
                SourceFilePathBox.Text = selectedFilePath;
            }
        }

        private void EncryptTextButton_Click (object sender, RoutedEventArgs e)
        {
            string key = PasswordBox.Password;

            if (string.IsNullOrWhiteSpace (key))
            {
                MessageBox.Show ("Key cannot be empty.");
                return;
            }

            string? directoryPath = System.IO.Path.GetDirectoryName (TargetFilePathBox.Text);
            if (!Directory.Exists (directoryPath))
            {
                MessageBox.Show ("The directory where the file located does not exist or is invalid.");
                return;
            }

            bool status = EncryptTextAndWriteToFile (key, ContentBox.Text, TargetFilePathBox.Text);

            if (status)
            {
                MessageBox.Show ("Encryption succeeded.");
            }
            else
            {
                MessageBox.Show ("Encryption failed.");
            }
        }

        private void DecryptTextButton_Click (object sender, RoutedEventArgs e)
        {
            string key = PasswordBox.Password;

            if (string.IsNullOrWhiteSpace (key))
            {
                MessageBox.Show ("Key cannot be empty.");
                return;
            }

            if (!File.Exists (TargetFilePathBox.Text))
            {
                MessageBox.Show ("Encrypted file does not exist.");
                return;
            }

            string tempFilePath = System.IO.Path.Combine (AppDomain.CurrentDomain.BaseDirectory, "temp");

            bool status = DecryptTextAndWriteToFile (key, TargetFilePathBox.Text, tempFilePath);

            if (status)
            {
                string decryptedContent = File.ReadAllText (tempFilePath);
                ContentBox.Text = decryptedContent;
                File.Delete (tempFilePath);
            }
            else
            {
                MessageBox.Show ("Decryption failed.");
            }
        }

        private void EncryptFileButton_Click (object sender, RoutedEventArgs e)
        {
            string key = PasswordBox.Password;

            if (string.IsNullOrWhiteSpace (key))
            {
                MessageBox.Show ("Key cannot be empty.");
                return;
            }

            if (!File.Exists (SourceFilePathBox.Text))
            {
                MessageBox.Show ("Source file does not exist.");
                return;
            }

            string? directoryPath = System.IO.Path.GetDirectoryName (TargetFilePathBox.Text);
            if (!Directory.Exists (directoryPath))
            {
                MessageBox.Show ("The directory where the target file located does not exist or is invalid.");
                return;
            }

            bool status = EncryptFileAndWriteToFile (key, SourceFilePathBox.Text, TargetFilePathBox.Text);

            if (status)
            {
                MessageBox.Show ("Encryption succeeded.");
            }
            else
            {
                MessageBox.Show ("Encryption failed.");
            }
        }

        private void DecryptFileButton_Click (object sender, RoutedEventArgs e)
        {
            string key = PasswordBox.Password;

            if (string.IsNullOrWhiteSpace (key))
            {
                MessageBox.Show ("Key cannot be empty.");
                return;
            }

            if (!File.Exists (TargetFilePathBox.Text))
            {
                MessageBox.Show ("Target file does not exist.");
                return;
            }

            string? directoryPath = System.IO.Path.GetDirectoryName (SourceFilePathBox.Text);
            if (!Directory.Exists (directoryPath))
            {
                MessageBox.Show ("The directory where the source file located does not exist or is invalid.");
                return;
            }

            bool status = DecryptFileAndWriteToFile (key, TargetFilePathBox.Text, SourceFilePathBox.Text);

            if (status)
            {
                MessageBox.Show ("Decryption succeeded.");
            }
            else
            {
                MessageBox.Show ("Decryption failed.");
            }

        }
    }
}
