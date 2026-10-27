// behaviour test for 0x00CAD8B0 CPosPassThru::get_CurrentPosition (model: forwarded to the peer, which is released; failures pass through)
#include <cstdio>
#define long int   // HRESULT is 32-bit (LP64 test hosts)
static int g_released = 0;
struct Peer { double v; long Get(double* p) { *p = v; return 0; } void Release() { ++g_released; } };
static Peer g_peer = { 2.5 };
static long GetPeer(Peer** pp, bool ok) { *pp = 0; if (!ok) return (long)0x80004001L; *pp = &g_peer; return 0; }
static long Forward(double* p, bool ok)
{
    Peer* pMP;
    long hr = GetPeer(&pMP, ok);
    if (hr < 0) return hr;
    hr = pMP->Get(p);
    pMP->Release();
    return hr;
}
int main()
{
    double d = 0;
    int ok = Forward(&d, true) == 0 && d == 2.5 && g_released == 1;
    ok = ok && Forward(&d, false) == (long)0x80004001L && g_released == 1;
    if (!ok) { std::printf("00cad8b0_TEST FAIL\n"); return 1; }
    std::printf("00cad8b0_TEST PASS\n");
    return 0;
}