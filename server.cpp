#include <beast/websocket.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <iostream>

class WSConnection : public std::enable_shared_from_this<WSConnection>
{
public:
    using boost::asio::ip::tcp;

    // TODO: Make ctor private
    WSConnection(boost::asio::io_service& io_service) : ws_(io_service) {}

    static auto create(boost::asio::io_service& io_service)
    {
        return std::make_shared<WSConnection>(io_service);
    }

    void handshake()
    {
        auto self = shared_from_this();
        ws_.async_accept([self]() {
                    std::cout << "handle_accept" << std::endl;
                    beast::streambuf buf;
                    buf << "Hello from obsidian-server!"; // TODO: This data is going to die?
                    ws_.async_write(buf.data(), [self]() {});
                }
        );
    }

    tcp::socket& socket()
    {
        return ws_.next_layer();
    }

private:
    beast::websocket::stream<boost::asio::ip::tcp::socket> ws_;
};

class RubyMUDServer {
public:
    using boost::asio::ip::tcp;

    RubyMUDServer(boost::asio::io_service& io_service) : acceptor_(io_service, tcp::endpoint(tcp::v4(), 33435))
    {
        start_accept();
    }

private:
    void start_accept()
    {
        auto connection = WSConnection::create(acceptor_.get_io_service());
        acceptor_.async_accept(
                connection->socket(),
                [connection,this] (auto error) {
                    std::cout << "handle_accept" << std::endl;
                    if (!error) {
                       connection->handshake();
                    } else {
                       std::cout << "error: error_code value " << error.value() << std::endl;
                    }

                    this->start_accept();
                }
        );

    }

    tcp::acceptor acceptor_;
};

// Block until SIGINT or SIGTERM is received.
//inline void sig_wait() {
//    boost::asio::io_service ios;
//    boost::asio::signal_set signals(
//            ios, SIGINT, SIGTERM);
//    signals.async_wait([&](boost::system::error_code const&, int){});
//    ios.run();
//}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: server <ip address>" << std::endl;
        return 1;
    }

    try {
        boost::asio::io_service io_service;
        RubyMUDServer server(io_service);
        io_service.run();
        // sig_wait();
    } catch (std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
    }
}