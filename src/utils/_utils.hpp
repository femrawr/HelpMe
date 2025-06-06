#include <string>
#include <stringapiset.h>

using std::string;
using std::wstring;

namespace utils {
	inline string wstringToString(const wstring& wstr) {
		int size = WideCharToMultiByte(
			CP_UTF8, NULL,
			wstr.data(),
			(int)wstr.size(),
			NULL, NULL,
			NULL, NULL
		);

		string str(size, 0);

		WideCharToMultiByte(
			CP_UTF8, NULL,
			wstr.data(),
			(int)wstr.size(),
			&str[0], size,
			NULL, NULL
		);

		return str;
	}

	inline string repeatString(const string& str, int count) {
		string final;

		final.reserve(str.size() * count);
		for (int i = 0; i < count; ++i) {
			final += str;
		}

		return final;
	}
}