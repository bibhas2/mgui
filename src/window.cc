#include "../include/mgui.h"
/*
 * Have to enable
 * #define _WIN32_IE	0x0300
 * in commctrl.h
 * Also link comctl32.
 */
#include <commctrl.h>
#include <iostream>

HINSTANCE g_instance;
bool g_bContinue = true;

void trace(const char* msg) {
	/*
	 static FILE *pf = NULL;

	 if (!pf) {
	 pf = fopen("trace.log", "a");
	 if (pf) {
	 fputs("Initializing trace********\n", pf);
	 }
	 }
	 if (pf) {
	 fputs("TRACE: ", pf);
	 fputs(msg, pf);
	 fputs("\n", pf);
	 fflush(pf);
	 }
	 */
	cout << msg << endl;
}

CWindow::CWindow() {
	m_wnd = NULL;
}

CWindow::~CWindow() {
	detach();
}

void CWindow::detach() {
	//cout << "CWindow::detach this: " << this << " HWND: " << m_wnd << endl;
	if (m_wnd != NULL) {
::		RemoveProp(m_wnd, "cwnd");
		m_wnd = NULL;
	}
}

void CWindow::attach(HWND w) {
	detach();
	//cout << "CWindow::attach this: " << this << " HWND: " << w << endl;
	m_wnd = w;
::	SetProp(m_wnd, "cwnd", this);
}

void CWindow::create(DWORD exStyle, const char* wndClass, const char* title,
		DWORD style, int x, int y, int width, int height, HWND parent,
		HMENU idOrMenu) {
	if (false) {
		cout << "CreateWindowEx: class: " << wndClass;
		cout << " parent: " << parent;
		cout << " this: " << this;
		cout << endl;
	}

	HWND w =:: CreateWindowEx(exStyle,
			wndClass,
			title,
			style,
			x, y, width, height,
			parent, (HMENU) idOrMenu,
			g_instance,
			NULL);
	if (!w)
	throw "CreateWindowEx failed";
	attach(w);
}

void CWindow::show(bool bShow) {
::	ShowWindow(m_wnd, bShow ? SW_SHOW : SW_HIDE);
}

HWND CWindow::getWindow() {
	return m_wnd;
}

CWindow* CWindow::fromWindowSafe(HWND wnd) {
	CWindow *pw = (CWindow*):: GetProp(wnd, "cwnd");

	if (pw == NULL) {
		throw "CWindow::fromWindow. No CWindow is associated.";
	}

	return pw;
}

CWindow* CWindow::fromWindow(HWND wnd) {
	CWindow *pw = (CWindow*):: GetProp(wnd, "cwnd");

	return pw;
}

void CALLBACK timerProc(
		HWND hWnd, UINT message, UINT_PTR id, DWORD notused) {
	CWindow* w;

	w = CWindow::fromWindow(hWnd);
	if (w == NULL) {
		trace("Can not determine window from timer.");
	}
	w->onTimer((int) id);
}

