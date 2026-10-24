// Behaviour test for 0x00840070 LHNetworkLib::LHHttp2::Close
#include <stdio.h>
static int g_closed = 0, g_deleted = 0, g_reset = 0;
struct LSocket {
    bool Close() { ++g_closed; return true; }
    ~LSocket() { ++g_deleted; }
};
struct LHHttp2 {
    LSocket* Host;
    void Reset() { ++g_reset; }
    long Close();
};
long LHHttp2::Close()
{
    if (Host) {
        Host->Close();
        delete Host;
        Host = 0;
    }
    Reset();
    return 0;
}
int main()
{
    int ok = 1;
    LHHttp2 h;
    h.Host = new LSocket;
    if (h.Close() != 0) ok = 0;
    if (h.Host != 0 || g_closed != 1 || g_deleted != 1 || g_reset != 1) ok = 0;
    if (h.Close() != 0) ok = 0;
    if (g_closed != 1 || g_deleted != 1 || g_reset != 2) ok = 0;
    printf(ok ? "00840070_TEST PASS\n" : "00840070_TEST FAIL\n");
    return ok ? 0 : 1;
}