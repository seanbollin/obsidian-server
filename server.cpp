#include "websocket_async_echo_server.hpp"

/// Block until SIGINT or SIGTERM is received.
inline void sig_wait() {
    boost::asio::io_service ios;
    boost::asio::signal_set signals(
        ios, SIGINT, SIGTERM);
    signals.async_wait([&](boost::system::error_code const&, int){});
    ios.run();
}

int main() {
    using endpoint_type = boost::asio::ip::tcp::endpoint;
    using address_type = boost::asio::ip::address;

    boost::system::error_code ec;

    try {
        beast::websocket::async_echo_server server{&std::cout, 1};
        server.open(true, endpoint_type{address_type::from_string("127.0.0.1"), 33435}, ec);
        sig_wait();
    } catch(std::exception const& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}