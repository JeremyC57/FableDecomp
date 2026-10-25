// Behaviour test for 0x00CA7850 CBaseFilter::JoinFilterGraph (DirectShow amfilter.cpp): under the filter lock remember the graph, fetch its IMediaEventSink without keeping a reference (NULL when leaving), and replace the stored name with a copy.
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
static const GUID IID_IMediaEventSink = { 0x56A868A2, 0x0AD4, 0x11CE, { 0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70 } };
static int lstrlenW(const unsigned short* s) { int n = 0; while (s[n]) n++; return n; }
struct IMediaEventSink { int refs; IMediaEventSink() : refs(1) {} virtual unsigned int Release() { return --refs; } };
struct IFilterGraph {
    IMediaEventSink sink; int fail; int qi_calls;
    IFilterGraph() : fail(0), qi_calls(0) {}
    virtual HRESULT QueryInterface(const GUID& riid, void** ppv)
    {
        qi_calls++;
        if (fail || !(riid == IID_IMediaEventSink)) return (HRESULT)0x80004002u;   // leaves *ppv alone
        sink.refs++; *ppv = &sink; return S_OK;
    }
};
typedef unsigned short WCHAR;
typedef unsigned int DWORD;
struct CBaseFilter {
    CCritSec* m_pLock; WCHAR* m_pName; IFilterGraph* m_pGraph; IMediaEventSink* m_pSink;
    HRESULT JoinFilterGraph(IFilterGraph* pGraph, const unsigned short* pName);
};
HRESULT CBaseFilter::JoinFilterGraph(IFilterGraph* pGraph, const unsigned short* pName)
{
    CAutoLock cObjectLock(m_pLock);
    m_pGraph = pGraph;
    if (m_pGraph) {
        HRESULT hr = m_pGraph->QueryInterface(IID_IMediaEventSink, (void**) &m_pSink);
        if (FAILED(hr)) {
        }
        else m_pSink->Release();
    } else {
        m_pSink = 0;
    }
    if (m_pName) {
        delete[] m_pName;
        m_pName = 0;
    }
    if (pName) {
        DWORD nameLen = lstrlenW(pName)+1;
        m_pName = new WCHAR[nameLen];
        if (m_pName) {
            std::memcpy(m_pName, pName, nameLen*sizeof(WCHAR));
        }
    }
    return NOERROR;
}
int main()
{
    CCritSec lock; IFilterGraph g;
    CBaseFilter f; f.m_pLock = &lock; f.m_pName = 0; f.m_pGraph = 0; f.m_pSink = 0;
    static const unsigned short kName[] = { 'M', 'o', 'v', 'i', 'e', 0 };
    static const unsigned short kName2[] = { 'X', 0 };
    int ok = 1;
    ok &= f.JoinFilterGraph(&g, kName) == NOERROR && f.m_pGraph == &g && f.m_pSink == &g.sink && g.sink.refs == 1;
    ok &= f.m_pName != 0 && f.m_pName != kName && lstrlenW(f.m_pName) == 5 && f.m_pName[0] == 'M' && f.m_pName[4] == 'e';
    ok &= lock.entered == 1 && lock.depth == 0;
    ok &= f.JoinFilterGraph(&g, kName2) == NOERROR && f.m_pName[0] == 'X' && f.m_pName[1] == 0 && g.qi_calls == 2;
    ok &= f.JoinFilterGraph(0, 0) == NOERROR && f.m_pGraph == 0 && f.m_pSink == 0 && f.m_pName == 0 && g.qi_calls == 2;
    g.fail = 1; f.m_pSink = 0;
    ok &= f.JoinFilterGraph(&g, 0) == NOERROR && f.m_pGraph == &g && f.m_pSink == 0 && g.sink.refs == 1;
    ok &= lock.entered == 4 && lock.depth == 0;
    if (!ok) { std::printf("00ca7850_TEST FAIL\n"); return 1; }
    std::printf("00ca7850_TEST PASS\n");
    return 0;
}