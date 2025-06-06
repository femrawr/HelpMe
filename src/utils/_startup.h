#pragma once

#include <string>

using std::string;
using std::wstring;

namespace startup {
	extern const char* STARTUP_PATH;
	extern const char* SUFFIX_KEY;

	string genSuffix(int n);

	bool createRegKey(const string& name, const wstring& data);
}