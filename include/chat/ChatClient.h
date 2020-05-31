#pragma once

#include <boost/asio.hpp>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>

#include "chat/ChatMessage.h"

using boost::asio::ip::tcp;

typedef std::deque<ChatMessage> ChatMessage_queue;

class ChatClient {
 public:
  ChatClient(boost::asio::io_context &io_context,
             const tcp::resolver::results_type &endpoints);

  void write(const ChatMessage &msg);

  void close();

 private:
  void do_connect(const tcp::resolver::results_type &endpoints);

  void do_read_header();

  void do_read_body();

  void do_write();

 private:
  boost::asio::io_context &io_context_;
  tcp::socket socket_;
  ChatMessage read_msg_;
  ChatMessage_queue write_msgs_;
};
