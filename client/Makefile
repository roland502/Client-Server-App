CXX = g++ -std=c++17

LIB = -lpthread -ldl -fpermissive 
BIN = client

all : $(BIN)
client : client.o 
	$(CXX) -o $@ $^ $(LIB) 

clean : 
	rm -f $(BIN)

.PHONY: all, clean
