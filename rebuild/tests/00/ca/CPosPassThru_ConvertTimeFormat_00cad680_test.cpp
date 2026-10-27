#include <cstdio>
// CPosPassThru::ConvertTimeFormat: forward to the upstream peer (GetPeer/GetPeerSeeking failure returned as is) and release the peer.
struct Peer { int refs, calls; double d; long long* p1; virtual int ConvertTimeFormat(long long* pTarget, const int* tf, long long Source, const int* sf) { ++calls; *pTarget = Source * 2; return 9; } void Release() { --refs; } };
struct PosPassThru {
    Peer* peer; int fail;
    int GetPeerX(Peer** pp) { *pp = 0; if (fail) return (int)0x80004001; ++peer->refs; *pp = peer; return 0; }
    int ConvertTimeFormat(long long* pTarget, const int* tf, long long Source, const int* sf)
    {
        Peer* pMS;
        int hr = GetPeerX(&pMS);
        if (hr < 0) return hr;
        hr = pMS->ConvertTimeFormat(pTarget, tf, Source, sf);
        pMS->Release();
        return hr;
    }
};
int main()
{
    Peer peer; peer.refs = 1; peer.calls = 0; peer.d = 0; peer.p1 = 0; PosPassThru p; p.peer = &peer; p.fail = 1;
    long long x = 0, y = 0;
    if (p.ConvertTimeFormat(&x, 0, 21, 0) != (int)0x80004001 || peer.calls != 0) { std::printf("00cad680_TEST FAIL\n"); return 1; }
    p.fail = 0; if (p.ConvertTimeFormat(&x, 0, 0x100000000LL, 0) != 9 || x != 0x200000000LL || peer.refs != 1) { std::printf("00cad680_TEST FAIL\n"); return 1; }
    std::printf("00cad680_TEST PASS\n");
    return 0;
}