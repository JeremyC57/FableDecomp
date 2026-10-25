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
// CBaseInputPin::ReceiveCanBlock: S_OK (blocks) when there are no connected output pins or any downstream
// IMemInputPin may block; S_FALSE only when every connected output pin's peer says it never blocks.
enum PIN_DIRECTION { PINDIR_INPUT = 0, PINDIR_OUTPUT = 1 };
struct IMemInputPin { HRESULT canBlock; int refs; IMemInputPin() : canBlock(1), refs(0) {} HRESULT ReceiveCanBlock() { return canBlock; } ULONG Release() { return --refs; } };
struct IPin {
    IMemInputPin* mip; int refs;
    IPin() : mip(0), refs(0) {}
    HRESULT QueryInterface(const IID&, void** ppv) { if (!mip) { *ppv = 0; return HR(0x80004002); } mip->refs++; *ppv = mip; return 0; }
    ULONG Release() { return --refs; }
};
struct CBasePin {
    PIN_DIRECTION dir; IPin* peer; HRESULT hrDir;
    CBasePin() : dir(PINDIR_INPUT), peer(0), hrDir(0) {}
    HRESULT QueryDirection(PIN_DIRECTION* p) { *p = dir; return hrDir; }
    HRESULT ConnectedTo(IPin** pp) { if (!peer) { *pp = 0; return HR(0x80040209); } peer->refs++; *pp = peer; return 0; }
};
struct CBaseFilter {
    CBasePin* pins[4]; int n;
    CBaseFilter() : n(0) {}
    int GetPinCount() { return n; }
    CBasePin* GetPin(int i) { return pins[i]; }
};
static const IID IID_IMemInputPin = { 0x56A8689D, 0x0AD4, 0x11CE, { 0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70 } };
class CBaseInputPin {
public:
    CBaseFilter* m_pFilter;
    HRESULT ReceiveCanBlock()
    {
        int cPins = m_pFilter->GetPinCount();
        int cOutputPins = 0;
        for (int c = 0; c < cPins; c++) {
            CBasePin* pPin = m_pFilter->GetPin(c);
            PIN_DIRECTION pd;
            HRESULT hr = pPin->QueryDirection(&pd);
            if (hr < 0) {
                return hr;
            }
            if (pd == PINDIR_OUTPUT) {
                IPin* pConnected;
                hr = pPin->ConnectedTo(&pConnected);
                if (hr >= 0) {
                    cOutputPins++;
                    IMemInputPin* pInputPin;
                    hr = pConnected->QueryInterface(IID_IMemInputPin, (void**)&pInputPin);
                    pConnected->Release();
                    if (hr >= 0) {
                        hr = pInputPin->ReceiveCanBlock();
                        pInputPin->Release();
                        if (hr != 1) {
                            return 0;
                        }
                    } else {
                        return 0;
                    }
                }
            }
        }
        return cOutputPins == 0 ? 0 : 1;
    }
};
static void run()
{
    CBaseFilter f; CBaseInputPin in; in.m_pFilter = &f;
    CBasePin inPin, out1, out2; out1.dir = PINDIR_OUTPUT; out2.dir = PINDIR_OUTPUT;
    f.pins[0] = &inPin; f.pins[1] = &out1; f.pins[2] = &out2; f.n = 3;
    CHECK(in.ReceiveCanBlock() == 0);                      // no connected output pins
    IPin p1, p2; IMemInputPin m1, m2; p1.mip = &m1; p2.mip = &m2;
    out1.peer = &p1; out2.peer = &p2;
    CHECK(in.ReceiveCanBlock() == 1);                      // both peers never block
    CHECK(p1.refs == 0 && p2.refs == 0 && m1.refs == 0 && m2.refs == 0);
    m2.canBlock = 0;
    CHECK(in.ReceiveCanBlock() == 0);                      // one peer may block
    p1.mip = 0;
    CHECK(in.ReceiveCanBlock() == 0);                      // unknown transport
    inPin.hrDir = HR(0x80004005);
    CHECK(in.ReceiveCanBlock() == HR(0x80004005));
}

int main()
{
    run();
    if (g_fail) { std::printf("00ca8d50_TEST FAIL\n"); return 1; }
    std::printf("00ca8d50_TEST PASS\n");
    return 0;
}