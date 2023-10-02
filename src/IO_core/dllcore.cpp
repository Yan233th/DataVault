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

#pragma comment(lib, "Crypto++/lib/Release/cryptlib.lib")

using namespace std;
using namespace CryptoPP;

void EncryptAndWriteToFile(const string& key, const string& file_path, const string& content)
{
    // Step 1: Generate SHA512 from the provided key
    SHA512 sha;
    CryptoPP::byte shaKey[SHA512::DIGESTSIZE];
    sha.Update(reinterpret_cast<const CryptoPP::byte*>(key.data()), key.size());
    sha.Final(shaKey);

    // Step 2: Use SHA512 as the AES key
    SecByteBlock aesKey(shaKey, SHA512::DIGESTSIZE);

    // Step 3: Encrypt content with AES
    string ciphertext;
    try
    {
        CBC_Mode<AES>::Encryption encryption;
        encryption.SetKeyWithIV(aesKey, aesKey.size(), aesKey);

        StringSource(content, true,
            new StreamTransformationFilter(encryption,
                new StringSink(ciphertext)
            )
        );
    }
    catch (const CryptoPP::Exception& e)
    {
        cerr << "AES encryption error: " << e.what() << endl;
        return;
    }

    // Step 4: Write encrypted content to the specified file
    ofstream outputFile(file_path, ios::binary);
    if (!outputFile)
    {
        cerr << "Unable to open file: " << file_path << endl;
        return;
    }
    outputFile.write(ciphertext.c_str(), ciphertext.size());
    outputFile.close();

    cout << "Encryption and write to file completed." << endl;
}
