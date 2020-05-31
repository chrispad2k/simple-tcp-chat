#include "chat/ChatServer.h"

void ChatRoom::join(ChatParticipant_ptr participant) {
  participants_.insert(participant);
  for (auto msg : recent_msgs_) participant->deliver(msg);
}

void ChatRoom::leave(ChatParticipant_ptr participant) {
  participants_.erase(participant);
}

void ChatRoom::deliver(const ChatMessage &msg) {
  recent_msgs_.push_back(msg);
  while (recent_msgs_.size() > max_recent_msgs) recent_msgs_.pop_front();

  for (auto participant : participants_) participant->deliver(msg);
}

//----------------------------------------------------------------------

ChatSession::ChatSession(tcp::socket socket, ChatRoom &room)
    : socket_(std::move(socket)), room_(room) {}

void ChatSession::start() {
  room_.join(shared_from_this());
  do_read_header();
}

void ChatSession::deliver(const ChatMessage &msg) {
  bool write_in_progress = !write_msgs_.empty();
  write_msgs_.push_back(msg);
  if (!write_in_progress) {
    do_write();
  }
}

void ChatSession::do_read_header() {
  auto self(shared_from_this());
  boost::asio::async_read(
      socket_,
      boost::asio::buffer(read_msg_.data(), ChatMessage::header_length),
      [this, self](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec && read_msg_.decode_header()) {
          do_read_body();
        } else {
          room_.leave(shared_from_this());
        }
      });
}

void ChatSession::do_read_body() {
  auto self(shared_from_this());
  boost::asio::async_read(
      socket_, boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
      [this, self](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec) {
          room_.deliver(read_msg_);
          do_read_header();
        } else {
          room_.leave(shared_from_this());
        }
      });
}

void ChatSession::do_write() {
  auto self(shared_from_this());
  boost::asio::async_write(
      socket_,
      boost::asio::buffer(write_msgs_.front().data(),
                          write_msgs_.front().length()),
      [this, self](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec) {
          write_msgs_.pop_front();
          if (!write_msgs_.empty()) {
            do_write();
          }
        } else {
          room_.leave(shared_from_this());
        }
      });
}

//----------------------------------------------------------------------

ChatServer::ChatServer(boost::asio::io_context &io_context,
                       const tcp::endpoint &endpoint)
    : acceptor_(io_context, endpoint) {
  do_accept();
}

void ChatServer::do_accept() {
  acceptor_.async_accept(
      [this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
          std::make_shared<ChatSession>(std::move(socket), room_)->start();
        }

        do_accept();
      });
}
