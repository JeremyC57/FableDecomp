#include <cstdio>
// CBaseOutputPin::DeliverEndOfStream: VFW_E_NOT_CONNECTED, else m_Connected->EndOfStream().
struct Peer { int eos; virtual int EndOfStream() { ++eos; return 0; } };
struct OutPin { Peer* m_Connected; int DeliverEndOfStream() { if (m_Connected == 0) return (int)0x80040209; return m_Connected->EndOfStream(); } };
int main()
{
    Peer peer; peer.eos = 0; OutPin p;
    p.m_Connected = 0;
    if (p.DeliverEndOfStream() != (int)0x80040209 || peer.eos != 0) { std::printf("00ca8820_TEST FAIL\n"); return 1; }
    p.m_Connected = &peer;
    if (p.DeliverEndOfStream() != 0 || peer.eos != 1) { std::printf("00ca8820_TEST FAIL\n"); return 1; }
    std::printf("00ca8820_TEST PASS\n");
    return 0;
}