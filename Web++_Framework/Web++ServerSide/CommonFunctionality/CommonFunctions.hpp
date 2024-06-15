//WEBPLUSPLUS_COMMONFUNCIONS
#ifndef WEBPLUSPLUS_COMMONFUNCIONS
#define WEBPLUSPLUS_COMMONFUNCIONS
#include "../../Includes/asio.hpp"


namespace WebPlusPlus
{
    namespace CommonFunctions
    {
        std::string GetLocalIp(asio::io_context& io_context)
        {
            TcpResolver resolver(io_context);
            TcpResolver::query query(asio::ip::host_name(), "");
            TcpResolver::iterator iter = resolver.resolve(query);
            TcpResolver::iterator end;

            while (iter != end) {
                TcpEndpoint ep = *iter++;
                asio::ip::address addr = ep.address();
                if (addr.is_v4() && !addr.is_loopback()) { // We only want non-loopback IPv4 addresses
                    return addr.to_string();
                }
            }

            return "127.0.0.1"; // Default to localhost if no suitable address is found
        }
    }


}

#endif // !1


