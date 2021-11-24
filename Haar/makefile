CC = g++
CFLAGS = -g -Wall
SRCS = objectDetection.cpp
PROG = objectDetection

OPENCV =`pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)