// IMediaSeeking::vcall{76}(IMediaSeeking* object) -- compiler-generated __stdcall vcall thunk `[thunk]: __stdcall 'vcall'{76,{flat}}' for the member pointer to GetPreroll (slot 19): read the stacked `this`, jump through its vtable slot
// Leaf: ??_9@$BEM@AG
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CAEF50, 9 bytes: `mov eax,[esp+4] ; mov eax,[eax] ; jmp [eax+4Ch]`.
// Identity: compiler-generated (no PDB record). DirectShow base classes ctlutil.cpp: CPosPassThru::GetDuration / GetStopPosition /
//   GetPreroll each call `GetSeekingLongLong(&IMediaSeeking::<method>, p)`, which dispatches `(pMS->*pMethod)(pll)`; taking the
//   address of a virtual __stdcall member makes VC7.1 emit this thunk (`??_9@$B<offset>@AG`: VC7.1 leaves the class out of the
//   name; A = near, G = __stdcall) in the TU, and the call `(pMS->*pMethod)` inlined into the caller reaches it with a direct call.
//   IMediaSeeking slot 19 (+0x4C) = GetPreroll (strmif.h order: IUnknown 0-2, GetCapabilities 3 .. SetTimeFormat 9, GetDuration
//   10, GetStopPosition 11, ... GetPreroll 19). Sole caller: CPosPassThru::GetPreroll 0x00CAF160 (landed; reloc_bindings binds ??_9@$BEM@AG -> 0x00CAEF50).
//   The catalog label `CThingFilter_IsValid::operator()` is propagated across the three byte-identical-shape thunks.
// Model: the landed CPosPassThru::GetDuration 0x00CAF120 source (the IMediaSeeking hierarchy, GetPeerSeeking, GetSeekingLongLong);
//   this TU defines the three ctlutil.cpp getters that take the member pointers, so it emits all three thunks.
#ifdef _MSC_VER
typedef long HRESULT;
#else
typedef int HRESULT;
#define __stdcall
#define __declspec(x)
#endif
typedef unsigned long ULONG;
typedef unsigned long DWORD;
typedef long LONG;
typedef unsigned short WCHAR;
#ifdef _MSC_VER
typedef __int64 LONGLONG;
#else
typedef __INT64_TYPE__ LONGLONG;
#endif
typedef double REFTIME;
#define STDMETHODIMP HRESULT __stdcall
typedef struct _GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; } GUID;   
typedef GUID IID;
struct __declspec(novtable) IUnknown {
    virtual HRESULT __stdcall QueryInterface(const IID& riid, void** ppv) = 0;   
    virtual ULONG __stdcall AddRef() = 0;                                        
    virtual ULONG __stdcall Release() = 0;                                       
};
struct __declspec(novtable) INonDelegatingUnknown {
    virtual HRESULT __stdcall NonDelegatingQueryInterface(const IID& riid, void** ppv) = 0;
    virtual ULONG __stdcall NonDelegatingAddRef() = 0;
    virtual ULONG __stdcall NonDelegatingRelease() = 0;
};
class CBaseObject {                               
    static LONG m_cObjects;
public:
    CBaseObject(const WCHAR* pName);
    ~CBaseObject();
};
class __declspec(novtable) CUnknown : public INonDelegatingUnknown, public CBaseObject {
    IUnknown* const m_pUnknown;                   
protected:
    LONG m_cRef;                                  
public:
    CUnknown(const WCHAR* pName, IUnknown* pUnk);
    virtual ~CUnknown() {}
};
struct __declspec(novtable) IPin : IUnknown {
    virtual void v3() = 0; virtual void v4() = 0; virtual void v5() = 0;       
    virtual HRESULT __stdcall ConnectedTo(IPin** pPin) = 0;                     
};
struct __declspec(novtable) IDispatch : IUnknown {
    virtual HRESULT __stdcall GetTypeInfoCount(unsigned int* pctinfo) = 0;      
    virtual void v4() = 0; virtual void v5() = 0; virtual void v6() = 0;        
};
struct __declspec(novtable) IMediaPosition : IDispatch {                        
    virtual HRESULT __stdcall get_Duration(REFTIME* plength) = 0;               
    virtual HRESULT __stdcall put_CurrentPosition(REFTIME llTime) = 0;          
    virtual HRESULT __stdcall get_CurrentPosition(REFTIME* pllTime) = 0;        
    virtual HRESULT __stdcall get_StopTime(REFTIME* pllTime) = 0;               
    virtual HRESULT __stdcall put_StopTime(REFTIME llTime) = 0;                 
    virtual HRESULT __stdcall get_PrerollTime(REFTIME* pllTime) = 0;            
    virtual HRESULT __stdcall put_PrerollTime(REFTIME llTime) = 0;              
    virtual HRESULT __stdcall put_Rate(double dRate) = 0;                       
    virtual HRESULT __stdcall get_Rate(double* pdRate) = 0;                     
    virtual HRESULT __stdcall CanSeekForward(LONG* pCanSeekForward) = 0;        
    virtual HRESULT __stdcall CanSeekBackward(LONG* pCanSeekBackward) = 0;      
};
struct __declspec(novtable) IMediaSeeking : IUnknown {
    virtual HRESULT __stdcall GetCapabilities(DWORD* pCapabilities) = 0;                        
    virtual HRESULT __stdcall CheckCapabilities(DWORD* pCapabilities) = 0;                      
    virtual HRESULT __stdcall IsFormatSupported(const GUID* pFormat) = 0;                       
    virtual HRESULT __stdcall QueryPreferredFormat(GUID* pFormat) = 0;                          
    virtual HRESULT __stdcall GetTimeFormat(GUID* pFormat) = 0;                                 
    virtual HRESULT __stdcall IsUsingTimeFormat(const GUID* pFormat) = 0;                       
    virtual HRESULT __stdcall SetTimeFormat(const GUID* pFormat) = 0;                           
    virtual HRESULT __stdcall GetDuration(LONGLONG* pDuration) = 0;                             
    virtual HRESULT __stdcall GetStopPosition(LONGLONG* pStop) = 0;                             
    virtual HRESULT __stdcall GetCurrentPosition(LONGLONG* pCurrent) = 0;                       
    virtual HRESULT __stdcall ConvertTimeFormat(LONGLONG* pTarget, const GUID* pTargetFormat, LONGLONG Source, const GUID* pSourceFormat) = 0;   
    virtual HRESULT __stdcall SetPositions(LONGLONG* pCurrent, DWORD dwCurrentFlags, LONGLONG* pStop, DWORD dwStopFlags) = 0;   
    virtual HRESULT __stdcall GetPositions(LONGLONG* pCurrent, LONGLONG* pStop) = 0;            
    virtual HRESULT __stdcall GetAvailable(LONGLONG* pEarliest, LONGLONG* pLatest) = 0;         
    virtual HRESULT __stdcall SetRate(double dRate) = 0;                                        
    virtual HRESULT __stdcall GetRate(double* pdRate) = 0;                                      
    virtual HRESULT __stdcall GetPreroll(LONGLONG* pllPreroll) = 0;                             
};
class CBaseDispatch {                             
    void* m_pti;                                  
public:
    ~CBaseDispatch();
};
class __declspec(novtable) CMediaPosition : public IMediaPosition, public CUnknown {   
    CBaseDispatch m_basedisp;                     
public:
    CMediaPosition(const WCHAR* pName, IUnknown* pUnk);
};
class CPosPassThru:public IMediaSeeking,public CMediaPosition{IPin* m_pPin;HRESULT GetPeer(IMediaPosition**);HRESULT GetPeerSeeking(IMediaSeeking**);HRESULT GetSeekingLongLong(HRESULT(__stdcall IMediaSeeking::*)(LONGLONG*),LONGLONG*);public:CPosPassThru(const WCHAR*,IUnknown*,HRESULT*,IPin*);HRESULT __stdcall GetDuration(LONGLONG* pDuration);HRESULT __stdcall GetStopPosition(LONGLONG* pStop);HRESULT __stdcall GetPreroll(LONGLONG* pllPreroll);};
extern "C" const IID IID_IMediaPosition;extern "C" const IID IID_IMediaSeeking;
__declspec(noinline) HRESULT   
CPosPassThru::GetPeerSeeking(IMediaSeeking** ppMS)
{
    *ppMS = 0;

    IPin* pConnected;
    HRESULT hr = m_pPin->ConnectedTo(&pConnected);
    if (hr < 0) {
        return (HRESULT)0x80004001L;   
    }
    IMediaSeeking* pMS;
    hr = pConnected->QueryInterface(IID_IMediaSeeking, (void**)&pMS);
    pConnected->Release();
    if (hr < 0) {
        return (HRESULT)0x80004001L;   
    }

    *ppMS = pMS;
    return 0;
}

HRESULT
CPosPassThru::GetSeekingLongLong
( HRESULT (__stdcall IMediaSeeking::*pMethod)( LONGLONG * )
, LONGLONG * pll
)
{
    IMediaSeeking* pMS;
    HRESULT hr = GetPeerSeeking(&pMS);
    if (hr >= 0)
    {
        hr = (pMS->*pMethod)(pll);
        pMS->Release();
    }
    return hr;
}

STDMETHODIMP
CPosPassThru::GetDuration(LONGLONG *pDuration)
{
    return GetSeekingLongLong( &IMediaSeeking::GetDuration, pDuration );
}

STDMETHODIMP
CPosPassThru::GetStopPosition(LONGLONG *pStop)
{
    return GetSeekingLongLong( &IMediaSeeking::GetStopPosition, pStop );
}

STDMETHODIMP
CPosPassThru::GetPreroll(LONGLONG *pllPreroll)
{
    return GetSeekingLongLong( &IMediaSeeking::GetPreroll, pllPreroll );
}