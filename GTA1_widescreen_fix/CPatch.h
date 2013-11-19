#include "stdafx.h"

class CPatch
{
  public:
	inline static void Patch(void *address, void *data, int size)
	{
		unsigned long protect[2];
		VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &protect[0]);
		memcpy(address, data, size);
		VirtualProtect(address, size, protect[0], &protect[1]);
	}
	inline static void Patch(int address, void *data, int size)
	{
		Patch((void *)address, data, size);
	}
	inline static void Unprotect(int address, int size)
	{
		unsigned long protect;
		VirtualProtect((void *)address, size, PAGE_EXECUTE_READWRITE, &protect);
	}
	inline static void Nop(int address, int size)
	{
		unsigned long protect[2];
		VirtualProtect((void *)address, size, PAGE_EXECUTE_READWRITE, &protect[0]);
		memset((void *)address, 0x90, size);
		VirtualProtect((void *)address, size, protect[0], &protect[1]);
	}
	inline static void FillWithZeroes(int address, int size)
	{
		unsigned long protect[2];
		VirtualProtect((void *)address, size, PAGE_EXECUTE_READWRITE, &protect[0]);
		memset((void *)address, 0x00, size);
		VirtualProtect((void *)address, size, protect[0], &protect[1]);
	}
	inline static void RedirectCall(int address, void *func)
	{
		int temp = 0xE8;
		Patch(address, &temp, 1);
		temp = (int)func - (address + 5);
		Patch((address + 1), &temp, 4);
	}
	inline static void RedirectJump(int address, void *func)
	{
		int temp = 0xE9;
		Patch(address, &temp, 1);
		temp = (int)func - (address + 5);
		Patch((address + 1), &temp, 4);
	}
	inline static void SetChar(int address, char value)
	{
		Patch(address, &value, 1);
	}
	inline static void SetUChar(int address, unsigned char value)
	{
		Patch(address, &value, 1);
	}
	inline static void SetShort(int address, short value)
	{
		Patch(address, &value, 2);
	}
	inline static void SetUShort(int address, unsigned short value)
	{
		Patch(address, &value, 2);
	}
	inline static void SetInt(int address, int value)
	{
		Patch(address, &value, 4);
	}
	inline static void SetUInt(int address, unsigned int value)
	{
		Patch(address, &value, 4);
	}
	inline static void SetFloat(int address, float value)
	{
		Patch(address, &value, 4);
	}
	inline static void SetPointer(int address, void *value)
	{
		Patch(address, &value, 4);
	}
	inline static void AdjustPointer(int address, void *value, int offset, int end)
	{
		int lastaddr = address + 8, content, result;
		do {
			content = *(int *)address;
			if(content >= offset && content <= end) {
				result = (int)value + (content - offset);
				Patch(address, &result, 4);
				break;
			}
		} while(++address < lastaddr);
	}
	inline static bool FileExists(const TCHAR *fileName)
	{
		DWORD fileAttr;
		fileAttr = GetFileAttributes(fileName);
		return fileAttr != INVALID_FILE_ATTRIBUTES && !(fileAttr & FILE_ATTRIBUTE_DIRECTORY);
	}
};
