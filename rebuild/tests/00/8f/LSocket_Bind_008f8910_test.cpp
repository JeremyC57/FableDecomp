// Behaviour test for 0x008F8910 LSocket::Bind
#include <stdio.h>
struct LAddress { unsigned long ServerIP; long TransportProtocol; };
class LSocket {
public:
    unsigned int Socket;
    long         ConnectionMode;
    static unsigned int CreateSocket(const LAddress& address, long protocol)
    {
        return address.ServerIP == 0 ? (unsigned int)-1 : (unsigned int)(100 + protocol);
    }
    bool Bind(LAddress& address);
};
bool LSocket::Bind(LAddress& address)
{
    Socket = CreateSocket(address, address.TransportProtocol);
    ConnectionMode = address.TransportProtocol;
    return Socket != (unsigned int)-1 ? true : false;
}
int main()
{
    int ok = 1;
    LSocket s;
    LAddress a;
    a.ServerIP = 1; a.TransportProtocol = 2;
    if (s.Bind(a) != true || s.Socket != 102 || s.ConnectionMode != 2) ok = 0;
    a.ServerIP = 0; a.TransportProtocol = 1;
    if (s.Bind(a) != false || s.Socket != (unsigned int)-1 || s.ConnectionMode != 1) ok = 0;
    printf(ok ? "008f8910_TEST PASS\n" : "008f8910_TEST FAIL\n");
    return ok ? 0 : 1;
}