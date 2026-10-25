// Behaviour test for 0x00CA5E70 CBaseVideoRenderer::get_AvgSyncOffset (DirectShow renbase.cpp, IQualProp): average sync error m_iTotAcc / (frames - 1), 0 without a clock or with at most one frame.
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
struct IReferenceClock { int dummy; };
struct CBaseVideoRenderer {
    CCritSec m_InterfaceLock; IReferenceClock* m_pClock; int m_cFramesDrawn; LONGLONG m_iTotAcc;
    HRESULT get_AvgSyncOffset(int* piAvg);
};
HRESULT CBaseVideoRenderer::get_AvgSyncOffset(int* piAvg)
{
    CheckPointer(piAvg, E_POINTER);
    CAutoLock cVideoLock(&m_InterfaceLock);
    if (0 == m_pClock) {
        *piAvg = 0;
        return NOERROR;
    }
    if (m_cFramesDrawn <= 1) {
        *piAvg = 0;
    } else {
        *piAvg = (int)(m_iTotAcc / (m_cFramesDrawn - 1));
    }
    return NOERROR;
}
int main()
{
    IReferenceClock clock;
    CBaseVideoRenderer v; v.m_pClock = 0; v.m_cFramesDrawn = 11; v.m_iTotAcc = -500;
    int r = -1;
    int ok = 1;
    ok &= v.get_AvgSyncOffset(0) == E_POINTER;
    ok &= v.get_AvgSyncOffset(&r) == NOERROR && r == 0;
    v.m_pClock = &clock; ok &= v.get_AvgSyncOffset(&r) == NOERROR && r == -50;
    v.m_iTotAcc = (LONGLONG)30000 * 100000; v.m_cFramesDrawn = 100001; ok &= v.get_AvgSyncOffset(&r) == NOERROR && r == 30000;
    v.m_cFramesDrawn = 1; ok &= v.get_AvgSyncOffset(&r) == NOERROR && r == 0;
    ok &= v.m_InterfaceLock.depth == 0;
    if (!ok) { std::printf("00ca5e70_TEST FAIL\n"); return 1; }
    std::printf("00ca5e70_TEST PASS\n");
    return 0;
}