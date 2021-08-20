#include <Windows.h>
#include <vector>

typedef struct rgb
{
	int r;
	int g;
	int b;
}rgb;

typedef struct monitorLocData {
	int x;
	int y;
	int width;
	int height;
}monitorLocData;

struct cMonitorsVec
{
	std::vector<int>       iMonitors;
	std::vector<HMONITOR>  hMonitors;
	std::vector<HDC>       hdcMonitors;
	std::vector<RECT>      rcMonitors;

	static BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM pData)
	{
		cMonitorsVec* pThis = reinterpret_cast<cMonitorsVec*>(pData);

		pThis->hMonitors.push_back(hMon);
		pThis->hdcMonitors.push_back(hdc);
		pThis->rcMonitors.push_back(*lprcMonitor);
		pThis->iMonitors.push_back(pThis->hdcMonitors.size());
		return TRUE;
	}

	cMonitorsVec()
	{
		EnumDisplayMonitors(0, 0, MonitorEnum, (LPARAM)this);
	}
};