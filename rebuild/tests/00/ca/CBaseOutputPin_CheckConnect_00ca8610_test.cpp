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
// CBaseOutputPin::CheckConnect: refuse a peer with our own direction, then fetch the peer's IMemInputPin.
#define VFW_E_INVALID_DIRECTION HR(0x80040208)
enum PIN_DIRECTION { PINDIR_INPUT = 0, PINDIR_OUTPUT = 1 };
static const IID IID_IMemInputPin = { 0x56A8689D, 0x0AD4, 0x11CE, { 0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70 } };
struct IMemInputPin { int dummy; };
static IMemInputPin g_mip;
struct IPin {
    PIN_DIRECTION dir; HRESULT hrQI; int qis;
    IPin() : dir(PINDIR_INPUT), hrQI(0), qis(0) {}
    HRESULT QueryDirection(PIN_DIRECTION* p) { *p = dir; return 0; }
    HRESULT QueryInterface(const IID& riid, void** ppv)
    {
        qis++;
        if (hrQI < 0 || !(riid == IID_IMemInputPin)) { *ppv = 0; return hrQI < 0 ? hrQI : HR(0x80004002); }
        *ppv = &g_mip; return 0;
    }
};
class CBasePin {
public:
    PIN_DIRECTION m_dir;
    HRESULT CheckConnect(IPin* pPin)
    {
        PIN_DIRECTION pd;
        pPin->QueryDirection(&pd);
        if (pd == m_dir) {
            return VFW_E_INVALID_DIRECTION;
        }
        return 0;
    }
};
class CBaseOutputPin : public CBasePin {
public:
    IMemInputPin* m_pInputPin;
    CBaseOutputPin() : m_pInputPin(0) { m_dir = PINDIR_OUTPUT; }
    HRESULT CheckConnect(IPin* pPin)
    {
        HRESULT hr = CBasePin::CheckConnect(pPin);
        if (hr < 0) {
            return hr;
        }
        hr = pPin->QueryInterface(IID_IMemInputPin, (void**)&m_pInputPin);
        if (hr < 0) {
            return hr;
        }
        return 0;
    }
};
static void run()
{
    { CBaseOutputPin o; IPin peer; peer.dir = PINDIR_OUTPUT;
      CHECK(o.CheckConnect(&peer) == VFW_E_INVALID_DIRECTION); CHECK(peer.qis == 0); CHECK(o.m_pInputPin == 0); }
    { CBaseOutputPin o; IPin peer; peer.hrQI = HR(0x80004002);
      CHECK(o.CheckConnect(&peer) == HR(0x80004002)); CHECK(peer.qis == 1); CHECK(o.m_pInputPin == 0); }
    { CBaseOutputPin o; IPin peer;
      CHECK(o.CheckConnect(&peer) == 0); CHECK(o.m_pInputPin == &g_mip); }
}

int main()
{
    run();
    if (g_fail) { std::printf("00ca8610_TEST FAIL\n"); return 1; }
    std::printf("00ca8610_TEST PASS\n");
    return 0;
}