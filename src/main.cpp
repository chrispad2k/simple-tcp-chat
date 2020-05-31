#include <cxxopts.hpp>
#include <iostream>
#include <string>

#include "chat/ChatClient.h"
#include "chat/ChatServer.h"

#define PORT 3000

int server(int port) {
  try {
    boost::asio::io_context io_context;

    std::list<ChatServer> servers;
    tcp::endpoint endpoint(tcp::v4(), port);
    servers.emplace_back(io_context, endpoint);

    io_context.run();
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

int client(std::string host, int port) {
  try {
    boost::asio::io_context io_context;
    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(host.c_str(), std::to_string(port));
    ChatClient c(io_context, endpoints);

    std::thread t([&io_context]() { io_context.run(); });

    char line[ChatMessage::max_body_length + 1];
    while (std::cin.getline(line, ChatMessage::max_body_length + 1)) {
      ChatMessage msg;
      msg.body_length(std::strlen(line));
      std::memcpy(msg.body(), line, msg.body_length());
      msg.encode_header();
      c.write(msg);
    }

    c.close();
    t.join();
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

int main(int argc, char *argv[]) {
  // Parsing command line arguments
  cxxopts::Options options("Simple TCP-Application",
                           "This is a simple TCP Server-Client Implementation");
  options.add_options()("t,type", "Server or Client?",
                        cxxopts::value<std::string>())(
      "h,host", "Host to connect to or to bind the server",
      cxxopts::value<std::string>())("p,port",
                                     "Port to connect to or to bind the server",
                                     cxxopts::value<int>());
  auto result = options.parse(argc, argv);
  const std::string type = result["type"].as<std::string>();

  std::cout << "Simple TCP-Application" << std::endl;
  std::cout << "You choose type: " << result["type"].as<std::string>()
            << std::endl;

  if (type == "server") {
    server(PORT);
  } else {
    client("127.0.0.1", PORT);
  }

  return 0;
}
