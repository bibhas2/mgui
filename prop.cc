#include "mgui.h"
#include <iostream>

CPropertyPage::CPropertyPage(const char* title, int id) {
	this->title = title;
	this->id = id;
}

void CPropertyPage::bind(CWindow& ch, int id) {
	HWND w = ::GetDlgItem(m_wnd, id);

	if (w == NULL) {
		throw "CDialog::bind: invalid ID";
	}
	ch.attach(w);
}

void CPropertyPage::onInitDialog() {}
void CPropertyPage::onActivate() {}
bool CPropertyPage::onDeactivate() {return true;}
void CPropertyPage::onCancel() {}
bool CPropertyPage::onApply() {return true;}

void CPropertyPage::onCommand(int id, int type, CWindow* source){
	CWindow::onCommand(id, type, source);
}
static BOOL APIENTRY DataInput(HWND hDlg, UINT message, UINT wParam, LONG lParam) {
	CPropertyPage *ps = NULL;
	PROPSHEETPAGE *pP = NULL;
	BOOL ret = FALSE;
	bool status = false;

	switch (message) {
	case WM_INITDIALOG: 
		//cout << "WM_INITDIALOG: " << hDlg << endl;
		pP = (PROPSHEETPAGE*) lParam;
		ps = (CPropertyPage*) pP->lParam;
		ps->attach(hDlg);
		ps->onInitDialog();

		ret = TRUE;

		break;
	case WM_CLOSE:
		//cout << "WM_CLOSE"  << hDlg << endl;
		ps = (CPropertyPage*) CWindow::fromWindowSafe(hDlg);
		ps->onCancel();

		ret = TRUE;
		break;
	case WM_NOTIFY:
		//cout << "WM_NOTIFY: "  << hDlg << ":" << ((NMHDR FAR *)lParam)->code << endl;
		ps = (CPropertyPage*) CWindow::fromWindowSafe(hDlg);
		switch (((NMHDR FAR *)lParam)->code) {
			case PSN_SETACTIVE:
				//cout << "PSN_SETACTIVE" << endl;
				ps->onActivate();
				ret = TRUE;
				break;

			case PSN_APPLY:
				//cout << "PSN_APPLY" << endl;
				status = ps->onApply();
				::SetWindowLong(ps->m_wnd,
					DWL_MSGRESULT,
					status == false ? 
					PSNRET_INVALID_NOCHANGEPAGE :
					PSNRET_NOERROR);
				ret = TRUE;
				break;

			case PSN_KILLACTIVE:
				//cout << "PSN_KILLACTIVE" << endl;
				status = ps->onDeactivate();
				ret = status ? FALSE : TRUE;
				break;

			case PSN_RESET:
				ps->onCancel();
				//cout << "PSN_RESET" << endl;
				ret = TRUE;
				break;
		}
		break;
	case WM_COMMAND:
		{
		//cout << "WM_COMMAND"  << hDlg << endl;
		CWindow *pw = NULL;
		if (lParam != 0) {
			pw = CWindow::fromWindow((HWND) lParam);
		}
		CWindow::fromWindowSafe(hDlg)->
			onCommand(LOWORD(wParam), HIWORD(wParam), pw);
		}
		ret = TRUE;
		break;
	}

	cout << "Returning: " << (ret == TRUE ? "TRUE" : "FALSE") << endl;
	return ret;
} 

CPropertySheet::CPropertySheet(const char* title, CWindow* parent) {

	parentW = parent;
	mainTitle = title;

	psh.dwSize = sizeof(PROPSHEETHEADER);//PROPSHEETHEADER_V1_SIZE;
	psh.dwFlags = PSH_PROPSHEETPAGE;
	psh.hwndParent = parent->m_wnd;
	psh.hInstance = g_instance;
	psh.pszIcon = NULL;
	psh.pszCaption = (LPSTR) mainTitle.c_str();
	psh.nPages = 0;
	psh.ppsp = (LPCPROPSHEETPAGE) &psp;
	pageCount = 0;
}

void CPropertySheet::addPage(CPropertyPage& p) {
	if (pageCount >= (sizeof(psp) / sizeof(PROPSHEETPAGE))) {
		return;
	}

	psp[pageCount].dwSize = sizeof (PROPSHEETPAGE);
	psp[pageCount].dwFlags = PSP_USETITLE;
	psp[pageCount].hInstance = g_instance;
	psp[pageCount].pszTemplate = MAKEINTRESOURCE (p.getResourceId());
	psp[pageCount].pszIcon = NULL;
	psp[pageCount].pfnDlgProc = DataInput;
	psp[pageCount].pszTitle = p.getTitle();
	psp[pageCount].lParam = (LPARAM) &p;

	++pageCount;
}

bool CPropertySheet::doModal() {
	psh.nPages = pageCount;

	int ret = PropertySheet (&psh);

	return ret == 1;
}

CPropertySheet::~CPropertySheet(){}
