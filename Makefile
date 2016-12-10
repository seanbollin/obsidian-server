obsidian-server : server.o
	$(CXX) -o obsidian-server server.o -lboost_system -lstdc++ -std=c++11

server.o : server.cpp
	$(CXX) -c server.cpp -I ~/websocketpp/ -std=c++11

clean : 
	rm obsidian-server server.o 
