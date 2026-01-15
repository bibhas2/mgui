#define _WIN32_IE	0x0400
#include <windows.h>
#include <commctrl.h>
#include <string>
using namespace std;

extern HINSTANCE g_instance;
#define Msg(msg) MessageBox (NULL, msg, "Message", MB_OK)
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
extern void trace(const char* msg);

class CWindow {
public:
void detach();
void attach(HWND w);
HWND m_wnd;

CWindow();
~CWindow();
void create(DWORD exStyle, const char* wndClass, const char* title, DWORD style,
int x, int y, int width, int height, HWND parent, HMENU idOrMenu);
void show(bool bShow = true);
HWND getWindow();
void resize(int x, int y, int w, int h) {
	::MoveWindow(m_wnd, x, y, w, h, TRUE);
}
HFONT getFont() {
	return (HFONT) SendMessage(m_wnd, WM_GETFONT, (WPARAM) 0, (LPARAM) 0);
}

void setFont(HFONT hF) {
	SendMessage(m_wnd, WM_SETFONT, (WPARAM) hF, (LPARAM) TRUE);
}
static CWindow* fromWindow(HWND wnd);
static CWindow* fromWindowSafe(HWND wnd);
static void init(HINSTANCE hi);
static void loop();
static void stop();

void messageBox(const char* msg) {
	::MessageBox(m_wnd, msg, "Message", MB_OK);
}

void errorBox(const char* msg) {
	::MessageBox(m_wnd, msg, "Error", MB_OK | MB_ICONERROR);
}

bool questionBox(const char* msg) {
	return ::MessageBox(m_wnd, msg, "Question", MB_YESNO | MB_ICONQUESTION)
		== IDYES;
}

bool confirmBox(const char* msg) {
	return ::MessageBox(m_wnd, msg, "Confirm", MB_OKCANCEL | MB_OKCANCEL)
		== IDOK;
}

void setText(const char* txt) {
	::SendMessage(m_wnd, WM_SETTEXT, 0, (LPARAM) txt);
}

void setText(const string& txt) {
	setText(txt.c_str());
}

int64_t getTextLength() {
	return SendMessage(m_wnd, WM_GETTEXTLENGTH, 0, 0);
}

void acceptFiles(bool val = true);

void getText(string& str) {
	int64_t len = getTextLength();

	str.resize(len);

	//The requested text length must include the null termination character.
	//Hence, we add 1 to len. The str.data() buffer always has one extra byte
	//more than string::size() for the `\0` so it is OK for Win32 to write the 
	//null byte at the end.
	SendMessage(m_wnd, WM_GETTEXT, (WPARAM) len + 1, (LPARAM) str.data());
}

void setRedraw(bool val) {
	SendMessage(m_wnd, WM_SETREDRAW, (WPARAM) (val ? TRUE : FALSE), (LPARAM) 0);
}

void setTimer(int id, int interval);
void killTimer(int id);

virtual bool handleEvent(UINT message, WPARAM wParam, LPARAM lParam);
virtual void onNotify(int controlId, NMHDR* pNmhdr);
virtual void onSize(int w, int h){}
virtual void onMove(int x, int y){}
virtual void onClose(){}
virtual void onDestroy(){}
virtual void onCommand(int id, int type, CWindow* source);
virtual void onTimer(int id){}
};

class CFrame : public CWindow {
HCURSOR hOldCursor;

public:
void create(const char* title, int width, int height);
void create(const char* title, int width, int height, const char* menuId);
void create(const char* title, int width, int height, int menuId);
void create(const char* title, int width, int height, const char* menuId, const char* iconId);
void create(const char* title, int width, int height, int menuId, int iconId);

CFrame();
virtual ~CFrame();
virtual void onDropFiles(std::string *list, int count);
virtual bool handleEvent(UINT message, WPARAM wParam, LPARAM lParam);
void showWaitCursor(bool bShow);
};

