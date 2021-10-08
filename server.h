//
// Created by Владислав Сарахан on 07.10.2021.
//


#ifndef FTS_SARAHAN_SERVER_H
#define FTS_SARAHAN_SERVER_H

#include "helper.h"
#include "session.h"


namespace server {
    class server : public std::enable_shared_from_this<server> {
    public:
        server(executor_type exe, short port, Callback callback = [] {});

        void start();

        void stop();

    private:
        void do_accept() {
            acceptor_.async_accept(
                    [this, self = shared_from_this()] //
                            (boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
                        if (!ec) {
                            std::make_shared<session::session>(std::move(socket))->start();
                            do_accept();
                        } else {
                            if (on_close_)
                                on_close_();
                        }
                    });
        }

        boost::asio::ip::tcp::acceptor acceptor_;
        Callback on_close_;
    };
}
#endif //FTS_SARAHAN_SERVER_H
