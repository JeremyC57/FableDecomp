// CBaseDispatch::GetTypeInfo(const IID&, UINT, LCID, ITypeInfo**) -- DirectShow BaseClasses (ctlutil.cpp): loads the quartz type library on first use (LoadRegTypeLib, else LoadTypeLib "control.tlb" via OleAut32.dll), caches the ITypeInfo for riid and returns it AddRef'd
// Leaf: ?GetTypeInfo@CBaseDispatch
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CACD90, 221 bytes (+3 int3), STDMETHODIMP (`this` on the stack), `ret 14h`.
// Callees: LoadOLEAut32 (0x00CACB20), GetLastError (IAT 0x0143FD58; AmHresultFromWin32 = `or eax, 80070000h`),
// GetProcAddress (IAT 0x0143FE44, held in edi for both lookups), the loaded LoadRegTypeLib / LoadTypeLib pointers,
// ITypeLib::GetTypeInfoOfGuid (slot 6) / Release, ITypeInfo::AddRef.
// Data: LIBID_QuartzTypeLib 0x012AA0B4 {56A868B0-0AD4-11CE-B03A-0020AF0BA770}; static szRegTypeLib "LoadRegTypeLib" 0x012BF894,
// szTypeLib "LoadTypeLib" 0x012BF888, szControl L"control.tlb" 0x012BF870. TYPE_E_ELEMENTNOTFOUND 0x8002802B, E_POINTER 0x80004003.
// Shape: the second `if (NULL == pptinfo)` test folds away after `*pptinfo = NULL`. Manifest label "COM_LoadTypeLibrary" is descriptive.
// ---- DirectShow BaseClasses model (strmbase, DX9 SDK era, UNICODE build; combase.h / wxutil.h) ----
typedef long HRESULT;
typedef unsigned long ULONG;
typedef unsigned long DWORD;
typedef int BOOL;
typedef long LONG;
typedef void* HANDLE;
typedef unsigned short WCHAR;
typedef unsigned char BYTE;
typedef __int64 LONGLONG;
typedef LONGLONG REFERENCE_TIME;
typedef struct _GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; } GUID;   // guiddef.h
typedef GUID IID;
typedef GUID CLSID;
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int n);
#pragma intrinsic(memcmp)
inline int operator==(const GUID& a, const GUID& b) { return !memcmp(&a, &b, sizeof(GUID)); }   // guiddef.h IsEqualGUID
inline int operator!=(const GUID& a, const GUID& b) { return !(a == b); }
struct __declspec(novtable) IUnknown {
    virtual HRESULT __stdcall QueryInterface(const IID& riid, void** ppv) = 0;   // 0
    virtual ULONG __stdcall AddRef() = 0;                                        // 1
    virtual ULONG __stdcall Release() = 0;                                       // 2
};
struct __declspec(novtable) INonDelegatingUnknown {
    virtual HRESULT __stdcall NonDelegatingQueryInterface(const IID& riid, void** ppv) = 0;
    virtual ULONG __stdcall NonDelegatingAddRef() = 0;
    virtual ULONG __stdcall NonDelegatingRelease() = 0;
};
class CBaseObject {                               // release build: only the static live-object count
    CBaseObject(const CBaseObject&);
    void operator=(const CBaseObject&);
    static LONG m_cObjects;
public:
    CBaseObject(const WCHAR* pName);
    CBaseObject(const char* pName);
    ~CBaseObject();
};
class __declspec(novtable) CUnknown : public INonDelegatingUnknown, public CBaseObject {
    IUnknown* const m_pUnknown;                   // +0x04
protected:
    LONG m_cRef;                                  // +0x08
public:
    CUnknown(const WCHAR* pName, IUnknown* pUnk);
    virtual ~CUnknown() {}                        // slot 3
    IUnknown* GetOwner() const { return m_pUnknown; }
    HRESULT __stdcall NonDelegatingQueryInterface(const IID& riid, void** ppv);
    ULONG __stdcall NonDelegatingAddRef();
    ULONG __stdcall NonDelegatingRelease();
};
#define DECLARE_IUNKNOWN \
    HRESULT __stdcall QueryInterface(const IID& riid, void** ppv) { return GetOwner()->QueryInterface(riid, ppv); } \
    ULONG __stdcall AddRef() { return GetOwner()->AddRef(); } \
    ULONG __stdcall Release() { return GetOwner()->Release(); }
