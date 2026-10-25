// Behaviour test for 0x00CA51C0 CRendererInputPin::QueryId (DirectShow renbase.cpp): return a CoTaskMemAlloc'd copy of the pin name "In" (8 bytes requested).
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
static int g_fail_alloc; static unsigned int g_alloc_size;
static unsigned short g_block[8];
static void* CoTaskMemAlloc(unsigned int cb) { g_alloc_size = cb; return g_fail_alloc ? 0 : g_block; }
static unsigned short* lstrcpyW(unsigned short* d, const unsigned short* s) { unsigned short* r = d; while ((*d++ = *s++) != 0) {} return r; }
static const unsigned short kIn[] = { 'I', 'n', 0 };
struct CRendererInputPin { HRESULT QueryId(unsigned short** Id); };
HRESULT CRendererInputPin::QueryId(unsigned short** Id)
{
    CheckPointer(Id, E_POINTER);
    *Id = (unsigned short*)CoTaskMemAlloc(8);
    if (*Id == 0) {
        return E_OUTOFMEMORY;
    }
    lstrcpyW(*Id, kIn);
    return NOERROR;
}
int main()
{
    CRendererInputPin pin; unsigned short* id = 0;
    int ok = 1;
    ok &= pin.QueryId(0) == E_POINTER;
    g_fail_alloc = 1; ok &= pin.QueryId(&id) == E_OUTOFMEMORY && id == 0 && g_alloc_size == 8;
    g_fail_alloc = 0; g_alloc_size = 0;
    ok &= pin.QueryId(&id) == NOERROR && id == g_block && g_alloc_size == 8 && id[0] == 'I' && id[1] == 'n' && id[2] == 0;
    if (!ok) { std::printf("00ca51c0_TEST FAIL\n"); return 1; }
    std::printf("00ca51c0_TEST PASS\n");
    return 0;
}