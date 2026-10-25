// Behaviour test for 0x00CA4780 CBaseRenderer::NonDelegatingQueryInterface (DirectShow renbase.cpp): IMediaPosition and IMediaSeeking go to GetMediaPositionInterface, anything else to CBaseFilter.
// Test model: plain C++98 copies of the members the function touches; Win32 / COM callees are recording stand-ins.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifdef _MSC_VER
typedef __int64 LONGLONG;
#else
typedef long long LONGLONG;
#endif
typedef LONGLONG REFERENCE_TIME;
typedef int HRESULT;
struct GUID { unsigned int Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
inline int operator==(const GUID& a, const GUID& b) { return std::memcmp(&a, &b, sizeof(GUID)) == 0; }
#define NOERROR 0
#define S_OK 0
#define S_FALSE 1
#define E_POINTER ((HRESULT)0x80004003u)
#define E_OUTOFMEMORY ((HRESULT)0x8007000Eu)
#define E_FAIL ((HRESULT)0x80004005u)
#define FAILED(hr) (((HRESULT)(hr)) < 0)
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define CheckPointer(p, ret) { if ((p) == 0) return (ret); }
#define State_Stopped 0
#define State_Paused 1
#define State_Running 2
struct CCritSec {
    int depth; int entered;
    CCritSec() : depth(0), entered(0) {}
    void Lock() { depth++; entered++; }
    void Unlock() { depth--; }
};
struct CAutoLock {
    CCritSec* m_pLock;
    CAutoLock(CCritSec* plock) : m_pLock(plock) { m_pLock->Lock(); }
    ~CAutoLock() { m_pLock->Unlock(); }
};
struct CAMEvent {                                // the test's event: a flag instead of a kernel HANDLE
    int signalled;
    CAMEvent() : signalled(0) {}
    void Set() { signalled = 1; }
    void Reset() { signalled = 0; }
};
static const GUID IID_IMediaPosition = { 0x56A868B1, 0x0AD4, 0x11CE, { 0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70 } };
static const GUID IID_IMediaSeeking = { 0x36B73880, 0xC2C8, 0x11CF, { 0x8B, 0x46, 0x00, 0x80, 0x5F, 0x6C, 0xEF, 0x60 } };
static const GUID IID_IBaseFilter = { 0x56A86895, 0x0AD4, 0x11CE, { 0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70 } };
struct CBaseFilter {
    int base_calls;
    CBaseFilter() : base_calls(0) {}
    HRESULT NonDelegatingQueryInterface(const GUID& riid, void** ppv) { base_calls++; *ppv = 0; return 0x4004; }
};
struct CBaseRenderer : CBaseFilter {
    int pos_calls; const GUID* pos_riid;
    CBaseRenderer() : pos_calls(0), pos_riid(0) {}
    virtual HRESULT GetMediaPositionInterface(const GUID& riid, void** ppv) { pos_calls++; pos_riid = &riid; *ppv = this; return 0x77; }
    HRESULT NonDelegatingQueryInterface(const GUID& riid, void** ppv);
};
HRESULT CBaseRenderer::NonDelegatingQueryInterface(const GUID& riid, void** ppv)
{
    if (riid == IID_IMediaPosition || riid == IID_IMediaSeeking) {
        return GetMediaPositionInterface(riid, ppv);
    } else {
        return CBaseFilter::NonDelegatingQueryInterface(riid, ppv);
    }
}
int main()
{
    CBaseRenderer r;
    void* p = 0;
    int ok = 1;
    ok &= r.NonDelegatingQueryInterface(IID_IMediaPosition, &p) == 0x77 && p == &r && r.pos_calls == 1 && r.pos_riid == &IID_IMediaPosition;
    ok &= r.NonDelegatingQueryInterface(IID_IMediaSeeking, &p) == 0x77 && r.pos_calls == 2 && r.pos_riid == &IID_IMediaSeeking;
    ok &= r.base_calls == 0;
    ok &= r.NonDelegatingQueryInterface(IID_IBaseFilter, &p) == 0x4004 && p == 0 && r.base_calls == 1 && r.pos_calls == 2;
    if (!ok) { std::printf("00ca4780_TEST FAIL\n"); return 1; }
    std::printf("00ca4780_TEST PASS\n");
    return 0;
}