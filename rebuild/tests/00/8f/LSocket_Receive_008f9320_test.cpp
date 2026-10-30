// behaviour test for 0x008F9320 LSocket::Receive (TCP and UDP paths, error mapping, counters)
#include <cstdio>
#include <vector>
#include <cstring>
static int g_recvResult, g_lastError, g_readSize; static bool g_selectOk;
struct LSocketT {
    unsigned int Socket; int ConnectionMode; bool NoListeningBinding; long NrTotal, NrLast;
    bool SelectRead(int) { return g_selectOk; }
    int CheckReadSize() { return g_readSize; }
    bool Receive(std::vector<char>& data, bool namedUdp)
    {
        int received = 0;
        NrLast = 0;
        if (ConnectionMode == 3) return false;
        if (namedUdp) {
            if (SelectRead(50)) { received = g_recvResult; if (received == -1) return false; }
            data.resize(received, char());
        } else {
            if (Socket == (unsigned int)-1) return false;
            if (!SelectRead(50)) return false;
            unsigned long size = data.size();
            if (size == 0) size = CheckReadSize();
            if (data.size() == 0) data.resize(size);
            received = g_recvResult;
            if (received == -1) {
                switch (g_lastError) { case 10053: case 10054: received = 0; break; default: return false; }
            }
            data.resize(received, char());
            if (received == 0) return false;
        }
        NrTotal += received; NrLast = received;
        return true;
    }
};
int main()
{
    LSocketT s = { 5, 1, false, 0, 0 }; std::vector<char> d; bool ok = true;
    g_selectOk = true; g_readSize = 16; g_recvResult = 10;
    ok = ok && s.Receive(d, false) && d.size() == 10 && s.NrTotal == 10 && s.NrLast == 10;
    g_recvResult = -1; g_lastError = 10054; ok = ok && !s.Receive(d, false) && d.size() == 0 && s.NrLast == 0;
    g_lastError = 10035; d.resize(4); ok = ok && !s.Receive(d, false);
    g_recvResult = 7; ok = ok && s.Receive(d, true) && d.size() == 7 && s.NrTotal == 17;
    s.ConnectionMode = 3; ok = ok && !s.Receive(d, false);
    s.ConnectionMode = 1; s.Socket = (unsigned int)-1; ok = ok && !s.Receive(d, false);
    std::printf(ok ? "008f9320_TEST PASS\n" : "008f9320_TEST FAIL\n");
    return ok ? 0 : 1;
}