#include "InetAddress.h"
#include <cstring>
#include <string>

namespace Server{
            InetAddress::InetAddress(uint16_t port, const std::string &ip){
                memset(&addr_, 0 ,sizeof addr_);
                //ipv4, ad_inetv6 is ipv6
                addr_.sin_family = AF_INET;
                //host to net (regarding big and small endian)
                addr_.sin_port = htons(port);
                //ip to net p to n
                inet_pton(AF_INET, ip.c_str(), &addr_.sin_addr);
            }
            InetAddress::InetAddress(const sockaddr_in &addr):addr_(addr){}
            std::string InetAddress::toIp() const{
                char buf[64] = {0};
                //n to p
                inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof buf);
                return buf;
            }
            std::string InetAddress::toIpPort() const{
                return toIp() + ":" +std::to_string(toPort());
            }
            uint16_t InetAddress::toPort() const{
                return ntohs(addr_.sin_port);
            }
            sockaddr_in InetAddress::getSockAddr() const{return addr_;}
            void InetAddress::setSockAddr(const sockaddr_in &addr){addr_ = addr;}
}
