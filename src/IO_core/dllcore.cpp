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

// ��ȡ�����ļ��е�����
string readEncryptedFile(const string& filename) {
    string encryptedData;

    // ���ļ�
    ifstream input(filename.c_str(), ios::binary);

    if (input) {
        // ��ȡ�ļ���С
        input.seekg(0, ios::end);
        streamsize size = input.tellg();
        input.seekg(0, ios::beg);

        // ���仺����
        encryptedData.resize(static_cast<size_t>(size));

        // ��ȡ�ļ�����
        if (input.read(&encryptedData[0], size)) {
            cout << "��ȡ�����ļ��ɹ�" << endl;
        }
        else {
            cerr << "��ȡ�����ļ�ʧ��" << endl;
        }
    }
    else {
        cerr << "�򿪼����ļ�ʧ��" << endl;
    }

    return encryptedData;
}

// ������д������ļ���
bool writeEncryptedFile(const string& filename, const string& encryptedData) {
    // ���ļ�
    ofstream output(filename.c_str(), ios::binary);

    if (output) {
        // д������
        if (output.write(&encryptedData[0], encryptedData.size())) {
            cout << "д������ļ��ɹ�" << endl;
            return true;
        }
        else {
            cerr << "д������ļ�ʧ��" << endl;
        }
    }
    else {
        cerr << "�򿪼����ļ�ʧ��" << endl;
    }

    return false;
}

// ʹ��AES�㷨��������
string encrypt(const string& plaintext, const string& key) {
    string ciphertext;

    // �趨AES�����㷨
    CryptoPP::AES::Encryption aesEncryption((unsigned char*)key.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH);

    // �趨����ģʽ��CBCģʽ
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, (unsigned char*)key.c_str());

    // ʹ�ù��������м���
    CryptoPP::StringSource(plaintext, true,
        new CryptoPP::StreamTransformationFilter(cbcEncryption,
            new CryptoPP::StringSink(ciphertext), CryptoPP::StreamTransformationFilter::NO_PADDING)
    );

    return ciphertext;
}

// ʹ��AES�㷨��������
string decrypt(const string& ciphertext, const string& key) {
    string plaintext;

    // �趨AES�����㷨
    CryptoPP::AES::Decryption aesDecryption((unsigned char*)key.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH);

    // �趨����ģʽ��CBCģʽ
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, (unsigned char*)key.c_str());

    // ʹ�ù��������н���
    CryptoPP::StringSource(ciphertext, true,
        new CryptoPP::StreamTransformationFilter(cbcDecryption,
            new CryptoPP::StringSink(plaintext), CryptoPP::StreamTransformationFilter::NO_PADDING)
    );

    return plaintext;
}
