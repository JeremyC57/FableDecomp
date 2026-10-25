// Behaviour test for 0x00CA7500 CBaseFilter::NonDelegatingQueryInterface (DirectShow amfilter.cpp): IBaseFilter / IMediaFilter / IPersist give the IBaseFilter sub-object, IAMovieSetup its own, the rest goes to CUnknown.
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
static const GUID IID_IBaseFilter = { 0x56A86895, 0x0AD4, 0x11CE, { 0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70 } };
static const GUID IID_IMediaFilter = { 0x56A86899, 0x0AD4, 0x11CE, { 0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70 } };
static const GUID IID_IPersist = { 0x0000010C, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
static const GUID IID_IAMovieSetup = { 0xA3D8CEC0, 0x7E5A, 0x11CF, { 0xBB, 0xC5, 0x00, 0x80, 0x5F, 0x6C, 0xEF, 0x20 } };
static const GUID IID_IPin = { 0x56A86891, 0x0AD4, 0x11CE, { 0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70 } };
struct IUnknown { int refs; IUnknown() : refs(0) {} virtual unsigned int AddRef() { return ++refs; } };
struct IPersist : IUnknown { };
struct IMediaFilter : IPersist { };
struct IBaseFilter : IMediaFilter { };
struct IAMovieSetup : IUnknown { };
static HRESULT GetInterface(IUnknown* pUnk, void** ppv)    // combase.cpp
{
    CheckPointer(ppv, E_POINTER);
    *ppv = pUnk;
    pUnk->AddRef();
    return NOERROR;
}
struct CUnknown {
    int unk_calls;
    CUnknown() : unk_calls(0) {}
    virtual ~CUnknown() {}
    HRESULT NonDelegatingQueryInterface(const GUID& riid, void** ppv) { unk_calls++; *ppv = 0; return (HRESULT)0x80004002u; }
};
struct CBaseFilter : CUnknown, IBaseFilter, IAMovieSetup {
    HRESULT NonDelegatingQueryInterface(const GUID& riid, void** ppv);
};
HRESULT CBaseFilter::NonDelegatingQueryInterface(const GUID& riid, void** ppv)
{
    if (riid == IID_IBaseFilter) {
        return GetInterface((IBaseFilter *) this, ppv);
    } else if (riid == IID_IMediaFilter) {
        return GetInterface((IMediaFilter *) this, ppv);
    } else if (riid == IID_IPersist) {
        return GetInterface((IPersist *) this, ppv);
    } else if (riid == IID_IAMovieSetup) {
        return GetInterface((IAMovieSetup *) this, ppv);
    } else {
        return CUnknown::NonDelegatingQueryInterface(riid, ppv);
    }
}
int main()
{
    CBaseFilter f; void* p = 0;
    IBaseFilter* pbf = &f; IAMovieSetup* pms = &f;
    int ok = 1;
    ok &= (void*)pbf != (void*)&f && (void*)pms != (void*)pbf;     // three distinct sub-objects
    ok &= f.NonDelegatingQueryInterface(IID_IBaseFilter, &p) == NOERROR && p == (void*)(IUnknown*)pbf;
    ok &= f.NonDelegatingQueryInterface(IID_IMediaFilter, &p) == NOERROR && p == (void*)(IUnknown*)pbf;
    ok &= f.NonDelegatingQueryInterface(IID_IPersist, &p) == NOERROR && p == (void*)(IUnknown*)pbf;
    ok &= pbf->refs == 3;
    ok &= f.NonDelegatingQueryInterface(IID_IAMovieSetup, &p) == NOERROR && p == (void*)(IUnknown*)pms && pms->refs == 1;
    ok &= f.NonDelegatingQueryInterface(IID_IPin, &p) == (HRESULT)0x80004002u && p == 0 && f.unk_calls == 1;
    ok &= f.NonDelegatingQueryInterface(IID_IBaseFilter, 0) == E_POINTER && f.unk_calls == 1;
    if (!ok) { std::printf("00ca7500_TEST FAIL\n"); return 1; }
    std::printf("00ca7500_TEST PASS\n");
    return 0;
}