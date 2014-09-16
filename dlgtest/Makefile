OBJS = main.o resource.o
CFLAGS = -I..

test.exe: ../libmgui.a $(OBJS)
	g++ -o dlgtest.exe $(OBJS) -L.. -lmgui -lcomctl32 -lgdi32 -lcomdlg32
main.o: main.cc 
	g++ -c $(CFLAGS) main.cc
#resource.res: resource.rc 
#windres -i resource.rc -o resource.res
resource.o: resource.res 
	windres -i resource.res -o resource.o
