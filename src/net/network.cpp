#include "network.hpp"

namespace Network
{
    void networkStartup()
    {
        WSAData wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);
    }
    void networkCleanup()
    {
        WSACleanup();
    }
    const IPAddress Socket::getAddress() const
    {
        return address;
    }
    Socket::Socket()
    {
        descriptor = INVALID_SOCKET;
        address.IP = 0;
        address.port = 0;
    }
    void Socket::send(Socket sock, const char *msg)
    {
        ::send(sock.descriptor, msg, strlen(msg), 0);
    }
    void Socket::recv(Socket sock, char *s)
    {
        int len = ::recv(sock.descriptor, s, 65536, 0);
        s[len] = '\0';
    }
    Socket::~Socket()
    {
        if (descriptor == INVALID_SOCKET)
            return;
        closesocket(descriptor);
    }
    template <int port, int bklg>
    Server<port, bklg>::Server() : Server<port, bklg>::Socket()
    {
        address.IP = 0;
        address.port = port;
    }
    template <int port, int bklg>
    Server<port, bklg>::~Server()
    {
    }
    template <int port, int bklg>
    void Server<port, bklg>::start()
    {
        descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (descriptor == INVALID_SOCKET)
        {
            throw "Cannot create socket";
            return;
        }
        SOCKADDR_IN addr = {0};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(address.port);
        addr.sin_addr.s_addr = inet_addr(address.toString().c_str());
        if (bind(descriptor, (SOCKADDR *)&addr, sizeof(addr)))
        {
            throw "Cannot bind socket";
            return;
        }
        if (listen(descriptor, bklg))
        {
            throw "Cannot listen socket";
            return;
        }
    }
    template <int port, int bklg>
    void Server<port, bklg>::close()
    {
        if (descriptor == INVALID_SOCKET)
            return;
        closesocket(descriptor);
        descriptor = INVALID_SOCKET;
    }
    template <int port, int bklg>
    Socket Server<port, bklg>::accept()
    {
        Socket ret;
        ret.address.setIP("0.0.0.0");
        ret.address.port = 0;
        ret.descriptor = INVALID_SOCKET;
        SOCKADDR_IN add = {0};
        int len = sizeof add;
        SOCKET sock = ::accept(descriptor, (SOCKADDR *)&add, &len);
        if (sock == INVALID_SOCKET)
        {
            throw "Cannot accept socket";
            return ret;
        }
        ret.address.setIP(inet_ntoa(add.sin_addr));
        ret.address.port = ntohs(add.sin_port);
        ret.descriptor = sock;
        return ret;
    }
    std::string IPAddress::toString() const
    {
        int a, b, c, d;
        unsigned int iptmp = IP;
        d = iptmp & 0xff;
        iptmp >>= 8;
        c = iptmp & 0xff;
        iptmp >>= 8;
        b = iptmp & 0xff;
        iptmp >>= 8;
        a = iptmp & 0xff;
        return std::to_string(a) + "." + std::to_string(b) + "." + std::to_string(c) + "." + std::to_string(d);
    }
    void IPAddress::setIP(const char *s)
    {
        int t = 0;
        for (int i = 0; s[i] != '\0'; i++)
        {
            if (s[i] != '.')
                t = t * 10 + (s[i] & 15);
            else
            {
                IP = IP << 8 | t;
                t = 0;
            }
        }
        IP = IP << 8 | t;
        t = 0;
    }
}
