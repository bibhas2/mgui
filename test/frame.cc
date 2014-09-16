#include "mgui.h"
#include "canvas.h"
#include <iostream>
#include <vector>
#include <commctrl.h>
#include <math.h>
#include <stdio.h>
#include "resource.h"

#include "control.h"

#define BTNW 75
#define BTNH 25

class CDrawBox;
class Tool {
public:
	CDrawBox *pDrawBox;
	Tool() {
		pDrawBox = NULL;
	}
	void setDrawBox(CDrawBox* d) {
		pDrawBox = d;
	}
	virtual void onBtnDown(int x, int y) = 0;
	virtual void onBtnUp(int x, int y) = 0;
	virtual void onMove(int x, int y) = 0;
};
class CDrawBox : public CCanvas {
public:
	int gridw;
	vector <Control*> controlList;
	Tool *tool;
	Control *selControl;
	int xfactor, yfactor;
	int nextId;
	DialogArea *dlgBox;
	DialogArea *selDialog;
	Control* clipBoard;
	Tool *defaultTool;
	bool bDirty;
public:
	CDrawBox() {
		gridw = 8;
		selControl = NULL;
		selDialog = NULL;
		tool = NULL;
		LONG l = ::GetDialogBaseUnits();
		yfactor = HIWORD(l);
		xfactor = LOWORD(l);
		nextId = 10001;
		clipBoard = NULL;
		bDirty = false;

		dlgBox = new DialogArea();
		dlgBox->x = dlgBox->y = 0;
		dlgBox->w = dlgBox->h = 250;
	}

	void setDirty(bool val) {
		bDirty = val;
	}
	bool isDirty() {
		return bDirty;
	}
	void addControl(Control *c) {
		controlList.push_back(c);
		setDirty(true);
	}
	void addNewControl(Control *c) {
		c->intId = generateId();
		c->x += dlgBox->x;
		c->y += dlgBox->y;
		addControl(c);
	}
	int generateId() {
		return nextId++;
	}
	int getNextId() {
		return nextId;
	}
	void setNextId(int i) {
		nextId = i;
	}
	int xConv(int i) {
		return (i * 4) / xfactor;
	}
	int yConv(int i) {
		return (i * 8) / yfactor;
	}

	void snap(int& x, int& y) {
		double d, fd, cd;

		d = ((double) x) / gridw;
		fd = ::floor(d);
		cd = ::ceil(d);
		if ((d - fd) < (cd - d)) {
			x = (int) (fd * gridw);
		} else {
			x = (int) (cd * gridw);
		}

		d = ((double) y) / gridw;
		fd = ::floor(d);
		cd = ::ceil(d);
		if ((d - fd) < (cd - d)) {
			y = (int) (fd * gridw);
		} else {
			y = (int) (cd * gridw);
		}
		/*
		int dx = x % gridw;
		int dy = y % gridw;
		if (dx < gridw / 2) {
			x -= dx;
		} else {
			x += gridw;
		}
		if (dy < gridw / 2) {
			y -= dy;
		} else {
			y += gridw;
		}
		*/
	}

	void onLeftMouseDown(int x, int y) {
		if (tool == NULL) {
			return;
		}
		tool->onBtnDown(x, y);
	}
	void onLeftMouseUp(int x, int y) {
		if (tool == NULL) {
			return;
		}
		tool->onBtnUp(x, y);
	}
	void onMouseMove(int x, int y) {
		if (tool == NULL) {
			return;
		}
		tool->onMove(x, y);
	}
	void onPaint(PAINTSTRUCT& ps) {
		dlgBox->draw(ps.hdc);
		for (int i = 0; i < controlList.size(); ++i) {
			controlList[i]->draw(ps.hdc);
		}
		if (selControl != NULL) {
			selControl->drawHandles(ps.hdc);
		} else if (selDialog != NULL) {
			selDialog->drawHandles(ps.hdc);
		}
	}

	void transformControls(int dx, int dy) {
		if (dx == 0 && dy == 0) {
			return;
		}

		for (int i = 0; i < controlList.size(); ++i) {
			Control *c = controlList[i];

			c->x += dx;
			c->y += dy;
		}
		redraw();
	}

