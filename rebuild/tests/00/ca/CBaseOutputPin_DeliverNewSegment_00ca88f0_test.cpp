#include <cstdio>
// CBaseOutputPin::DeliverNewSegment: VFW_E_NOT_CONNECTED, else m_Connected->NewSegment(tStart, tStop, dRate).
struct Peer { long long s, e; double r; virtual int NewSegment(long long a, long long b, double c) { s = a; e = b; r = c; return 3; } };
struct OutPin {
    Peer* m_Connected;
    int DeliverNewSegment(long long a, long long b, double c) { if (m_Connected == 0) return (int)0x80040209; return m_Connected->NewSegment(a, b, c); }
};
int main()
{
    Peer peer; peer.s = peer.e = 0; peer.r = 0; OutPin p;
    p.m_Connected = 0;
    if (p.DeliverNewSegment(1, 2, 1.0) != (int)0x80040209) { std::printf("00ca88f0_TEST FAIL\n"); return 1; }
    p.m_Connected = &peer;
    if (p.DeliverNewSegment(5, 0x100000000LL, 0.5) != 3 || peer.s != 5 || peer.e != 0x100000000LL || peer.r != 0.5) { std::printf("00ca88f0_TEST FAIL\n"); return 1; }
    std::printf("00ca88f0_TEST PASS\n");
    return 0;
}