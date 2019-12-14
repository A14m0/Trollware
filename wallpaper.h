#pragma once
#include "definitions.h"
#include <WinInet.h> // Req header for wallpaper-set function
#include <ShlObj.h> // Reg header for wallpaper-set objects
#include <curl/curl.h> // For downloading files from internet sources


void SetWallpaper(LPCWSTR file);
size_t DownloadFile(std::string url, char* outfilename);