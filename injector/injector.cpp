#include <Windows.h>
#include <TlHelp32.h>
#include <PathCch.h>
#include <Psapi.h>


HMODULE get_hookdll_handle(HANDLE process_handle) {
    // because the GetExitCodeThread() method doesn't work with 64-bit processes, i had to do it like this.
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetProcessId(process_handle));

    MODULEENTRY32 me32;
    me32.dwSize = sizeof(MODULEENTRY32);

    HMODULE module_handle = NULL;

    if (Module32First(snapshot, &me32)) {
        do {
            if (wcscmp(L"hook.dll", me32.szModule) == 0) {
                module_handle = me32.hModule;
                break;
            }
        } while (Module32Next(snapshot, &me32));
    }

    CloseHandle(snapshot);

    if (module_handle != 0) {
        return module_handle;
    } else {
        return NULL;
    }
}

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
    BEGINNING:

    HANDLE process_handle = get_fnhotkeyutility_handle();

    while (process_handle == NULL) {
        process_handle = get_fnhotkeyutility_handle();
        Sleep(2500);
    }

    WCHAR dll_path[_MAX_PATH];
    GetModuleFileName(NULL, dll_path, _MAX_PATH);
    PathCchRemoveFileSpec(dll_path, _MAX_PATH);
    PathCchAppend(dll_path, _MAX_PATH, L"hook.dll");

    LPVOID remote_lib_path = VirtualAllocEx(process_handle, NULL, sizeof(dll_path), MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(process_handle, remote_lib_path, dll_path, sizeof(dll_path), NULL);

    HMODULE kernel32_handle = GetModuleHandle(L"Kernel32");
    HANDLE thread_handle = CreateRemoteThread(process_handle, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(kernel32_handle, "LoadLibraryW"), remote_lib_path, 0, NULL);
    WaitForSingleObject(thread_handle, INFINITE);

    CloseHandle(thread_handle);
    VirtualFreeEx(process_handle, remote_lib_path, sizeof(dll_path), MEM_RELEASE);

    HMODULE dll_handle = get_hookdll_handle(process_handle);
    WCHAR module_name[_MAX_PATH];
    DWORD process_exit_code;

    do {
        GetExitCodeProcess(process_handle, &process_exit_code);

        module_name[0] = '\0';
        GetModuleFileNameEx(process_handle, dll_handle, module_name, _MAX_PATH);

        if ((module_name[0] == '\0') && (process_exit_code == STILL_ACTIVE)) return 0;

        Sleep(2500);
    } while (process_exit_code == STILL_ACTIVE);

    goto BEGINNING;
}
