#include "_registry.h"

namespace registry {
	bool createKey(
		HKEY root,
		const wstring key,
		const wstring name,
		const wstring data
	) {
		HKEY handle = NULL;

		LONG result = RegCreateKeyExW(
			root, key.c_str(),
			NULL, NULL, NULL,
			KEY_SET_VALUE,
			NULL, &handle, NULL
		);

		if (result != ERROR_SUCCESS) {
			return false;
		}

		result = RegSetValueExW(
			handle, name.c_str(),
			NULL, REG_SZ,
			reinterpret_cast<CONST BYTE*>(data.c_str()),
			static_cast<DWORD>((data.size() + 1) * sizeof(wchar_t))
		);

		RegCloseKey(handle);
		return result == ERROR_SUCCESS;
	};

	bool createKey(
		HKEY root,
		const std::wstring& key,
		const std::wstring& name,
		DWORD data
	) {
		HKEY handle = NULL;

		LONG result = RegCreateKeyExW(
			root, key.c_str(),
			NULL, NULL, NULL,
			KEY_SET_VALUE,
			NULL, &handle, NULL
		);

		if (result != ERROR_SUCCESS) {
			return false;
		}

		result = RegSetValueExW(
			handle, name.c_str(),
			NULL, REG_SZ,
			reinterpret_cast<CONST BYTE*>(&data),
			sizeof(DWORD)
		);

		RegCloseKey(handle);
		return result == ERROR_SUCCESS;
	}
}