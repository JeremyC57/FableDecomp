// Behaviour test for 0x00CA6110 CBaseVideoRenderer::JoinFilterGraph (DirectShow renbase.cpp): when leaving a graph, tell it EC_WINDOW_DESTROYED with our IBaseFilter, then let CBaseFilter record the new graph.
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
#define EC_WINDOW_DESTROYED 0x15
static const GUID IID_IBaseFilter = { 0x56A86895, 0x0AD4, 0x11CE, { 0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70 } };
struct IFilterGraph { int dummy; };
struct IBaseFilter {
    int refs;
    IBaseFilter() : refs(1) {}
    virtual unsigned int Release() { return --refs; }
};
struct CBaseVideoRenderer : IBaseFilter {
    IFilterGraph* m_pGraph;
    int qi_calls; int events; int ev_code; void* ev_p1; int ev_p2; int base_calls; IFilterGraph* base_graph; const unsigned short* base_name;
    CBaseVideoRenderer() : m_pGraph(0), qi_calls(0), events(0), ev_code(0), ev_p1(0), ev_p2(-1), base_calls(0), base_graph(0), base_name(0) {}
    HRESULT QueryInterface(const GUID& riid, void** ppv) { qi_calls++; if (riid == IID_IBaseFilter) { refs++; *ppv = (IBaseFilter*)this; return S_OK; } *ppv = 0; return E_FAIL; }
    HRESULT NotifyEvent(int code, void* p1, int p2) { events++; ev_code = code; ev_p1 = p1; ev_p2 = p2; return S_OK; }
    HRESULT BaseJoinFilterGraph(IFilterGraph* g, const unsigned short* n) { base_calls++; base_graph = g; base_name = n; m_pGraph = g; return 0x66; }
    HRESULT JoinFilterGraph(IFilterGraph* pGraph, const unsigned short* pName);
};
HRESULT CBaseVideoRenderer::JoinFilterGraph(IFilterGraph* pGraph, const unsigned short* pName)
{
    if (!pGraph && m_pGraph) {
        IBaseFilter* pFilter;
        QueryInterface(IID_IBaseFilter, (void**)&pFilter);
        NotifyEvent(EC_WINDOW_DESTROYED, pFilter, 0);
        pFilter->Release();
    }
    return BaseJoinFilterGraph(pGraph, pName);   // CBaseFilter::JoinFilterGraph
}
int main()
{
    CBaseVideoRenderer v; IFilterGraph g;
    static const unsigned short kName[] = { 'V', 0 };
    int ok = 1;
    ok &= v.JoinFilterGraph(&g, kName) == 0x66 && v.events == 0 && v.qi_calls == 0 && v.base_graph == &g && v.base_name == kName;
    ok &= v.JoinFilterGraph(&g, 0) == 0x66 && v.events == 0;      // joining while in a graph: no event
    ok &= v.JoinFilterGraph(0, 0) == 0x66 && v.events == 1 && v.ev_code == EC_WINDOW_DESTROYED && v.ev_p1 == (IBaseFilter*)&v && v.ev_p2 == 0;
    ok &= v.refs == 1 && v.base_calls == 3 && v.m_pGraph == 0;   // the QI reference was released
    ok &= v.JoinFilterGraph(0, 0) == 0x66 && v.events == 1;      // not in a graph: no event
    if (!ok) { std::printf("00ca6110_TEST FAIL\n"); return 1; }
    std::printf("00ca6110_TEST PASS\n");
    return 0;
}