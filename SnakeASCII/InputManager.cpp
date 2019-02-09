#include "InputManager.h"
#include <Windows.h>

namespace Input
{
	bool isUpPressed()
	{
		if (GetKeyState(VK_UP) & 0x8000)
			return true;

		return false;
	}

	bool isDownPressed()
	{
		if (GetKeyState(VK_DOWN) & 0x8000)
			return true;

		return false;
	}

	bool isLeftPressed()
	{
		if (GetKeyState(VK_LEFT) & 0x8000)
			return true;

		return false;
	}

	bool isRightPressed()
	{
		if (GetKeyState(VK_RIGHT) & 0x8000)
			return true;

		return false;
	}

	bool isReturnPressed()
	{
		if (GetKeyState(VK_RETURN) & 0x8000)
			return true;

		return false;
	}
}