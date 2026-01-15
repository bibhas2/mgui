#include "../include/mgui.h"

void
CComboBox::create(int x, int y, int w, int h, CWindow* parent, HMENU id, bool editable) {
	DWORD style = WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_VSCROLL;
	if (editable == true) {
		style |= CBS_DROPDOWN;
	} else {
		style |= CBS_DROPDOWNLIST;
	}
	CWindow::create(0, "COMBOBOX", "", 
		style,
		x, y,
		w, h,
		parent->m_wnd,
		id);
	setFont((HFONT) GetStockObject(DEFAULT_GUI_FONT));
}
