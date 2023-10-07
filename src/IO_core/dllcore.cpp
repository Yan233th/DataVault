#define BUILD_MYDLL

#include "pch.h"
#include "dllcore.h"

#include <iostream>
#include <fstream>
#include <string>

#include "cryptlib.h"
#include "sha.h"
#include "filters.h"
#include "hex.h"
#include "aes.h"
#include "modes.h"
#include "osrng.h"

using namespace std;
using namespace CryptoPP;

bool EncryptTextAndWriteToFile (const char* keyStr, const char* contentStr, const char* filePathStr)
{
    string key = keyStr;
    string filePath = filePathStr;
    string content = contentStr;

    SHA256 sha;
    byte shaKey[SHA256::DIGESTSIZE];
    sha.Update (reinterpret_cast<const byte*>(key.data ()), key.size ());
    sha.Final (shaKey);

    SecByteBlock aesKey (shaKey, SHA256::DIGESTSIZE);

    string ciphertext;
    try
    {
        CBC_Mode<AES>::Encryption encryption;
        encryption.SetKeyWithIV (aesKey, aesKey.size (), aesKey);

        StringSource (content, true,
            new StreamTransformationFilter (encryption,
                new StringSink (ciphertext)
            )
        );
    }
    catch (const Exception& e)
    {
        cerr << "AES encryption error: " << e.what () << endl;
        return false;
    }

    ofstream outputFile (filePath, ios::binary);
    if (!outputFile)
    {
        cerr << "Unable to open file: " << filePath << endl;
        return false;
    }
    outputFile.write (ciphertext.c_str (), ciphertext.size ());
    outputFile.close ();

    cout << "Encryption and write to file completed." << endl;
    return true;
}

bool DecryptTextAndWriteToFile (const char* keyStr, const char* encryptedFilePathStr, const char* decryptedFilePathStr)
{
    string key = keyStr;
    string encryptedFilePath = encryptedFilePathStr;
    string decryptedFilePath = decryptedFilePathStr;

    SHA256 sha;
    byte shaKey[SHA256::DIGESTSIZE];
    sha.Update (reinterpret_cast<const byte*>(key.data ()), key.size ());
    sha.Final (shaKey);

    SecByteBlock aesKey (shaKey, SHA256::DIGESTSIZE);

    string ciphertext;
    ifstream encryptedFile (encryptedFilePath, ios::binary);
    if (!encryptedFile)
    {
        cerr << "Unable to open encrypted file: " << encryptedFilePath << endl;
        return false;
    }

    encryptedFile.seekg (0, ios::end);
    ciphertext.resize (encryptedFile.tellg ());
    encryptedFile.seekg (0, ios::beg);
    encryptedFile.read (&ciphertext[0], ciphertext.size ());
    encryptedFile.close ();

    string decryptedText;
    try
    {
        CBC_Mode<AES>::Decryption decryption;
        decryption.SetKeyWithIV (aesKey, aesKey.size (), aesKey);

        StringSource (ciphertext, true,
            new StreamTransformationFilter (decryption,
                new StringSink (decryptedText)
            )
        );
    }
    catch (const Exception& e)
    {
        cerr << "AES decryption error: " << e.what () << endl;
        return false;
    }

    ofstream decryptedFile (decryptedFilePath, ios::binary);
    if (!decryptedFile)
    {
        cerr << "Unable to open decrypted file for writing: " << decryptedFilePath << endl;
        return false;
    }
    decryptedFile.write (decryptedText.c_str (), decryptedText.size ());
    decryptedFile.close ();

    cout << "Decryption and write to file completed." << endl;
    return true;
}

bool EncryptFileAndWriteToFile (const char* keyStr, const char* sourcePathStr, const char* targetPathStr)
{
    string key = keyStr;
    string sourcePath = sourcePathStr;
    string targetPath = targetPathStr;

    SHA256 sha;
    byte shaKey[SHA256::DIGESTSIZE];
    sha.Update (reinterpret_cast<const byte*>(key.data ()), key.size ());
    sha.Final (shaKey);

    SecByteBlock aesKey (shaKey, SHA256::DIGESTSIZE);

    string content;
    ifstream sourceFile (sourcePath, ios::binary);
    if (!sourceFile)
    {
        cerr << "Unable to open source file: " << sourcePath << endl;
        return false;
    }
    content.assign ((istreambuf_iterator<char> (sourceFile)), (istreambuf_iterator<char> ()));
    sourceFile.close ();

    string ciphertext;
    try
    {
        CBC_Mode<AES>::Encryption encryption;
        encryption.SetKeyWithIV (aesKey, aesKey.size (), aesKey);

        StringSource (content, true,
            new StreamTransformationFilter (encryption,
                new StringSink (ciphertext)
            )
        );
    }
    catch (const Exception& e)
    {
        cerr << "AES encryption error: " << e.what () << endl;
        return false;
    }

    ofstream targetFile (targetPath, ios::binary);
    if (!targetFile)
    {
        cerr << "Unable to open target file: " << targetPath << endl;
        return false;
    }
    targetFile.write (ciphertext.c_str (), ciphertext.size ());
    targetFile.close ();

    cout << "Encryption and write to file completed." << endl;
    return true;
}

bool DecryptFileAndWriteToFile (const char* keyStr, const char* targetPathStr, const char* sourcePathStr)
{
    string key = keyStr;
    string targetPath = targetPathStr;
    string sourcePath = sourcePathStr;

    SHA256 sha;
    byte shaKey[SHA256::DIGESTSIZE];
    sha.Update (reinterpret_cast<const byte*>(key.data ()), key.size ());
    sha.Final (shaKey);

    SecByteBlock aesKey (shaKey, SHA256::DIGESTSIZE);

    string ciphertext;
    ifstream targetFile (targetPath, ios::binary);
    if (!targetFile)
    {
        cerr << "Unable to open target file: " << targetPath << endl;
        return false;
    }
    ciphertext.assign ((istreambuf_iterator<char> (targetFile)), (istreambuf_iterator<char> ()));
    targetFile.close ();

    string plaintext;
    try
    {
        CBC_Mode<AES>::Decryption decryption;
        decryption.SetKeyWithIV (aesKey, aesKey.size (), aesKey);

        StringSource (ciphertext, true,
            new StreamTransformationFilter (decryption,
                new StringSink (plaintext)
            )
        );
    }
    catch (const Exception& e)
    {
        cerr << "AES decryption error: " << e.what () << endl;
        return false;
    }

    ofstream sourceFile (sourcePath, ios::binary);
    if (!sourceFile)
    {
        cerr << "Unable to open source file: " << sourcePath << endl;
        return false;
    }
    sourceFile.write (plaintext.c_str (), plaintext.size ());
    sourceFile.close ();

    cout << "Decryption and write to file completed." << endl;
    return true;
}