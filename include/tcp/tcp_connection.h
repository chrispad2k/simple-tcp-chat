#pragma once

#include <iostream>
#include <string>
#include <boost.h>

class tcp_connection
    : public boost::enable_shared_from_this<tcp_connection>
{
public:
  typedef boost::shared_ptr<tcp_connection> pointer;

  static pointer create(boost::asio::io_service &io_service);

  tcp::socket &socket();

  void start();

private:
  tcp_connection(boost::asio::io_service &io_service) : socket_(io_service) {}

  void handle_write(const boost::system::error_code & /*error*/, size_t /*bytes_transferred*/);

  tcp::socket socket_;
  std::string message_;
};
