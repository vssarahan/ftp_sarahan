//
// Created by Владислав Сарахан on 08.10.2021.
//

#ifndef FTS_SARAHAN_HELPER_H
#define FTS_SARAHAN_HELPER_H

#include <boost/asio.hpp>
#include <deque>
#include <iostream>
#include <iomanip>
#include <boost/asio/spawn.hpp>

using boost::asio::ip::tcp;
using boost::system::error_code;
using executor_type = boost::asio::any_io_executor;
using namespace std::literals;

using Port = uint16_t;
using Callback = std::function<void()>;

#endif //FTS_SARAHAN_HELPER_H
