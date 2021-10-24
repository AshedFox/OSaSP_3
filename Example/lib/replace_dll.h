#ifndef LAB3_LIBRARY_H
#define LAB3_LIBRARY_H

#include <windows.h>
#include <string>
#include "replace_dll_export.h"

extern "C" REPLACE_DLL_EXPORT void virtualFindAndReplaceString(DWORD processId, const char* oldString, const char* newString);

#endif //LAB3_LIBRARY_H
