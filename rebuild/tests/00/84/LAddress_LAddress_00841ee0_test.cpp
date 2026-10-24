// Behaviour test for LAddress::LAddress(const string&, unsigned short) (0x00841EE0): the default state (empty name, IP /
// protocol / flags zero) then the given name and port; constructing from the name of another address copies it.
#include <cstdio>
#include <string>
struct LAddress {
    std::string ServerName;
    unsigned long ServerIP;
    unsigned short ServerPort;
    unsigned long TransportProtocol;
    unsigned long TransportFlags;
    LAddress(const std::string& serverName, unsigned short serverPort)
    {
        ServerName = "";
        ServerPort = 0;
        ServerIP = 0;
        TransportProtocol = 0;
        TransportFlags = 0;
        if (&serverName != &ServerName)
            ServerName = serverName;
        ServerPort = serverPort;
    }
};
int main()
{
    LAddress a(std::string("fable.example.net"), 8080);
    LAddress b(a.ServerName, 25);
    if (a.ServerName != "fable.example.net" || a.ServerPort != 8080 || a.ServerIP != 0 || a.TransportProtocol != 0
        || a.TransportFlags != 0 || b.ServerName != "fable.example.net" || b.ServerPort != 25 || b.ServerIP != 0) {
        std::printf("00841ee0_TEST FAIL\n");
        return 1;
    }
    std::printf("00841ee0_TEST PASS\n");
    return 0;
}