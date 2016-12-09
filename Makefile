obsidian-server : server.o
	clang-3.8 -o obsidian-server server.o -lboost_system -lstdc++ -std=c++11

server.o : server.cpp
	clang-3.8 -c server.cpp -I ~/websocketpp/ -std=c++11

clean : 
	rm obsidian-server server.o 
