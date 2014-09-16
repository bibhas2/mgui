OBJS = frame.o resource.o
CFLAGS = -I..

frame.exe: ../libmgui.a $(OBJS)
	g++ -o frame.exe $(OBJS) -L.. -lmgui -lcomctl32 -lgdi32 -lcomdlg32
frame.o: frame.cc control.h
	g++ -c $(CFLAGS) frame.cc
resource.o: resource.rc
	windres -i resource.rc -o resource.o
