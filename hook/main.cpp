#include <Windows.h>
#include <MinHook.h>

#include <cstdint>


typedef int64_t (__fastcall *osdfunc_p)(uint16_t);
typedef int64_t (__fastcall *runfunc_p)(LPVOID);

int64_t __fastcall osdfunc_hook(uint16_t resource_id) {

}

int64_t __fastcall runfunc_hook(LPVOID lpThreadParameter) {

}


BOOL WINAPI DllMain(HMODULE handle, DWORD reason, LPVOID reserved) {
	switch (reason) {
		case DLL_PROCESS_ATTACH: {
			uint64_t base_address = (uint64_t)GetModuleHandle(L"FnHotkeyUtility.exe");

			int error_code = MH_Initialize();
				error_code |= MH_CreateHook((osdfunc_p)(base_address + 0x12070), osdfunc_hook, 0);
				error_code |= MH_CreateHook((runfunc_p)(base_address + 0x12510), runfunc_hook, 0);
				error_code |= MH_EnableHook(MH_ALL_HOOKS);

			if (error_code != MH_OK) {
				MessageBox(NULL, L"One or more errors were raised while initialising MinHook!", L"MinHook Error", MB_OK | MB_ICONERROR);
				return FALSE;
			}

			break;
		}
		case DLL_PROCESS_DETACH:
			break;
	}
}