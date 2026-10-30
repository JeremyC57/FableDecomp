// Behaviour test (standalone; compiled separately from the source). Plain C++98: the COM interfaces are modelled
// as ordinary virtual classes (no calling-convention keywords), the Win32 imports as local fakes.
#include <cstdio>
#include <cstring>
typedef int HRESULT;
typedef unsigned int ULONG;
typedef unsigned int DWORD;
typedef int LONG;
typedef int BOOL;
typedef void* HANDLE;
typedef unsigned short WCHAR;
typedef unsigned char BYTE;
#ifdef _MSC_VER
typedef __int64 LONGLONG;
#else
typedef long long LONGLONG;
#endif
typedef LONGLONG REFERENCE_TIME;
#ifdef _MSC_VER
#define LL(x) x##i64
#else
#define LL(x) x##LL
#endif
struct GUID { unsigned int Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
typedef GUID IID;
inline bool operator==(const GUID& a, const GUID& b) { return std::memcmp(&a, &b, sizeof(GUID)) == 0; }
#define HR(x) ((HRESULT)(x))
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed at line %d\n", __LINE__); } } while (0)
// CBaseDispatch::GetTypeInfo: E_POINTER / TYPE_E_ELEMENTNOTFOUND checks, then on first use load the quartz type
// library (LoadRegTypeLib, falling back to LoadTypeLib "control.tlb") through OleAut32.dll, cache the ITypeInfo for
// riid, and hand it out AddRef'd; Win32 failures map to 0x8007xxxx.
typedef unsigned int UINT; typedef DWORD LCID; typedef WCHAR OLECHAR; typedef unsigned short WORD;
struct HINSTANCE__ { int unused; };
typedef HINSTANCE__* HINSTANCE;
struct IUnknown { int refs, releases; HRESULT hrQI; const IID* qiSeen; IUnknown() : refs(0), releases(0), hrQI(0), qiSeen(0) {} HRESULT QueryInterface(const IID& riid, void** ppv) { qiSeen = &riid; if (hrQI < 0) return hrQI; *ppv = this; return 0; } ULONG AddRef() { return ++refs; } ULONG Release() { return ++releases; } };
struct VARIANT { int v; };
struct DISPPARAMS { VARIANT* rgvarg; int* rgdispidNamedArgs; UINT cArgs; UINT cNamedArgs; };
struct EXCEPINFO { int pad[8]; };
struct ITypeInfo { int refs, releases, invokes; HRESULT hrInvoke; void* inst; long memid; WORD flags; DISPPARAMS* params; VARIANT* result; ITypeInfo() : refs(0), releases(0), invokes(0), hrInvoke(0), inst(0), memid(0), flags(0), params(0), result(0) {}
    ULONG AddRef() { return ++refs; } ULONG Release() { return ++releases; }
    HRESULT Invoke(void* pv, long id, WORD f, DISPPARAMS* p, VARIANT* r, EXCEPINFO* e, UINT* a) { invokes++; inst = pv; memid = id; flags = f; params = p; result = r; *a = 0; e->pad[0] = 0; return hrInvoke; } };
static ITypeInfo g_ti;
struct ITypeLib {
    int releases; HRESULT hrGet; const IID* seen;
    ITypeLib() : releases(0), hrGet(0), seen(0) {}
    HRESULT GetTypeInfoOfGuid(const GUID& g, ITypeInfo** pp) { seen = &g; if (hrGet < 0) return hrGet; *pp = &g_ti; return 0; }
    ULONG Release() { return ++releases; }
};
static ITypeLib g_lib;
static HINSTANCE__ g_ole; static BOOL g_noOle = 0, g_noRegProc = 0, g_noProc = 0;
static HRESULT g_hrReg = 0, g_hrLoad = 0; static int g_regCalls = 0, g_loadCalls = 0;
static WORD g_major = 9, g_minor = 9; static LCID g_lcid = 0; static OLECHAR g_file[16];
static const GUID LIBID_QuartzTypeLib = { 0x56A868B0, 0x0AD4, 0x11CE, { 0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70 } };
static const GUID* g_libid = 0;
static DWORD GetLastError() { return 0x7E; }
static HINSTANCE LoadOLEAut32() { return g_noOle ? 0 : &g_ole; }
typedef HRESULT (*LPLOADTYPELIB)(const OLECHAR* szFile, ITypeLib** pptlib);
typedef HRESULT (*LPLOADREGTYPELIB)(const GUID& rguid, WORD wVerMajor, WORD wVerMinor, LCID lcid, ITypeLib** pptlib);
static HRESULT fakeLoadRegTypeLib(const GUID& g, WORD ma, WORD mi, LCID lcid, ITypeLib** pp)
{ g_regCalls++; g_libid = &g; g_major = ma; g_minor = mi; g_lcid = lcid; if (g_hrReg < 0) return g_hrReg; *pp = &g_lib; return 0; }
static HRESULT fakeLoadTypeLib(const OLECHAR* f, ITypeLib** pp)
{ g_loadCalls++; int i = 0; for (; f[i] && i < 15; i++) g_file[i] = f[i]; g_file[i] = 0; if (g_hrLoad < 0) return g_hrLoad; *pp = &g_lib; return 0; }
typedef void (*FARPROC)();
static FARPROC GetProcAddress(HINSTANCE, const char* name)
{
    if (std::strcmp(name, "LoadRegTypeLib") == 0) return g_noRegProc ? 0 : (FARPROC)fakeLoadRegTypeLib;
    if (std::strcmp(name, "LoadTypeLib") == 0) return g_noProc ? 0 : (FARPROC)fakeLoadTypeLib;
    return 0;
}
class CBaseDispatch {
public:
    ITypeInfo* m_pti;
    CBaseDispatch() : m_pti(0) {}
    HRESULT GetTypeInfo(const IID& riid, UINT itinfo, LCID lcid, ITypeInfo** pptinfo)
    {
        if (pptinfo == 0) return HR(0x80004003);
        HRESULT hr;
        *pptinfo = 0;
        if (0 != itinfo) {
            return HR(0x8002802B);
        }
        if (0 == pptinfo) {
            return HR(0x80004003);
        }
        if (0 == m_pti) {
            LPLOADTYPELIB lpfnLoadTypeLib;
            LPLOADREGTYPELIB lpfnLoadRegTypeLib;
            ITypeLib* ptlib;
            HINSTANCE hInst;
            static const char szTypeLib[] = "LoadTypeLib";
            static const char szRegTypeLib[] = "LoadRegTypeLib";
            static const WCHAR szControl[] = { 'c','o','n','t','r','o','l','.','t','l','b', 0 };
            hInst = LoadOLEAut32();
            if (hInst == 0) {
                DWORD dwError = GetLastError();
                return HR(0x80070000UL | dwError);
            }
            lpfnLoadRegTypeLib = (LPLOADREGTYPELIB)GetProcAddress(hInst, szRegTypeLib);
            if (lpfnLoadRegTypeLib == 0) {
                DWORD dwError = GetLastError();
                return HR(0x80070000UL | dwError);
            }
            hr = (*lpfnLoadRegTypeLib)(LIBID_QuartzTypeLib, 1, 0, lcid, &ptlib);
            if (hr < 0) {
                lpfnLoadTypeLib = (LPLOADTYPELIB)GetProcAddress(hInst, szTypeLib);
                if (lpfnLoadTypeLib == 0) {
                    DWORD dwError = GetLastError();
                    return HR(0x80070000UL | dwError);
                }
                hr = (*lpfnLoadTypeLib)(szControl, &ptlib);
                if (hr < 0) {
                    return hr;
                }
            }
            hr = ptlib->GetTypeInfoOfGuid(riid, &m_pti);
            ptlib->Release();
            if (hr < 0) {
                return hr;
            }
        }
        *pptinfo = m_pti;
        m_pti->AddRef();
        return 0;
    }
};
// ctlutil.cpp CDeferredCommand::Invoke over the members it touches (offsets are the retail ones on x86; here plain members).
class CDeferredCommand;
class CCmdQueue {
public:
    int removes; CDeferredCommand* removed; HRESULT hrRemove;
    CCmdQueue() : removes(0), removed(0), hrRemove(0) {}
    HRESULT Remove(CDeferredCommand* p) { removes++; removed = p; return hrRemove; }
};
class CDeferredCommand {
public:
    CCmdQueue* m_pQueue;
    IUnknown* m_pUnk;
    GUID* m_iid;
    long m_dispidMethod;
    short m_wFlags;
    VARIANT* m_pvarResult;
    DISPPARAMS m_DispParams;
    CBaseDispatch m_Dispatch;
    HRESULT m_hrResult;
    const GUID& GetIID() { return *m_iid; }
    long GetMethod() { return m_dispidMethod; }
    short GetFlags() { return m_wFlags; }
    DISPPARAMS* GetParams() { return &m_DispParams; }
    VARIANT* GetResult() { return m_pvarResult; }
    HRESULT Invoke()
    {
        if (m_pQueue == 0) {
            return HR(0x80040234);
        }
        ITypeInfo* pti;
        HRESULT hr = m_Dispatch.GetTypeInfo(GetIID(), 0, 0, &pti);
        if (hr < 0) {
            return hr;
        }
        IUnknown* pInterface;
        hr = m_pUnk->QueryInterface(GetIID(), (void**) &pInterface);
        if (hr < 0) {
            pti->Release();
            return hr;
        }
        EXCEPINFO expinfo;
        UINT uArgErr;
        m_hrResult = pti->Invoke(pInterface, GetMethod(), GetFlags(), GetParams(), GetResult(), &expinfo, &uArgErr);
        pInterface->Release();
        pti->Release();
        hr = m_pQueue->Remove(this);
        m_pQueue = 0;
        return hr;
    }
};
static void run()
{
    static GUID iid = { 0x56A868B2, 0x0AD4, 0x11CE, { 0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70 } };
    VARIANT res;
    CCmdQueue q; IUnknown target;
    CDeferredCommand c;
    c.m_pQueue = 0; c.m_pUnk = &target; c.m_iid = &iid; c.m_dispidMethod = 0x60020001; c.m_wFlags = 1; c.m_pvarResult = &res;
    c.m_hrResult = 7;
    // cancelled command
    CHECK(c.Invoke() == HR(0x80040234)); CHECK(g_regCalls == 0);
    // type library load fails (no OleAut32): the Win32 error comes back, nothing is invoked
    c.m_pQueue = &q; g_noOle = 1;
    CHECK(c.Invoke() == HR(0x8007007E)); CHECK(g_ti.invokes == 0); CHECK(c.m_pQueue == &q); g_noOle = 0;
    // QI failure releases the type info and keeps the command queued
    target.hrQI = HR(0x80004002);
    CHECK(c.Invoke() == HR(0x80004002)); CHECK(g_regCalls == 1); CHECK(g_lib.seen == &iid); CHECK(target.qiSeen == &iid);
    CHECK(g_ti.refs == 1 && g_ti.releases == 1); CHECK(g_ti.invokes == 0); CHECK(c.m_pQueue == &q); CHECK(q.removes == 0);
    // success: cached type info (no second load), invoke with the stored method, flags, params and result, then dequeue
    target.hrQI = 0; g_ti.hrInvoke = HR(0x80020009); q.hrRemove = 3;
    CHECK(c.Invoke() == 3); CHECK(g_regCalls == 1);
    CHECK(g_ti.invokes == 1 && g_ti.inst == &target && g_ti.memid == 0x60020001 && g_ti.flags == 1);
    CHECK(g_ti.params == &c.m_DispParams && g_ti.result == &res); CHECK(c.m_hrResult == HR(0x80020009));
    CHECK(target.releases == 1); CHECK(g_ti.refs == 2 && g_ti.releases == 2);
    CHECK(q.removes == 1 && q.removed == &c); CHECK(c.m_pQueue == 0);
    // and now it is cancelled
    CHECK(c.Invoke() == HR(0x80040234)); CHECK(g_ti.invokes == 1);
}

int main()
{
    run();
    if (g_fail) { std::printf("00caed50_TEST FAIL\n"); return 1; }
    std::printf("00caed50_TEST PASS\n");
    return 0;
}