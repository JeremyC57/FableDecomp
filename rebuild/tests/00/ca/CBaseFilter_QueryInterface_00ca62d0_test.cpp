// Test for CBaseFilter::QueryInterface (0x00CA62D0, DECLARE_IUNKNOWN): the filter forwards the call to its owner
// (CUnknown::GetOwner() = m_pUnknown): the outer object when aggregated, else its own non-delegating IUnknown.
#include <cstdio>
typedef long HRESULT;
struct GUID { unsigned long d; };
static const GUID IID_IUnknown = { 0 }, IID_IBaseFilter = { 0x56a86895 }, IID_Other = { 7 };
struct IUnknownLike { virtual HRESULT QueryInterface(const GUID& riid, void** ppv) = 0; };
struct CUnknown : IUnknownLike {
    IUnknownLike* m_pUnknown;
    IUnknownLike* GetOwner() const { return m_pUnknown; }
};
static int g_outerCalls = 0;
struct COuter : IUnknownLike {
    HRESULT QueryInterface(const GUID&, void** ppv) { ++g_outerCalls; *ppv = this; return 0; }
};
struct CBaseFilter : CUnknown {
    CBaseFilter(IUnknownLike* outer) { m_pUnknown = outer ? outer : static_cast<IUnknownLike*>(this); }
    HRESULT NonDelegating(const GUID& riid, void** ppv)
    {
        if (riid.d == IID_IUnknown.d || riid.d == IID_IBaseFilter.d) { *ppv = this; return 0; }
        *ppv = 0; return (HRESULT)0x80004002L;
    }
    HRESULT QueryInterface(const GUID& riid, void** ppv)
    {
        if (GetOwner() == static_cast<IUnknownLike*>(this)) return NonDelegating(riid, ppv);
        return GetOwner()->QueryInterface(riid, ppv);
    }
};
int main()
{
    COuter outer; CBaseFilter agg(&outer); void* p = 0;
    if (agg.QueryInterface(IID_IBaseFilter, &p) != 0 || p != &outer || g_outerCalls != 1) { std::printf("00ca62d0_TEST FAIL\n"); return 1; }
    CBaseFilter self(0);
    if (self.QueryInterface(IID_IBaseFilter, &p) != 0 || p != &self || g_outerCalls != 1) { std::printf("00ca62d0_TEST FAIL\n"); return 1; }
    if (self.QueryInterface(IID_Other, &p) != (HRESULT)0x80004002L || p != 0) { std::printf("00ca62d0_TEST FAIL\n"); return 1; }
    std::printf("00ca62d0_TEST PASS\n");
    return 0;
}