class CButton : public CWindow {
public:
void create(const char* label, int x, int y, int w, int h, CWindow* parent,
	HMENU id);
};
class CCheckBox : public CWindow {
public:
void create(const char* label, int x, int y, int w, int h, CWindow* parent,
	HMENU id);
bool getCheck();
void setCheck(bool bCheck);
};

class CEdit : public CWindow {
public:
void create(int x, int y, int w, int h, CWindow* parent, bool bmulti=false);
void appendText(const char* txt) {
	int64_t iPrevLen = getTextLength();

	::SendMessage(m_wnd, EM_SETSEL, iPrevLen, iPrevLen);
	::SendMessage(m_wnd, EM_REPLACESEL, 0, (LPARAM) txt);
}
void appendText(string& txt) {
	appendText(txt.c_str());
}

void setLimit(int max) {
	::SendMessage(m_wnd, EM_LIMITTEXT, (WPARAM) max, (LPARAM) 0);
}

int getLimit() {
	return (int) ::SendMessage(m_wnd, EM_GETLIMITTEXT, (WPARAM) 0, (LPARAM) 0);
}

bool getDouble(double& d);
bool getInt(int& i);
void setDouble(double d);
void setInt(int i);
};

class CRichEdit : public CWindow {
public:
void create(int x, int y, int w, int h, CWindow* parent);
void appendText(const char* txt);
void appendText(string& txt) {
	appendText(txt.c_str());
}
void setSel(int start, int end);
void getSel(int& start, int& end);
};


class CComboBox : public CWindow {
public:
void create(int x, int y, int w, int h, CWindow* parent, HMENU id, bool editable);
void addItem(const char* txt) {
	SendMessage(m_wnd, CB_ADDSTRING, 0, (LPARAM) txt);
}
void addItem(string& txt) {
	addItem(txt.c_str());
}
void deleteItem(int idx) {
	SendMessage(m_wnd, CB_DELETESTRING, idx, (LPARAM) 0);
}
int64_t getItemCount() {
	return SendMessage(m_wnd, CB_GETCOUNT, 0, (LPARAM) 0);
}
void setItemData(int idx, void* data) {
	SendMessage(m_wnd, CB_SETITEMDATA, idx, (LPARAM) data);
}
void* getItemData(int idx) {
	return (void*)SendMessage(m_wnd, CB_GETITEMDATA, idx, (LPARAM) 0);
}
int64_t getSel() {
	return SendMessage(m_wnd, CB_GETCURSEL, 0, (LPARAM) 0);
}
void setSel(int idx) {
	SendMessage(m_wnd, CB_SETCURSEL, idx, (LPARAM) 0);
}
int64_t clear() {
	return SendMessage(m_wnd, CB_RESETCONTENT, 0, (LPARAM) 0);
}
void getItemText(int idx, string& str) {
	int64_t len = ::SendMessage(m_wnd, CB_GETLBTEXTLEN, (WPARAM) idx, 0);

	str.resize(len);
	::SendMessage(m_wnd, CB_GETLBTEXT, (WPARAM) idx, (LPARAM) str.data());
}
};

class CListBox : public CWindow {
public:
void create(int x, int y, int w, int h, CWindow* parent,
	HMENU id);
void addItem(const char* txt) {
	SendMessage(m_wnd, LB_ADDSTRING, 0, (LPARAM) txt);
}
void addItem(string& txt) {
	addItem(txt.c_str());
}
void setItemData(int idx, void* data) {
	SendMessage(m_wnd, LB_SETITEMDATA, idx, (LPARAM) data);
}
void* getItemData(int idx) {
	return (void*)SendMessage(m_wnd, LB_GETITEMDATA, idx, (LPARAM) 0);
}
void deleteItem(int idx) {
	SendMessage(m_wnd, LB_DELETESTRING, idx, (LPARAM) 0);
}
int64_t getItemCount() {
	return SendMessage(m_wnd, LB_GETCOUNT, 0, (LPARAM) 0);
}
int64_t clear() {
	return SendMessage(m_wnd, LB_RESETCONTENT, 0, (LPARAM) 0);
}
int64_t getSel() {
	return SendMessage(m_wnd, LB_GETCURSEL, 0, (LPARAM) 0);
}
void getItemText(int idx, string& str) {
	int64_t len = ::SendMessage(m_wnd, LB_GETTEXTLEN, (WPARAM) idx, 0);
	str.resize(len);
	::SendMessage(m_wnd, LB_GETTEXT, (WPARAM) idx, (LPARAM) str.data());
}
};

