#pragma once
#include "addresses.h"

#ifndef CLIENT_H
#define CLIENT_H

class Client {
	public:
		Client();
		uint32_t getBaseAddress() {
			return baseAddress;
		}

		template<typename T>
		T readData(uintptr_t addressRead) {
			T dataRead;
			memcpy(&dataRead, (void*)addressRead, sizeof(T));
			return dataRead;
		}

		std::string readDataString(uintptr_t addressRead, size_t readSize = 0) {
			char dataRead[255] = "";
			ReadProcessMemory(GetCurrentProcess(), (void*)addressRead, &dataRead, 32, NULL);
			return dataRead;
		}

		template<typename T>
		void writeData(uintptr_t addressWrite, T dataWrite) {
			DWORD oldProtect;
			VirtualProtectEx(GetCurrentProcess(), (void*)addressWrite, sizeof(T), PAGE_READWRITE, &oldProtect);
			memcpy((void*)addressWrite, &dataWrite, sizeof(T));
			VirtualProtectEx(GetCurrentProcess(), (void*)addressWrite, sizeof(T), oldProtect, NULL);
		}

		void hookCall(uintptr_t clientAddress, void* replaceFunc);
	protected:
		uint32_t baseAddress = 0;
};

#endif