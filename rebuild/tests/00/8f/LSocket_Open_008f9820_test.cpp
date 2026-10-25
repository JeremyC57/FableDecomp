// Behaviour test for LSocket::Open (0x008F9820): remote resolve failure, unknown protocol, bind / connect failure,
// non-blocking failure and the success path (both addresses and the local socket information recorded).
#include <cstdio>
#include <cstring>

struct Address { int name; unsigned long ip; unsigned short port; long protocol; };
struct SockAddr { short family; unsigned short port; unsigned long addr; char zero[8]; };
static int g_bindResult, g_connectResult, g_lastType; static bool g_nonBlockingOk;
static unsigned short htons_(unsigned short v) { return (unsigned short)((v >> 8) | (v << 8)); }
static unsigned int socket_(int af, int type, int) { g_lastType = type; return af == 2 ? 100u + type : ~0u; }
static int bind_(unsigned int, const SockAddr*, int) { return g_bindResult; }
static int connect_(unsigned int, const SockAddr*, int) { return g_connectResult; }
static int g_errors;
struct Socket {
    Address Remote, Local; unsigned int Handle; SockAddr Info, InterfaceInfo; long ConnectionMode;
    static bool ResolveHost(Address& a, unsigned long& ip) { ip = a.ip; return a.ip != 0xffffffff; }
    bool SetNonBlocking(bool) { return g_nonBlockingOk; }
    void Report() { ++g_errors; }
    bool Open(Address& remote, Address& local, long protocol)
    {
        Info.port = htons_(remote.port); Info.family = 2;
        ResolveHost(remote, Info.addr);
        if (Info.addr == 0xffffffff) return false;
        ConnectionMode = protocol;
        switch (protocol) {
        case 1: case 3: Handle = socket_(2, 1, 0); break;
        case 2: Handle = socket_(2, 2, 0); break;
        default: return false;
        }
        SockAddr li; li.port = htons_(local.port); li.family = 2;
        ResolveHost(local, li.addr);
        if (Info.addr == 0xffffffff) return false;
        if (bind_(Handle, &li, sizeof(li)) != 0) { Report(); return false; }
        if (connect_(Handle, &Info, sizeof(Info)) != 0) { Report(); return false; }
        if (!SetNonBlocking(true)) return false;
        Remote = remote; Local = local; InterfaceInfo = li;
        return true;
    }
};

int main()
{
    Address remote = { 1, 0x0100007f, 80, 1 }, local = { 2, 0, 0, 1 }, bad = { 3, 0xffffffff, 1, 1 };
    Socket s; std::memset(&s, 0, sizeof(s));
    bool ok = !s.Open(bad, local, 1);
    ok = ok && !s.Open(remote, local, 7) && s.ConnectionMode == 7;
    g_bindResult = -1; ok = ok && !s.Open(remote, local, 1) && g_errors == 1;
    g_bindResult = 0; g_connectResult = -1; ok = ok && !s.Open(remote, local, 2) && g_errors == 2 && g_lastType == 2;
    g_connectResult = 0; g_nonBlockingOk = false; ok = ok && !s.Open(remote, local, 3) && g_lastType == 1;
    g_nonBlockingOk = true;
    ok = ok && s.Open(remote, local, 1) && s.Remote.name == 1 && s.Local.name == 2 && s.Info.port == htons_(80)
        && s.Info.addr == 0x0100007f && s.InterfaceInfo.family == 2 && s.Handle == 101 && g_errors == 2;
    if (!ok) { std::printf("008f9820_TEST FAIL\n"); return 1; }
    std::printf("008f9820_TEST PASS\n");
    return 0;
}