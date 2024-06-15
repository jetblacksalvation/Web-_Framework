//WEBPLUSPLUS_ROOT
#ifndef WEBPLUSPLUS_ROOT
#define WEBPLUSPLUS_ROOT

#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>
#include "../Includes/asio.hpp"
#include "./CommonFunctionality/CommonFunctions.hpp"

#include <utility>
namespace WebPlusPlus
{
	using TcpEndpoint = asio::ip::tcp::endpoint;
	using TcpResolver = asio::ip::tcp::resolver;
	class ServerResponseContent
	{


	};

	namespace ServerSideSettings
	{

	};
	class ServerSideOptions {};
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
		virtual void HandleRequest(asio::io_context& io_context, asio::ip::tcp::acceptor& acceptor, TcpEndpoint& TcpEndpoint)
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
				char response[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: 12\r\n\r\nHello World";

				asio::write(sock, asio::buffer(response, strlen(response)));
				std::cout << "Done writing!\n";
			}

			std::cout << "Closed: " << ec.message() << "\n";
		}
	private:
	};
	TcpEndpoint GetLocalEndpoint(asio::io_context& io_context, int port)
	{
		static auto ip = CommonFunctions::GetLocalIp(io_context);
		TcpEndpoint endpoint(asio::ip::make_address(ip), port);
		return endpoint;

	}
	asio::ip::tcp::acceptor InitLocalHost(asio::io_context& io_context, TcpEndpoint endpoint) {

		asio::ip::tcp::acceptor acceptor(io_context, endpoint);

		acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));

		acceptor.listen();

		return acceptor;
	}
	void HandleIncomingConnection(asio::io_context& io_context, asio::ip::tcp::acceptor& acceptor, TcpEndpoint& endpoint) {
		acceptor.async_accept([&io_context, &acceptor, &endpoint](const asio::error_code& error, asio::ip::tcp::socket socket) {
			if (!error) {

				std::make_shared<ServerRequestHandler>()->HandleRequest(io_context, acceptor, endpoint);
			}

			HandleIncomingConnection(io_context, acceptor, endpoint); // Continue accepting connections
			});
	}
	//void HandleIncomingConnection(asio::io_context& io_context, asio::ip::tcp::acceptor& acceptor, endpoint& endpoint) 
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
		auto acceptor = InitLocalHost(io_context, endpoint);
		std::cout << "Listen to http://" << endpoint.address() << ":" << port << std::endl;

		HandleIncomingConnection(io_context, acceptor, endpoint);

		io_context.run(); // Start the ASIO event loop


	}
	void RespondToClient(asio::io_context& io_context, const ClientRequestContent& client_request)
	{

	}



}




#endif // !WEBPLUSPLUS_ROOT
