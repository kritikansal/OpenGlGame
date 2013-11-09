CC = g++
OPTS =  -I"../../include" -L"/usr/lib" ../../bin/linux-gcc/libIrrKlang.so -pthread 
CFLAGS = -Wall
PROG = transform 

SRCS = 9main.cpp imageloader.cpp text3d.cpp glm.cpp  
LIBS = -lglut -lGL -lGLU

all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
