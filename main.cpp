#include <boost/asio.hpp>
#include <deque>
#include <iostream>
#include <iomanip>
#include <boost/asio/spawn.hpp>
#include "server.h"
#include "session.h"

using boost::asio::ip::tcp;
using boost::system::error_code;
using executor_type = boost::asio::any_io_executor;
using namespace std::literals;

using Port     = uint16_t;
using Callback = std::function<void()>;
using Server   = std::weak_ptr<class server>;
std::map<Port, Server> g_listeners;

int main()
{
    try {

        boost::asio::io_context io_context;

        {
            Port port = 21;
            auto s = std::make_shared<server>(io_context.get_executor(), port);
            s->start();

            g_listeners.emplace(port, s);
        }

        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}
