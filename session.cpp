//
// Created by Владислав Сарахан on 07.10.2021.
//

#include "session.h"

namespace session {
    session::session(tcp::socket socket) : socket_(std::move(socket)) {}

    void session::start()
    {
        boost::asio::spawn(socket_.get_executor(), [this](boost::asio::yield_context yield) {
            do_read_init(yield);
        });
        message("Welcome");
    }

    void session::start_listener(Port port)
    {
        auto [it, inserted] = g_listeners.try_emplace(port);

        if (!inserted) {
            message("Already listening on port " + std::to_string(port));
        } else {
            auto on_close = [handle = weak_from_this(), port] {
                if (auto self = handle.lock())
                    self->message("The listener for port " + std::to_string(port) + " has closed");
            };
            auto s = std::make_shared<server::server>( //
                    socket_.get_executor(), port, on_close);
            it->second = s;
            s->start();
            message("Started listening on port " + std::to_string(port));
        }
    }

    void session::stop_listener(Port port)
    {
        auto it = g_listeners.find(port);
        if (it != g_listeners.end()) {
            if (auto server = it->second.lock())
            {
                message("Stopping listener on port " + std::to_string(port));
                server->stop();
            } else {
                // when two connections simultaneously STOP the same listener?
                message("Listener on port " + std::to_string(port) + " already stopped");
            }
            g_listeners.erase(it);
        } else {
            message("No listener on port " + std::to_string(port));
        }
    }
}