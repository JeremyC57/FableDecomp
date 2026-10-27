#include <cstdio>
// CPosPassThru::GetPreroll via GetSeekingLongLong(&IMediaSeeking::GetPreroll, pll): member-pointer call on the peer.
struct Seek { int refs; virtual int GetPreroll(long long* p) { *p = 0x1234567890LL; return 0; } virtual int GetDuration(long long* p) { *p = 1; return 1; } void Release() { --refs; } };
struct PosPassThru {
    Seek* peer; int fail;
    int GetPeerSeeking(Seek** pp) { *pp = 0; if (fail) return (int)0x80004001; ++peer->refs; *pp = peer; return 0; }
    int GetSeekingLongLong(int (Seek::*pMethod)(long long*), long long* pll)
    {
        Seek* pMS;
        int hr = GetPeerSeeking(&pMS);
        if (hr >= 0) { hr = (pMS->*pMethod)(pll); pMS->Release(); }
        return hr;
    }
    int GetPreroll(long long* pll) { return GetSeekingLongLong(&Seek::GetPreroll, pll); }
};
int main()
{
    Seek s; s.refs = 1; PosPassThru p; p.peer = &s; p.fail = 1; long long v = 0;
    if (p.GetPreroll(&v) != (int)0x80004001 || v != 0) { std::printf("00caf160_TEST FAIL\n"); return 1; }
    p.fail = 0; if (p.GetPreroll(&v) != 0 || v != 0x1234567890LL || s.refs != 1) { std::printf("00caf160_TEST FAIL\n"); return 1; }
    std::printf("00caf160_TEST PASS\n");
    return 0;
}