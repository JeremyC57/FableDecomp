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
struct ITypeInfo { int refs; ITypeInfo() : refs(0) {} ULONG AddRef() { return ++refs; } };
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
static void run()
{
    static const IID riid = { 0x56A868B1, 0x0AD4, 0x11CE, { 0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70 } };
    ITypeInfo* pti = &g_ti;
    { CBaseDispatch d; CHECK(d.GetTypeInfo(riid, 0, 0, 0) == HR(0x80004003)); }
    { CBaseDispatch d; CHECK(d.GetTypeInfo(riid, 1, 0, &pti) == HR(0x8002802B)); CHECK(pti == 0); }
    { CBaseDispatch d; g_noOle = 1; CHECK(d.GetTypeInfo(riid, 0, 0, &pti) == HR(0x8007007E)); g_noOle = 0; }
    { CBaseDispatch d; g_noRegProc = 1; CHECK(d.GetTypeInfo(riid, 0, 0, &pti) == HR(0x8007007E)); g_noRegProc = 0; }
    { CBaseDispatch d;
      CHECK(d.GetTypeInfo(riid, 0, 0x409, &pti) == 0); CHECK(pti == &g_ti); CHECK(d.m_pti == &g_ti); CHECK(g_ti.refs == 1);
      CHECK(g_regCalls == 1 && g_loadCalls == 0); CHECK(g_major == 1 && g_minor == 0 && g_lcid == 0x409); CHECK(g_libid == &LIBID_QuartzTypeLib);
      CHECK(g_lib.releases == 1); CHECK(g_lib.seen == &riid);
      CHECK(d.GetTypeInfo(riid, 0, 0x409, &pti) == 0); CHECK(g_regCalls == 1); CHECK(g_ti.refs == 2); }
    { CBaseDispatch d; g_hrReg = HR(0x8002801D);
      CHECK(d.GetTypeInfo(riid, 0, 0, &pti) == 0); CHECK(g_loadCalls == 1); CHECK(g_file[0] == 'c' && g_file[7] == '.' && g_file[11] == 0);
      CHECK(g_lib.releases == 2); }
    { CBaseDispatch d; g_hrLoad = HR(0x80029C4A);
      CHECK(d.GetTypeInfo(riid, 0, 0, &pti) == HR(0x80029C4A)); CHECK(d.m_pti == 0); CHECK(g_lib.releases == 2); }
    { CBaseDispatch d; g_noProc = 1; CHECK(d.GetTypeInfo(riid, 0, 0, &pti) == HR(0x8007007E)); g_noProc = 0; }
    { CBaseDispatch d; g_hrReg = 0; g_lib.hrGet = HR(0x8002802B);
      CHECK(d.GetTypeInfo(riid, 0, 0, &pti) == HR(0x8002802B)); CHECK(g_lib.releases == 3); CHECK(pti == 0); }
}

int main()
{
    run();
    if (g_fail) { std::printf("00cacd90_TEST FAIL\n"); return 1; }
    std::printf("00cacd90_TEST PASS\n");
    return 0;
}