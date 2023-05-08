#define BUILD_MYDLL

#include "pch.h"
#include "dllcore.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "cryptopp/aes.h"
#include "cryptopp/modes.h"
#include "cryptopp/filters.h"

using namespace std;

// 读取加密文件中的数据
string readEncryptedFile(const string& filename) {
    string encryptedData;

    // 打开文件
    ifstream input(filename.c_str(), ios::binary);

    if (input) {
        // 获取文件大小
        input.seekg(0, ios::end);
        streamsize size = input.tellg();
        input.seekg(0, ios::beg);

        // 分配缓冲区
        encryptedData.resize(static_cast<size_t>(size));

        // 读取文件数据
        if (input.read(&encryptedData[0], size)) {
            cout << "读取加密文件成功" << endl;
        }
        else {
            cerr << "读取加密文件失败" << endl;
        }
    }
    else {
        cerr << "打开加密文件失败" << endl;
    }

    return encryptedData;
}

// 将数据写入加密文件中
bool writeEncryptedFile(const string& filename, const string& encryptedData) {
    // 打开文件
    ofstream output(filename.c_str(), ios::binary);

    if (output) {
        // 写入数据
        if (output.write(&encryptedData[0], encryptedData.size())) {
            cout << "写入加密文件成功" << endl;
            return true;
        }
        else {
            cerr << "写入加密文件失败" << endl;
        }
    }
    else {
        cerr << "打开加密文件失败" << endl;
    }

    return false;
}

// 使用AES算法加密数据
string encrypt(const string& plaintext, const string& key) {
    string ciphertext;

    // 设定AES加密算法
    CryptoPP::AES::Encryption aesEncryption((unsigned char*)key.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH);

    // 设定加密模式：CBC模式
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, (unsigned char*)key.c_str());

    // 使用过滤器进行加密
    CryptoPP::StringSource(plaintext, true,
        new CryptoPP::StreamTransformationFilter(cbcEncryption,
            new CryptoPP::StringSink(ciphertext), CryptoPP::StreamTransformationFilter::NO_PADDING)
    );

    return ciphertext;
}

// 使用AES算法解密数据
string decrypt(const string& ciphertext, const string& key) {
    string plaintext;

    // 设定AES解密算法
    CryptoPP::AES::Decryption aesDecryption((unsigned char*)key.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH);

    // 设定解密模式：CBC模式
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, (unsigned char*)key.c_str());

    // 使用过滤器进行解密
    CryptoPP::StringSource(ciphertext, true,
        new CryptoPP::StreamTransformationFilter(cbcDecryption,
            new CryptoPP::StringSink(plaintext), CryptoPP::StreamTransformationFilter::NO_PADDING)
    );

    return plaintext;
}
