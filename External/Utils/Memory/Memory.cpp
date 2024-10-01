#include "Memory.h"

bool Memory::Init()
{
    TargetHwnd = FindWindowA(NULL, TargetName);

    if (!TargetHwnd)
    {
        MessageBoxA(nullptr, "Waiting BF4...", "Info", MB_TOPMOST | MB_OK);

        while (!TargetHwnd)
        {
            TargetHwnd = FindWindowA(NULL, TargetName);

            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
    }

    GetWindowThreadProcessId(TargetHwnd, &PID);
    pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

    if (!pHandle)
    {
        MessageBoxA(nullptr, "Failed to get process handle", "ERROR", MB_TOPMOST | MB_ICONERROR | MB_OK);

        return false;
    }

    return true;
}

Memory m;