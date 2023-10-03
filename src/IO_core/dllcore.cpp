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

bool DecryptAndWriteToFile (const char* keyStr, const char* encryptedFilePathStr, const char* decryptedFilePathStr)
{
    // Convert C-style strings to C++ std::string using assignment
    string key = keyStr;
    string encryptedFilePath = encryptedFilePathStr;
    string decryptedFilePath = decryptedFilePathStr;

    // Step 1: Generate SHA256 from the provided key
    SHA256 sha;
    byte shaKey[SHA256::DIGESTSIZE];
    sha.Update (reinterpret_cast<const byte*>(key.data ()), key.size ());
    sha.Final (shaKey);

    // Step 2: Use SHA256 as the AES key
    SecByteBlock aesKey (shaKey, SHA256::DIGESTSIZE);

    // Step 3: Read the encrypted content from the file
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

    // Step 4: Decrypt content with AES
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

    // Step 5: Write decrypted content to the specified file
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

bool EncryptAndWriteToFile (const char* keyStr, const char* contentStr, const char* filePathStr)
{
    // Convert C-style strings to C++ std::string using assignment
    string key = keyStr;
    string filePath = filePathStr;
    string content = contentStr;

    // Step 1: Generate SHA256 from the provided key
    SHA256 sha;
    byte shaKey[SHA256::DIGESTSIZE];
    sha.Update (reinterpret_cast<const byte*>(key.data ()), key.size ());
    sha.Final (shaKey);

    // Step 2: Use SHA256 as the AES key
    SecByteBlock aesKey (shaKey, SHA256::DIGESTSIZE);

    // Step 3: Encrypt content with AES
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

    // Step 4: Write encrypted content to the specified file
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
