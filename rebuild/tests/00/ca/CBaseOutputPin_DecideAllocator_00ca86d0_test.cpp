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
// CBaseOutputPin::DecideAllocator: try the input pin's allocator, then our own (InitAllocator), sizing each with
// the input pin's requirements (cbAlign 0 -> 1) and notifying the input pin; release a refused allocator.
struct ALLOCATOR_PROPERTIES { long cBuffers; long cbBuffer; long cbAlign; long cbPrefix; };
struct IMemAllocator { int id; int refs; IMemAllocator(int i) : id(i), refs(1) {} ULONG Release() { return --refs; } };
static IMemAllocator g_inAlloc(1), g_outAlloc(2);
struct IMemInputPin {
    HRESULT hrGet; HRESULT hrNotifyIn; HRESULT hrNotifyOut; long reqAlign; int notifies; BOOL lastReadOnly;
    IMemInputPin() : hrGet(0), hrNotifyIn(0), hrNotifyOut(0), reqAlign(0), notifies(0), lastReadOnly(7) {}
    HRESULT GetAllocatorRequirements(ALLOCATOR_PROPERTIES* p) { p->cBuffers = 3; p->cbAlign = reqAlign; return 0; }
    HRESULT GetAllocator(IMemAllocator** pp) { if (hrGet < 0) return hrGet; *pp = &g_inAlloc; return 0; }
    HRESULT NotifyAllocator(IMemAllocator* p, BOOL ro) { notifies++; lastReadOnly = ro; return p->id == 1 ? hrNotifyIn : hrNotifyOut; }
};
class CBaseOutputPin {
public:
    HRESULT hrInit; HRESULT hrSize; long seenAlign; long seenBuffers; int sizes;
    CBaseOutputPin() : hrInit(0), hrSize(0), seenAlign(-1), seenBuffers(-1), sizes(0) {}
    virtual HRESULT DecideBufferSize(IMemAllocator*, ALLOCATOR_PROPERTIES* p) { sizes++; seenAlign = p->cbAlign; seenBuffers = p->cBuffers; return hrSize; }
    virtual HRESULT InitAllocator(IMemAllocator** pp) { if (hrInit < 0) return hrInit; *pp = &g_outAlloc; return 0; }
    HRESULT DecideAllocator(IMemInputPin* pPin, IMemAllocator** ppAlloc)
    {
        HRESULT hr = 0;
        *ppAlloc = 0;
        ALLOCATOR_PROPERTIES prop;
        std::memset(&prop, 0, sizeof(prop));
        pPin->GetAllocatorRequirements(&prop);
        if (prop.cbAlign == 0) {
            prop.cbAlign = 1;
        }
        hr = pPin->GetAllocator(ppAlloc);
        if (hr >= 0) {
            hr = DecideBufferSize(*ppAlloc, &prop);
            if (hr >= 0) {
                hr = pPin->NotifyAllocator(*ppAlloc, 0);
                if (hr >= 0) {
                    return 0;
                }
            }
        }
        if (*ppAlloc) {
            (*ppAlloc)->Release();
            *ppAlloc = 0;
        }
        hr = InitAllocator(ppAlloc);
        if (hr >= 0) {
            hr = DecideBufferSize(*ppAlloc, &prop);
            if (hr >= 0) {
                hr = pPin->NotifyAllocator(*ppAlloc, 0);
                if (hr >= 0) {
                    return 0;
                }
            }
        }
        if (*ppAlloc) {
            (*ppAlloc)->Release();
            *ppAlloc = 0;
        }
        return hr;
    }
};
static void run()
{
    IMemAllocator* pa;
    { CBaseOutputPin o; IMemInputPin in;
      CHECK(o.DecideAllocator(&in, &pa) == 0); CHECK(pa == &g_inAlloc); CHECK(o.seenAlign == 1); CHECK(o.seenBuffers == 3);
      CHECK(in.lastReadOnly == 0); CHECK(g_inAlloc.refs == 1); }
    { CBaseOutputPin o; IMemInputPin in; in.reqAlign = 16; in.hrNotifyIn = HR(0x80040211);
      CHECK(o.DecideAllocator(&in, &pa) == 0); CHECK(pa == &g_outAlloc); CHECK(o.seenAlign == 16); CHECK(o.sizes == 2);
      CHECK(g_inAlloc.refs == 0); g_inAlloc.refs = 1; }
    { CBaseOutputPin o; IMemInputPin in; in.hrGet = HR(0x80004002); o.hrInit = HR(0x8007000E);
      CHECK(o.DecideAllocator(&in, &pa) == HR(0x8007000E)); CHECK(pa == 0); CHECK(o.sizes == 0); }
    { CBaseOutputPin o; IMemInputPin in; o.hrSize = HR(0x80004005);
      CHECK(o.DecideAllocator(&in, &pa) == HR(0x80004005)); CHECK(pa == 0); CHECK(g_inAlloc.refs == 0); CHECK(g_outAlloc.refs == 0); }
}

int main()
{
    run();
    if (g_fail) { std::printf("00ca86d0_TEST FAIL\n"); return 1; }
    std::printf("00ca86d0_TEST PASS\n");
    return 0;
}