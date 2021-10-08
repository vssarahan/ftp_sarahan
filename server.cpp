//
// Created by Владислав Сарахан on 07.10.2021.
//

#include "server.h"

namespace server {
    server::server(
            executor_type exe, short port, Callback callback)
            : acceptor_(exe, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), on_close_(callback) {
    }

    void server::start() { do_accept(); }

    void server::stop() { acceptor_.close(); }
}
