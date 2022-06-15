#include <Windows.h>
#include <TlHelp32.h>
#include <PathCch.h>


HANDLE get_fnhotkeyutility_handle() {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    int pid;

    if (Process32First(snapshot, &pe32)) {
        do {
            if (wcscmp(L"FnHotkeyUtility.exe", pe32.szExeFile) == 0) {
                pid = pe32.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &pe32));
    }

    CloseHandle(snapshot);

    if (pid != 0) {
        return OpenProcess(PROCESS_CREATE_THREAD |
                           PROCESS_QUERY_INFORMATION |
                           PROCESS_VM_OPERATION |
                           PROCESS_VM_WRITE |
                           PROCESS_VM_READ, FALSE, pid);
    } else {
        return NULL;
    }
}

int main() {
    WCHAR dll_path[_MAX_PATH];
    GetModuleFileName(NULL, dll_path, _MAX_PATH);
    PathCchRemoveFileSpec(dll_path, _MAX_PATH);
    PathCchAppend(dll_path, _MAX_PATH, L"hook.dll");

    HANDLE process_handle = get_fnhotkeyutility_handle();
    LPVOID remote_lib_path = VirtualAllocEx(process_handle, NULL, sizeof(dll_path), MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(process_handle, remote_lib_path, dll_path, sizeof(dll_path), NULL);

    HMODULE kernel32_handle = GetModuleHandle(L"Kernel32");
    HANDLE thread_handle = CreateRemoteThread(process_handle, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(kernel32_handle, "LoadLibraryW"), remote_lib_path, 0, NULL);
    WaitForSingleObject(thread_handle, INFINITE);

    CloseHandle(thread_handle);
    VirtualFreeEx(process_handle, remote_lib_path, sizeof(dll_path), MEM_RELEASE);

    return 0;
}
