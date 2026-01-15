#include "../include/mgui.h"

void CDateTimePicker::create(int x, int y, int w, int h, CWindow* p, HMENU id) {
	CWindow::create(0,
		"SysDateTimePick32",
		"DateTime",
		WS_BORDER|WS_CHILD|WS_VISIBLE | DTS_SHOWNONE,
		x, y, w, h,
		p->m_wnd,
		id);
}

bool CDateTimePicker::getTime(SYSTEMTIME& t) {
	return DateTime_GetSystemtime(m_wnd, &t) == GDT_VALID;
}

void CDateTimePicker::setTime(SYSTEMTIME& t) {
	DateTime_SetSystemtime(m_wnd, GDT_VALID, &t);
}

void CDateTimePicker::setCurrentTime() {
	SYSTEMTIME t;

	::GetSystemTime(&t);
	setTime(t);
}

void CDateTimePicker::clearTime() {
	DateTime_SetSystemtime(m_wnd, GDT_NONE, NULL);
}

bool CDateTimePicker::hasTime() {
	SYSTEMTIME t;
	return !(DateTime_GetSystemtime(m_wnd, &t) == GDT_NONE);
}
