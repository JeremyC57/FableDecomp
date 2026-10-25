// Behaviour test for LSocket::Listen (0x008F8CC0): setsockopt / bind / listen failures each report and fail; the port
// is only taken when the address resolved; success records the local address and marks the socket listening.
#include <cstdio>
#include <cstring>

struct Address { int name; unsigned long ip; unsigned short port; long protocol; };
struct SockAddr { short family; unsigned short port; unsigned long addr; char zero[8]; };
static int g_optResult, g_bindResult, g_listenResult, g_errors, g_backlog; static char g_optValue;
static unsigned short htons_(unsigned short v) { return (unsigned short)((v >> 8) | (v << 8)); }
static unsigned int socket_(int, int, int) { return 42; }
static int setsockopt_(unsigned int, int level, int name, const char* v, int n) { g_optValue = (level == 0xffff && name == 4 && n == 1) ? *v : 0; return g_optResult; }
static int bind_(unsigned int, const SockAddr*, int) { return g_bindResult; }
static int listen_(unsigned int, int backlog) { g_backlog = backlog; return g_listenResult; }
struct Socket {
    Address Local; unsigned int Handle; SockAddr InterfaceInfo; bool Listening; long ConnectionMode;
    static bool ResolveHost(Address& a, unsigned long& ip) { ip = a.ip; return true; }
    bool Listen(Address& address)
    {
        bool reuse = true;
        Handle = socket_(2, 1, 0);
        if (setsockopt_(Handle, 0xffff, 4, (const char*)&reuse, sizeof(reuse)) != 0) { ++g_errors; return false; }
        SockAddr info; info.port = 0; info.family = 2;
        ResolveHost(address, info.addr);
        if (info.addr != 0xffffffff) info.port = htons_(address.port);
        InterfaceInfo = info; Local = address;
        if (bind_(Handle, &info, sizeof(info)) != 0) { ++g_errors; return false; }
        if (listen_(Handle, 0x7fffffff) != 0) { ++g_errors; return false; }
        ConnectionMode = 1; Listening = true;
        return true;
    }
};

int main()
{
    Address a = { 5, 0x0a000001, 8080, 1 }, unresolved = { 6, 0xffffffff, 8080, 1 };
    Socket s; std::memset(&s, 0, sizeof(s));
    g_optResult = -1; bool ok = !s.Listen(a) && g_errors == 1 && g_optValue == 1;
    g_optResult = 0; g_bindResult = -1; ok = ok && !s.Listen(unresolved) && s.InterfaceInfo.port == 0 && s.Local.name == 6 && g_errors == 2;
    g_bindResult = 0; g_listenResult = -1; ok = ok && !s.Listen(a) && g_errors == 3 && !s.Listening;
    g_listenResult = 0;
    ok = ok && s.Listen(a) && s.Listening && s.ConnectionMode == 1 && s.InterfaceInfo.port == htons_(8080)
        && s.InterfaceInfo.addr == 0x0a000001 && s.Local.name == 5 && g_backlog == 0x7fffffff && s.Handle == 42;
    if (!ok) { std::printf("008f8cc0_TEST FAIL\n"); return 1; }
    std::printf("008f8cc0_TEST PASS\n");
    return 0;
}