includes = includes/websocketpp/

obsidian-server : server.o
	$(CXX) -D BOOST_ASIO_HAS_STD_CHRONO -o obsidian-server server.o -lboost_system -lstdc++ -std=c++11

server.o : server.cpp
	$(CXX) -c server.cpp -I $(includes) -std=c++11

clean : 
	rm obsidian-server server.o 
