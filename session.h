//
// Created by Владислав Сарахан on 07.10.2021.
//

#ifndef FTS_SARAHAN_SESSION_H
#define FTS_SARAHAN_SESSION_H

#include "helper.h"
#include "server.h"

using Server = std::weak_ptr<class server>;
std::map<Port, Server> g_listeners;

namespace session {
    class session : public std::enable_shared_from_this<session> {
    public:
        session(tcp::socket socket);

        void start();

        void start_listener(Port port);
        void stop_listener(Port port);

    private:
        void do_read_init(boost::asio::yield_context yield) {
            auto self(shared_from_this());
            do_read(yield);
            std::string line;
            getline(std::istream(&data_), line);
            if (handle(line))
                boost::asio::spawn(socket_.get_executor(), [this](boost::asio::yield_context yield) {
                    do_read_init(yield);
                });
            else
                socket_.shutdown(tcp::socket::shutdown_both);
        }

        void do_read(boost::asio::yield_context yield)
        {
            async_read_until(
                    socket_, data_, "\n",
                    yield);
        }

        bool handle(std::string const& command)
        {
            try {
                std::istringstream parser(command);
                parser.exceptions(std::ios::failbit | std::ios::badbit);

                std::string cmd;
                Port        port;

                if (parser >> cmd && (cmd == "LISTEN") && parser >> port) {
                    start_listener(port);
                    return true;
                }

                parser.clear();
                parser.str(command);
                if (parser >> cmd && (cmd == "STOP") && parser >> port) {
                    stop_listener(port);
                    return true;
                }

                parser.clear();
                parser.str(command);
                if (parser >> cmd && (cmd == "EXIT")) {
                    message("Goodbye");
                    return false;
                }
                message("Invalid command");
            } catch (std::exception const& e) {
                message("Invalid argument");
            }
            return true;
        }

        void message(std::string msg) {
            outbox_.push_back(std::move(msg) + "\n");
            if (outbox_.size() == 1) {
                auto self(shared_from_this());
                boost::asio::spawn(socket_.get_executor(), [this](boost::asio::yield_context yield) {
                    do_write_init(yield);
                });
            }
        }

        void do_write_init(boost::asio::yield_context yield) {
            auto self(shared_from_this());
            do_write(yield);
            outbox_.pop_front();
            if (!outbox_.empty())
                boost::asio::spawn(socket_.get_executor(), [this](boost::asio::yield_context yield) {
                    do_write_init(yield);
                });
        }

        void do_write(boost::asio::yield_context yield) {
            async_write( //
                    socket_, boost::asio::buffer(outbox_.front()),
                    yield);
        }

        tcp::socket socket_;
        boost::asio::streambuf data_{32*1024}; // max size
        std::deque<std::string> outbox_;
    };

}

#endif //FTS_SARAHAN_SESSION_H
