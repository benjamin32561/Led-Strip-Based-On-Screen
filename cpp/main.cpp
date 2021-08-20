#pragma comment (lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <time.h>
#include "WSAInitializer.h"
#include "func.h"

#define IP "10.100.102.100"
#define PORT 1234


int main()
{
	WSAInitializer init;
	cMonitorsVec Monitors;
	string to_send;
	int screenx, screeny, width, height, choice, frames = 0;
	clock_t tStart;

	RECT mon;
	HBITMAP hbwindow;
	BITMAPINFOHEADER bi;
	Mat img;

	//connecting to esp32
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(PORT);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(IP);
	int status = ::connect(clientSocket, (struct sockaddr*)&sa, sizeof(sa));
	if (status == INVALID_SOCKET)
	{
		std::cout << "Cant connect to server" << std::endl;
		return -1;
	}

	//choosing monitor
	for (int monitorIndex = 0; monitorIndex < Monitors.iMonitors.size(); monitorIndex++)
	{
		std::wcout << "Screen id: " << monitorIndex << std::endl;
		std::wcout << "-----------------------------------------------------" << std::endl;
		std::wcout << " - screen left-top corner coordinates : (" << Monitors.rcMonitors[monitorIndex].left
			<< "," << Monitors.rcMonitors[monitorIndex].top
			<< ")" << std::endl;
		std::wcout << " - screen dimensions (width x height) : (" << std::abs(Monitors.rcMonitors[monitorIndex].right - Monitors.rcMonitors[monitorIndex].left)
			<< "," << std::abs(Monitors.rcMonitors[monitorIndex].top - Monitors.rcMonitors[monitorIndex].bottom)
			<< ")" << std::endl;
		std::wcout << "-----------------------------------------------------" << std::endl;
	}
	cout << "Select Monitor To Screen Shot: ";
	cin >> choice;
	getchar();
	mon = Monitors.rcMonitors[choice];

	//screenshot stuff
	screenx = mon.left;
	screeny = mon.top;
	width = std::abs(mon.right - mon.left);
	height = std::abs(mon.top - mon.bottom);
	HWND hwnd = GetDesktopWindow();
	HDC hwindowDC = GetDC(hwnd);
	HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);
	img.create(height, width, CV_8UC4);
	hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
	bi = createBitmapHeader(width, height);
	SelectObject(hwindowCompatibleDC, hbwindow);

	//loop
	cout << "Starting Loop" << endl;
	tStart = clock();
	while (true)
	{
		StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, screenx, screeny, width, height, SRCCOPY);
		GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, img.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
		to_send = buildData(img);
		::send(clientSocket, to_send.c_str(), to_send.size(), 0);
		frames++;
		if (GetAsyncKeyState(VK_ESCAPE)) break;
	}
	double time = (double)(clock() - tStart) / CLOCKS_PER_SEC;
	cout << "Exited Loop" << endl;
	cout << "fps: " << frames / time << endl;

	DeleteObject(hbwindow);
	DeleteDC(hwindowCompatibleDC);
	ReleaseDC(hwnd, hwindowDC);

	getchar();

	return 0;
}