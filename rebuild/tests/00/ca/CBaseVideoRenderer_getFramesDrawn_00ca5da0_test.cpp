// Behaviour test for 0x00CA5DA0 CBaseVideoRenderer::get_FramesDrawn (DirectShow renbase.cpp, IQualProp): read m_cFramesDrawn under the interface lock.
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
struct CBaseVideoRenderer {
    CCritSec m_InterfaceLock; int m_cFramesDropped; int m_cFramesDrawn;
    HRESULT get_FramesDrawn(int* pcFramesDrawn);
};
HRESULT CBaseVideoRenderer::get_FramesDrawn(int* pcFramesDrawn)
{
    CheckPointer(pcFramesDrawn, E_POINTER);
    CAutoLock cVideoLock(&m_InterfaceLock);
    *pcFramesDrawn = m_cFramesDrawn;
    return NOERROR;
}
int main()
{
    CBaseVideoRenderer v; v.m_cFramesDropped = 17; v.m_cFramesDrawn = 321;
    int n = 0;
    int ok = 1;
    ok &= v.get_FramesDrawn(0) == E_POINTER && v.m_InterfaceLock.entered == 0;
    ok &= v.get_FramesDrawn(&n) == NOERROR && n == 321 && v.m_InterfaceLock.entered == 1 && v.m_InterfaceLock.depth == 0;
    if (!ok) { std::printf("00ca5da0_TEST FAIL\n"); return 1; }
    std::printf("00ca5da0_TEST PASS\n");
    return 0;
}