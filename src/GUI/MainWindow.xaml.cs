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
        static extern bool DecryptAndWriteToFile (string keyStr, string encryptedFilePathStr, string decryptedFilePathStr);
        [DllImport ("IO_core.dll")]
        static extern bool EncryptAndWriteToFile (string keyStr, string contentStr, string filePathStr);

        private void ChooseFileButton_Click (object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog ();
            openFileDialog.Title = "请选择文件";
            if (openFileDialog.ShowDialog () == true)
            {
                string selectedFilePath = openFileDialog.FileName;
                FilePathBox.Text = selectedFilePath;
            }
        }

        private void DecryptButton_Click (object sender, RoutedEventArgs e)
        {
            // 弹出输入密钥的对话框
            string key = Microsoft.VisualBasic.Interaction.InputBox ("Enter Key", "Key Input", "");

            // 检查密钥是否为空
            if (string.IsNullOrWhiteSpace (key))
            {
                MessageBox.Show ("Key cannot be empty.");
                return;
            }

            // 检查文件是否存在
            if (!File.Exists (FilePathBox.Text))
            {
                MessageBox.Show ("Encrypted file does not exist.");
                return;
            }

            string tempFilePath = System.IO.Path.Combine (AppDomain.CurrentDomain.BaseDirectory, "temp");

            // 调用 C++ 函数来解密和写入文件
            bool success = DecryptAndWriteToFile (key, FilePathBox.Text, tempFilePath);

            if (success)
            {
                // 读取解密后的文件内容
                string decryptedContent = File.ReadAllText (tempFilePath);

                // 显示解密后的内容
                ContentBox.Text = decryptedContent;

                // 删除临时文件
                File.Delete (tempFilePath);
            }
            else
            {
                MessageBox.Show ("Decryption failed.");
            }
        }

        private void EncryptButton_Click (object sender, RoutedEventArgs e)
        {
            // 弹出输入密钥的对话框
            string key = Microsoft.VisualBasic.Interaction.InputBox ("Enter Key", "Key Input", "");

            // 检查密钥是否为空
            if (string.IsNullOrWhiteSpace (key))
            {
                MessageBox.Show ("Key cannot be empty.");
                return;
            }

            // 检查文件所在的目录是否存在
            string? directoryPath = System.IO.Path.GetDirectoryName (FilePathBox.Text);
            if (!Directory.Exists (directoryPath))
            {
                MessageBox.Show ("The directory where the file located does not exist or is invalid.");
                return;
            }

            bool success = EncryptAndWriteToFile (key, ContentBox.Text, FilePathBox.Text);

            if (success)
            {
                MessageBox.Show ("Encryption succeeded.");
            }
            else
            {
                MessageBox.Show ("Encryption failed.");
            }
        }
    }
}
