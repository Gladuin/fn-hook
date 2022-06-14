#include <Windows.h>
#include <PathCch.h>

#include <cstdint>
#include <string>

#include <MinHook.h>
#include <SimpleIni.h>


CSimpleIni *config;


typedef int64_t (__fastcall *osdfunc_p)(uint16_t);
typedef int64_t (__fastcall *runfunc_p)(LPVOID);
osdfunc_p osdfunc_original = NULL;

int64_t __fastcall osdfunc_hook(uint16_t resource_id) {
    // this is to keep the large block down below more readable.
    #define GETBOOL(key) config->GetBoolValue(L"osd", key, true)

    // there is a better way to do this right? well, i'm too lazy to figure that out, if it even exists.
    if (resource_id == 2029 && GETBOOL(L"download_skypeforbusiness")) {
        return osdfunc_original(resource_id);
    } else if (resource_id == 2030 && GETBOOL(L"download_microsoftteams")) {
        return osdfunc_original(resource_id);
    } else if ((resource_id == 2011 || resource_id == 2012) && GETBOOL(L"num_lock")) {
        return osdfunc_original(resource_id);
    } else if ((resource_id == 2013 || resource_id == 2014) && GETBOOL(L"caps_lock")) {
        return osdfunc_original(resource_id);
    } else if ((resource_id == 2015 || resource_id == 2016) && GETBOOL(L"microphone")) {
        return osdfunc_original(resource_id);
    } else if ((resource_id == 2017 || resource_id == 2018) && GETBOOL(L"camera")) {
        return osdfunc_original(resource_id);
    } else if ((resource_id == 2019 || resource_id == 2020) && GETBOOL(L"touchpad")) {
        return osdfunc_original(resource_id);
    } else if ((resource_id == 2027 || resource_id == 2028) && GETBOOL(L"fn_lock")) {
        return osdfunc_original(resource_id);
    } else if ((resource_id == 10224 || resource_id == 10225) && GETBOOL(L"legion_rear_connector_leds")) {
        return osdfunc_original(resource_id);
    } else if ((resource_id == 10229 || resource_id == 10230) && GETBOOL(L"legion_logo_led")) {
        return osdfunc_original(resource_id);
    } else if ((resource_id == 10250 || resource_id == 10251) && GETBOOL(L"day_night_mode")) {
        return osdfunc_original(resource_id);
    } else if ((resource_id == 10237 || 
                resource_id == 10238 || 
                resource_id == 10239) && GETBOOL(L"dolby_mode")) {
        return osdfunc_original(resource_id);
    } else if ((resource_id == 2031 ||
                resource_id == 2032 || 
                resource_id == 2033 || 
                resource_id == 2034) && GETBOOL(L"keyboard_backlight_brightness")) {
        return osdfunc_original(resource_id);
    } else if ((resource_id == 10226 ||
                resource_id == 10227 ||
                resource_id == 10228 ||
                resource_id == 10240) && GETBOOL(L"legion_keyboard_backlight_brightness")) {
        return osdfunc_original(resource_id);
    } else if ((resource_id == 2037 || 
                resource_id == 2038 || 
                resource_id == 2039 ||
                resource_id == 2040 || 
                resource_id == 2041 || 
                resource_id == 2042) && GETBOOL(L"refresh_rate")) {
        return osdfunc_original(resource_id);
    } else if ((resource_id == 10231 || 
                resource_id == 10232 || 
                resource_id == 10233 ||
                resource_id == 10234 || 
                resource_id == 10235 || 
                resource_id == 10236) && GETBOOL(L"legion_rgb_mode")) {
        return osdfunc_original(resource_id);
    } else if ((resource_id == 2021 ||
                resource_id == 2022 ||
                resource_id == 2023 ||
                resource_id == 2024 ||
                resource_id == 2025 ||
                resource_id == 2026 ||
                resource_id == 2043 ||
                resource_id == 2044 ||
                resource_id == 2045) && GETBOOL(L"performance_mode")) {
        return osdfunc_original(resource_id);
    } else {
        return 0;
    }
}

int64_t __fastcall runfunc_hook(LPVOID lpThreadParameter) {
    LPCWSTR run_config = config->GetValue(L"run", L"run", L"notepad.exe");
    LPWSTR expanded_run;
    DWORD size_run = ExpandEnvironmentStrings(run_config, NULL, 0);

    if (size_run == sizeof(run_config)) {
        expanded_run = (LPWSTR)run_config;
    } else {
        expanded_run = (LPWSTR)malloc(sizeof(LPWSTR) * size_run);
        ExpandEnvironmentStrings(run_config, expanded_run, size_run);
    }


    LPCWSTR parameters_config = config->GetValue(L"run", L"parameters", NULL);
    LPWSTR expanded_parameters;
    DWORD size_parameters = ExpandEnvironmentStrings(parameters_config, NULL, 0);

    if (size_parameters == sizeof(parameters_config)) {
        expanded_parameters = (LPWSTR)parameters_config;
    } else {
        expanded_parameters = (LPWSTR)malloc(sizeof(LPWSTR) * size_parameters);
        ExpandEnvironmentStrings(parameters_config, expanded_parameters, size_parameters);
    }


    LPCWSTR directory_config = config->GetValue(L"run", L"directory", NULL);
    LPWSTR expanded_directory;
    DWORD size_directory = ExpandEnvironmentStrings(directory_config, NULL, 0);

    if (size_directory == sizeof(directory_config)) {
        expanded_directory = (LPWSTR)directory_config;
    } else {
        expanded_directory = (LPWSTR)malloc(sizeof(LPWSTR) * size_directory);
        ExpandEnvironmentStrings(directory_config, expanded_directory, size_directory);
    }


    if ((INT_PTR)ShellExecute(NULL, L"open", expanded_run, expanded_parameters, expanded_directory, SW_NORMAL) > 32) {
        if (size_run != sizeof(run_config)) free(expanded_run);
        if (size_parameters != sizeof(parameters_config)) free(expanded_parameters);
        if (size_directory != sizeof(directory_config)) free(expanded_directory);

        return 0;
    } else {
        return 1;
    }
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
    check_mh_error(MH_CreateHook((osdfunc_p)(base_address + 0x12070), osdfunc_hook, (LPVOID *)&osdfunc_original),
                   "MH_CreateHook (osdfunc)",
                   (HMODULE)dll_handle);
    check_mh_error(MH_CreateHook((runfunc_p)(base_address + 0x12510), runfunc_hook, 0),
                   "MH_CreateHook (runfunc)",
                   (HMODULE)dll_handle);
    check_mh_error(MH_EnableHook(MH_ALL_HOOKS),
                   "MH_EnableHook",
                   (HMODULE)dll_handle);

    WCHAR config_path[_MAX_PATH];
    GetModuleFileName((HMODULE)dll_handle, config_path, _MAX_PATH);
    PathCchRemoveFileSpec(config_path, _MAX_PATH);
    PathCchAppend(config_path, _MAX_PATH, L"\\fn-hook-config.ini");

    config = new CSimpleIni;
    config->SetUnicode();
    config->LoadFile(config_path);

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