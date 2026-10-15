#include <cstdio>
#include <string.h>
struct sockaddr_in { short sin_family; unsigned short sin_port; unsigned long sin_addr; char sin_zero[8]; };
struct LAddress { unsigned char ServerName[0x0c]; unsigned long ServerIP; unsigned short ServerPort; long TransportProtocol; long TransportFlags; };
static int g_calls = 0;
static bool ConvertSockaddr2LAddress(sockaddr_in* record, LAddress* address)
{
    ++g_calls;
    address->ServerIP = record->sin_addr;
    address->ServerPort = (unsigned short)((record->sin_port >> 8) | (record->sin_port << 8));
    return true;
}
struct LSocket {
    LAddress RemoteAddress, LocalAddress; unsigned int Socket; sockaddr_in SocketInformation, SocketInterfaceInformation;
    bool Attach(unsigned int socket, const sockaddr_in& remote, const sockaddr_in& local)
    {
        if (socket == (unsigned int)-1)
            return false;
        Socket = socket;
        SocketInformation = remote;
        SocketInterfaceInformation = local;
        ConvertSockaddr2LAddress(&SocketInformation, &RemoteAddress);
        ConvertSockaddr2LAddress(&SocketInterfaceInformation, &LocalAddress);
        return true;
    }
};
int main()
{
    LSocket s; memset(&s, 0, sizeof(s));
    sockaddr_in r, l; memset(&r, 0, sizeof(r)); memset(&l, 0, sizeof(l));
    r.sin_addr = 0x0100007f; r.sin_port = 0x5000; l.sin_addr = 0x0200000a; l.sin_port = 0x1000;
    if (s.Attach((unsigned int)-1, r, l) || g_calls != 0 || s.Socket != 0) { std::printf("008f8c00_TEST FAIL\n"); return 1; }
    if (!s.Attach(42, r, l) || g_calls != 2 || s.Socket != 42 || s.RemoteAddress.ServerIP != 0x0100007f ||
        s.RemoteAddress.ServerPort != 0x50 || s.LocalAddress.ServerIP != 0x0200000a || s.LocalAddress.ServerPort != 0x10 ||
        s.SocketInterfaceInformation.sin_port != 0x1000)
    { std::printf("008f8c00_TEST FAIL\n"); return 1; }
    std::printf("008f8c00_TEST PASS\n");
    return 0;
}
