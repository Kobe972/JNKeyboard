#include <Windows.h>
#include <mmsystem.h>
#include <string>
#include "resource.h"
#pragma comment(lib, "winmm")
#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )

// 将应用程序路径写入注册表，使它在 Windows 启动时自动运行
void SetStartup(std::wstring path)
{
	HKEY hKey;
	LONG lResult = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey);
	if (lResult != ERROR_SUCCESS)
		return;

	lResult = RegSetValueExW(hKey, L"JNKeyboard", 0, REG_SZ, reinterpret_cast<const BYTE*>(path.c_str()), static_cast<DWORD>((path.length() + 1) * sizeof(wchar_t)));
	if (lResult != ERROR_SUCCESS)
		return;

	RegCloseKey(hKey);
}
int main()
{
	int ctrlHit = 0, jHit = 0, nHit = 0, tHit = 0, mHit = 0, state = 0;
	wchar_t buffer[MAX_PATH];
	GetModuleFileNameW(NULL, buffer, MAX_PATH);
	std::wstring path(buffer);
	// 将应用程序路径写入注册表
	SetStartup(path);
	while (true)
	{
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			ctrlHit = 1;
		}
		else
		{
			if (ctrlHit == 1)
			{
				ctrlHit = 0;
				state = 0;
				PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);
			}
		}
		if (GetAsyncKeyState('J') & 0x8000)
		{
			jHit = 1;
		}
		else
		{
			if (jHit == 1)
			{
				jHit = 0;
				state = 1;
				PlaySound(MAKEINTRESOURCE(IDR_WAVE2), NULL, SND_RESOURCE | SND_ASYNC);
			}
		}
		if (GetAsyncKeyState('N') & 0x8000)
		{
			nHit = 1;
		}
		else
		{
			if (nHit == 1)
			{
				nHit = 0;
				if (state == 1) state = 2;
				else state = 0;
				PlaySound(MAKEINTRESOURCE(IDR_WAVE5), NULL, SND_RESOURCE | SND_ASYNC);
			}
		}
		if (GetAsyncKeyState('T') & 0x8000)
		{
			tHit = 1;
		}
		else
		{
			if (tHit == 1)
			{
				tHit = 0;
				if (state == 2) state = 3;
				else state = 0;
				PlaySound(MAKEINTRESOURCE(IDR_WAVE6), NULL, SND_RESOURCE | SND_ASYNC);
			}
		}
		if (GetAsyncKeyState('M') & 0x8000)
		{
			mHit = 1;
		}
		else
		{
			if (mHit == 1)
			{
				mHit = 0;
				if (state == 3)
				{
					state = 0;
					PlaySound(MAKEINTRESOURCE(IDR_WAVE3), NULL, SND_RESOURCE | SND_ASYNC);
				}
				else
				{
					state = 0;
					PlaySound(MAKEINTRESOURCE(IDR_WAVE4), NULL, SND_RESOURCE | SND_ASYNC);
				}
			}
		}
	}
	return 0;
}