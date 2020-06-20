CXX = g++ -std=c++17
cc = gcc

LIB = -lpthread -ldl  
BIN = sqlite server 

all : $(BIN)
sqlite : sqlite3.c shell.c
	$(cc) -o $@ $^ $(LIB)
server : server.cpp database.cpp sqlite.o 
	$(CXX) -o $@ $^ $(LIB) 
sqlite.o : sqlite3.c
	$(cc) -o $@ -c $^

clean : 
	rm -f $(BIN)

.PHONY: all, clean
