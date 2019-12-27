#include "cursor.h"

extern bool armed;

void MoveCursor() {
	INPUT Input = { 0 };
	Input.type = INPUT_MOUSE;

	Input.mi.dx = (LONG)0;
	Input.mi.dy = (LONG)0;

	// set move cursor directly and left click
	Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

	if (!armed)
	{
		printf("Moving mouse cursor\n");
		printf("Moving mouse to: %ld, %ld", Input.mi.dx, Input.mi.dy);
		return;
	}

	SendInput(1, &Input, sizeof(INPUT));
}

DWORD WINAPI troll_the_mouse(LPVOID args) {
	if (!armed) printf("Beginning mouse troll\n");
	while (true)
	{
		MoveCursor();
		if (!armed) return 0;
	}
}