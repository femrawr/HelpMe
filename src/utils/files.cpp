#include <Windows.h>
#include <string>

#include "_files.h"

const wchar_t* BASE_FOLDER = L"\\Microsoft\\Edge\\User Data\\";

namespace files {
	wstring createFolder(const wstring& name) {
		wchar_t appdata[MAX_PATH];

		DWORD res = GetEnvironmentVariableW(
			L"LOCALAPPDATA",
			appdata,
			MAX_PATH
		);

		if (res == 0)
			return L"";

		wstring path = wstring(appdata) + BASE_FOLDER + name;

		BOOL suc = CreateDirectoryW(
			path.c_str(),
			NULL
		);

		if (suc)
			return path;

		DWORD err = GetLastError();
		if (err == ERROR_ALREADY_EXISTS)
			return path;

		return L"";
	}

	bool checkFolder(const wstring& name) {
		wchar_t appdata[MAX_PATH];

		DWORD res = GetEnvironmentVariableW(
			L"LOCALAPPDATA",
			appdata,
			MAX_PATH
		);

		if (res == 0)
			return false;

		wstring path = wstring(appdata) + BASE_FOLDER + name;
		DWORD attributes = GetFileAttributesW(path.c_str());

		return (attributes != INVALID_FILE_ATTRIBUTES &&
			(attributes & FILE_ATTRIBUTE_DIRECTORY));
	}

	wstring createFile(
		const wstring& parent,
		const wstring& name,
		const wstring& contents
	) {
		wstring path = parent + L"\\" + name;

		HANDLE file = CreateFileW(
			path.c_str(),
			GENERIC_WRITE,
			NULL, NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (file == INVALID_HANDLE_VALUE)
			return L"";

		INT size = WideCharToMultiByte(
			CP_UTF8, NULL,
			contents.c_str(),
			-1, NULL, NULL,
			NULL, NULL
		);

		if (size == 0) {
			CloseHandle(file);
			return L"";
		}

		char* buffer = new char[size];

		WideCharToMultiByte(
			CP_UTF8, NULL,
			contents.c_str(),
			-1, buffer, size,
			NULL, NULL
		);

		DWORD written;
		BOOL success = WriteFile(
			file, buffer, size - 1,
			&written, NULL
		);

		delete[] buffer;
		CloseHandle(file);

		if (success && (written == size - 1))
			return path;

		return L"";
	}
}