extern "C" HRESULT __stdcall GetInterface(IUnknown* pUnk, void** ppv);   // 0x00CACBE0 (STDAPI)
struct CRITICAL_SECTION { void* DebugInfo; LONG LockCount; LONG RecursionCount; HANDLE OwningThread; HANDLE LockSemaphore; ULONG SpinCount; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FE10
class CCritSec {                                  // wxutil.h, release build
    CCritSec(const CCritSec&);
    CCritSec& operator=(const CCritSec&);
    CRITICAL_SECTION m_CritSec;
public:
    CCritSec();
    ~CCritSec();
    void Lock() { EnterCriticalSection(&m_CritSec); }
    void Unlock() { LeaveCriticalSection(&m_CritSec); }
};
class CAutoLock {                                 // wxutil.h
    CAutoLock(const CAutoLock&);
    CAutoLock& operator=(const CAutoLock&);
protected:
    CCritSec* m_pLock;
public:
    CAutoLock(CCritSec* plock) { m_pLock = plock; m_pLock->Lock(); }
    ~CAutoLock() { m_pLock->Unlock(); }
};
class CRefTime {                                  // reftime.h
public:
    REFERENCE_TIME m_time;
    CRefTime() { m_time = 0; }
    CRefTime(REFERENCE_TIME rt) { m_time = rt; }
    operator REFERENCE_TIME() const { return m_time; }
    CRefTime& operator=(const CRefTime& rt) { m_time = rt.m_time; return *this; }
    CRefTime& operator=(const LONGLONG ll) { m_time = ll; return *this; }
    CRefTime& operator+=(const CRefTime& rt) { return (*this = *this + rt); }
};
// ---- oaidl.h / ctlutil.h model: automation types, CBaseDispatch, CDispParams ----
typedef unsigned int UINT;
typedef unsigned short WORD;
typedef DWORD LCID;
typedef WCHAR OLECHAR;
typedef OLECHAR* BSTR;
typedef OLECHAR* LPOLESTR;
typedef long DISPID;
typedef double REFTIME;
struct IDispatch;
enum VARENUM { VT_EMPTY = 0, VT_I4 = 3, VT_BSTR = 8, VT_DISPATCH = 9, VT_UNKNOWN = 13 };
struct VARIANT {                                  // oaidl.h, 16 bytes
    unsigned short vt;                            // +0x00
    WORD wReserved1, wReserved2, wReserved3;
    union {                                       // +0x08
        LONG lVal;
        double dblVal;
        BSTR bstrVal;
        IUnknown* punkVal;
        IDispatch* pdispVal;
    };
};
struct DISPPARAMS {                               // oaidl.h
    VARIANT* rgvarg;                              // +0x00
    DISPID* rgdispidNamedArgs;                    // +0x04
    UINT cArgs;                                   // +0x08
    UINT cNamedArgs;                              // +0x0C
};
struct EXCEPINFO;
struct __declspec(novtable) ITypeInfo : IUnknown {
    virtual HRESULT __stdcall GetTypeAttr(void** ppTypeAttr) = 0;                               // 3
};
struct __declspec(novtable) ITypeLib : IUnknown {
    virtual UINT __stdcall GetTypeInfoCount() = 0;                                              // 3
    virtual HRESULT __stdcall GetTypeInfo(UINT index, ITypeInfo** ppTInfo) = 0;                 // 4
    virtual HRESULT __stdcall GetTypeInfoType(UINT index, int* pTKind) = 0;                     // 5
    virtual HRESULT __stdcall GetTypeInfoOfGuid(const GUID& guid, ITypeInfo** ppTinfo) = 0;     // 6 (+0x18)
};
struct __declspec(novtable) IDispatch : IUnknown {
    virtual HRESULT __stdcall GetTypeInfoCount(UINT* pctinfo) = 0;                              // 3
    virtual HRESULT __stdcall GetTypeInfo(UINT itinfo, LCID lcid, ITypeInfo** pptinfo) = 0;    // 4
    virtual HRESULT __stdcall GetIDsOfNames(const IID& riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid) = 0;   // 5
    virtual HRESULT __stdcall Invoke(DISPID dispidMember, const IID& riid, LCID lcid, WORD wFlags, DISPPARAMS* pdispparams,
                                     VARIANT* pvarResult, EXCEPINFO* pexcepinfo, UINT* puArgErr) = 0;   // 6
};
class CBaseDispatch {                             // ctlutil.h
    ITypeInfo* m_pti;                             // +0x00
public:
    CBaseDispatch() : m_pti(0) {}
    ~CBaseDispatch();
    HRESULT __stdcall GetTypeInfoCount(UINT* pctinfo);
    HRESULT __stdcall GetTypeInfo(const IID& riid, UINT itinfo, LCID lcid, ITypeInfo** pptinfo);
    HRESULT __stdcall GetIDsOfNames(const IID& riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid);
};
class CDispParams : public DISPPARAMS {           // ctlutil.h
public:
    CDispParams(UINT nArgs, VARIANT* pArgs, HRESULT* phr = 0);
    ~CDispParams();
};
struct ITypeLib;
struct HINSTANCE__ { int unused; };
typedef HINSTANCE__* HINSTANCE;
typedef int (__stdcall* FARPROC)();
HINSTANCE LoadOLEAut32();                                                                  // 0x00CACB20 (combase.cpp)
extern "C" __declspec(dllimport) DWORD __stdcall GetLastError();                           // IAT 0x0143FD58
extern "C" __declspec(dllimport) FARPROC __stdcall GetProcAddress(HINSTANCE hModule, const char* lpProcName);   // IAT 0x0143FE44
extern "C" const GUID LIBID_QuartzTypeLib;                                                 // 0x012AA0B4

typedef HRESULT (__stdcall *LPLOADTYPELIB)(
                            const OLECHAR *szFile,
                            ITypeLib ** pptlib);

typedef HRESULT (__stdcall *LPLOADREGTYPELIB)(const GUID& rguid,
                            WORD wVerMajor,
                            WORD wVerMinor,
                            LCID lcid,
                            ITypeLib ** pptlib);

// attempt to find our type library

HRESULT __stdcall
CBaseDispatch::GetTypeInfo(
  const IID& riid,
  UINT itinfo,
  LCID lcid,
  ITypeInfo ** pptinfo)
{
    if (pptinfo == 0) return (HRESULT)0x80004003L;   // CheckPointer(pptinfo, E_POINTER)
    HRESULT hr;

    *pptinfo = 0;

    // we only support one type element
    if (0 != itinfo) {
        return (HRESULT)0x8002802BL;   // TYPE_E_ELEMENTNOTFOUND
    }

    if (0 == pptinfo) {
        return (HRESULT)0x80004003L;   // E_POINTER
    }

    // always look for neutral
    if (0 == m_pti) {

        LPLOADTYPELIB       lpfnLoadTypeLib;
        LPLOADREGTYPELIB    lpfnLoadRegTypeLib;
        ITypeLib            *ptlib;
        HINSTANCE           hInst;

        static const char  szTypeLib[]    = "LoadTypeLib";
        static const char  szRegTypeLib[] = "LoadRegTypeLib";
        static const WCHAR szControl[]    = L"control.tlb";

        //
        // Try to get the Ole32Aut.dll module handle.
        //

        hInst = LoadOLEAut32();
        if (hInst == 0) {
            DWORD dwError = GetLastError();
            return (HRESULT)(0x80070000UL | dwError);   // AmHresultFromWin32
        }
        lpfnLoadRegTypeLib = (LPLOADREGTYPELIB)GetProcAddress(hInst,
                                                              szRegTypeLib);
        if (lpfnLoadRegTypeLib == 0) {
            DWORD dwError = GetLastError();
            return (HRESULT)(0x80070000UL | dwError);   // AmHresultFromWin32
        }

        hr = (*lpfnLoadRegTypeLib)(LIBID_QuartzTypeLib, 1, 0, // version 1.0
                                   lcid, &ptlib);

        if (hr < 0) {

            // attempt to load directly - this will fill the
            // registry in if it finds it

            lpfnLoadTypeLib = (LPLOADTYPELIB)GetProcAddress(hInst, szTypeLib);
            if (lpfnLoadTypeLib == 0) {
                DWORD dwError = GetLastError();
                return (HRESULT)(0x80070000UL | dwError);   // AmHresultFromWin32
            }

            hr = (*lpfnLoadTypeLib)(szControl, &ptlib);
            if (hr < 0) {
                return hr;
            }
        }

        hr = ptlib->GetTypeInfoOfGuid(
                    riid,
                    &m_pti);

        ptlib->Release();

        if (hr < 0) {
            return hr;
        }
    }

    *pptinfo = m_pti;
    m_pti->AddRef();
    return 0;
}