	void setDefaultTool(Tool *d) {
		defaultTool = d;
	}
	void switchToDefaultTool() {
		setTool(defaultTool);
	}
	void setTool(Tool* t) {
		tool = t;
		tool->setDrawBox(this);
	}
	bool ptInRect(int x, int y, int rx, int ry, int rw, int rh, int grace) {
		if (x >= rx - grace && 
		    y >= ry - grace &&
		    x <= rx + rw + grace &&
		    y <= ry + rh + grace) {
		    return true;
		}
		return false;
	}
	Control* hitTest(int x, int y) {

		for (int i = controlList.size() - 1; i >= 0; --i) {
			Control *c = controlList[i];

			if (ptInRect(x, y, 
				c->x, c->y,
				c->w, c->h, MARKERW)) {
				return c;
			}
		}

		return NULL;
	}

	DialogArea* hitDialogTest(int x, int y) {
		if (ptInRect(x, y, 
			dlgBox->x, dlgBox->y,
			dlgBox->w, dlgBox->h, MARKERW)) {
			return dlgBox;
		}

		return NULL;
	}
	void select(Control *c) {
		if (selControl != NULL) {
			selControl->setSelected(false);
			redraw(selControl);
		}
		selControl = c;
		if (selControl != NULL) {
			selControl->setSelected(true);
			redraw(selControl);
			selectDialog(NULL);
		}
	}
	void selectDialog(DialogArea* d) {
		if (d == NULL) {
			if (selDialog != NULL) {
				redraw(selDialog);
			}
			selDialog = NULL;
			return;
		}

		if (d != dlgBox) {
			throw "Invalid dialog selected.";
		}
		select(NULL);
		selDialog = d;
		redraw(selDialog);
	}
	Control* getSelected() {
		return selControl;
	}
	DialogArea* getSelectedDialog() {
		return selDialog;
	}
	void erase(Box *ec) {
		if (ec == NULL) {
			return;
		}
		int count = controlList.size();
		for (int i = 0; i < count; ++i) {
			Control *c = controlList[i];
			if (c == ec) {
				controlList.erase(controlList.begin() + i);
				if (c == getSelected()) {
					select(NULL);
				}
				delete c;
				break;
			}
		}
		setDirty(true);
	}
	void redraw() {
		::InvalidateRect(m_wnd, NULL, TRUE);
		cout << "Complete redraw" << endl;
	}
	void redraw(Box *c) {
		RECT r;

		r.left = c->x - MARKERW;
		r.top = c->y - MARKERW;
		r.right = c->x + c->w + MARKERW;
		r.bottom = c->y + c->h + MARKERW;
		::InvalidateRect(m_wnd, &r, TRUE);
	}
	void clearList() {
		int count = controlList.size();
		for (int i = 0; i < count; ++i) {
			Control *c = controlList[i];

			delete c;
		}
		controlList.clear();
		select(NULL);
		setDirty(true);
	}

	void initClipBoard() {
		if (clipBoard == NULL) {
			clipBoard = new Control();
		}
	}
	void clearClipBoard() {
		if (clipBoard != NULL) {
			delete clipBoard;
			clipBoard = NULL;
		}
	}
	void copy() {
		Control *c = getSelected();

		clearClipBoard();
		if (c != NULL) {
			clipBoard = cloneTemplate(c);
		}
	}
	void paste() {
		if (clipBoard == NULL) {
			return;
		}
		//Move it slightly
		clipBoard->x += gridw;
		clipBoard->y += gridw;

		Control *c = cloneTemplate(clipBoard);
		addControl(c);
		select(c);
	}
	void cut() {
		copy();
		erase(getSelected());
	}

	void openFile(const char* fileName) {
		FILE *pF = ::fopen(fileName, "r");
		if (pF == NULL) {
			throw "Could not open file.";
		}

		int count, nextId, version;
		char fmt[64];

		::fread(fmt, 1, sizeof(fmt), pF);
		if (::strcmp(fmt, "MGUI") != 0) {
			::fclose(pF);
			throw "Invalid file format.";
		}
		::fread(&version, sizeof(version), 1, pF);
		cout << "File version: " << version << endl;
		if (version != 1) {
			::fclose(pF);
			throw "Wrong file version.";
		}
		::fread(&count, sizeof(count), 1, pF);
		cout << "Item count: " << count << endl;
		::fread(&nextId, sizeof(nextId), 1, pF);
		setNextId(nextId);

		::fread(dlgBox, 1, sizeof(DialogArea), pF);
		clearList();
		Control c; 
		for (int i = 0; i < count; ++i) {
			::fread(&c, 1, sizeof(Control), pF);
			if (feof(pF) != 0) {
				throw "Premature end of file.";
			}
			Control *pC = cloneTemplate(&c);
			addControl(pC);
			if (pC->isSelected) {
				select(pC);
			}
		}
		::fclose(pF);
		setDirty(false);
	}

