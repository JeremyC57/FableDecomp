// Behaviour test for 0x00CA5F50 CBaseVideoRenderer::GetStdDev (DirectShow renbase.cpp): sample standard deviation sqrt((sumsq - tot^2/n)/(n-1)) with the integer isqrt; 0 without a clock or with at most one sample.
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
static LONGLONG llMulDiv(LONGLONG a, LONGLONG b, LONGLONG c, LONGLONG rnd) { return (a * b + rnd) / c; }
static int isqrt(int x)
{
    int s = 1;
    if (x > 0x40000000) {
        s = 0x8000;
    } else {
        while (s * s < x) s = 2 * s;
        if (x == 0) {
            s = 0;
        } else {
            s = (s * s + x) / (2 * s);
            if (s >= 0) s = (s * s + x) / (2 * s);
            if (s >= 0) s = (s * s + x) / (2 * s);
        }
    }
    return s;
}
struct IReferenceClock { int dummy; };
struct CBaseVideoRenderer {
    CCritSec m_InterfaceLock; IReferenceClock* m_pClock;
    HRESULT GetStdDev(int nSamples, int* piResult, LONGLONG llSumSq, LONGLONG iTot);
};
HRESULT CBaseVideoRenderer::GetStdDev(int nSamples, int* piResult, LONGLONG llSumSq, LONGLONG iTot)
{
    CheckPointer(piResult, E_POINTER);
    CAutoLock cVideoLock(&m_InterfaceLock);
    if (0 == m_pClock) {
        *piResult = 0;
        return NOERROR;
    }
    if (nSamples <= 1) {
        *piResult = 0;
    } else {
        *piResult = isqrt((int)((llSumSq - llMulDiv(iTot, iTot, nSamples, 0)) / (nSamples - 1)));
    }
    return NOERROR;
}
int main()
{
    IReferenceClock clock;
    CBaseVideoRenderer v; v.m_pClock = 0;
    int r = -1;
    int ok = 1;
    ok &= v.GetStdDev(4, 0, 0, 0) == E_POINTER;
    ok &= v.GetStdDev(4, &r, 100, 10) == NOERROR && r == 0;
    v.m_pClock = &clock;
    // samples 2, 4, 4, 4, 5, 5, 7, 9: tot 40, sumsq 232, (232 - 1600/8) / 7 = 4.57 -> isqrt(4) = 2
    ok &= v.GetStdDev(8, &r, 232, 40) == NOERROR && r == 2;
    // samples 100, 300: tot 400, sumsq 100000, (100000 - 80000) / 1 = 20000 -> 141
    ok &= v.GetStdDev(2, &r, 100000, 400) == NOERROR && r >= 141 && r <= 142;
    ok &= v.GetStdDev(1, &r, 100, 10) == NOERROR && r == 0;
    ok &= v.m_InterfaceLock.depth == 0;
    if (!ok) { std::printf("00ca5f50_TEST FAIL\n"); return 1; }
    std::printf("00ca5f50_TEST PASS\n");
    return 0;
}