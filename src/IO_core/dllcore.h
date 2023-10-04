#pragma once

#ifdef BUILD_MYDLL
#define API_SYMBOL __declspec (dllexport)
#else
#define API_SYMBOL __declspec (dllimport)
#endif

// examples
// extern "C" API_SYMBOL char* GetFilePath (char* input);
// extern "C" API_SYMBOL void VideoReverse (char* input, void (*CallBack) (int status));
extern "C" API_SYMBOL bool EncryptTextAndWriteToFile (const char* keyStr, const char* contentStr, const char* filePathStr);
extern "C" API_SYMBOL bool DecryptTextAndWriteToFile (const char* keyStr, const char* encryptedFilePathStr, const char* decryptedFilePathStr);
extern "C" API_SYMBOL bool EncryptFileAndWriteToFile (const char* keyStr, const char* sourcePathStr, const char* targetPathStr);
extern "C" API_SYMBOL bool DecryptFileAndWriteToFile (const char* keyStr, const char* targetPathStr, const char* sourcePathStr);
