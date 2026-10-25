// CDispParams::~CDispParams() -- DirectShow BaseClasses (ctlutil.cpp): frees the copied deferred-command arguments (BSTR blocks by their length prefix, Release for IUnknown/IDispatch) and then the VARIANT array
// Leaf: ??1CDispParams
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CAEBB0, 104 bytes, thiscall, plain `ret`.
// Callees: operator delete (0x00BFE9BC) for each BSTR block (bstrVal - 1 OLECHAR) and for rgvarg; IUnknown::Release (slot 2).
// Layout: DISPPARAMS rgvarg +0x00, cArgs +0x08; VARIANT 16 bytes (vt word +0, union +8). VT_BSTR 8, VT_DISPATCH 9, VT_UNKNOWN 13.
// Manifest label "Unwind_Cleanup" is a wrong propagation.
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
void __cdecl operator delete(void* p);   // 0x00BFE9BC
// No operator delete[] declaration is visible (strmbase never includes <new>), so VC7.1 binds `delete[] rgvarg` to
// the scalar operator delete, exactly as retail calls 0x00BFE9BC for both frees (the ctor's new[] likewise calls 0x00BFEA1A).

CDispParams::~CDispParams()
{
    for (UINT i = 0; i < cArgs; i++) {
        switch(rgvarg[i].vt) {
        case VT_BSTR:
            if ((void*)rgvarg[i].bstrVal != 0) {
                OLECHAR * pch = rgvarg[i].bstrVal - (sizeof(WORD)/sizeof(OLECHAR));
                delete pch;
            }
            break;

        case VT_UNKNOWN:
            rgvarg[i].punkVal->Release();
            break;

        case VT_DISPATCH:
            rgvarg[i].pdispVal->Release();
            break;
        }
    }
    delete[] rgvarg;
}