	Control* cloneTemplate(Control* c) {
		Control *pC = NULL;

		if (::strcmp(c->type, "LABEL") == 0) {
			pC = new Label();
		} else if (::strcmp(c->type, "BUTTON") == 0) {
			pC = new Button();
		} else if (::strcmp(c->type, "EDIT") == 0) {
			pC = new EditSingle();
		} else if (::strcmp(c->type, "EDIT_MULTI") == 0) {
			pC = new EditMulti();
		} else if (::strcmp(c->type, "LISTBOX") == 0) {
			pC = new ListBox();
		} else if (::strcmp(c->type, "COMBO") == 0) {
			pC = new ComboBox();
		} else if (::strcmp(c->type, "CHECKBOX") == 0) {
			pC = new CheckBox();
		} else if (::strcmp(c->type, "RADIO") == 0) {
			pC = new RadioButton();
		}
		if (pC == NULL) {
			throw "Invalid control type.";
		}
		pC->copyFrom(c);

		return pC;
	}

	void saveFile(const char* fileName) {
		FILE *pF = ::fopen(fileName, "w");
		if (pF == NULL) {
			throw "Could not open file to save.";
		}

		int count = controlList.size();
		int nextId = getNextId();
		int version = 1;
		char fmt[64];

		strcpy(fmt, "MGUI");
		::fwrite(fmt, 1, sizeof(fmt), pF);
		::fwrite(&version, sizeof(version), 1, pF);
		::fwrite(&count, sizeof(count), 1, pF);
		::fwrite(&nextId, sizeof(int), 1, pF);

		::fwrite(dlgBox, 1, sizeof(DialogArea), pF);
		cout << xConv(dlgBox->x) <<
			", " << yConv(dlgBox->y) <<
			", " << xConv(dlgBox->w) <<
			", " << yConv(dlgBox->h) << endl;
		for (int i = 0; i < count; ++i) {
			Control *c = controlList[i];
			::fwrite(c, 1, sizeof(Control), pF);
			cout << c->type << " \"" << c->text << 
				"\", " << c->id <<
				", " << xConv(c->x) <<
				", " << yConv(c->y) <<
				", " << xConv(c->w) <<
				", " << yConv(c->h) << endl;

		}
		::fclose(pF);

		string resfile(fileName);

		resfile.append(".rc");
		/*
		pF = ::fopen(resfile.c_str(), "w");
		if (pF == NULL) {
			throw "Could not open file to save .rc file.";
		}
		for (int i = 0; i < count; ++i) {
			Control *c = cv.controlList[i];
			::fwrite(c, 1, sizeof(Control), pF);
		}
		*/
		setDirty(false);
	}
};

class CreateTool : public Tool {
	bool bDrawing;
	int ix, iy, px, py;
public:
	CreateTool() {
		bDrawing = false;
	}

	void onBtnDown(int x, int y) {
		pDrawBox->snap(x, y);
		ix = px = x;
		iy = py = y;
		bDrawing = true;
		::SetCapture(pDrawBox->m_wnd);
	}

	void onBtnUp(int x, int y) {
		pDrawBox->snap(x, y);
		px = x;
		py = y;
		bDrawing = false;
		::ReleaseCapture();

		Control *c = new Control();
		c->x = ix < px ? ix : px;
		c->y = iy < py ? iy : py;
		c->w = abs(ix - px);
		c->h = abs(iy - py);

		pDrawBox->addNewControl(c);
		pDrawBox->select(c);
		pDrawBox->switchToDefaultTool();
	}
	void onMove(int x, int y) {
		if (bDrawing == false)
			return;
		
		pDrawBox->snap(x, y);
		HDC dc = ::GetDC(pDrawBox->m_wnd);
		::SetROP2(dc, R2_NOT);
		::Rectangle(dc, ix, iy, px, py);
		px = x;
		py = y;
		::Rectangle(dc, ix, iy, px, py);
		::ReleaseDC(pDrawBox->m_wnd, dc);
	}
};

class SelectTool : public Tool {
	bool bDragMode, bMoveMode;
	int ix, iy, px, py;
	int fx, fy, fw, fh; //factors
public:
	SelectTool() {
		bDragMode = false;
		bMoveMode = false;
		ix = iy = px = py = 0;
		fx = fy = fx = fy = 0;
	}

