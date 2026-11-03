// Behaviour test for CDeferredCommand::CDeferredCommand 0x00CAF230 (standalone copy of the model and logic).
#include <cstdio>
typedef int HRESULT;   // 32-bit on both hosts (long is 64-bit under x64 g++)
typedef double REFTIME;
typedef long long REFERENCE_TIME;
struct GUID { unsigned int d[4]; };
struct VARIANT { unsigned short vt; unsigned short r[3]; unsigned int lo, hi; };
static HRESULT g_qi = 0, g_ti = 0, g_ins = 0;
static int g_released = 0, g_inserted = 0;
struct IUnknown {
    virtual HRESULT QueryInterface(const GUID&, void** ppv) { *ppv = this; return g_qi; }
    virtual unsigned int AddRef() { return 1; }
    virtual unsigned int Release() { ++g_released; return 0; }
};
struct ITypeInfo : IUnknown {};
static ITypeInfo g_tinfo;
struct CBaseDispatch {
    HRESULT GetTypeInfo(const GUID&, unsigned int, unsigned int, ITypeInfo** p) { *p = &g_tinfo; return g_ti; }
};
struct CDispParams {
    VARIANT* rgvarg; long* rgdispidNamedArgs; unsigned int cArgs; unsigned int cNamedArgs;
    CDispParams(unsigned int n, VARIANT* a, HRESULT*) : rgvarg(a), rgdispidNamedArgs(0), cArgs(n), cNamedArgs(0) {}
};
struct CDeferredCommand;
struct CCmdQueue { HRESULT Insert(CDeferredCommand*) { ++g_inserted; return g_ins; } };
struct CDeferredCommand {
    CCmdQueue* m_pQueue; IUnknown* m_pUnk; REFERENCE_TIME m_time; GUID* m_iid; long m_dispidMethod; short m_wFlags;
    CDispParams m_DispParams; VARIANT* m_pvarResult; int m_bStream; long m_DispId; CBaseDispatch m_Dispatch; HRESULT m_hrResult;
    CDeferredCommand(CCmdQueue* pQ, IUnknown*, HRESULT* phr, IUnknown* pUnkExecutor, REFTIME time, GUID* iid,
                     long dispidMethod, short wFlags, long nArgs, VARIANT* pDispParams, VARIANT* pvarResult,
                     short*, int bStream)
        : m_pQueue(pQ), m_pUnk(pUnkExecutor), m_iid(iid), m_dispidMethod(dispidMethod), m_wFlags(wFlags),
          m_DispParams(nArgs, pDispParams, phr), m_pvarResult(pvarResult), m_bStream(bStream), m_DispId(0),
          m_hrResult((HRESULT)0x80004004L)
    {
        m_time = (REFERENCE_TIME)(time * 10000000);
        IUnknown* pInterface;
        HRESULT hr = m_pUnk->QueryInterface(*m_iid, (void**)&pInterface);
        if (hr < 0) { *phr = hr; return; }
        pInterface->Release();
        ITypeInfo* pti;
        hr = m_Dispatch.GetTypeInfo(*iid, 0, 0, &pti);
        if (hr < 0) { *phr = hr; return; }
        pti->Release();
        if (wFlags == 4) { m_DispParams.cNamedArgs = 1; m_DispId = -3; m_DispParams.rgdispidNamedArgs = &m_DispId; }
        hr = pQ->Insert(this);
        if (hr < 0) *phr = hr;
    }
};
int main()
{
    int ok = 1; CCmdQueue q; IUnknown exec; GUID iid = {{1,2,3,4}}; VARIANT args[2]; HRESULT hr = 0;
    { CDeferredCommand c(&q, 0, &hr, &exec, 1.5, &iid, 7, 4, 2, args, 0, 0, 1);
      ok &= hr == 0 && g_inserted == 1 && g_released == 2 && c.m_time == 15000000 && c.m_hrResult == (HRESULT)0x80004004L;
      ok &= c.m_DispParams.cNamedArgs == 1 && c.m_DispId == -3 && c.m_DispParams.rgdispidNamedArgs == &c.m_DispId && c.m_DispParams.cArgs == 2; }
    { g_qi = (HRESULT)0x80004002L; hr = 0; CDeferredCommand c(&q, 0, &hr, &exec, 0.0, &iid, 7, 1, 0, 0, 0, 0, 0);
      ok &= hr == (HRESULT)0x80004002L && g_inserted == 1 && c.m_DispParams.cNamedArgs == 0; g_qi = 0; }
    { g_ti = (HRESULT)0x80029C4AL; hr = 0; CDeferredCommand c(&q, 0, &hr, &exec, 0.0, &iid, 7, 1, 0, 0, 0, 0, 0);
      ok &= hr == (HRESULT)0x80029C4AL && g_inserted == 1; g_ti = 0; }
    { g_ins = (HRESULT)0x8000FFFFL; hr = 0; CDeferredCommand c(&q, 0, &hr, &exec, 0.0, &iid, 7, 2, 0, 0, 0, 0, 0);
      ok &= hr == (HRESULT)0x8000FFFFL && g_inserted == 2 && c.m_DispId == 0; g_ins = 0; }
    printf(ok ? "012171f3_TEST PASS\n" : "00caf230_TEST FAIL\n");
    return ok ? 0 : 1;
}