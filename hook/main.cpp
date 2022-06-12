#include <Windows.h>
#include <MinHook.h>

BOOL WINAPI DllMain(HMODULE handle, DWORD reason, LPVOID reserved) {
	switch (reason) {
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
	}
}