// Behaviour test for 0x008F8BB0 LSocket::ConvertSockaddr2LAddress.
#include <cstdio>
#include <cstring>

struct in_addr { unsigned int S_addr; };
struct sockaddr_in { short sin_family; unsigned short sin_port; in_addr sin_addr; char sin_zero[8]; };

static char g_dotted[16];
static char* FakeInetNtoa(in_addr in)
{
    unsigned int a = in.S_addr;
    std::sprintf(g_dotted, "%u.%u.%u.%u", a & 0xFF, (a >> 8) & 0xFF, (a >> 16) & 0xFF, a >> 24);
    return g_dotted;
}
static unsigned short FakeNtohs(unsigned short v) { return (unsigned short)((v >> 8) | (v << 8)); }

struct SString {
    char m_text[32];
    SString& assign(const char* f, const char* l) { unsigned int n = (unsigned int)(l - f); std::memcpy(m_text, f, n); m_text[n] = 0; return *this; }
    SString& operator=(const char* s) { return assign(s, s + std::strlen(s)); }
};

struct LAddress { SString ServerName; unsigned int ServerIP; unsigned short ServerPort; };

static bool ConvertSockaddr2LAddress(const sockaddr_in& sa, LAddress& address)
{
    address.ServerName = FakeInetNtoa(sa.sin_addr);
    address.ServerIP = sa.sin_addr.S_addr;
    address.ServerPort = FakeNtohs(sa.sin_port);
    return true;
}

int main()
{
    int fail = 0;
    sockaddr_in sa; sa.sin_family = 2; sa.sin_port = 0x5000; sa.sin_addr.S_addr = 0x0401A8C0U;   // 192.168.1.4:80
    LAddress addr; addr.ServerIP = 0; addr.ServerPort = 0;
    if (!ConvertSockaddr2LAddress(sa, addr)) fail = 1;
    if (std::strcmp(addr.ServerName.m_text, "192.168.1.4") != 0 || addr.ServerIP != 0x0401A8C0U || addr.ServerPort != 80) fail = 1;
    if (fail) { std::printf("008f8bb0_TEST FAIL\n"); return 1; }
    std::printf("008f8bb0_TEST PASS\n");
    return 0;
}