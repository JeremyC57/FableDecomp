#include <cstdio>
// CPosPassThru::GetAvailable: forward to the upstream peer (GetPeer/GetPeerSeeking failure returned as is) and release the peer.
struct Peer { int refs, calls; double d; long long* p1; virtual int GetAvailable(long long* pE, long long* pL) { ++calls; *pE = 3; *pL = 4; return 9; } void Release() { --refs; } };
struct PosPassThru {
    Peer* peer; int fail;
    int GetPeerX(Peer** pp) { *pp = 0; if (fail) return (int)0x80004001; ++peer->refs; *pp = peer; return 0; }
    int GetAvailable(long long* pE, long long* pL)
    {
        Peer* pMS;
        int hr = GetPeerX(&pMS);
        if (hr < 0) return hr;
        hr = pMS->GetAvailable(pE, pL);
        pMS->Release();
        return hr;
    }
};
int main()
{
    Peer peer; peer.refs = 1; peer.calls = 0; peer.d = 0; peer.p1 = 0; PosPassThru p; p.peer = &peer; p.fail = 1;
    long long x = 0, y = 0;
    if (p.GetAvailable(&x, &y) != (int)0x80004001 || x != 0) { std::printf("00cad790_TEST FAIL\n"); return 1; }
    p.fail = 0; if (p.GetAvailable(&x, &y) != 9 || x != 3 || y != 4 || peer.refs != 1) { std::printf("00cad790_TEST FAIL\n"); return 1; }
    std::printf("00cad790_TEST PASS\n");
    return 0;
}