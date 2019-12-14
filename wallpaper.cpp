#include "wallpaper.h"

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

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}

size_t DownloadFile(std::string url, char* outfilename) {
	CURL* curl;
	FILE* fp;
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