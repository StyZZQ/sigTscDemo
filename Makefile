TARGET=gb2017
CXXFLAGS=-std=c++11 
CXXFLAGS += -Wno-format-security \
           -I../RapidJSON/include

LIBSPATH=-L/opt/sql_build/lib
LIBS=-lsqlite3 -latomic -lrt -lpthread

OBJS = main.o value.o gb2017.o frame.o operate.o tcpServer.o

CXX=g++

all: clean $(TARGET)
	$(RM) *.o

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LIBSPATH) $(LIBS)

clean:
	$(RM) *.o $(TARGET)