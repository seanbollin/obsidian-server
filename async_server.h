//
// Created by seanbo on 3/11/17.
//

#ifndef OBSIDIANSERVER_WEBSOCKET_ASYNC_SERVER_H
#define OBSIDIANSERVER_WEBSOCKET_ASYNC_SERVER_H

#include <beast/core/placeholders.hpp>
#include <beast/core/streambuf.hpp>
#include <beast/websocket.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>

#include <ostream>

namespace rubymud {
namespace websocket {

class async_server {

public:
    using endpoint_type = boost::asio::ip::tcp::endpoint;
    using address_type = boost::asio::ip::address;
    using socket_type = boost::asio::ip::tcp::socket;
    using error_code = boost::system::error_code;

private:
    boost::asio::io_service ios;
    std::ostream* log;
    socket_type sock;
    boost::asio::ip::tcp::acceptor acceptor;
    boost::optional<boost::asio::io_service::work> work;

public:
    std::vector<std::thread> threads;

    async_server(async_server const&) = delete;
    async_server& operator=(async_server const&) = delete;

    async_server(std::ostream* log, std::size_t num_threads) : log(log), sock(ios), acceptor(ios), work(ios) {
        threads.reserve(num_threads);

        for (std::size_t i = 0; i < num_threads; ++i) {
            threads.emplace_back([this](){ ios.run(); });
        }
    }

    ~async_server() {
        work = boost::none;
        error_code ec;
        ios.dispatch([this,&ec](){ acceptor.close(ec); });

        for (auto& thread : threads) {
            thread.join();
        }
    }

    void open(bool server, endpoint_type const& ep, error_code& ec) {
        if (server) {
            acceptor.open(ep.protocol(), ec);

            if (log_if_error("open", ec))
                return;

            acceptor.set_option(boost::asio::socket_base::reuse_address{true});
            acceptor.bind(ep, ec);

            if (log_if_error("bind", ec))
                return;

            acceptor.listen(boost::asio::socket_base::max_connections, ec);

            if (log_if_error("listen", ec))
                return;

            acceptor.async_accept(
                    sock,
                    std::bind(
                            &async_server::on_accept,
                            this,
                            beast::asio::placeholders::error
                    )
            );
        }
    }

private:
    bool log_if_error(const std::string& log_prefix, const error_code& ec) {
        if (!ec) { return false; }

        if (log)
            (*log) << log_prefix + ": " << ec.message() << std::endl;

        return true;
    }

    void on_accept(error_code ec) {
        if(!acceptor.is_open())
            return;
        if(ec == boost::asio::error::operation_aborted)
            return;
        maybe_throw(ec, "accept");
        socket_type sock(std::move(sock));
        acceptor.async_accept(sock, std::bind(&async_server::on_accept, this, beast::asio::placeholders::error));
        Peer{*this, std::move(sock)};
    }

    void fail(error_code ec, std::string what) {
        if(log)
            (*log) << what << ": " << ec.message() << std::endl;
    }

    void maybe_throw(error_code ec, std::string what) {
        if(ec) {
            fail(ec, what);
            throw ec;
        }
    }

    class Peer {
    public:
        template<typename... Args>
        explicit Peer(async_server& server, socket_type&& sock, Args&&... args)
                : d_(std::make_shared<data>(server, std::forward<socket_type>(sock), std::forward<Args>(args)...))
        {
            auto& d = *d_;
            d.ws.set_option(beast::websocket::read_message_max(64 * 1024 * 1024));
            d.ws.set_option(beast::websocket::auto_fragment{false});
            run();
        }

        void run() {
            auto& d = *d_;
            d.ws.async_accept(std::move(*this));
        }

        void operator()(error_code ec) {}
    private:
        struct data {
            data(async_server& server, socket_type&& sock)
                    : server(server), ws(std::move(sock)), strand(ws.get_io_service()),
                      id([] { static int n = 0; return ++n; }()) {}

            async_server& server;
            int state = 0;
            beast::websocket::stream<socket_type> ws;
            boost::asio::io_service::strand strand;
            beast::websocket::opcode op;
            beast::streambuf db;
            int id;
        };

        std::shared_ptr<data> d_;
    };
};

}
}

#endif //OBSIDIANSERVER_WEBSOCKET_ASYNC_SERVER_H
