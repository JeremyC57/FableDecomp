// Behaviour test for LHHttp2::Open (0x0083F9B0): an empty host name -> 1 and no socket; otherwise a socket is created
// and opened to the proxy (name, port) when a proxy host is set, else to the host (name, port), always from ANY:0;
// the socket's result maps to 0 (opened) or 3 (failed).
#include <cstdio>
#include <string>

struct Address {
    std::string ServerName;
    unsigned short ServerPort;
    Address() : ServerName(""), ServerPort(0) { }
    Address(const std::string& name, unsigned short port) : ServerName(name), ServerPort(port) { }
};

static bool g_openResult = true;
struct Socket {
    Address remote, local;
    bool Open(const Address& r, const Address& l, bool) { remote = r; local = l; return g_openResult; }
};

struct Http {
    Socket* Host;
    std::string Hostname;
    unsigned short Port;
    std::string ProxyHost;
    unsigned long ProxyPort;
    int Open()
    {
        if (Hostname == "")
            return 1;
        Host = new Socket;
        Address remote;
        if (ProxyHost != "")
            remote = Address(ProxyHost, (unsigned short)ProxyPort);
        else
            remote = Address(Hostname, Port);
        Address local(std::string("ANY"), 0);
        if (Host->Open(remote, local, true))
            return 0;
        return 3;
    }
};

int main()
{
    int fails = 0;
    Http h;
    h.Host = 0; h.Port = 80; h.ProxyPort = 0x10000 + 8080;
    if (h.Open() != 1 || h.Host != 0) ++fails;
    h.Hostname = "scores.example";
    if (h.Open() != 0 || !h.Host || h.Host->remote.ServerName != "scores.example" || h.Host->remote.ServerPort != 80 ||
        h.Host->local.ServerName != "ANY" || h.Host->local.ServerPort != 0) ++fails;
    delete h.Host;
    h.ProxyHost = "proxy";
    g_openResult = false;
    if (h.Open() != 3 || h.Host->remote.ServerName != "proxy" || h.Host->remote.ServerPort != 8080) ++fails;
    delete h.Host;
    if (fails) { std::printf("0083f9b0_TEST FAIL\n"); return 1; }
    std::printf("0083f9b0_TEST PASS\n");
    return 0;
}