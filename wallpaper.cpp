#include "wallpaper.h"

extern bool armed;

std::vector<std::string> wallpaper_urls = { "http://cdn.miscellaneoushi.com/2027x1200/20121018/video%20games%20nuclear%20cold%20war%202027x1200%20wallpaper_www.miscellaneoushi.com_30.jpg",
	"https://i.ytimg.com/vi/ibr6egCSqiE/maxresdefault.jpg",
	"https://3.bp.blogspot.com/-Q-PS9dGSbeE/Tp2JPor4S1I/AAAAAAAAIP4/_UohFa8IRj0/s1600/7484-desktop-wallpapers-funny.jpg",
	"http://hdqwalls.com/wallpapers/real-wallpaper-story-wide.jpg",
	"https://images-wixmp-ed30a86b8c4ca887773594c2.wixmp.com/f/19482348-6b29-45d1-a921-a207b9519eff/d727oq2-2a66cf76-b8da-435b-b236-216bbd04e0e2.png?token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJ1cm46YXBwOjdlMGQxODg5ODIyNjQzNzNhNWYwZDQxNWVhMGQyNmUwIiwic3ViIjoidXJuOmFwcDo3ZTBkMTg4OTgyMjY0MzczYTVmMGQ0MTVlYTBkMjZlMCIsImF1ZCI6WyJ1cm46c2VydmljZTpmaWxlLmRvd25sb2FkIl0sIm9iaiI6W1t7InBhdGgiOiIvZi8xOTQ4MjM0OC02YjI5LTQ1ZDEtYTkyMS1hMjA3Yjk1MTllZmYvZDcyN29xMi0yYTY2Y2Y3Ni1iOGRhLTQzNWItYjIzNi0yMTZiYmQwNGUwZTIucG5nIn1dXX0.CxhOB2s2My10uFPTxhhdeXuKKuk0MaAwM858xA-UlEI" };


void SetWallpaper(const char *file) {
	int result = 0;
	if(armed) result = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void*) file, SPIF_UPDATEINIFILE);
	printf("Result: %d\n", result);
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

void wallpaper_setter() {
	char outfilename[FILENAME_MAX] = "file.jpg";
	char file[FILENAME_MAX] = "\\file.jpg";
	char cCurrentPath[FILENAME_MAX];
	errno_t succ;
	std::string url;

	while (true)
	{
		if (!armed) {
			Sleep(100);
			continue;
		}
		if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
		{
			return;
		}

		cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; // not really required, but for safety

		succ = strcat_s(cCurrentPath, MAX_PATH, file);
		printf("Path: %s\n", cCurrentPath);
		url = wallpaper_urls[rand() % wallpaper_urls.size()];
		std::cout << url << std::endl;
		DownloadFile(url, outfilename);
		SetWallpaper(cCurrentPath);

		Sleep(10000);
	}	
}
