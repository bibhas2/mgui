/*
 * richedt.cc
 *
 *  Created on: Jun 7, 2012
 *      Author: bibhas
 */
#include "mgui.h"
#include <Richedit.h>

void CRichEdit::create(int x, int y, int w, int h, CWindow* parent) {
	LoadLibrary(TEXT("Riched32.dll"));
	CWindow::create(WS_EX_CLIENTEDGE, RICHEDIT_CLASS,"", WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOVSCROLL|WS_VSCROLL, x, y, w, h, parent->m_wnd, (HMENU) NULL);
	setFont((HFONT) GetStockObject(DEFAULT_GUI_FONT));
}

void CRichEdit::appendText(const char* txt) {
	CHARRANGE rng;
	rng.cpMin = -1;
	rng.cpMax = -1;
	::SendMessage(m_wnd, EM_EXSETSEL, 0, (LPARAM) &rng);
	::SendMessage(m_wnd,EM_REPLACESEL, FALSE, (LPARAM)txt);
}

void CRichEdit::setSel(int start, int end) {
	CHARRANGE cr;
	cr.cpMin = start;
	cr.cpMax = end;
	::SendMessage(m_wnd, EM_EXSETSEL, 0, (LPARAM)&cr);
}

void CRichEdit::getSel(int& start, int& end) {
	CHARRANGE cr;
	cr.cpMin = start;
	cr.cpMax = end;
	::SendMessage(m_wnd, EM_EXGETSEL, 0, (LPARAM)&cr);

	start = cr.cpMin;
	end = cr.cpMax;
}
