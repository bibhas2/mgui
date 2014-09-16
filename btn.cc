#include "mgui.h"

void
CButton::create(const char* label, int x, int y, int w, int h, 
	CWindow* parent, int id) {
	DWORD style = WS_VISIBLE | WS_TABSTOP | WS_CHILD | BS_PUSHBUTTON;
	CWindow::create(0, "BUTTON", label, 
		style,
		x, y,
		w, h,
		parent->m_wnd,
		(HMENU) id);
	setFont((HFONT) GetStockObject(DEFAULT_GUI_FONT));
}

void
CCheckBox::create(const char* label, int x, int y, int w, int h, 
	CWindow* parent, int id) {
	DWORD style = WS_VISIBLE | WS_TABSTOP | WS_CHILD | BS_CHECKBOX;
	CWindow::create(0, "BUTTON", label, 
		style,
		x, y,
		w, h,
		parent->m_wnd,
		(HMENU) id);
	setFont((HFONT) GetStockObject(DEFAULT_GUI_FONT));
}

bool CCheckBox::getCheck(){
	if (::SendMessage(m_wnd, BM_GETCHECK, 0, 0) == BST_CHECKED) {
		return true;
	}
	return false;
}
void CCheckBox::setCheck(bool bCheck){
	::SendMessage(m_wnd, BM_SETCHECK, 
		bCheck ? BST_CHECKED : BST_UNCHECKED, 0);
}

void
CGroupBox::create(const char* label, int x, int y, int w, int h,
	CWindow* parent) {
	DWORD style = WS_VISIBLE | WS_TABSTOP | WS_CHILD | BS_GROUPBOX;
	CWindow::create(0, "BUTTON", label, 
		style,
		x, y,
		w, h,
		parent->m_wnd,
		(HMENU) NULL);
	setFont((HFONT) GetStockObject(DEFAULT_GUI_FONT));
}
