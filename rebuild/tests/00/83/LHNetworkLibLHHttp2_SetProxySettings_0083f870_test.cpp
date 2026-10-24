// Behaviour test for LHHttp2::SetProxySettings (0x0083F870): the proxy host and port are stored (the port widened to
// unsigned long); self-assignment of the host keeps it; the server host / port are untouched.
#include <cstdio>
#include <string>
struct LHHttp2 {
    std::string Hostname;
    unsigned short Port;
    std::string ProxyHost;
    unsigned long ProxyPort;
    void SetProxySettings(const std::string& host, unsigned short port)
    {
        if (&host != &ProxyHost)
            ProxyHost = host;
        ProxyPort = port;
    }
};
int main()
{
    LHHttp2 h;
    h.Hostname = "server";
    h.Port = 80;
    h.ProxyPort = 0xFFFFFFFFu;
    h.SetProxySettings(std::string("proxy.local"), 3128);
    int ok = h.ProxyHost == "proxy.local" && h.ProxyPort == 3128u && h.Hostname == "server" && h.Port == 80;
    h.SetProxySettings(h.ProxyHost, 65535);
    ok = ok && h.ProxyHost == "proxy.local" && h.ProxyPort == 65535u;
    if (!ok) {
        std::printf("0083f870_TEST FAIL\n");
        return 1;
    }
    std::printf("0083f870_TEST PASS\n");
    return 0;
}