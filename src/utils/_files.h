#pragma once

#include <string>

using std::wstring;

namespace files {
	wstring createFolder(const wstring& name);

	bool checkFolder(const wstring& name);

	wstring createFile(
		const wstring& parent,
		const wstring& name,
		const wstring& contents
	);
}