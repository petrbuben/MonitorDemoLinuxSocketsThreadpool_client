client: main.o
	g++ main.o -o client -lpthread
client.o: main.cpp
	g++ -c main.cpp myqeue.hpp
clean:
	rm -f *.o