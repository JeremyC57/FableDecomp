// behaviour test for 0x00cad640 CPosPassThru::IsUsingTimeFormat: a failed peer lookup returns its HRESULT untouched; otherwise the
// peer IMediaSeeking's IsUsingTimeFormat result is returned and the peer reference released exactly once
#include <cstdio>
struct Peer {
    int calls, releases; void* lastArg; int result;
    int IsUsingTimeFormat(void* arg) { ++calls; lastArg = arg; return result; }
    void Release() { ++releases; }
};
struct PassThru {
    Peer* peer; int lookup;
    int GetPeerSeeking(Peer** pp) { *pp = 0; if (lookup < 0) return lookup; *pp = peer; return 0; }
    int IsUsingTimeFormat(void* arg)
    {
        Peer* pMS;
        int hr = GetPeerSeeking(&pMS);
        if (hr < 0) return hr;
        hr = pMS->IsUsingTimeFormat(arg);
        pMS->Release();
        return hr;
    }
};
int main()
{
    int fails = 0;
    Peer peer = { 0, 0, 0, 0x40003 };
    PassThru pt; pt.peer = &peer; pt.lookup = (int)0x80004001u;
    int arg = 5;
    if (pt.IsUsingTimeFormat(&arg) != (int)0x80004001u || peer.calls || peer.releases) fails |= 1;
    pt.lookup = 0;
    if (pt.IsUsingTimeFormat(&arg) != 0x40003 || peer.calls != 1 || peer.releases != 1 || peer.lastArg != &arg) fails |= 2;
    peer.result = (int)0x80070057u;
    if (pt.IsUsingTimeFormat(&arg) != (int)0x80070057u || peer.releases != 2) fails |= 4;   // a failing peer call is still released
    if (fails) { std::printf("00cad640_TEST FAIL %d\n", fails); return 1; }
    std::printf("00cad640_TEST PASS\n");
    return 0;
}