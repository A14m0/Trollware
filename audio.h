#pragma once
#include "definitions.h"
#include <mmdeviceapi.h> // For playing audio through the microsoft's media service

#pragma comment(lib, "Winmm.lib") // Lib for mp3 playing


void MaxVol();
DWORD WINAPI PlaySong(LPVOID lpParameter);
DWORD WINAPI SongPlayin(LPVOID lpparameter);