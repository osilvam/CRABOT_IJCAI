EXECUTABLE = HyperNeat
VPATH = ./src ./headers ./objects
CC = g++ -O3
CFLAGS = -g -Wall -fPIC -I./headers
LDFLAGS = -lpthread -L. -lneat
OBJS = ./objects/QVlearning.o 

all: QVlearning.o 
	
QVlearning.o: QVlearning.cpp
	@mkdir -p objects
	@echo "Compiling QVlearning.cpp to QVlearning.o"
	@$(CC) $(CFLAGS) -c ./src/QVlearning.cpp  -o ./objects/QVlearning.o

clean:
	@rm -f ./objects/*.o

#cleandocs:
#	@rm -f -R ./doc

install:
	g++ -shared -Wl,-soname,libqvlearning.so.1 -o libqvlearning.so.1.0 $(OBJS) $(LDFLAGS)
	ln -sf libqvlearning.so.1.0 libqvlearning.so
	ln -sf libqvlearning.so.1.0 libqvlearning.so.1
	mv libqvlearning.so.1.0 libqvlearning.so libqvlearning.so.1 /usr/lib
	mkdir -p /usr/include/QVLEARNING_headers/
	cp ./headers/* /usr/include/QVLEARNING_headers/
	cp QVLEARNING /usr/include
	chmod go+r /usr/include/QVLEARNING_headers/*
	chmod go+r /usr/include/QVLEARNING

#docs:
#	@mkdir -p doc
#	@doxygen HYPERNEAT_doxyfile 