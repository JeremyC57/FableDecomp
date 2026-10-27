// CPosPassThru::get_Duration(double*) -- DirectShow BaseClasses (ctlutil.cpp): forwards IMediaPosition::get_Duration to the upstream peer's IMediaPosition and releases it
// Leaf: ?get_Duration@CPosPassThru@@UAGJPAN@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CAD870, 53 bytes, STDMETHODIMP (__stdcall), `ret 8`.
// Identity (proven): the DirectShow BaseClasses source (strmbase is in neither PDB): IMediaPosition slot 7 (+0x1C) of
//   both pass-through vtables' CMediaPosition parts (entries at 0x012BF914 / 012BF9C4: CPosPassThru / CRendererPosPassThru), calls the landed
//   CPosPassThru::GetPeer 0x00CAD400 (IID_IMediaPosition 0x012AA064) and the peer's slot 7. No direct callers.
// Shape (family K065, representative 0x00CAD870): the landed CPosPassThru::GetRate 0x00CAD7D0 on the second base: the
//   CMediaPosition subobject is at +4, so the override's `this` arrives as that subobject and is adjusted by -4 before
//   GetPeer; pMP is homed in the dead `this` slot and kept in esi (GetPeer's body is in the TU, noinline, as retail).
typedef long HRESULT;
typedef unsigned long ULONG;
typedef unsigned long DWORD;
typedef long LONG;
typedef unsigned short WCHAR;
typedef __int64 LONGLONG;
typedef double REFTIME;
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
struct __declspec(novtable) IMediaPosition : IDispatch {                        // strmif.h / control.h
    virtual HRESULT __stdcall get_Duration(REFTIME* plength) = 0;               // 7
    virtual HRESULT __stdcall put_CurrentPosition(REFTIME llTime) = 0;          // 8
    virtual HRESULT __stdcall get_CurrentPosition(REFTIME* pllTime) = 0;        // 9
    virtual HRESULT __stdcall get_StopTime(REFTIME* pllTime) = 0;               // 10
    virtual HRESULT __stdcall put_StopTime(REFTIME llTime) = 0;                 // 11
    virtual HRESULT __stdcall get_PrerollTime(REFTIME* pllTime) = 0;            // 12
    virtual HRESULT __stdcall put_PrerollTime(REFTIME llTime) = 0;              // 13
    virtual HRESULT __stdcall put_Rate(double dRate) = 0;                       // 14
    virtual HRESULT __stdcall get_Rate(double* pdRate) = 0;                     // 15
    virtual HRESULT __stdcall CanSeekForward(LONG* pCanSeekForward) = 0;        // 16
    virtual HRESULT __stdcall CanSeekBackward(LONG* pCanSeekBackward) = 0;      // 17
};
struct __declspec(novtable) IMediaSeeking : IUnknown {
    virtual HRESULT __stdcall GetCapabilities(DWORD* pCapabilities) = 0;        // 3 (the rest not used here)
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
    HRESULT GetPeer(IMediaPosition** ppMP);       // 0x00CAD400
public:
    HRESULT __stdcall get_Duration(REFTIME* plength);
};
extern "C" const IID IID_IMediaPosition;         // 0x012AA064

// Return the IMediaPosition interface from our peer

__declspec(noinline) HRESULT   // same-TU stand-in of 0x00CAD400 (retail ctlutil.cpp holds both bodies)
CPosPassThru::GetPeer(IMediaPosition** ppMP)
{
    *ppMP = 0;

    IPin* pConnected;
    HRESULT hr = m_pPin->ConnectedTo(&pConnected);
    if (hr < 0) {
        return (HRESULT)0x80004001L;   // E_NOTIMPL
    }
    IMediaPosition* pMP;
    hr = pConnected->QueryInterface(IID_IMediaPosition, (void**)&pMP);
    pConnected->Release();
    if (hr < 0) {
        return (HRESULT)0x80004001L;   // E_NOTIMPL
    }

    *ppMP = pMP;
    return 0;
}

HRESULT __stdcall
CPosPassThru::get_Duration(REFTIME* plength)
{
    IMediaPosition* pMP;
    HRESULT hr = GetPeer(&pMP);
    if (hr < 0) {                                 // FAILED(hr)
        return hr;
    }
    hr = pMP->get_Duration(plength);
    pMP->Release();
    return hr;
}