// CPosPassThru::IsUsingTimeFormat(const GUID*) -- DirectShow BaseClasses (ctlutil.cpp): forwards IMediaSeeking::IsUsingTimeFormat to the upstream peer's IMediaSeeking and releases it
// Leaf: ?IsUsingTimeFormat@CPosPassThru@@UAGJPBU_GUID@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CAD640, 50 bytes (+ int3 padding), STDMETHODIMP (__stdcall), `ret 8`.
// Identity: CPosPassThru::IsUsingTimeFormat(const GUID*) -- proven by the DirectShow BaseClasses source (strmbase names are in neither
//   PDB); evidence: no direct callers, it sits in IMediaSeeking slot 8 of both pass-through vtables
//   0x012BF940 and 0x012BF9F0 (CPosPassThru / CRendererPosPassThru: QueryInterface..Release, GetCapabilities 0x00CAD4C0 ..
//   GetCurrentPosition = the landed CPosPassThru::GetCurrentPosition 0x00CAF090 .. GetPreroll), calls the landed
//   CPosPassThru::GetPeerSeeking 0x00CAD460 (IID_IMediaSeeking 0x012A9A04) and the peer's slot 8 (+0x20). Family G049
//   (8 IMediaSeeking pass-throughs, catalogued only as "_gapscan::sub_<addr>").
// Shape: the pMS out-variable is homed in the dead `this` argument slot and then kept in esi across the forwarded call;
//   that needs GetPeerSeeking's body in the TU as a same-TU noinline stand-in (the landed 0x00CAD460 source, same symbol),
//   as for the landed CPosPassThru::GetCurrentPosition 0x00CAF090; declaration-only reloads pMS from the stack (52 v 50 bytes).
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
    HRESULT __stdcall GetCapabilities(DWORD* pCapabilities);
    HRESULT __stdcall CheckCapabilities(DWORD* pCapabilities);
    HRESULT __stdcall SetTimeFormat(const GUID* pFormat);
    HRESULT __stdcall GetTimeFormat(GUID* pFormat);
    HRESULT __stdcall IsUsingTimeFormat(const GUID* pFormat);
    HRESULT __stdcall IsFormatSupported(const GUID* pFormat);
    HRESULT __stdcall QueryPreferredFormat(GUID* pFormat);
    HRESULT __stdcall GetRate(double* pdRate);
};
extern "C" const IID IID_IMediaSeeking;          // 0x012A9A04

// Return the IMediaSeeking interface from our peer

__declspec(noinline) HRESULT   // same-TU stand-in of 0x00CAD460 (retail ctlutil.cpp holds both bodies)
CPosPassThru::GetPeerSeeking(IMediaSeeking** ppMS)
{
    *ppMS = 0;

    IPin* pConnected;
    HRESULT hr = m_pPin->ConnectedTo(&pConnected);
    if (hr < 0) {
        return (HRESULT)0x80004001L;   // E_NOTIMPL
    }
    IMediaSeeking* pMS;
    hr = pConnected->QueryInterface(IID_IMediaSeeking, (void**)&pMS);
    pConnected->Release();
    if (hr < 0) {
        return (HRESULT)0x80004001L;   // E_NOTIMPL
    }

    *ppMS = pMS;
    return 0;
}

HRESULT __stdcall
CPosPassThru::IsUsingTimeFormat(const GUID* pFormat)
{
    IMediaSeeking* pMS;
    HRESULT hr = GetPeerSeeking(&pMS);
    if (hr < 0) {                                 // FAILED(hr)
        return hr;
    }

    hr = pMS->IsUsingTimeFormat(pFormat);
    pMS->Release();
    return hr;
}