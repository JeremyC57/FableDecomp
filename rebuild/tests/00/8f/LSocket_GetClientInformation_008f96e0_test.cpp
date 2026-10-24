#include <stdio.h>
#include <string.h>
struct CRetailString {
    char text[32];
    CRetailString() { text[0] = 0; }
    CRetailString& operator=(const CRetailString& other)
    {
        if (&other != this)
            strcpy(text, other.text);
        return *this;
    }
};
struct LAddress {
    CRetailString  ServerName;
    unsigned long  ServerIP;
    unsigned short ServerPort;
    long           TransportProtocol;
    long           TransportFlags;
};
class LSocket {
public:
    LAddress     RemoteAddress;
    LAddress     LocalAddress;
    unsigned int Socket;
    bool GetClientInformation(LAddress& address);
};
bool LSocket::GetClientInformation(LAddress& address)
{
    if (Socket == (unsigned int)-1)
        return false;
    address = RemoteAddress;
    return true;
}
int main()
{
    int ok = 1;
    LSocket s;
    strcpy(s.RemoteAddress.ServerName.text, "peer"); s.RemoteAddress.ServerIP = 9; s.RemoteAddress.ServerPort = 7;
    s.RemoteAddress.TransportProtocol = 1; s.RemoteAddress.TransportFlags = 0;
    LAddress out;
    out.ServerIP = 0;
    s.Socket = (unsigned int)-1;
    if (s.GetClientInformation(out) != false || out.ServerIP != 0) ok = 0;
    s.Socket = 12;
    if (s.GetClientInformation(out) != true || out.ServerIP != 9 || out.ServerPort != 7 || strcmp(out.ServerName.text, "peer") != 0) ok = 0;
    printf(ok ? "008f96e0_TEST PASS\n" : "008f96e0_TEST FAIL\n");
    return ok ? 0 : 1;
}