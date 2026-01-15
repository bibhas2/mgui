#include "../include/mgui.h"

void
CListBox::create(int x, int y, int w, int h, 
	CWindow* parent, HMENU id) {

	DWORD style = WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | LBS_HASSTRINGS;

	CWindow::create(0, "LISTBOX", "", 
		style,
		x, y,
		w, h,
		parent->m_wnd,
		id);
	setFont((HFONT) GetStockObject(DEFAULT_GUI_FONT));
}