	void computeFactors(int x, int y, Box *c) {
		int grace = 4;

		fx = fy = fw = fh = 0;
		if (pDrawBox->ptInRect(x, y, c->x, c->y, c->w, 0, grace)) {
			//Top line was clicked
			fy = 1;
			fh = -1;
		}
		if (pDrawBox->ptInRect(x, y, c->x, c->y, 0, c->h, grace)) {
			//Left line was clicked
			fx = 1;
			fw = -1;
		}
		if (pDrawBox->ptInRect(x, y, c->x, c->y + c->h, c->w, 0, grace)) {
			//Bottom line was clicked
			fh = 1;
		}
		if (pDrawBox->ptInRect(x, y, c->x + c->w, c->y, 0, c->h, grace)) {
			//Right line was clicked
			fw = 1;
		}
		cout << fx << fy << fw << fh << endl;
		if (fx == 0 && fy == 0 && fw == 0 && fh == 0) {
			//Clicked right in middle
			fx = fy = 1;
		}
	}

	void onBtnDown(int x, int y) {
		//Hit test
		Box *c = pDrawBox->hitTest(x, y);
		pDrawBox->select(NULL); //Clear
		pDrawBox->selectDialog(NULL); //Clear
		if (c != NULL) {
			pDrawBox->select((Control*) c);
		} else {
			DialogArea *d = pDrawBox->hitDialogTest(x, y);
			if (d != NULL) {
				pDrawBox->selectDialog(d);

				c = d;
			} else {
				return;
			}
		}
		//See where we clicked.
		computeFactors(x, y, c);

		pDrawBox->snap(x, y);
		bDragMode = true;
		bMoveMode = false;
		ix = px = x;
		iy = py = y;
	}
	void onBtnUp(int x, int y) {
		bDragMode = false;
		
		if (bMoveMode == false) {
			return;
		}

		bMoveMode = false;

		Box *c = pDrawBox->getSelected();
		if (c == NULL) {
			c = pDrawBox->getSelectedDialog();
			if (c == NULL) {
				return;
			}
		}
		pDrawBox->snap(x, y);
		int dx = px - ix;
		int dy = py - iy;

		pDrawBox->redraw(c);
		c->x = c->x + dx * fx;
		c->y = c->y + dy * fy;
		c->w = c->w + dx * fw;
		c->h = c->h + dy * fh;
		pDrawBox->redraw(c);
		
		if (c == pDrawBox->getSelectedDialog()) {
			//The dialog was moved
			pDrawBox->transformControls(dx * fx, dy * fy);
		}
	}
	void onMove(int x, int y) {
		Box *c = pDrawBox->getSelected();
		if (c == NULL) {
			c = pDrawBox->getSelectedDialog();
			if (c == NULL) {
				return;
			}
		}

		if (bDragMode == false || c == NULL) {
			return;
		}

		if (bMoveMode == false) {
			if (::abs(x - ix) > 4 || ::abs(y - iy) > 4) {
				bMoveMode = true;
			}
		}
		if (bMoveMode == false) {
			return;
		}

		pDrawBox->snap(x, y);
		HDC dc = ::GetDC(pDrawBox->m_wnd);
		int dx = px - ix;
		int dy = py - iy;

		::SetROP2(dc, R2_NOT);
		::Rectangle(dc, 
			c->x + dx * fx, c->y + dy * fy, 
			c->x + dx * fx + c->w + dx * fw, c->y + dy * fy + c->h + dy * fh);
		px = x;
		py = y;

		dx = px - ix;
		dy = py - iy;
		::Rectangle(dc, 
			c->x + dx * fx, c->y + dy * fy, 
			c->x + dx * fx + c->w + dx * fw, c->y + dy * fy + c->h + dy * fh);
		::ReleaseDC(pDrawBox->m_wnd, dc);
	}
};

class CPropertyDialog : public CDialog {
public:
	CEdit id, text;
	Control *control;
	CComboBox types;

