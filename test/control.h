
#define MARKERW 4

class DrawUtil {
public:

	static void drawWhiteBox(HDC hdc, int x, int y, int w, int h) {
		RECT r;

		r.left = x;
		r.top = y;
		r.right = w + x;
		r.bottom = h + y;
		::DrawEdge(hdc, &r, EDGE_SUNKEN, BF_RECT | BF_MIDDLE);
		r.left += 2;
		r.top += 2;
		r.right -= 2;
		r.bottom -= 2;

		::FillRect(hdc, &r, (HBRUSH) (COLOR_WINDOW+1));
	}

	static void drawEdge(HDC hdc, 
		int x, int y, 
		int w, int h) {

		RECT r;

		r.left = x;
		r.top = y;
		r.right = w + x;
		r.bottom = h + y;
		::FillRect(hdc, &r, (HBRUSH) (COLOR_BTNFACE+1));
		::DrawEdge(hdc, &r, EDGE_RAISED, BF_RECT);
	}
	static void drawArrowBox(HDC hdc, 
		int x, int y, 
		int w, int h, 
		int orientation) {

		POINT pts[3];
		int gap = 6;

		drawEdge(hdc, x, y, w, h);
		HBRUSH hbr = (HBRUSH) ::SelectObject(hdc, 
			::GetStockObject(BLACK_BRUSH));


		if (orientation == 1) { //Up
			pts[0].x = x + w / 2;
			pts[0].y = y + gap;

			pts[1].x = x + gap;
			pts[1].y = y + h - gap;

			pts[2].x = x + w - gap;
			pts[2].y = y +h - gap;
		} else if (orientation == 2) { //Down
			pts[0].x = x + w / 2;
			pts[0].y = y + h - gap;

			pts[1].x = x + gap;
			pts[1].y = y + gap;

			pts[2].x = x + w - gap;
			pts[2].y = y + gap;
		}
		::Polygon(hdc, pts, 3);
		::SelectObject(hdc, hbr);
	}
	//Control's dimension not scrollbar's
	static void drawScroll(HDC hdc, int x, int y, int w, int h) {
		int endW = 18;
		drawArrowBox(hdc, x + w - endW, y, endW, endW, 1);
		drawEdge(hdc, x + w - endW, y + endW, endW, h - 2 * endW);
		drawArrowBox(hdc, x + w - endW, y + h - endW, endW, endW, 2);
	}
};

class Box {
public:
	int x, y, w, h;
	bool isSelected;

	void setSelected(bool val) {
		isSelected = val;
	}
	virtual void draw(HDC hdc) = 0;
	virtual void drawHandles(HDC hdc) {
		int W = MARKERW;

		::Rectangle(hdc, x - W, y - W, x + W, y + W);
		::Rectangle(hdc, x + w - W, y - W, x + w + W, y + W);
		::Rectangle(hdc, x - W, y + h - W, x + W, y + h + W);
		::Rectangle(hdc, x + w - W, y + h + - W, x + w + W, y + h + W);

		::Rectangle(hdc, x - W, y + h / 2 - W, x + W, y + h / 2 + W);
		::Rectangle(hdc, x + w / 2 - W, y - W, x + w / 2 + W, y + W);
		::Rectangle(hdc, x + w / 2 - W, y + h - W, x + w / 2 + W, y + h + W);
		::Rectangle(hdc, x + w - W, y + h / 2 - W, x + w + W, y + h / 2 + W);
	}
};

class DialogArea : public Box {
public:
	void draw(HDC hdc) {
		RECT r;

		r.left = x;
		r.top = y;
		r.right = w + x;
		r.bottom = h + y;
		::DrawEdge(hdc, &r, EDGE_RAISED, BF_RECT);
	}
};

class Control : public Box {
public:
	char id[128];
	char type[128];
	int intId;
	char text[128];

	Control() {
		strcpy(id, "0");
		strcpy(text, "Label");
		strcpy(type, "LTEXT");
		x = y = w = h = 0;
		isSelected = false;
		intId = -1;
	}
	//Copy
	Control(Control* pCtrl) {
		copyFrom(pCtrl);
	}

	void copyFrom(Control* c) {
		x = c->x;
		y = c->y;
		w = c->w;
		h = c->h;
		intId = c->intId;
		isSelected = c->isSelected;
		strcpy(id, c->id);
		strcpy(text, c->text);
		strcpy(type, c->type);
	}

