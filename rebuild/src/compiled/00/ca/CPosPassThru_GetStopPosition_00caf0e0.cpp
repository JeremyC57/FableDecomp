// CPosPassThru::GetStopPosition(LONGLONG* output) -- Forward the original DirectShow interface query to the connected pin.
// Leaf: ?GetStopPosition@CPosPassThru@@UAGJPA_J@Z
// Identity: proven. Original DirectShow ctlutil CPosPassThru method in pass-through tables012bf8f8/012bf940 and derived renderer tables; exact COM interface slot and own GetPeer00cad400 or GetPeerSeeking00cad460 establish identity. Actual IMediaSeeking/CMediaPosition/CUnknown/INonDelegatingUnknown hierarchy explains secondary-this adjustment -4. Real noinline peer getter queries ConnectedTo and QueryInterface, releases connected pin, then forwards or returns E_NOTIMPL. Original GetSeekingLongLong member-pointer dispatch uses generated vcall thunk00caef60(slot11) or00caef40(slot10); do not replace with direct virtual-call source.
// Layout: original release-mode hierarchy, seeking0/media-position4/unknown8, pin24; no applicable engine header.
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
class CPosPassThru:public IMediaSeeking,public CMediaPosition{IPin* m_pPin;HRESULT GetPeer(IMediaPosition**);HRESULT GetPeerSeeking(IMediaSeeking**);HRESULT GetSeekingLongLong(HRESULT(__stdcall IMediaSeeking::*)(LONGLONG*),LONGLONG*);public:CPosPassThru(const WCHAR*,IUnknown*,HRESULT*,IPin*);HRESULT __stdcall GetStopPosition(LONGLONG* output);};
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

HRESULT __stdcall CPosPassThru::GetStopPosition(LONGLONG* output){return GetSeekingLongLong(&IMediaSeeking::GetStopPosition,output);}