#include <beast/websocket.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <iostream>

using boost::asio::ip::tcp;

class ws_connection : public boost::enable_shared_from_this<ws_connection>
{
public:
    typedef boost::shared_ptr<ws_connection> pointer;

    static pointer create(boost::asio::io_service& io_service)
    {
        return pointer(new ws_connection(io_service));
    }

    void handshake()
    {
        ws_.async_accept(boost::bind(&ws_connection::handle_accept, shared_from_this()));
    }

    tcp::socket& socket()
    {
        return ws_.next_layer();
    }

private:
    ws_connection(boost::asio::io_service& io_service) : ws_(io_service)
    {
        response_ << "Hello from obsidian-server!";
    }

    void handle_accept() {
        std::cout << "handle_accept" << std::endl;
        ws_.async_write(response_.data(), boost::bind(&ws_connection::handle_write, shared_from_this()));
    }

    void handle_write() {
        std::cout << "handle_write" << std::endl;
    }

    beast::websocket::stream<boost::asio::ip::tcp::socket> ws_;
    beast::streambuf response_;
};

class rubymud_server {
public:
    using error_code = boost::system::error_code;

    rubymud_server(boost::asio::io_service& io_service) : acceptor_(io_service, tcp::endpoint(tcp::v4(), 33435))
    {
        start_accept();
    }

private:
    void start_accept()
    {
        ws_connection::pointer new_connection = ws_connection::create(acceptor_.get_io_service());
        acceptor_.async_accept(new_connection->socket(),
                boost::bind(
                        &rubymud_server::handle_accept,
                        this,
                        new_connection,
                        boost::asio::placeholders::error
                )
        );
    }

    void handle_accept(ws_connection::pointer new_connection, const error_code& error) {
        std::cout << "handle_accept" << std::endl;

        if (!error) {
            new_connection->handshake();
        } else {
            std::cout << "error: error_code value " << error.value() << std::endl;
        }

        start_accept();
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
        rubymud_server server(io_service);
        io_service.run();
        // sig_wait();
    } catch (std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
    }
}