	void draw(HDC hdc) {
		::Rectangle(hdc, x, y, x + w, y + h);

		char str[128];
		::sprintf(str, "%s", type);
		::TextOut(hdc, x + 2, y + 2, str, strlen(str));
	}
};

class Label : public Control {
public:
	Label() {
		x = y = 10;
		w = 75;
		h = 15;
		strcpy(text, "Label");
		strcpy(type, "LABEL");
	}
	void draw(HDC hdc) {
		HFONT font = (HFONT) ::SelectObject(hdc, 
			::GetStockObject(DEFAULT_GUI_FONT));
		::SetBkMode(hdc, TRANSPARENT);
		::SetTextAlign(hdc, TA_LEFT | TA_TOP);
		::TextOut(hdc, x, y, text, strlen(text));
		::SelectObject(hdc, font);
	}
};

class Button : public Control {
public:
	Button() {
		x = y = 10;
		w = 75;
		h = 25;
		strcpy(text, "Button");
		strcpy(type, "BUTTON");
	}
	void draw(HDC hdc) {
		HFONT font = (HFONT) ::SelectObject(hdc, 
			::GetStockObject(DEFAULT_GUI_FONT));
		RECT r;

		r.left = x;
		r.top = y;
		r.right = w + x;
		r.bottom = h + y;
		::DrawEdge(hdc, &r, EDGE_RAISED, BF_RECT);
		::SetBkMode(hdc, TRANSPARENT);
		::SetTextAlign(hdc, TA_CENTER | TA_BASELINE);
		::TextOut(hdc, x + w / 2, y + h / 2 + 5, text, strlen(text));
		::SelectObject(hdc, font);
	}
};

class EditSingle : public Control {
public:
	EditSingle() {
		x = y = 10;
		w = 115;
		h = 25;
		strcpy(type, "EDIT");
	}
	void draw(HDC hdc) {
		DrawUtil::drawWhiteBox(hdc, x, y, w, h);
	}
};

class EditMulti : public Control {
public:
	EditMulti() {
		x = y = 10;
		w = 115;
		h = 75;
		strcpy(type, "EDIT_MULTI");
	}
	void draw(HDC hdc) {
		DrawUtil::drawWhiteBox(hdc, x, y, w, h);
		DrawUtil::drawScroll(hdc, x, y, w, h);
	}
};

class ListBox : public Control {
public:
	ListBox() {
		x = y = 10;
		w = 115;
		h = 75;
		strcpy(type, "LISTBOX");
	}
	void draw(HDC hdc) {
		DrawUtil::drawWhiteBox(hdc, x, y, w, h);
		DrawUtil::drawScroll(hdc, x, y, w, h);
	}
};

class ComboBox : public Control {
public:
	ComboBox() {
		x = y = 10;
		w = 115;
		h = 45;
		strcpy(type, "COMBO");
	}
	void draw(HDC hdc) {
		int endW = 22;

		DrawUtil::drawWhiteBox(hdc, x, y, w, endW);
		DrawUtil::drawArrowBox(hdc, x + w - endW, y, endW, endW, 2);
	}
};

class CheckBox : public Control {
public:
	CheckBox() {
		x = y = 10;
		w = 115;
		h = 25;
		strcpy(text, "Check box");
		strcpy(type, "CHECKBOX");
	}
	void draw(HDC hdc) {
		DrawUtil::drawWhiteBox(hdc, x, y, 14, 14);
		HFONT font = (HFONT) ::SelectObject(hdc, 
			::GetStockObject(DEFAULT_GUI_FONT));
		::SetBkMode(hdc, TRANSPARENT);
		::SetTextAlign(hdc, TA_LEFT | TA_TOP);
		::TextOut(hdc, x + 14 + 2, y, text, strlen(text));
		::SelectObject(hdc, font);
	}
};

class RadioButton : public Control {
public:
	RadioButton() {
		x = y = 10;
		w = 115;
		h = 25;
		strcpy(text, "Radio button");
		strcpy(type, "RADIO");
	}
	void draw(HDC hdc) {
		::Ellipse(hdc, x, y, x + 14, y + 14);
		HFONT font = (HFONT) ::SelectObject(hdc, 
			::GetStockObject(DEFAULT_GUI_FONT));
		::SetBkMode(hdc, TRANSPARENT);
		::SetTextAlign(hdc, TA_LEFT | TA_TOP);
		::TextOut(hdc, x + 14 + 2, y, text, strlen(text));
		::SelectObject(hdc, font);
	}
};
