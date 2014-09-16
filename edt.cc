#include <stdio.h>
#include "mgui.h"

void
CEdit::create(int x, int y, int w, int h, 
	CWindow* parent, bool bMultiLine) {

	DWORD style = WS_CHILD | WS_VISIBLE;

	if (bMultiLine) {
		style = style | WS_VSCROLL | 
                    ES_WANTRETURN | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL;
        }
	CWindow::create(WS_EX_CLIENTEDGE, "EDIT", "",
		style,
		x, y,
		w, h,
		parent->m_wnd,
		(HMENU) NULL);
	setFont((HFONT) GetStockObject(DEFAULT_GUI_FONT));
}

bool CEdit::getDouble(double& d) {
	std::string str;

	getText(str);
	return ::sscanf(str.c_str(), "%lf", &d) == 1;
}

bool CEdit::getInt(int& d) {
	std::string str;

	getText(str);
	return ::sscanf(str.c_str(), "%d", &d) == 1;
}

void CEdit::setDouble(double d) {
	char buff[128];

	::sprintf(buff, "%g", d);
	setText(buff);
}

void CEdit::setInt(int d) {
	char buff[128];

	::sprintf(buff, "%d", d);
	setText(buff);
}