	CPropertyDialog(CWindow *p, Control *c) :
		CDialog("PropertyDialog", p) {
		control = c;
	}
	void onInitDialog() {
		bind(id, ID_CONTROL_ID);
		bind(text, ID_CONTROL_TEXT);
		bind(types, ID_CONTROL_TYPE);

		char *typeName[] = {
			"Label",
			"Edit",
			"Button",
			"Combo box",
			"List box",
			NULL
		};
		char *typeId[] = {
			"LTEXT",
			"EDITTEXT",
			"PUSHBUTTON",
			"COMBOBOX",
			"LISTBOX",
			NULL
		};

		id.setText(control->id);
		text.setText(control->text);
		for (int i = 0; typeName[i] != NULL; ++i) {
			types.addItem(typeName[i]);
			types.setItemData(i, typeId[i]);
			if (strcmp(control->type, typeId[i]) == 0) {
				types.setSel(i);
			}
		}
		if (types.getSel() < 0) {
			types.setSel(0);
		}
	}
	void onOK() {
		string str;
		id.getText(str);
		if (str.length() > 0) {
			strcpy(control->id, str.c_str());
		} else {
			strcpy(control->id, "0");
		}
		text.getText(str);
		strcpy(control->text, str.c_str());
		//Get type ID
		strcpy(control->type, 
			(const char*) types.getItemData(types.getSel()));
		CDialog::onOK();
	}
};

class CMyFrame : public CFrame {
	CDrawBox cv;
	CreateTool createTool;
	SelectTool selectTool;
public:
void create() {
	CFrame::create("Dialog Editor", 700, 450, "MainMenu");
	int i = 0;

	cv.create(0, 0, 0, 0, this);
	cv.setDefaultTool(&selectTool);
	cv.switchToDefaultTool();
}

void onClose() {
	if (cv.isDirty()) {
		if (questionBox("The design has changed.\nWould you like to save the file?")) {
		save();
		}
	}
	stop();
}

void save() {
	CFileDialog fd("Save file", 
		"All files|*.*",
		this);
	if (fd.saveFile()) {
		cv.saveFile(fd.getFileName());
	}
}

void onCommand(int id, int type, CWindow* source) {
try {
	if (id == _EXIT) {
		onClose();
	} else if (id == _SAVE) {
		save();
	} else if (id == _OPEN) {
		CFileDialog fd("Open file", 
			"All files|*.*",
			this);
		if (fd.openFile()) {
			cv.openFile(fd.getFileName());
			cv.redraw();
		}
	} else if (id == _PROPERTIES) {
		Box *c = cv.getSelected();
		if (c == NULL) {
			throw "Please select a control.";
		}
		CPropertyDialog dlg(this, (Control*) c);

		if (dlg.doModal() == true) {
			cv.redraw(c);
		}
	} else if (id == ID_NEW_LABEL) {
		Control *c = new Label();
		cv.addNewControl(c);
		cv.select(c);
	} else if (id == ID_NEW_RADIO) {
		Control *c = new RadioButton();
		cv.addNewControl(c);
		cv.select(c);
	} else if (id == ID_NEW_EDIT_MULTI) {
		Control *c = new EditMulti();
		cv.addNewControl(c);
		cv.select(c);
	} else if (id == ID_NEW_LISTBOX) {
		Control *c = new ListBox();
		cv.addNewControl(c);
		cv.select(c);
	} else if (id == ID_NEW_BUTTON) {
		Control *c = new Button();
		cv.addNewControl(c);
		cv.select(c);
	} else if (id == ID_NEW_CHECKBOX) {
		Control *c = new CheckBox();
		cv.addNewControl(c);
		cv.select(c);
	} else if (id == ID_NEW_COMBO) {
		Control *c = new ComboBox();
		cv.addNewControl(c);
		cv.select(c);
	} else if (id == ID_NEW_EDIT) {
		Control *c = new EditSingle();
		cv.addNewControl(c);
		cv.select(c);
	} else if (id == _SELECT) {
		cv.setTool(&selectTool);
	} else if (id == _DELETE) {
		cv.cut();
		cv.redraw();
	} else if (id == _COPY) {
		cv.copy();
	} else if (id == _PASTE) {
		cv.paste();
	}
} catch (const char* msg) {
	errorBox(msg);
}
}

void onSize(int w, int h) {
	cv.resize(5, BTNH + 10, w - 10, h - 65);
}
void onDestroy() {
}

};

int PASCAL WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
try {
	CWindow::init(hInstance);
	CMyFrame wnd;
	wnd.create();
	wnd.show();
	CWindow::loop();
} catch (const char* mesg) {
	cout << "GetLastError():" << ::GetLastError() << endl;
	Msg(mesg);
}
	return 0;
}
