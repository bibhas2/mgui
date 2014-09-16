#include "mgui.h"
#include <commctrl.h>
#include <string.h>
#include <stdio.h>

void
CListView::create(int x, int y, int w, int h, 
	CWindow* parent, int id) {

	DWORD style = WS_CHILD | WS_VISIBLE |  LVS_REPORT | LVS_AUTOARRANGE 
		| LVS_SINGLESEL | LVS_SHOWSELALWAYS;

	CWindow::create(WS_EX_CLIENTEDGE, WC_LISTVIEW, "", 
		style,
		x, y,
		w, h,
		parent->m_wnd,
		(HMENU) id);
}

void CListView::addColumn(int i, const char* label, int width) {
	LVCOLUMN col;

	col.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;

	col.fmt = LVCFMT_LEFT;
	col.pszText = (char*) label;
	col.cchTextMax = strlen(label);
	col.cx = width;

	SendMessage(m_wnd, LVM_INSERTCOLUMN, i, (LPARAM)&col);
}

int CListView::getItemCount() {
	return ListView_GetItemCount(m_wnd);
}

int CListView::addRow(const char* txt) {
	LVITEM lvI;
	int iLen = strlen(txt);

	lvI.mask = LVIF_TEXT;
	lvI.iItem = getItemCount() + 1;
	lvI.iSubItem = 0;
	lvI.pszText = (char*) txt;
	lvI.cchTextMax = iLen;

	return ListView_InsertItem(m_wnd, &lvI );
}

void CListView::setItemData(int row, void* data) {
	LVITEM lvI;

	lvI.mask = LVIF_PARAM;
	lvI.iItem = row;
	lvI.iSubItem = 0;
	lvI.lParam = (LPARAM) data;

	::SendMessage(m_wnd, LVM_SETITEM, 0, (LPARAM) &lvI);
}

void* CListView::getItemData(int row) {
	LVITEM lvI;

	lvI.mask = LVIF_PARAM;
	lvI.iItem = row;
	lvI.iSubItem = 0;

	::SendMessage(m_wnd, LVM_GETITEM, 0, (LPARAM) &lvI);
	return (void*) lvI.lParam;
}

int CListView::getColumnWidth(int col) {
	return ListView_GetColumnWidth(m_wnd, col);
}

void CListView::setText(int row, int col, const char* txt) {
	ListView_SetItemText( m_wnd, row, col, (char*) txt);
}

void CListView::setText(int row, int col, const std::string& txt) {
	setText(row, col, txt.c_str());
}

void CListView::clear() {
	ListView_DeleteAllItems(m_wnd);
}

void CListView::deleteItem(int i) {
	ListView_DeleteItem(m_wnd, i);
}

int CListView::getSel() {
	/*
	return ::SendMessage(m_wnd, 
		LVM_GETSELECTIONMARK,
		0, 0);
	*/
	return ::SendMessage(m_wnd, 
		LVM_GETNEXTITEM, -1, 
    	LVNI_SELECTED);	
}

int CListView::setSel(int i) {
	return ListView_SetSelectionMark(m_wnd, i);
}

void CListView::setFullRowSelect(bool bTrue) {
	DWORD style = ListView_GetExtendedListViewStyle(m_wnd);
	if (bTrue) {
		style |= LVS_EX_FULLROWSELECT; 
	} else {
		style = style & ~LVS_EX_FULLROWSELECT; 
	}
	ListView_SetExtendedListViewStyle(m_wnd, style);
}

void CListView::setShowGrid(bool bTrue) {
	DWORD style = ListView_GetExtendedListViewStyle(m_wnd);
	if (bTrue) {
		style |= LVS_EX_GRIDLINES; 
	} else {
		style = style & ~LVS_EX_GRIDLINES; 
	}
	ListView_SetExtendedListViewStyle(m_wnd, style);
}

void CListView::setInt(int row, int col, int i) {
	char buff[256];

	sprintf(buff, "%d", i);

	setText(row, col, buff);
}
