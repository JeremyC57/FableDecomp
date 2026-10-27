#include <cstdio>
// CBasePin::ConnectedTo: E_POINTER / the AddRef'd peer / VFW_E_NOT_CONNECTED with *pPin = NULL.
struct IPinM { int refs; void AddRef() { ++refs; } };
struct Pin {
    IPinM* m_Connected;
    int ConnectedTo(IPinM** pPin)
    {
        if (pPin == 0) return (int)0x80004003;
        IPinM* pConnected = m_Connected;
        *pPin = pConnected;
        if (pConnected != 0) { pConnected->AddRef(); return 0; }
        return (int)0x80040209;
    }
};
int main()
{
    IPinM peer; peer.refs = 1;
    Pin p; p.m_Connected = 0;
    IPinM* out = &peer;
    if (p.ConnectedTo(0) != (int)0x80004003) { std::printf("00ca8340_TEST FAIL\n"); return 1; }
    if (p.ConnectedTo(&out) != (int)0x80040209 || out != 0) { std::printf("00ca8340_TEST FAIL\n"); return 1; }
    p.m_Connected = &peer;
    if (p.ConnectedTo(&out) != 0 || out != &peer || peer.refs != 2) { std::printf("00ca8340_TEST FAIL\n"); return 1; }
    std::printf("00ca8340_TEST PASS\n");
    return 0;
}