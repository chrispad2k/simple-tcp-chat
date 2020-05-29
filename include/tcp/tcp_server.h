#pragma once

#include <iostream>
#include <string>
#include <boost.h>
#include <tcp/tcp_connection.h>

class tcp_server
{
public:
  tcp_server(boost::asio::io_service &io_service) : acceptor_(io_service, tcp::endpoint(tcp::v4(), 13)) {}

private:
  void start_accept();

  void handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code &error);

  tcp::acceptor acceptor_;
};
