#include "client.h"

Client::Client()
{
	char name[MAX_PATH];
	GetProcessImageFileNameA(GetCurrentProcess(), name, MAX_PATH);
	std::string realName(name);
	while (realName.find("\\") != std::string::npos) {
		realName = realName.substr(realName.find("\\") + 1);
	}

	HWND clientHWND = 0;

	while (HWND clientEx = FindWindowEx(0, clientHWND, L"TibiaClient", 0)) {
		clientHWND = clientEx;
		DWORD processId;
		GetWindowThreadProcessId(clientEx, &processId);

		if (processId == GetCurrentProcessId()) {
			break;
		}
	}

	if (clientHWND != 0) {
		SetWindowText(clientHWND, L"Tibia");
	}

	MODULEINFO modInfo;
	if (HANDLE client = GetCurrentProcess()) {
		if (HMODULE module = GetModuleHandleA(realName.c_str())) {
			GetModuleInformation(client, module, &modInfo, sizeof(modInfo));
		}
	}

	baseAddress = reinterpret_cast<uint32_t>(modInfo.lpBaseOfDll);
}

void Client::hookCall(uintptr_t clientAddress, void* replaceFunc)
{
	uint8_t callHex[5] = {0xE8, 0x00, 0x00, 0x00, 0x00};
	DWORD newCall = reinterpret_cast<DWORD>(replaceFunc) - clientAddress - 5;
	memcpy(callHex + 1, &newCall, 4);

	DWORD oldProtect;
	VirtualProtectEx(GetCurrentProcess(), (void*)clientAddress, 5, PAGE_READWRITE, &oldProtect);
	memcpy((void*)clientAddress, callHex, 5);
	VirtualProtectEx(GetCurrentProcess(), (void*)clientAddress, 5, oldProtect, NULL);
}