#pragma once

#include <Windows.h>
#include <string>

using std::wstring;

namespace registry {
	inline constexpr const wchar_t* AUTO_RUN = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	inline constexpr const wchar_t* POLICY = L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";
	inline constexpr const wchar_t* SYSTEM = L"Software\\Policies\\Microsoft\\Windows\\System";

	bool createKey(
		HKEY root,
		const wstring key,
		const wstring name,
		const wstring data
	);

	bool createKey(
		HKEY root,
		const std::wstring& key,
		const std::wstring& name,
		DWORD data
	);
}