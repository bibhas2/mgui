#include "../../include/mgui.h"
#include <iostream>
#include <commctrl.h>
#include "resource.h"

class CMyPage : public CPropertyPage {
public:
	string name;
	CMyPage(const char* title) : CPropertyPage(title, IDD_PERSON) {
	}
	void onInitDialog() {
		CEdit m_edt;
		bind(m_edt, ID_TXT_NAME);
		m_edt.setText("Bibhas");
	}
	bool onApply() {
		CEdit m_edt;
		bind(m_edt, ID_TXT_NAME);

		m_edt.getText(name);

		return true;
	}
};

class CMyDialog : public CDialog {
CEdit m_edt;
public:

CMyDialog() : CDialog(TEST_DIALOG, NULL) {
}
CMyDialog(CWindow *p) : CDialog(TEST_DIALOG, p) {
}

void onOK() {
	std::cout << "OnOK" << std::endl;
	/*
	CMyDialog mdlg(this);

	mdlg.doModal();
	*/
	CPropertySheet ps("Title", this);
	CMyPage p1("Person");
	CMyPage p2("Hello");

	ps.addPage(p1);
	ps.addPage(p2);

	if (ps.doModal()) {
		m_edt.setText(p1.name + " " + p2.name);
	}
}

void onInitDialog() {
	bind(m_edt, ID_EDT1);

	m_edt.setText("Hello");
}

};

int PASCAL WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
try {
	CWindow::init(hInstance);
	CMyDialog mdlg;

	mdlg.doModal();

} catch (const char* mesg) {
	cout << "Exception: " << mesg << " GetLastError():" << ::GetLastError() << endl;
}
	return 0;
}
