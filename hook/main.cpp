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
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
	}
}