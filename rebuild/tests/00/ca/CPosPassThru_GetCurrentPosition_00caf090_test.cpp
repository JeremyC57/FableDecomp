// Behaviour test (standalone; compiled separately from the source). Plain C++98: the COM interfaces are modelled
// as ordinary virtual classes (no calling-convention keywords), the Win32 imports as local fakes.
#include <cstdio>
#include <cstring>
typedef int HRESULT;
typedef unsigned int ULONG;
typedef unsigned int DWORD;
typedef int LONG;
typedef int BOOL;
typedef void* HANDLE;
typedef unsigned short WCHAR;
typedef unsigned char BYTE;
#ifdef _MSC_VER
typedef __int64 LONGLONG;
#else
typedef long long LONGLONG;
#endif
typedef LONGLONG REFERENCE_TIME;
#ifdef _MSC_VER
#define LL(x) x##i64
#else
#define LL(x) x##LL
#endif
struct GUID { unsigned int Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
typedef GUID IID;
inline bool operator==(const GUID& a, const GUID& b) { return std::memcmp(&a, &b, sizeof(GUID)) == 0; }
#define HR(x) ((HRESULT)(x))
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed at line %d\n", __LINE__); } } while (0)
// CPosPassThru::GetCurrentPosition: report the locally held media time when there is one (any success -> S_OK),
// else forward IMediaSeeking::GetCurrentPosition to the upstream peer (GetSeekingLongLong), E_NOTIMPL without one.
struct IMediaSeeking {
    int releases; LONGLONG pos; HRESULT hr;
    IMediaSeeking() : releases(0), pos(0), hr(0) {}
    virtual HRESULT GetCurrentPosition(LONGLONG* p) { *p = pos; return hr; }
    ULONG Release() { return ++releases; }
};
static IMediaSeeking g_peer; static BOOL g_hasPeer = 1;
class CPosPassThru {
public:
    HRESULT hrMedia; LONGLONG media;
    CPosPassThru() : hrMedia(HR(0x80004005)), media(0) {}
    virtual HRESULT GetMediaTime(LONGLONG* pStart, LONGLONG* pEnd) { if (hrMedia >= 0) *pStart = media; if (pEnd) *pEnd = 0; return hrMedia; }
    HRESULT GetPeerSeeking(IMediaSeeking** pp) { *pp = 0; if (!g_hasPeer) return HR(0x80004001); *pp = &g_peer; return 0; }
    HRESULT GetSeekingLongLong(HRESULT (IMediaSeeking::*pMethod)(LONGLONG*), LONGLONG* pll)
    {
        IMediaSeeking* pMS;
        HRESULT hr = GetPeerSeeking(&pMS);
        if (hr >= 0)
        {
            hr = (pMS->*pMethod)(pll);
            pMS->Release();
        }
        return hr;
    }
    HRESULT GetCurrentPosition(LONGLONG* pCurrent)
    {
        HRESULT hr = GetMediaTime(pCurrent, 0);
        if (hr >= 0) hr = 0;
        else hr = GetSeekingLongLong(&IMediaSeeking::GetCurrentPosition, pCurrent);
        return hr;
    }
};
static void run()
{
    LONGLONG cur = -1;
    CPosPassThru p; p.hrMedia = 1; p.media = LL(0x500000001);
    CHECK(p.GetCurrentPosition(&cur) == 0); CHECK(cur == LL(0x500000001)); CHECK(g_peer.releases == 0);
    p.hrMedia = HR(0x80004005); g_peer.pos = 77; g_peer.hr = 1;
    CHECK(p.GetCurrentPosition(&cur) == 1); CHECK(cur == 77); CHECK(g_peer.releases == 1);
    g_hasPeer = 0;
    CHECK(p.GetCurrentPosition(&cur) == HR(0x80004001)); CHECK(g_peer.releases == 1);
}

int main()
{
    run();
    if (g_fail) { std::printf("00caf090_TEST FAIL\n"); return 1; }
    std::printf("00caf090_TEST PASS\n");
    return 0;
}