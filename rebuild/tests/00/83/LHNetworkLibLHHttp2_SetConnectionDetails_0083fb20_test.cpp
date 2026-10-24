// Behaviour test for LHHttp2::SetConnectionDetails (0x0083FB20): the host name and port are stored, then the result of
// Open() is returned (modelled here as a counter that reports the stored details).
#include <cstdio>
#include <string>
struct LHHttp2 {
    std::string Hostname;
    unsigned short Port;
    int OpenCalls;
    int Open() { ++OpenCalls; return Hostname.empty() ? 1 : 0; }
    int SetConnectionDetails(const std::string& host, unsigned short port)
    {
        if (&host != &Hostname)
            Hostname = host;
        Port = port;
        return Open();
    }
};
int main()
{
    LHHttp2 h;
    h.OpenCalls = 0;
    h.Port = 0;
    int r1 = h.SetConnectionDetails(std::string("www.lionhead.example"), 80);
    int ok = r1 == 0 && h.Hostname == "www.lionhead.example" && h.Port == 80 && h.OpenCalls == 1;
    int r2 = h.SetConnectionDetails(std::string(""), 443);
    ok = ok && r2 == 1 && h.Hostname.empty() && h.Port == 443 && h.OpenCalls == 2;
    if (!ok) {
        std::printf("0083fb20_TEST FAIL\n");
        return 1;
    }
    std::printf("0083fb20_TEST PASS\n");
    return 0;
}