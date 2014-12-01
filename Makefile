CC = g++
OBJS = main.o utils.o

ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -DOSX
	LDFLAGS = -framework GLUT -framework OpenGL \
    	-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
    	-lGL -lGLU -lm -lstdc++
else
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Iglut-3.7.6-bin
	LDFLAGS = -lglut -lGLU
endif
	
RM = /bin/rm -f 
all: main 
main: $(OBJS)
	$(CC) $(CFLAGS) -o as3 $(OBJS) $(LDFLAGS) 
main.o: main.cpp utils.h
	$(CC) $(CFLAGS) -c main.cpp -o main.o
utils.o: utils.h
	$(CC) $(CFLAGS) -c utils.cpp -o utils.o
clean: 
	$(RM) *.o as3 *.png *.out
