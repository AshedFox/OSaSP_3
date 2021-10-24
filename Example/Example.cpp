// Example.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "lib/replace_dll.h"
#pragma comment(lib, "lib/replace_dll.lib")

typedef void (*MyFunc)(DWORD, const char*, const char*);

bool dynamicReplace(DWORD processId, const char* oldString, const char* newString);
void injectReplace(DWORD processId);

int main()
{
    char choise = '\0';
    while (choise != 'e') 
    {
        const char str1[] = "Hello world!";
        const char str2[] = "Bye-bye wrld";

        std::cout << "1 - Static\n2 - Dynamic\n3 - Injection\ne - Exit" << std::endl;
        std::cin >> choise;
        switch (choise)
        {
            case '1':
            {
                std::cout << "Before: " << str1 << std::endl;
                virtualFindAndReplaceString(GetCurrentProcessId(), str1, str2);
                std::cout << "After: " << str1 << std::endl;
            }
            break;
            case '2':
            {
                std::cout << "Before: " << str1 << std::endl;
                if (dynamicReplace(GetCurrentProcessId(), str1, str2)) 
                {
                    std::cout << "After: " << str1 << std::endl;
                }
                else 
                {
                    std::cout << "Error!" << std::endl;
                }
            }
            break;
            case '3':
            {
                std::cout << "Before: " << str1 << std::endl;
                injectReplace(GetCurrentProcessId());
                std::cout << "After: " << str1 << std::endl;
            }
            break;
            case 'e': 
            {
                break;
            }
            default: 
            {
                std::cout << "Incorrect command";
            }
            break;
        }
    }
}

bool dynamicReplace(DWORD processId, const char* oldString, const char* newString) 
{
    HMODULE lib = LoadLibraryA("replace_dll.dll");

    if (lib)
    {
        MyFunc func = (MyFunc)GetProcAddress(lib, "virtualFindAndReplaceString");

        if (func)
        {
            (func)(GetCurrentProcessId(), oldString, newString);
            return true;
        }

        FreeLibrary(lib);
    }
    return false;
}

void injectReplace(DWORD processId) 
{
    HANDLE processHandle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_OPERATION | 
        PROCESS_VM_WRITE | PROCESS_CREATE_THREAD | PROCESS_CREATE_PROCESS, 
        false, processId);

    if (processHandle)
    {
        LPVOID baseAddress = VirtualAllocEx(processHandle, NULL, strlen("replace_dll.dll") + 1,
            MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

        if (baseAddress)
        {
            WriteProcessMemory(processHandle, baseAddress, "replace_dll.dll",
                strlen("replace_dll.dll") + 1, NULL);

            DWORD threadId;

            HANDLE threadHandle = CreateRemoteThread(processHandle, NULL, NULL,
                (LPTHREAD_START_ROUTINE)LoadLibraryA, (LPVOID)baseAddress, NULL, &threadId);

            if (threadHandle != nullptr) 
            {
                WaitForSingleObject(threadHandle, INFINITE);
            }
        }

        CloseHandle(processHandle);
    }
}