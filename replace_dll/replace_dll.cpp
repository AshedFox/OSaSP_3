#include "replace_dll.h"

#include <vector>
#include <algorithm>

extern "C" REPLACE_DLL_EXPORT void virtualFindAndReplaceString(DWORD processId, const char* oldString, const char* newString)
{
    HANDLE processHandle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION,
                                       FALSE, processId);

    if (processHandle)
    {
        SYSTEM_INFO systemInfo;
        GetSystemInfo(&systemInfo);

        MEMORY_BASIC_INFORMATION memoryInfo;
        std::vector<char> region;
        char* address = nullptr;

        while (address < systemInfo.lpMaximumApplicationAddress)
        {
            if (VirtualQueryEx(processHandle, address, &memoryInfo, sizeof(memoryInfo)) == sizeof(memoryInfo))
            {
                if (memoryInfo.State == MEM_COMMIT && memoryInfo.AllocationProtect == PAGE_READWRITE)
                {
                    address = (char*)memoryInfo.BaseAddress;

                    region.resize(memoryInfo.RegionSize);
                    SIZE_T bytesRead;

                    if (ReadProcessMemory(processHandle, address, &region[0], memoryInfo.RegionSize, &bytesRead))
                    {
                        for (size_t i = 0; i < (bytesRead - strlen(oldString)); i++)
                        {
                            if (memcmp(oldString, &region[i], strlen(oldString)) == 0)
                            {
                                WriteProcessMemory(processHandle, (char *) address + i, newString,
                                                   strlen(oldString), nullptr);
                            }
                        }
                    }
                }

                address += memoryInfo.RegionSize;
            }
        }

        CloseHandle(processHandle);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    DWORD pid = GetCurrentProcessId();

    const char oldString[] = "Hello world!";
    const char newString[] = "Bye-bye wrld";

    if (ul_reason_for_call == DLL_PROCESS_ATTACH || ul_reason_for_call == DLL_THREAD_ATTACH)
    {
        virtualFindAndReplaceString(pid, oldString, newString);
    }

    return true;
}