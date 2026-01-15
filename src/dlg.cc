#include "../include/mgui.h"

CDialog::CDialog(const char* id, CWindow* parent) {
	m_wnd = NULL;
	resId = id;
	parentW = parent;
}

CDialog::CDialog(int id, CWindow* parent) {
	m_wnd = NULL;
	resId = MAKEINTRESOURCE(id);
	parentW = parent;
}

void fixFont(HWND hDlg) {
	HWND hTmp;

	for(hTmp = GetWindow(hDlg, GW_CHILD); hTmp; 
		hTmp = GetWindow(hTmp, GW_HWNDNEXT))
        SendMessage(hTmp, WM_SETFONT, 
        	(WPARAM)GetStockObject(DEFAULT_GUI_FONT), FALSE);
}

static INT_PTR CALLBACK dlgFunc(HWND dlg, UINT msg, WPARAM wp, LPARAM lp) {
	int retVal = TRUE;
	CDialog *pd = NULL;

	//cout << "dlgFunc: " << msg << " " << wp << " " << lp << endl;

	switch (msg) {
		case WM_INITDIALOG:
			pd = (CDialog*) lp;
			pd->attach(dlg);
			fixFont(dlg);
			pd->onInitDialog();
			break;
		case WM_CLOSE:
			pd = (CDialog*) CWindow::fromWindowSafe(dlg);
			pd->onCancel();
			break;
		case WM_COMMAND:
			{
			CWindow *pw = NULL;
			if (lp != 0) {
				pw = CWindow::fromWindow((HWND) lp);
			}
			CWindow::fromWindowSafe(dlg)->
				onCommand(LOWORD(wp), HIWORD(wp), pw);
			}
			break;
		default:
			retVal = FALSE;
	}
	return retVal;
}

void CDialog::onCommand(int id, int type, CWindow* source){
	switch (id) {
		case IDOK:
		case IDYES:
			onOK();
			break;
		case IDCANCEL:
		case IDNO:
			onCancel();
			break;
		default:
			CWindow::onCommand(id, type, source);
	}
}

void CDialog::onOK() {
	//This should destroy the dialog
	EndDialog(m_wnd, IDOK);
}

void CDialog::onCancel() {
	//This should destroy the dialog
	EndDialog(m_wnd, IDCANCEL);
}

bool CDialog::doModal() {
	INT_PTR res = ::DialogBoxParam(
		g_instance, resId, 
		parentW != NULL ? parentW->m_wnd : NULL, 
		dlgFunc,
		(LPARAM) this);

	if (res <= 0)
		throw "CDialog::doModal: DialogBoxParam failed";

	return res == IDOK;
}

void CDialog::bind(CWindow& ch, int id) {
	HWND w = ::GetDlgItem(m_wnd, id);

	if (w == NULL) {
		throw "CDialog::bind: invalid ID";
	}
	ch.attach(w);
}

void CDialog::onInitDialog() {
}

CDialog::~CDialog(){}
