// Behaviour test for 0x008F86F0 LSocket::ResolveHost(LAddress&, unsigned long&).
#include <cstdio>
#include <cstring>

struct SString {
    const char* m_text;
    explicit SString(const char* t) : m_text(t) { }
    unsigned int size() const { return (unsigned int)std::strlen(m_text); }
};
static bool operator==(const SString& a, const SString& b)
{
    return a.size() == b.size() && std::memcmp(a.m_text, b.m_text, a.size()) == 0;
}

struct LAddress {
    SString        ServerName;
    unsigned int   ServerIP;
    unsigned short ServerPort;
    explicit LAddress(const char* name) : ServerName(name), ServerIP(0), ServerPort(0) { }
};

static int g_lookups = 0;
static unsigned int ResolveName(const SString& host)
{
    ++g_lookups;
    return std::strcmp(host.m_text, "server") == 0 ? 0x0A000001U : 0xFFFFFFFFU;
}

static bool ResolveHost(LAddress& address, unsigned int& ip)
{
    unsigned int serverIP = address.ServerIP;
    if (serverIP == 0) {
        if (address.ServerName == SString("ANY")) {
            ip = 0;
            return true;
        }
        serverIP = ResolveName(address.ServerName);
        address.ServerIP = serverIP;
        if (ip == 0xFFFFFFFFU)
            return false;
    }
    ip = serverIP;
    return true;
}

int main()
{
    int fail = 0;
    unsigned int ip = 7;
    LAddress known("ignored"); known.ServerIP = 0x01020304U;
    if (!ResolveHost(known, ip) || ip != 0x01020304U || g_lookups != 0) fail = 1;
    LAddress any("ANY"); ip = 7;
    if (!ResolveHost(any, ip) || ip != 0 || g_lookups != 0 || any.ServerIP != 0) fail = 1;
    LAddress named("server"); ip = 7;
    if (!ResolveHost(named, ip) || ip != 0x0A000001U || named.ServerIP != 0x0A000001U || g_lookups != 1) fail = 1;
    LAddress bad("nowhere"); ip = 0xFFFFFFFFU;             // the caller's INADDR_NONE makes it fail
    if (ResolveHost(bad, ip) || bad.ServerIP != 0xFFFFFFFFU || g_lookups != 2) fail = 1;
    if (fail) { std::printf("008f86f0_TEST FAIL\n"); return 1; }
    std::printf("008f86f0_TEST PASS\n");
    return 0;
}