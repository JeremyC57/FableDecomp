// CPosPassThru::QueryInterface(REFIID riid, void** ppv) -- DirectShow BaseClasses DECLARE_IUNKNOWN: delegate IUnknown::QueryInterface to the owning (outer) unknown, GetOwner()->QueryInterface(riid, ppv)
// Leaf: ?QueryInterface@CPosPassThru@@UAGJABU_GUID@@PAPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CAFC40, 15 bytes (+1 int3), STDMETHODIMP (__stdcall) on the IMediaSeeking sub-object (+0x00),
//   so m_pUnknown is [this+0x0C] (CMediaPosition at +4, its CUnknown at +8: m_pUnknown +4 within CUnknown); the call is a tail
//   `jmp [ecx]` with the owner written over the `this` slot.
// Identity (proven): slot 0 of the two IMediaSeeking vtables 0x012BF940 (CPosPassThru, stored by the ctor 0x00CAFBF0 and by
//   CRendererPosPassThru's ctor 0x00CAFCE0 before its own table) and 0x012BF9F0 (CRendererPosPassThru, which inherits it); slots
//   3.. of both are the landed CPosPassThru members (GetCapabilities 0x00CAD4C0, CheckCapabilities 0x00CAD500, GetCurrentPosition
//   0x00CAF090, ...); its neighbours 0x00CAFC50 / 0x00CAFC60 are AddRef / Release (`jmp [ecx+4]` / `jmp [ecx+8]`), and the
//   adjustor thunk 0x00CAFC80 (`sub [esp+4],4 ; jmp 0x00CAFC40`) sits in the IMediaPosition tables 0x012BF8F8 / 0x012BF9A8 (the
//   CMediaPosition sub-object at +4). The ctor shows the layout: CUnknown(pName, pUnk) on +8, CBaseDispatch::m_pti +0x14 = 0,
//   m_pPin +0x18. strmbase names are in neither PDB: the name is the DirectShow BaseClasses source (ctlutil.h CPosPassThru :
//   IMediaSeeking, CMediaPosition with DECLARE_IUNKNOWN). The catalog row "_gapscan::sub_00cafc40" carried no name.
// Not a toolchain-library object: DirectShow BaseClasses are compiled into the exe from the DX9 SDK source (GOTCHAS chunk 594,
//   chunk 498; Rich header: every compiled object is cl 3077) and there is no vendor strmbase.lib to prove with lib_verify.py.
// Layout: CBaseObject here is DirectShow combase.h's (release build: only the static object count), not engine/CBaseObject.h
//   (Lionhead's unrelated class of the same name); vland's SHARED_HEADER_AVAILABLE CBaseObject warning is a false positive.
// Model: the landed CPosPassThru::CheckCapabilities 0x00CAD500 classes (strmbase combase.h / ctlutil.h, UNICODE) plus GetOwner().
// Shape: DECLARE_IUNKNOWN defines QueryInterface inline in the class; written out of the class here so this TU emits the body
//   (same symbol, same code), as the landed CDynamicOutputPin::AddRef 0x00CA9700.
typedef long HRESULT;
typedef unsigned long ULONG;
typedef unsigned long DWORD;
typedef long LONG;
typedef unsigned short WCHAR;
typedef __int64 LONGLONG;
typedef struct _GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; } GUID;   // guiddef.h
typedef GUID IID;
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
    static LONG m_cObjects;
