CFLAGS=-g
OBJS = tv.o btn.o window.o edt.o lb.o lv.o cdlg.o dlg.o canvas.o combo.o label.o dtp.o prop.o richedt.o

all: libmgui.a
clean:
	rm *.o
	
libmgui.a: $(OBJS)
	ar r libmgui.a $(OBJS)
dlg.o: dlg.cc mgui.h
	g++ $(CFLAGS) -c dlg.cc
cdlg.o: cdlg.cc mgui.h
	g++ $(CFLAGS) -c cdlg.cc
lv.o: lv.cc mgui.h
	g++ $(CFLAGS) -c lv.cc
tv.o: tv.cc mgui.h
	g++ $(CFLAGS) -c tv.cc
btn.o: btn.cc mgui.h
	g++ $(CFLAGS) -c btn.cc
window.o: window.cc mgui.h
	g++ $(CFLAGS) -c window.cc
edt.o: edt.cc mgui.h
	g++ $(CFLAGS) -c edt.cc
lb.o: lb.cc mgui.h
	g++ $(CFLAGS) -c lb.cc
combo.o: combo.cc mgui.h
	g++ $(CFLAGS) -c combo.cc
label.o: label.cc mgui.h
	g++ $(CFLAGS) -c label.cc
dtp.o: dtp.cc mgui.h 
	g++ $(CFLAGS) -c dtp.cc
canvas.o: canvas.cc mgui.h canvas.h
	g++ $(CFLAGS) -c canvas.cc
prop.o: prop.cc mgui.h
	g++ $(CFLAGS) -c prop.cc
richedt.o: richedt.cc mgui.h
	g++ $(CFLAGS) -c richedt.cc
