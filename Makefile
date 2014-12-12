CC = g++
INCLUDE = -I ./eigen2
OBJS = main.o arm.o

ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -DOSX $(INCLUDE)
	LDFLAGS = -framework GLUT -framework OpenGL \
    	-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
    	-lGL -lGLU -lm -lstdc++
else
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Iglut-3.7.6-bin $(INCLUDE)
	LDFLAGS = -lglut -lGLU
endif
	
RM = /bin/rm -f 
all: main 
main: $(OBJS)
	$(CC) $(CFLAGS) -o as4 $(OBJS) $(LDFLAGS) 
main.o: main.cpp arm.h
	$(CC) $(CFLAGS) -c main.cpp -o main.o
arm.o: arm.h
	$(CC) $(CFLAGS) -c arm.cpp -o arm.o
clean: 
	$(RM) *.o as4 *.png *.out
