class CCanvas : public CWindow {
	PAINTSTRUCT ps;
public:
	void create(int x, int y, int width, int height, CWindow *parent);
	bool handleEvent(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void onPaint(PAINTSTRUCT& ps) { }
	virtual void onLeftMouseDown(int x, int y){ }
	virtual void onLeftMouseUp(int x, int y){ }
	virtual void onRightMouseDown(int x, int y){ }
	virtual void onRightMouseUp(int x, int y){ }
	virtual void onMouseMove(int x, int y){ }
};
