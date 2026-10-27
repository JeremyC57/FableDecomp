// CBaseDispatch::GetIDsOfNames(const IID& riid, OLECHAR** rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid) -- DirectShow BaseClasses (ctlutil.cpp): get the type info for riid through the inlined GetTypeInfo (on first use load OleAut32, LoadRegTypeLib(LIBID_QuartzTypeLib 1.0) else LoadTypeLib(L"control.tlb"), GetTypeInfoOfGuid into m_pti, release the library; a missing module or entry point is the HRESULT of GetLastError), then ask it for the DISPIDs and release it
// Leaf: ?GetIDsOfNames@CBaseDispatch@@QAGJABU_GUID@@PAPAGIKPAJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CACE70, 219 bytes, STDMETHODIMP (non-virtual `__stdcall`, `this` on the stack),
//   `ret 18h`.
// Identity (proven): ctlutil.cpp's CBaseDispatch::GetIDsOfNames with GetTypeInfo(riid, 0, lcid, &pti) inlined (the out-of-line
//   GetTypeInfo is 0x00CACD90, the landed LoadOLEAut32 0x00CACB20 names it as its caller); the six callers 0x00CAD013,
//   0x00CAD183, 0x00CAD2F3, 0x00CAE6B3, 0x00CAE7E3, 0x00CAE923 are the CMediaControl / CMediaEvent / CMediaPosition ...
//   IDispatch::GetIDsOfNames wrappers that tail-jump to m_basedisp.GetIDsOfNames(IID_I..., ...). `this` is the CBaseDispatch
//   whose only member is m_pti (+0). The constant-true CheckPointer(&pti) stays as `lea eax,[esp+4] ; test`.
// Label: the manifest's "sub_00cace70" is a gap-scan placeholder.
// Callees: LoadOLEAut32 0x00CACB20; GetProcAddress (IAT 0x0143FE44), GetLastError (IAT 0x0143FD58); ITypeLib::GetTypeInfoOfGuid
//   (slot 6), ITypeLib::Release, ITypeInfo::AddRef / GetIDsOfNames (slot 10) / Release.
// Data: "LoadRegTypeLib" 0x012BF894, "LoadTypeLib" 0x012BF888, L"control.tlb" 0x012BF870, LIBID_QuartzTypeLib 0x012AA0B4.
typedef long HRESULT;
typedef unsigned long ULONG;
typedef unsigned long DWORD;
typedef long LONG;
typedef unsigned int UINT;
typedef unsigned short WORD;
typedef unsigned short WCHAR;
typedef WCHAR OLECHAR;
typedef DWORD LCID;
typedef LONG DISPID;
typedef struct _GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; } GUID;   // guiddef.h
typedef GUID IID;
struct __declspec(novtable) IUnknown {
    virtual HRESULT __stdcall QueryInterface(const IID& riid, void** ppv) = 0;   // 0
    virtual ULONG __stdcall AddRef() = 0;                                        // 1
    virtual ULONG __stdcall Release() = 0;                                       // 2
};
struct ITypeComp;
struct __declspec(novtable) ITypeInfo : IUnknown {
    virtual HRESULT __stdcall GetTypeAttr(void** ppTypeAttr) = 0;                                      // 3
    virtual HRESULT __stdcall GetTypeComp(ITypeComp** ppTComp) = 0;                                    // 4
    virtual HRESULT __stdcall GetFuncDesc(UINT index, void** ppFuncDesc) = 0;                           // 5
    virtual HRESULT __stdcall GetVarDesc(UINT index, void** ppVarDesc) = 0;                             // 6
    virtual HRESULT __stdcall GetNames(DISPID memid, void* rgBstrNames, UINT cMaxNames, UINT* pcNames) = 0;   // 7
    virtual HRESULT __stdcall GetRefTypeOfImplType(UINT index, DWORD* pRefType) = 0;                    // 8
    virtual HRESULT __stdcall GetImplTypeFlags(UINT index, int* pImplTypeFlags) = 0;                    // 9
    virtual HRESULT __stdcall GetIDsOfNames(OLECHAR** rgszNames, UINT cNames, DISPID* pMemId) = 0;     // 10 (+0x28)
};
struct __declspec(novtable) ITypeLib : IUnknown {
    virtual UINT __stdcall GetTypeInfoCount() = 0;                                                     // 3
    virtual HRESULT __stdcall GetTypeInfo(UINT index, ITypeInfo** ppTInfo) = 0;                        // 4
    virtual HRESULT __stdcall GetTypeInfoType(UINT index, int* pTKind) = 0;                             // 5
    virtual HRESULT __stdcall GetTypeInfoOfGuid(const GUID& guid, ITypeInfo** ppTinfo) = 0;            // 6 (+0x18)
};
typedef struct HINSTANCE__* HINSTANCE;
typedef int (__stdcall* FARPROC)();
extern "C" __declspec(dllimport) FARPROC __stdcall GetProcAddress(HINSTANCE hModule, const char* lpProcName);   // IAT 0x0143FE44
extern "C" __declspec(dllimport) DWORD __stdcall GetLastError();                                                 // IAT 0x0143FD58
HINSTANCE LoadOLEAut32();                                                                                        // 0x00CACB20
typedef HRESULT (__stdcall* LPLOADTYPELIB)(const OLECHAR* szFile, ITypeLib** pptlib);
typedef HRESULT (__stdcall* LPLOADREGTYPELIB)(const GUID& rguid, WORD wVerMajor, WORD wVerMinor, LCID lcid, ITypeLib** pptlib);
extern "C" const IID LIBID_QuartzTypeLib;                                                                        // 0x012AA0B4

