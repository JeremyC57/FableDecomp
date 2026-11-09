// Behaviour test for LHNetworkLib::LSocket::Receive(vector<char>&) 0x008F99A0: two default (cleared) LAddress locals -- empty
// name, zero IP/port/protocol/flags -- are handed to the three-argument Receive as the sender and the local address, and its
// result is returned; the locals are destroyed afterwards.
#include <cstdio>
#include <string>
#include <vector>

struct LAddress {
    std::string ServerName;
    unsigned long ServerIP;
    unsigned short ServerPort;
    long TransportProtocol;
    long TransportFlags;
    LAddress() : ServerIP(7), ServerPort(7), TransportProtocol(7), TransportFlags(7) { ServerName = "junk"; ClearData(); }
    void ClearData() { ServerName = ""; ServerPort = 0; ServerIP = 0; TransportProtocol = 0; TransportFlags = 0; }
};
static bool Cleared(const LAddress& a)
{
    return a.ServerName.empty() && a.ServerIP == 0 && a.ServerPort == 0 && a.TransportProtocol == 0 && a.TransportFlags == 0;
}
struct LSocket {
    bool Result;
    int Calls;
    bool SawCleared;
    LSocket() : Result(false), Calls(0), SawCleared(false) { }
    bool Receive(std::vector<char>& data, LAddress& from, const LAddress& local)
    {
        ++Calls;
        SawCleared = Cleared(from) && Cleared(local) && &from != &local;
        if (Result) data.push_back('x');
        return Result;
    }
    bool Receive(std::vector<char>& data)
    {
        LAddress from;
        LAddress to;
        return Receive(data, from, to);
    }
};

int main()
{
    bool ok = true;
    LSocket s;
    std::vector<char> data;
    if (s.Receive(data) != false || s.Calls != 1 || !s.SawCleared || !data.empty()) ok = false;
    s.Result = true;
    if (s.Receive(data) != true || s.Calls != 2 || !s.SawCleared || data.size() != 1) ok = false;
    if (!ok) { std::printf("008f99a0_TEST FAIL\n"); return 1; }
    std::printf("008f99a0_TEST PASS\n");
    return 0;
}