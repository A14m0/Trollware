#include <windows.h> // Provides WinAPI access
#include <wtypes.h> // Wide character types for Windows access
#include <stdlib.h> // C standard library
#include <direct.h>
#include <vector>
#include <stdlib.h>
#include <time.h>


#include <iostream>
#include <string>

#define GetCurrentDir _getcwd
#ifndef UNICODE // Req definition for Windows.h
#define UNICODE
#endif