inline HRESULT AmHresultFromWin32(DWORD dwWin32Error)   // wxutil.h
{
    return (HRESULT)(dwWin32Error | 0x80070000);
}

class CBaseDispatch {                             // ctlutil.h
    ITypeInfo* m_pti;                             // +0x00
public:
    CBaseDispatch() : m_pti(0) {}
    ~CBaseDispatch();
    HRESULT __stdcall GetTypeInfoCount(UINT* pctinfo);
    HRESULT __stdcall GetTypeInfo(const IID& riid, UINT itinfo, LCID lcid, ITypeInfo** pptinfo);
    HRESULT __stdcall GetIDsOfNames(const IID& riid, OLECHAR** rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid);
};

HRESULT __stdcall CBaseDispatch::GetTypeInfo(const IID& riid, UINT itinfo, LCID lcid, ITypeInfo** pptinfo)
{
    if (pptinfo == 0) return (HRESULT)0x80004003L;   // CheckPointer(pptinfo, E_POINTER), as the landed GetTypeInfo 0x00CACD90 (keeps its static-local scope ?N@)
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

        LPLOADTYPELIB lpfnLoadTypeLib;
        LPLOADREGTYPELIB lpfnLoadRegTypeLib;
        ITypeLib* ptlib;
        HINSTANCE hInst;

        static const char szTypeLib[] = "LoadTypeLib";
        static const char szRegTypeLib[] = "LoadRegTypeLib";
        static const WCHAR szControl[] = { 'c','o','n','t','r','o','l','.','t','l','b', 0 };

        // Try to get the Ole32Aut.dll module handle.
        hInst = LoadOLEAut32();
        if (hInst == 0) {
            DWORD dwError = GetLastError();
            return AmHresultFromWin32(dwError);
        }
        lpfnLoadRegTypeLib = (LPLOADREGTYPELIB)GetProcAddress(hInst, szRegTypeLib);
        if (lpfnLoadRegTypeLib == 0) {
            DWORD dwError = GetLastError();
            return AmHresultFromWin32(dwError);
        }

        hr = (*lpfnLoadRegTypeLib)(LIBID_QuartzTypeLib, 1, 0,   // version 1.0
                                   lcid, &ptlib);

        if (hr < 0) {

            // attempt to load directly - this will fill the
            // registry in if it finds it
            lpfnLoadTypeLib = (LPLOADTYPELIB)GetProcAddress(hInst, szTypeLib);
            if (lpfnLoadTypeLib == 0) {
                DWORD dwError = GetLastError();
                return AmHresultFromWin32(dwError);
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

HRESULT __stdcall CBaseDispatch::GetIDsOfNames(const IID& riid, OLECHAR** rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid)
{
    // although the IDispatch riid is dead, we use this to pass from
    // the interface implementation class to us the iid we are talking about.

    ITypeInfo* pti;
    HRESULT hr = GetTypeInfo(riid, 0, lcid, &pti);

    if (hr >= 0) {
        hr = pti->GetIDsOfNames(rgszNames, cNames, rgdispid);

        pti->Release();
    }
    return hr;
}