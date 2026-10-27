#include <cstdio>
// CPosPassThru::SetPositions: forward to the upstream peer (GetPeer/GetPeerSeeking failure returned as is) and release the peer.
struct Peer { int refs, calls; double d; long long* p1; virtual int SetPositions(long long* pCur, unsigned cf, long long* pStop, unsigned sf) { ++calls; p1 = pStop; d = cf + sf; return 9; } void Release() { --refs; } };
struct PosPassThru {
    Peer* peer; int fail;
    int GetPeerX(Peer** pp) { *pp = 0; if (fail) return (int)0x80004001; ++peer->refs; *pp = peer; return 0; }
    int SetPositions(long long* pCur, unsigned cf, long long* pStop, unsigned sf)
    {
        Peer* pMS;
        int hr = GetPeerX(&pMS);
        if (hr < 0) return hr;
        hr = pMS->SetPositions(pCur, cf, pStop, sf);
        pMS->Release();
        return hr;
    }
};
int main()
{
    Peer peer; peer.refs = 1; peer.calls = 0; peer.d = 0; peer.p1 = 0; PosPassThru p; p.peer = &peer; p.fail = 1;
    long long x = 0, y = 0;
    if (p.SetPositions(&x, 1, &y, 2) != (int)0x80004001 || peer.calls != 0) { std::printf("00cad6d0_TEST FAIL\n"); return 1; }
    p.fail = 0; if (p.SetPositions(&x, 1, &y, 2) != 9 || peer.p1 != &y || peer.d != 3 || peer.refs != 1) { std::printf("00cad6d0_TEST FAIL\n"); return 1; }
    std::printf("00cad6d0_TEST PASS\n");
    return 0;
}