class CListView : public CWindow {
public:
void create(int x, int y, int w, int h, CWindow* parent, HMENU id);
void addColumn(int i, const char* label, int width);
int getItemCount();
int addRow(const char* txt="");
void setText(int row, int col, const char* txt);
void setText(int row, int col, const std::string& txt);
void setInt(int row, int col, int i);
void clear();
void deleteItem(int i);
int getColumnWidth(int col);
int64_t getSel();
int setSel(int i);
void setItemData(int row, void* data);
void* getItemData(int row);
void setFullRowSelect(bool bTrue = true);
void setShowGrid(bool bTrue = true);
};

class CGroupBox : public CWindow {
void create(const char* label, int x, int y, int w, int h, CWindow* parent);
};

class CFileDialog {
private:
OPENFILENAME of;
char fileName[1024];
char filterBuff[256];

public:
	/*
	 * Filter string convention:
	 * "Text file|*.txt;*log|Image files|*.gif;*.jpg"
	 * This convention is not from MS. MS expects the
	 * seperator to be '\0' instead of |.
	 */

CFileDialog(const char* title, const char* filter, CWindow* parent=NULL);
bool openFile();
bool saveFile();
const char* getFileName();
};

class CDialog : public CWindow {
CWindow *parentW;
const char* resId;
public:
CDialog(const char* id, CWindow* parent);
CDialog(int id, CWindow* parent);
void bind(CWindow& child, int id);
bool doModal();
~CDialog();
virtual void onInitDialog();
virtual void onOK();
virtual void onCancel();
void onCommand(int id, int type, CWindow* source);
};

class CLabel : public CWindow {
public:
void create(const char* label, int x, int y, int w, int h, CWindow* parent);
};

class CDateTimePicker : public CWindow {
public:
	void create(int x, int y, int w, int h, CWindow* p, HMENU id);
	bool getTime(SYSTEMTIME& t); //returns true for valid time
	void setTime(SYSTEMTIME& t);
	void setCurrentTime();
	void clearTime();
	bool hasTime(); //Is time entered?
};


class CPropertyPage : public CWindow {
	string title;
	int id;

public:
	virtual void onInitDialog();
	virtual void onActivate();
	virtual bool onDeactivate();
	virtual void onCancel();
	virtual bool onApply();
	void onCommand(int id, int type, CWindow* source);
	void bind(CWindow& child, int id);
	CPropertyPage(const char* title, int id);

	const char* getTitle() {
		return title.c_str();
	}

	int getResourceId() {
		return id;
	}

};

class CPropertySheet : public CWindow {
	CWindow *parentW;
	const char* resId;
	PROPSHEETPAGE psp [20];
	PROPSHEETHEADER psh;
	int pageCount;
	string mainTitle;

public:

	CPropertySheet(const char* title, CWindow* parent);
	void addPage(CPropertyPage& p);
	bool doModal();
	~CPropertySheet();
};

class CTreeView : public CWindow {
public:

	void create(int x, int y, int w, int h, CWindow* parent, HMENU id);
	void clear();
	HTREEITEM addItem(HTREEITEM parent, const char* text, void *data);
	void deleteItem(HTREEITEM i);
	void setItemData(HTREEITEM i, void* data);
	void* getItemData(HTREEITEM i);
	/*
	void setItemData(HTREEITEM* item, void* data);
	void* getItemData(HTREEITEM* item);
	*/
};
