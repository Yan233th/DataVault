#pragma once

#ifdef BUILD_MYDLL
#define API_SYMBOL __declspec (dllexport)
#else
#define API_SYMBOL __declspec (dllimport)
#endif

// examples
// extern "C" API_SYMBOL char* GetFilePath (char* input);
// extern "C" API_SYMBOL void VideoReverse(char* input, void (*CallBack) (int status));