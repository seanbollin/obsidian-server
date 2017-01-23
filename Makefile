INSTALL = /usr/bin/install
prefix = /usr/local
bindir = $(prefix)/bin

includes = includes/beast/include/

obsidian-server: server.o
	$(CXX) -o obsidian-server server.o -lpthread -lboost_system -lstdc++ -std=c++11

server.o: server.cpp
	$(CXX) -D BOOST_ASIO_HAS_STD_CHRONO -c server.cpp -I $(includes) -std=c++11

clean: 
	rm obsidian-server server.o 

install: obsidian-server
	$(INSTALL) obsidian-server $(bindir)/obsidian-server


