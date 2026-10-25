// Behaviour test for 0x00CA4910 CBaseRenderer::FindPin (DirectShow renbase.cpp): the only pin is "In"; hand it out AddRef'd, otherwise NULL and VFW_E_NOT_FOUND.
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
#define VFW_E_NOT_FOUND ((HRESULT)0x80040216u)
static int lstrcmpW(const unsigned short* a, const unsigned short* b)
{
    while (*a && *a == *b) { a++; b++; }
    return (int)*a - (int)*b;
}
struct IPin { int refs; IPin() : refs(0) {} virtual unsigned int AddRef() { return ++refs; } };
struct CBasePin : IPin { };
static const unsigned short kIn[] = { 'I', 'n', 0 };
struct CBaseRenderer {
    CBasePin m_pin; int getpin_arg;
    CBaseRenderer() : getpin_arg(-1) {}
    virtual CBasePin* GetPin(int n) { getpin_arg = n; return &m_pin; }
    HRESULT FindPin(const unsigned short* Id, IPin** ppPin);
};
HRESULT CBaseRenderer::FindPin(const unsigned short* Id, IPin** ppPin)
{
    CheckPointer(ppPin, E_POINTER);
    if (0 == lstrcmpW(Id, kIn)) {
        *ppPin = GetPin(0);
        (*ppPin)->AddRef();
    } else {
        *ppPin = 0;
        return VFW_E_NOT_FOUND;
    }
    return NOERROR;
}
int main()
{
    CBaseRenderer r;
    IPin* p = (IPin*)&r;
    static const unsigned short kIn2[] = { 'I', 'n', 0 };
    static const unsigned short kOut[] = { 'O', 'u', 't', 0 };
    int ok = 1;
    ok &= r.FindPin(kIn2, 0) == E_POINTER;
    ok &= r.FindPin(kOut, &p) == VFW_E_NOT_FOUND && p == 0 && r.m_pin.refs == 0;
    ok &= r.FindPin(kIn2, &p) == NOERROR && p == &r.m_pin && r.m_pin.refs == 1 && r.getpin_arg == 0;
    if (!ok) { std::printf("00ca4910_TEST FAIL\n"); return 1; }
    std::printf("00ca4910_TEST PASS\n");
    return 0;
}