LRESULT CALLBACK cwndProc(
		HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CWindow* w;

	w = CWindow::fromWindow(hWnd);
	if (w == NULL) {
		//trace("cwndProc: No associated window");
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	if (w->handleEvent(message, wParam, lParam) == true) {
		return 0;
	} else {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
void CWindow::onNotify(int id, NMHDR* pHdr) {
}

bool CWindow::handleEvent(UINT message, WPARAM wParam, LPARAM lParam) {
	bool processed = false;
	switch (message) {
	case WM_SIZE:
		onSize(LOWORD(lParam), HIWORD(lParam));
		processed = true;
		break;
	case WM_MOVE:
		onMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		processed = true;
		break;
	case WM_CLOSE:
		onClose();
		processed = true;
		break;
	case WM_DESTROY:
		onDestroy();
		processed = true;
		break;
	case WM_NOTIFY:
		onNotify((int) wParam, (NMHDR*) lParam);
		break;
	case WM_COMMAND: {
		CWindow *pw = NULL;
		if (lParam != 0) {
			pw = fromWindow((HWND) lParam);
		}
		onCommand(LOWORD(wParam), HIWORD(wParam), pw);
		processed = true;
	}
	}

	return processed;
}

void CWindow::onCommand(int id, int type, CWindow* source) {
	//cout << "COMMAND: ID: " << id << " Source: " << source << endl;
}

void CWindow::loop() {
	MSG s_currentMsg;

	while (g_bContinue) {
		BOOL rc =:: GetMessage(&s_currentMsg, (HWND) NULL, 0, 0);
		::TranslateMessage(&s_currentMsg);
		::DispatchMessage(&s_currentMsg);
	}
}

void CWindow::stop() {
	g_bContinue = false;
}

void CWindow::setTimer(int id, int interval) {
::	SetTimer(m_wnd, (UINT_PTR) id, (UINT) interval, timerProc);
}

void CWindow::killTimer(int id) {
::	KillTimer(m_wnd, id);
}

void CWindow::acceptFiles(bool val) {
::	DragAcceptFiles(m_wnd, val == true ? TRUE : FALSE);
}

void CWindow::init(HINSTANCE hInstance) {
	g_instance = hInstance;

	WNDCLASS wndclass;

	static const long styleNormal = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	static const long styleNoRedraw = CS_DBLCLKS;

	wndclass.lpfnWndProc = cwndProc; //DefWindowProc;//(WNDPROC)wxWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = sizeof(DWORD); // VZ: what is this DWORD used for?
	wndclass.hInstance = g_instance;
	wndclass.hIcon = (HICON) NULL;
	wndclass.hCursor =:: LoadCursor((HINSTANCE)NULL, IDC_ARROW);
	wndclass.lpszMenuName = NULL;

	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wndclass.lpszClassName = "MGUIWindow";
	wndclass.style = styleNormal;

	if ( !RegisterClass(&wndclass) )
	{
		throw "Failed to register window";
	}

	//Initialize common controls
	INITCOMMONCONTROLSEX whatever;

	whatever.dwSize = sizeof(INITCOMMONCONTROLSEX);
	whatever.dwICC = ICC_LISTVIEW_CLASSES | ICC_DATE_CLASSES;
	cout << "Initializing..." << endl;
	if (::InitCommonControlsEx(&whatever) == FALSE) {
		throw "Failed to load common controls.";
	}
}

void CFrame::create(const char* title, int width, int height) {
	CFrame::create(title, width, height, (const char*) NULL);
}

void CFrame::create(const char* title, int width, int height, int menuId) {
	CFrame::create(title, width, height, MAKEINTRESOURCE(menuId));
}

void CFrame::create(const char* title, int width, int height, const char* menuId) {
	HMENU hm = NULL;

	if (menuId != NULL) {
		hm =:: LoadMenu(g_instance, menuId);
		if (hm == NULL) {
			throw "Could not load menu.";
		}
	}

	CWindow::create(WS_EX_CLIENTEDGE, "MGUIWindow", title,
			WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, hm);

}

void CFrame::create(const char* title, int width, int height, int menuId, int iconId) {

	CFrame::create(title, width, height, menuId);

	HICON icon = LoadIcon(::g_instance, MAKEINTRESOURCE(iconId));
	::SendMessage(m_wnd, WM_SETICON, ICON_BIG, LPARAM(icon));
	::SendMessage(m_wnd, WM_SETICON, ICON_SMALL, LPARAM(icon));
}

void CFrame::create(const char* title, int width, int height,
		const char* menuId, const char* iconId) {

	CFrame::create(title, width, height, menuId);

	HICON icon = LoadIcon(::g_instance, iconId);
	::SendMessage(m_wnd, WM_SETICON, ICON_BIG, LPARAM(icon));
	::SendMessage(m_wnd, WM_SETICON, ICON_SMALL, LPARAM(icon));
}

CFrame::CFrame() {
	hOldCursor = NULL;
}

CFrame::~CFrame() {
::	DestroyWindow(m_wnd);
}

void CFrame::showWaitCursor(bool bShow) {
	if (bShow) {
		if (hOldCursor != NULL) {
			//We are already showing wait cursor
			return;
		}
		hOldCursor =:: SetCursor(
				::LoadCursor((HINSTANCE) NULL, IDC_WAIT));
	} else {
		::SetCursor(hOldCursor);
		hOldCursor = NULL;
	}
}

bool CFrame::handleEvent(UINT message, WPARAM wParam, LPARAM lParam) {
	bool processed = false;

	if (message != WM_DROPFILES) {
		return CWindow::handleEvent(message, wParam, lParam);
	}

	std::cout << "Processing file drop" << std::endl;
	HDROP hd = (HDROP) wParam;
	int count =:: DragQueryFile(hd, 0xFFFFFFFF, NULL, 0);
	std::string *list = new std::string[count];
	for (int i = 0; i < count; ++i) {
		int sz = ::DragQueryFile(hd, i, NULL, 0);
		std::string& str = list[i];
		str.resize(sz);
		::DragQueryFile(hd, i, (CHAR*) str.data(), sz + 1);
	}
	onDropFiles(list, count);
	delete [] list;
	std::cout << "Done." << std::endl;

	return true;
}

void CFrame::onDropFiles(std::string* list, int count) {
}
