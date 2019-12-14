#include "audio.h"
#include "structures.h"

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
	strcpy_s(RetStruct->timeToEnd, 32, buff);
	//RetStruct->timeToEnd = buff;

	mciSendString("play mp3 wait", NULL, 0, NULL);
	RetStruct->isPlaying = false;

	return 0;
}

DWORD WINAPI SongPlayin(LPVOID lpparameter) {
	bool* done = reinterpret_cast<bool*>(lpparameter);


	AudioRet passoff;
	passoff.isPlaying = true;
	passoff.timeToEnd = (char*)malloc(32);
	//while (true) {
	HANDLE threadHandler = CreateThread(0, 0, PlaySong, reinterpret_cast<void*>(&passoff), 0, 0); // starts rickroll thread to get out of way for other routines to execute


	Sleep(1000); // gives thread time to complete initial thing
	printf("Length of track: %s\n", passoff.timeToEnd);
	printf("SongPlaying value: %d\n", passoff.isPlaying);

	for (int i = 0; i * 1000 < std::stoi(passoff.timeToEnd); i++) {
		Sleep(1000);
		MaxVol();
		std::cout << i << std::endl;
	}
	CloseHandle(threadHandler);

	*done = true;
	free(passoff.timeToEnd);
	printf("Song complete. Exiting");
	return 0;

}