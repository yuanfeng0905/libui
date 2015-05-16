// 26 april 2015
#include "uipriv_windows.h"

// Code for containers. uiMakeContainer() creates a singleHWND of this window class.

#define containerClass L"libui_uiContainerClass"

static LRESULT CALLBACK containerWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
/* TODO
	RECT r;
	HDC dc;
	PAINTSTRUCT ps;

	switch (uMsg) {
	case WM_PAINT:
		dc = BeginPaint(hwnd, &ps);
		if (dc == NULL)
			logLastError("error beginning container paint in containerWndProc()");
		r = ps.rcPaint;
		paintContainerBackground(hwnd, dc, &r);
		EndPaint(hwnd, &ps);
		return 0;
	// tab controls use this to draw the background of the tab area
	case WM_PRINTCLIENT:
		if (GetClientRect(hwnd, &r) == 0)
			logLastError("error getting client rect in containerWndProc()");
		paintContainerBackground(hwnd, (HDC) wParam, &r);
		return 0;
	case WM_ERASEBKGND:
		// avoid some flicker
		// we draw the whole update area anyway
		return 1;
	}
*/

	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

ATOM initContainer(HICON hDefaultIcon, HCURSOR hDefaultCursor)
{
	WNDCLASSW wc;

	ZeroMemory(&wc, sizeof (WNDCLASSW));
	wc.lpszClassName = containerClass;
	wc.lpfnWndProc = containerWndProc;
	wc.hInstance = hInstance;
	wc.hIcon = hDefaultIcon;
	wc.hCursor = hDefaultCursor;
	wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
	return RegisterClassW(&wc);
}

void uninitContainer(void)
{
	if (UnregisterClassW(containerClass, hInstance) == 0)
		logLastError("error unregistering container window class in uninitContainer()");
}

static BOOL onWM_COMMAND(uiControl *c, WORD code, LRESULT *lResult)
{
	return FALSE;
}

static BOOL onWM_NOTIFY(uiControl *c, NMHDR *nm, LRESULT *lResult)
{
	return FALSE;
}

static void onDestroy(void *data)
{
	// do nothing
}

static void containerComputeChildSize(uiControl *c, intmax_t *x, intmax_t *y, intmax_t *width, intmax_t *height, uiSizing *d)
{
	HWND hwnd;
	RECT r;

	hwnd = (HWND) uiControlHandle(c);
	if (GetClientRect(hwnd, &r) == 0)
		logLastError("error getting container client rect in containerComputeChildSize()");
	*x = r.left;
	*y = r.top;
	*width = r.right - r.left;
	*height = r.bottom - r.top;
}

void uiMakeContainer(uiControl *c)
{
	uiWindowsMakeControlParams p;

	p.dwExStyle = 0;
	p.lpClassName = containerClass;
	p.lpWindowName = L"";
	p.dwStyle = 0;
	p.hInstance = hInstance;
	p.lpParam = NULL;
	p.useStandardControlFont = TRUE;
	p.onWM_COMMAND = onWM_COMMAND;
	p.onWM_NOTIFY = onWM_NOTIFY;
	p.onDestroy = onDestroy;
	p.onDestroyData = NULL;
	uiWindowsMakeControl(c, &p);

	uiControl(c)->ComputeChildSize = containerComputeChildSize;
}