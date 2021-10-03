#pragma once
#include <functional>
#include <string>
#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#include <windows.h>
#else
#error Unsupported platform.
#endif

namespace Network
{
    void networkStartup();
    void networkCleanup();
    class IPAddress
    {
    public:
        unsigned int IP;
        unsigned short port;
        std::string toString() const;
        void setIP(const char *);
    };
    class Socket
    {
    protected:
        SOCKET descriptor;
        IPAddress address;

    public:
        const IPAddress getAddress() const;
        Socket();
        ~Socket();
        void send(Socket, const char *);
        void recv(Socket, char *);
    };
    template <int port, int bklg = 25>
    class Server : public Socket
    {
    public:
        Server();
        void start();
        void close();
        Socket accept();
        ~Server();
    };
}
