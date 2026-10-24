// Behaviour test for 0x008F8830 LSocket::ConvertLAddress2Sockaddr.
#include <cstdio>

struct in_addr { unsigned int S_addr; };
struct sockaddr_in { short sin_family; unsigned short sin_port; in_addr sin_addr; char sin_zero[8]; };

static unsigned short FakeHtons(unsigned short v) { return (unsigned short)((v >> 8) | (v << 8)); }

struct LAddress { const char* ServerName; unsigned int ServerIP; unsigned short ServerPort; };

static bool ResolveHost(LAddress& address, unsigned int& ip)
{
    if (address.ServerIP == 0) {
        address.ServerIP = address.ServerName[0] == 'o' ? 0x0100007FU : 0xFFFFFFFFU;
        if (ip == 0xFFFFFFFFU) return false;
    }
    ip = address.ServerIP;
    return true;
}

static bool ConvertLAddress2Sockaddr(const LAddress& address, sockaddr_in& sa)
{
    sa.sin_port = 0;
    sa.sin_family = 2;
    ResolveHost(const_cast<LAddress&>(address), sa.sin_addr.S_addr);
    if (sa.sin_addr.S_addr == 0xFFFFFFFFU)
        return false;
    sa.sin_port = FakeHtons(address.ServerPort);
    return true;
}

int main()
{
    int fail = 0;
    LAddress good = { "ok", 0, 0x1234 };
    sockaddr_in sa; sa.sin_family = 0; sa.sin_port = 0x7777; sa.sin_addr.S_addr = 0;
    if (!ConvertLAddress2Sockaddr(good, sa) || sa.sin_family != 2 || sa.sin_port != 0x3412 || sa.sin_addr.S_addr != 0x0100007FU) fail = 1;
    LAddress bad = { "bad", 0, 0x1234 };
    sockaddr_in sb; sb.sin_family = 0; sb.sin_port = 0x7777; sb.sin_addr.S_addr = 0;
    if (ConvertLAddress2Sockaddr(bad, sb) || sb.sin_family != 2 || sb.sin_port != 0 || sb.sin_addr.S_addr != 0xFFFFFFFFU) fail = 1;
    if (fail) { std::printf("008f8830_TEST FAIL\n"); return 1; }
    std::printf("008f8830_TEST PASS\n");
    return 0;
}