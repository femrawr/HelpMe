#pragma once

#include <Windows.h>
#include <string>

using std::wstring;

namespace registry {
	inline constexpr const wchar_t* AUTO_RUN = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";

	bool createKey(
		HKEY root,
		const wstring key,
		const wstring name,
		const wstring data
	);
}