// Behaviour test for 0x008F87A0 LSocket::CreateSocket: protocol switch, resolve, bind; INVALID_SOCKET on failure.
#include <cstdio>

struct in_addr { unsigned int S_addr; };
struct sockaddr_in { short sin_family; unsigned short sin_port; in_addr sin_addr; char sin_zero[8]; };

static int g_lastType = 0;
static int g_bindResult = 0;
static sockaddr_in g_bound;
static unsigned int FakeSocket(int af, int type, int) { g_lastType = type; return af == 2 ? 40u + (unsigned int)type : 0xFFFFFFFFU; }
static unsigned short FakeHtons(unsigned short v) { return (unsigned short)((v >> 8) | (v << 8)); }
static int FakeBind(unsigned int, const sockaddr_in* name, int len) { if (len == 16) g_bound = *name; return g_bindResult; }

struct LAddress { unsigned int ServerIP; unsigned short ServerPort; };
static bool ResolveHost(LAddress& address, unsigned int& ip) { ip = address.ServerIP; return true; }

enum LSOCKET_PROTOCOL { LSOCKET_PROTOCOL_NONE = 0, LSOCKET_PROTOCOL_TCP = 1, LSOCKET_PROTOCOL_UDP = 2 };

static unsigned int CreateSocket(const LAddress& address, LSOCKET_PROTOCOL protocol)
{
    sockaddr_in sa;
    unsigned int s;
    switch (protocol) {
    case LSOCKET_PROTOCOL_TCP: s = FakeSocket(2, 1, 0); break;
    case LSOCKET_PROTOCOL_UDP: s = FakeSocket(2, 2, 0); break;
    default: return 0xFFFFFFFFU;
    }
    if (s == 0xFFFFFFFFU) return 0xFFFFFFFFU;
    ResolveHost(const_cast<LAddress&>(address), sa.sin_addr.S_addr);
    sa.sin_port = FakeHtons(address.ServerPort);
    sa.sin_family = 2;
    if (FakeBind(s, &sa, sizeof(sa)) != 0) return 0xFFFFFFFFU;
    return s;
}

int main()
{
    int fail = 0;
    LAddress addr = { 0x0100007FU, 0x1F90 };
    if (CreateSocket(addr, LSOCKET_PROTOCOL_TCP) != 41 || g_lastType != 1) fail = 1;
    if (g_bound.sin_family != 2 || g_bound.sin_port != 0x901F || g_bound.sin_addr.S_addr != 0x0100007FU) fail = 1;
    if (CreateSocket(addr, LSOCKET_PROTOCOL_UDP) != 42 || g_lastType != 2) fail = 1;
    if (CreateSocket(addr, LSOCKET_PROTOCOL_NONE) != 0xFFFFFFFFU) fail = 1;
    g_bindResult = -1;
    if (CreateSocket(addr, LSOCKET_PROTOCOL_TCP) != 0xFFFFFFFFU) fail = 1;
    if (fail) { std::printf("008f87a0_TEST FAIL\n"); return 1; }
    std::printf("008f87a0_TEST PASS\n");
    return 0;
}