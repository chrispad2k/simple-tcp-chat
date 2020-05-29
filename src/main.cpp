//
//  main.cpp
//  test
//
//  Created by Christoph Pader on 29.05.20.
//  Copyright © 2020 Christoph Pader. All rights reserved.
//

#include <iostream>
#include <string>
#include <cxxopts.hpp>
#include <boost.h>
#include <tcp/tcp_server.h>
#include <tcp/tcp_connection.h>

int server()
{
    try
    {
        boost::asio::io_service io_service;
        tcp_server server(io_service);
        io_service.run();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

int client(std::string host)
{
    try
    {
        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints =
            resolver.resolve(host, "daytime");

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        for (;;)
        {
            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            std::cout.write(buf.data(), len);
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    cxxopts::Options options("Simple TCP-Application", "This is a simple TCP Server-Client Implementation");
    options.add_options()("t,type", "Server or Client?", cxxopts::value<std::string>());
    auto result = options.parse(argc, argv);
    const std::string type = result["type"].as<std::string>();

    std::cout << "Simple TCP-Application" << std::endl;
    std::cout << "You choose type: " << result["type"].as<std::string>() << std::endl;

    if (type == "server")
    {
        std::cout << "SERVER!";
        server();
    }
    else
    {
        std::cout << "CLIENT!";
        client("127.0.0.1");
    }

    return 0;
}
