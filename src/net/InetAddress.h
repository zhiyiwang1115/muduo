#pragma once

#include <arpa/inet.h> 
#include <string>

namespace Server::Net{
    class InetAddress{
        public:
            explicit InetAddress(uint16_t port, const std::string &ip = "127.0.0.1");
            explicit InetAddress(const sockaddr_in &addr);
            std::string toIp() const;
            std::string toIpPort() const;
            uint16_t toPort() const;
            sockaddr_in getSockAddr() const;
            void setSockAddr(const sockaddr_in &addr);
        private:
            sockaddr_in addr_;
    };
}