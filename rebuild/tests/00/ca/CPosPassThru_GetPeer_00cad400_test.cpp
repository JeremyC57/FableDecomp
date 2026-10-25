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
// CPosPassThru::GetPeer: the connected peer pin's IMediaPosition, or E_NOTIMPL when unconnected or unsupported.
static const IID IID_IMediaPosition = { 0x56A868B2, 0x0AD4, 0x11CE, { 0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70 } };
struct IMediaPosition { int dummy; };
static IMediaPosition g_itf;
struct IPin {
    BOOL has; int refs; int qis;
    IPin() : has(1), refs(0), qis(0) {}
    HRESULT QueryInterface(const IID& riid, void** ppv) { qis++; if (!has || !(riid == IID_IMediaPosition)) { *ppv = 0; return HR(0x80004002); } *ppv = &g_itf; return 0; }
    ULONG Release() { return --refs; }
    HRESULT ConnectedTo(IPin** pp) { if (!peer) { *pp = 0; return HR(0x80040209); } peer->refs++; *pp = peer; return 0; }
    IPin* peer;
};
class CPosPassThru {
public:
    IPin* m_pPin;
    HRESULT GetPeer(IMediaPosition** pp)
    {
        *pp = 0;
        IPin* pConnected;
        HRESULT hr = m_pPin->ConnectedTo(&pConnected);
        if (hr < 0) {
            return HR(0x80004001);
        }
        IMediaPosition* p;
        hr = pConnected->QueryInterface(IID_IMediaPosition, (void**)&p);
        pConnected->Release();
        if (hr < 0) {
            return HR(0x80004001);
        }
        *pp = p;
        return 0;
    }
};
static void run()
{
    IPin pin, peer; pin.peer = 0; peer.peer = 0;
    CPosPassThru pt; pt.m_pPin = &pin;
    IMediaPosition* p = &g_itf;
    CHECK(pt.GetPeer(&p) == HR(0x80004001)); CHECK(p == 0);
    pin.peer = &peer;
    CHECK(pt.GetPeer(&p) == 0); CHECK(p == &g_itf); CHECK(peer.refs == 0); CHECK(peer.qis == 1);
    peer.has = 0; p = &g_itf;
    CHECK(pt.GetPeer(&p) == HR(0x80004001)); CHECK(p == 0); CHECK(peer.refs == 0); CHECK(peer.qis == 2);
}

int main()
{
    run();
    if (g_fail) { std::printf("00cad400_TEST FAIL\n"); return 1; }
    std::printf("00cad400_TEST PASS\n");
    return 0;
}