// Web++_Framework.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>
#include "./Includes/asio.hpp"
#include <utility>
namespace WebPlusPlus
{
    std::string GetLocalIp(asio::io_context& io_context)
    {
        asio::ip::tcp::resolver resolver(io_context);
        asio::ip::tcp::resolver::query query(asio::ip::host_name(), "");
        asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
        asio::ip::tcp::resolver::iterator end;

        while (iter != end) {
            asio::ip::tcp::endpoint ep = *iter++;
            asio::ip::address addr = ep.address();
            if (addr.is_v4() && !addr.is_loopback()) { // We only want non-loopback IPv4 addresses
                return addr.to_string();
            }
        }

        return "127.0.0.1"; // Default to localhost if no suitable address is found
    }
    class ServerResponseContent {};

    namespace ServerSideSettings
    {

    };
    class ServerSideOptions{};
    class ClientRequestContent
    {
    public:
        enum class ClientRequestType 
        {
            ANY = 0,

        };
    };
    using ServerResponseFunc = std::function<ServerResponseContent(ClientRequestContent&)>;
    
    class ServerRequestHandler 
    {
    public:
        ServerRequestHandler() {}
        virtual void HandleRequest(asio::io_context& io_context, asio::ip::tcp::acceptor& acceptor, asio::ip::tcp::endpoint& endpoint)
        {
            asio::ip::tcp::socket sock(io_context);
            acceptor.accept(sock);
            asio::error_code ec;

            while (!ec)
            {
                uint8_t data[512];

                size_t len = sock.read_some(asio::buffer(data), ec);

                if (len > 0)
                {
                    std::cout << "received " << len << " bytes\n";

                }
                char response[] = "HTTP/1.1 200 OK\r\Content-Type: text/html; charset=utf-8\r\nContent-Length: 12\r\n\r\nHello World";

                asio::write(sock, asio::buffer(response, strlen(response)));
                std::cout << "Done writing!\n";
            }

            std::cout << "Closed: " << ec.message() << "\n";
        }
    private:
    };
    asio::ip::tcp::endpoint GetLocalEndpoint(asio::io_context& io_context, int port)
    {
        static auto ip = GetLocalIp(io_context);
        asio::ip::tcp::endpoint endpoint(asio::ip::make_address(ip), port);
        return endpoint;

    }
    asio::ip::tcp::acceptor InitLocalHost(asio::io_context& io_context, asio::ip::tcp::endpoint endpoint) {

        asio::ip::tcp::acceptor acceptor(io_context, endpoint);
        
        acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
        
        acceptor.listen();

        return acceptor;
    }
    void HandleIncomingConnection(asio::io_context& io_context, asio::ip::tcp::acceptor& acceptor, asio::ip::tcp::endpoint& endpoint) {
        acceptor.async_accept([&io_context, &acceptor,&endpoint](const asio::error_code& error, asio::ip::tcp::socket socket) {
            if (!error) {

                std::make_shared<ServerRequestHandler>()->HandleRequest(io_context,acceptor, endpoint);
            }

            HandleIncomingConnection(io_context, acceptor,endpoint); // Continue accepting connections
            });
    }
    //void HandleIncomingConnection(asio::io_context& io_context, asio::ip::tcp::acceptor& acceptor, asio::ip::tcp::endpoint& endpoint) 
    /*{
        for (;;)
        {
            asio::ip::tcp::socket sock(io_context);
            acceptor.accept(sock);
            asio::error_code ec;

            while (!ec)
            {
                uint8_t data[512];

                size_t len = sock.read_some(asio::buffer(data), ec);

                if (len > 0)
                {
                    std::cout << "received " << len << " bytes\n";

                }
                char response[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello World";

                asio::write(sock, asio::buffer(response, sizeof(response)));
            }

            std::cout << "Closed: " << ec.message() << "\n";


        }
    }
    */
    void InitServer(asio::io_context& io_context, std::vector<ServerSideOptions> options)
    {
        int port = 9874;
        auto endpoint = GetLocalEndpoint(io_context, port);
        auto acceptor = InitLocalHost(io_context,endpoint);
        std::cout << "Listen to http://" << endpoint.address()<<":" << port << std::endl;
            
        HandleIncomingConnection(io_context, acceptor, endpoint);
        
        io_context.run(); // Start the ASIO event loop


    }
    void RespondToClient(asio::io_context& io_context, const ClientRequestContent& client_request)
    {

    }

    

}