public:
    CBaseObject(const WCHAR* pName);
    ~CBaseObject();
};
class __declspec(novtable) CUnknown : public INonDelegatingUnknown, public CBaseObject {
    IUnknown* const m_pUnknown;                   // +0x04
protected:
    LONG m_cRef;                                  // +0x08
public:
    CUnknown(const WCHAR* pName, IUnknown* pUnk);
    virtual ~CUnknown() {}
    IUnknown* GetOwner() const { return m_pUnknown; }
};
struct __declspec(novtable) IPin : IUnknown {
    virtual void v3() = 0; virtual void v4() = 0; virtual void v5() = 0;       // Connect, ReceiveConnection, Disconnect
    virtual HRESULT __stdcall ConnectedTo(IPin** pPin) = 0;                     // 6 (+0x18)
};
struct __declspec(novtable) IDispatch : IUnknown {
    virtual HRESULT __stdcall GetTypeInfoCount(unsigned int* pctinfo) = 0;      // 3
    virtual void v4() = 0; virtual void v5() = 0; virtual void v6() = 0;        // GetTypeInfo, GetIDsOfNames, Invoke
};
struct __declspec(novtable) IMediaPosition : IDispatch { };                     // slots 7..17 not used here
struct __declspec(novtable) IMediaSeeking : IUnknown {
    virtual HRESULT __stdcall GetCapabilities(DWORD* pCapabilities) = 0;        // 3
    virtual HRESULT __stdcall CheckCapabilities(DWORD* pCapabilities) = 0;      // 4
    virtual HRESULT __stdcall IsFormatSupported(const GUID* pFormat) = 0;       // 5
    virtual HRESULT __stdcall QueryPreferredFormat(GUID* pFormat) = 0;          // 6
    virtual HRESULT __stdcall GetTimeFormat(GUID* pFormat) = 0;                 // 7
    virtual HRESULT __stdcall IsUsingTimeFormat(const GUID* pFormat) = 0;       // 8
    virtual HRESULT __stdcall SetTimeFormat(const GUID* pFormat) = 0;           // 9
    virtual HRESULT __stdcall GetDuration(LONGLONG* pDuration) = 0;             // 10
    virtual HRESULT __stdcall GetStopPosition(LONGLONG* pStop) = 0;             // 11
    virtual HRESULT __stdcall GetCurrentPosition(LONGLONG* pCurrent) = 0;       // 12
    virtual HRESULT __stdcall ConvertTimeFormat(LONGLONG* pTarget, const GUID* pTargetFormat, LONGLONG Source, const GUID* pSourceFormat) = 0;   // 13
    virtual HRESULT __stdcall SetPositions(LONGLONG* pCurrent, DWORD dwCurrentFlags, LONGLONG* pStop, DWORD dwStopFlags) = 0;   // 14
    virtual HRESULT __stdcall GetPositions(LONGLONG* pCurrent, LONGLONG* pStop) = 0;   // 15
    virtual HRESULT __stdcall GetAvailable(LONGLONG* pEarliest, LONGLONG* pLatest) = 0;   // 16
    virtual HRESULT __stdcall SetRate(double dRate) = 0;                        // 17
    virtual HRESULT __stdcall GetRate(double* pdRate) = 0;                      // 18
    virtual HRESULT __stdcall GetPreroll(LONGLONG* pllPreroll) = 0;             // 19
};
class CBaseDispatch {                             // ctlutil.h
    void* m_pti;                                  // ITypeInfo*
public:
    ~CBaseDispatch();
};
class __declspec(novtable) CMediaPosition : public IMediaPosition, public CUnknown {   // CUnknown +0x04
    CBaseDispatch m_basedisp;                     // +0x10
public:
    CMediaPosition(const WCHAR* pName, IUnknown* pUnk);
};
class CPosPassThru : public IMediaSeeking, public CMediaPosition {   // IMediaSeeking +0x00, CMediaPosition +0x04
    IPin* m_pPin;                                 // +0x18
    HRESULT GetPeerSeeking(IMediaSeeking** ppMS); // 0x00CAD460
public:
    HRESULT __stdcall QueryInterface(const IID& riid, void** ppv);   // DECLARE_IUNKNOWN (body out of class, below)
    ULONG __stdcall AddRef();
    ULONG __stdcall Release();
    HRESULT __stdcall GetCapabilities(DWORD* pCapabilities);
    HRESULT __stdcall CheckCapabilities(DWORD* pCapabilities);
    HRESULT __stdcall SetTimeFormat(const GUID* pFormat);
    HRESULT __stdcall GetTimeFormat(GUID* pFormat);
    HRESULT __stdcall IsUsingTimeFormat(const GUID* pFormat);
    HRESULT __stdcall IsFormatSupported(const GUID* pFormat);
    HRESULT __stdcall QueryPreferredFormat(GUID* pFormat);
    HRESULT __stdcall GetRate(double* pdRate);
};

HRESULT __stdcall CPosPassThru::QueryInterface(const IID& riid, void** ppv)
{
    return GetOwner()->QueryInterface(riid, ppv);
}