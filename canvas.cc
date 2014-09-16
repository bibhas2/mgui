#include "mgui.h"
#include "canvas.h"

void CCanvas::create(int x, int y, int width, int height, CWindow *parent) {
	DWORD style = WS_VISIBLE | WS_CHILD;
	CWindow::create(0, "MGUIWindow", "", 
		style, 
		x, y, width, height, parent->m_wnd, NULL);
}
bool CCanvas::handleEvent(UINT message, WPARAM wParam, LPARAM lParam) {
	bool processed;
	int x, y;

	switch (message) {
		//Frequent messages on top.
		case WM_MOUSEMOVE:
			x = GET_X_LPARAM(lParam);
			y = GET_Y_LPARAM(lParam);
			onMouseMove(x, y);
			processed = true;
			break;
		case WM_PAINT:
			::BeginPaint(m_wnd, &ps);
			onPaint(ps);
			::EndPaint(m_wnd, &ps);
			processed = true;
			break;
		case WM_RBUTTONDOWN:
			x = GET_X_LPARAM(lParam);
			y = GET_Y_LPARAM(lParam);
			onRightMouseDown(x, y);
			processed = true;
			break;
		case WM_RBUTTONUP:
			x = GET_X_LPARAM(lParam);
			y = GET_Y_LPARAM(lParam);
			onRightMouseUp(x, y);
			processed = true;
			break;
		case WM_LBUTTONDOWN:
			x = GET_X_LPARAM(lParam);
			y = GET_Y_LPARAM(lParam);
			onLeftMouseDown(x, y);
			processed = true;
			break;
		case WM_LBUTTONUP:
			x = GET_X_LPARAM(lParam);
			y = GET_Y_LPARAM(lParam);
			onLeftMouseUp(x, y);
			processed = true;
			break;
	}
	if (processed == false) {
		processed = CWindow::handleEvent(message, wParam, lParam);
	}

	return processed;
}

