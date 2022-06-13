#include <Windows.h>
#include <MinHook.h>

#include <cstdint>
#include <string>


typedef int64_t (__fastcall *osdfunc_p)(uint16_t);
typedef int64_t (__fastcall *runfunc_p)(LPVOID);

int64_t __fastcall osdfunc_hook(uint16_t resource_id) {
    return 0;
}

int64_t __fastcall runfunc_hook(LPVOID lpThreadParameter) {
    return 0;
}


void check_mh_error(MH_STATUS status, std::string mh_type, HMODULE dll_handle) {
    if (status != MH_OK) {
        std::string msgbox_str = "Error " + std::string(MH_StatusToString(status)) + " occurred in function " + mh_type;
        MessageBoxA(NULL, msgbox_str.c_str(), "MinHook error", MB_OK | MB_ICONERROR);

        FreeLibraryAndExitThread(dll_handle, EXIT_FAILURE);
    }
}

DWORD WINAPI hook_init(LPVOID dll_handle) {
    uint64_t base_address = (uint64_t)GetModuleHandle(L"FnHotkeyUtility.exe");

    check_mh_error(MH_Initialize(),
                   "MH_Initialize",
                   (HMODULE)dll_handle);
    check_mh_error(MH_CreateHook((osdfunc_p)(base_address + 0x12070), osdfunc_hook, 0),
                   "MH_CreateHook (osdfunc)",
                   (HMODULE)dll_handle);
    check_mh_error(MH_CreateHook((runfunc_p)(base_address + 0x12510), runfunc_hook, 0),
                   "MH_CreateHook (runfunc)",
                   (HMODULE)dll_handle);
    check_mh_error(MH_EnableHook(MH_ALL_HOOKS),
                   "MH_EnableHook",
                   (HMODULE)dll_handle);

    return EXIT_SUCCESS;
}

BOOL WINAPI DllMain(HMODULE handle, DWORD reason, LPVOID reserved) {
    switch (reason) {
        case DLL_PROCESS_ATTACH:
            CreateThread(NULL, NULL, hook_init, handle, NULL, NULL);
            break;

        case DLL_PROCESS_DETACH:
            MH_DisableHook(MH_ALL_HOOKS);
            MH_Uninitialize();
            break;
    }

    return TRUE;
}