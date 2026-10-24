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
    LAddress& operator=(const LAddress& other);
};
LAddress& LAddress::operator=(const LAddress& other)
{
    ServerName = other.ServerName;
    ServerIP = other.ServerIP;
    ServerPort = other.ServerPort;
    TransportProtocol = other.TransportProtocol;
    TransportFlags = other.TransportFlags;
    return *this;
}
int main()
{
    int ok = 1;
    LAddress a, b;
    strcpy(a.ServerName.text, "host"); a.ServerIP = 0x7F000001; a.ServerPort = 80; a.TransportProtocol = 1; a.TransportFlags = 2;
    LAddress& r = (b = a);
    if (&r != &b) ok = 0;
    if (strcmp(b.ServerName.text, "host") != 0 || b.ServerIP != 0x7F000001 || b.ServerPort != 80 || b.TransportProtocol != 1 || b.TransportFlags != 2) ok = 0;
    b = b;
    if (strcmp(b.ServerName.text, "host") != 0) ok = 0;
    printf(ok ? "00841f60_TEST PASS\n" : "00841f60_TEST FAIL\n");
    return ok ? 0 : 1;
}