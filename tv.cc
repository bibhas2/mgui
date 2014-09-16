#include "mgui.h"
#include <string.h>


void
CTreeView::create(int x, int y, int w, int h, 
	CWindow* parent, int id) {

	DWORD style = WS_CHILD | WS_VISIBLE |  WS_BORDER | TVS_HASLINES | 
		TVS_LINESATROOT | TVS_HASBUTTONS;

	CWindow::create(WS_EX_CLIENTEDGE, WC_TREEVIEW, "", 
		style,
		x, y,
		w, h,
		parent->m_wnd,
		(HMENU) id);
}


HTREEITEM CTreeView::addItem(HTREEITEM parent, const char* text, void* data) {
	TVITEM tvi; 
	TVINSERTSTRUCT tvins; 

	tvi.mask = TVIF_TEXT | TVIF_PARAM;

	tvi.pszText = (CHAR*) text; 
	tvi.cchTextMax = strlen(text);
	tvi.lParam = (long int) data;
	
	tvins.item = tvi; 
	tvins.hInsertAfter = TVI_FIRST;
	tvins.hParent = parent;

	return TreeView_InsertItem(m_wnd, &tvins);
}

void* CTreeView::getItemData(HTREEITEM item) {
	TVITEMEX tvi; 

	tvi.mask = TVIF_PARAM;
	tvi.hItem = item;
	
	TreeView_GetItem(m_wnd, &tvi);
	
	return (void*) tvi.lParam;
}

void CTreeView::setItemData(HTREEITEM item, void* data) {
	TVITEM tvi; 

	tvi.mask = TVIF_PARAM;
	tvi.hItem = item;
	tvi.lParam = (long int) data;
		
	TreeView_SetItem(m_wnd, &tvi);
}

void CTreeView::deleteItem(HTREEITEM item) {
	TreeView_DeleteItem(m_wnd, item);
}

void CTreeView::clear() {
	TreeView_DeleteAllItems(m_wnd);
}


