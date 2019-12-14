#pragma once
#include "definitions.h"
#include <objbase.h> // Network objects
#include <wlanapi.h> // Network API

#pragma comment(lib, "wlanapi.lib") // Lib for networking
#pragma comment(lib, "ole32.lib") // Lib for networking 


int KillWifi();