#include <beast/websocket.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <iostream>

using boost::asio::ip::tcp;

class Connection : public std::enable_shared_from_this<Connection>
{
private:
    struct _constructor_tag { explicit _constructor_tag() = default; }; // make_shared private ctor hack

public:
    Connection(_constructor_tag tag, boost::asio::io_service& io_service) : ws_(io_service) {
        buf_ << "Hello from obsidian-server!";
    }

    static auto create(boost::asio::io_service& io_service)
    {
        return std::make_shared<Connection>(_constructor_tag{}, io_service);
    }

    void handshake()
    {
        ws_.async_accept([self = shared_from_this()] (auto error) {
            self->ws_.async_read(self->op_, self->db_, [self] (auto error) {
                self->ws_.async_write(self->buf_.data(), [self] (auto error) {
                    // and continue ...
                });
            });
        });
    }

    tcp::socket& socket()
    {
        return ws_.next_layer();
    }

private:
    beast::websocket::stream<boost::asio::ip::tcp::socket> ws_;
    beast::websocket::opcode op_;
    beast::streambuf db_;
    beast::streambuf buf_;
};

class RubyMUDServer {
public:
    RubyMUDServer(boost::asio::io_service& io_service) : acceptor_(io_service, tcp::endpoint(tcp::v4(), 33435))
    {
        start_accept();
    }

private:
    void start_accept()
    {
        auto connection = Connection::create(acceptor_.get_io_service());
        acceptor_.async_accept(
                connection->socket(),
                [connection, this] (auto error) {
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