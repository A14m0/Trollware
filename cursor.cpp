#include "cursor.h"

void MoveCursor() {
	INPUT Input = { 0 };
	Input.type = INPUT_MOUSE;

	Input.mi.dx = (LONG)0;
	Input.mi.dy = (LONG)0;

	// set move cursor directly and left click
	Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

	SendInput(1, &Input, sizeof(INPUT));
}

DWORD WINAPI troll_the_mouse(LPVOID args) {
	while (true)
	{
		MoveCursor();
	}
}