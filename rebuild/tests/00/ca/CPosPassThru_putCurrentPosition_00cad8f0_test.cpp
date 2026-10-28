#include <stdio.h>
// CPosPassThru::put_CurrentPosition(REFTIME value) -- Forward the original DirectShow interface query to the connected pin.
// Leaf: ?put_CurrentPosition@CPosPassThru@@UAGJN@Z
// Identity: proven. Original DirectShow ctlutil CPosPassThru method in pass-through tables012bf8f8/012bf940 and derived renderer tables; exact COM interface slot and own GetPeer00cad400 or GetPeerSeeking00cad460 establish identity. Actual IMediaSeeking/CMediaPosition/CUnknown/INonDelegatingUnknown hierarchy explains secondary-this adjustment -4. Real noinline peer getter queries ConnectedTo and QueryInterface, releases connected pin, then forwards or returns E_NOTIMPL. Forwarded double or LONG output pointer and Release ordering exactly follow original interface.
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
class CPosPassThru:public IMediaSeeking,public CMediaPosition{IPin* m_pPin;HRESULT GetPeer(IMediaPosition**);HRESULT GetPeerSeeking(IMediaSeeking**);HRESULT GetSeekingLongLong(HRESULT(__stdcall IMediaSeeking::*)(LONGLONG*),LONGLONG*);public:CPosPassThru(const WCHAR*,IUnknown*,HRESULT*,IPin*);HRESULT __stdcall put_CurrentPosition(REFTIME value);};
extern "C" const IID IID_IMediaPosition;extern "C" const IID IID_IMediaSeeking;
__declspec(noinline) HRESULT   
CPosPassThru::GetPeer(IMediaPosition** ppMP)
{
    *ppMP = 0;

    IPin* pConnected;
    HRESULT hr = m_pPin->ConnectedTo(&pConnected);
    if (hr < 0) {
        return (HRESULT)0x80004001L;   
    }
    IMediaPosition* pMP;
    hr = pConnected->QueryInterface(IID_IMediaPosition, (void**)&pMP);
    pConnected->Release();
    if (hr < 0) {
        return (HRESULT)0x80004001L;   
    }

    *ppMP = pMP;
    return 0;
}

HRESULT __stdcall CPosPassThru::put_CurrentPosition(REFTIME value){IMediaPosition* pMP;HRESULT hr=GetPeer(&pMP);if(hr<0)return hr;hr=pMP->put_CurrentPosition(value);pMP->Release();return hr;}
const IID IID_IMediaPosition={0,0,0,{0}};const IID IID_IMediaSeeking={1,0,0,{0}};
CBaseObject::CBaseObject(const WCHAR*){}CBaseObject::~CBaseObject(){}CUnknown::CUnknown(const WCHAR* n,IUnknown* u):CBaseObject(n),m_pUnknown(u),m_cRef(1){}CBaseDispatch::~CBaseDispatch(){}CMediaPosition::CMediaPosition(const WCHAR* n,IUnknown* u):CUnknown(n,u){}CPosPassThru::CPosPassThru(const WCHAR* n,IUnknown* u,HRESULT*,IPin* p):CMediaPosition(n,u),m_pPin(p){}
static int mode,queries,connects,releases,calls,bad;static double seen;static void* seenOutput;
class Peer:public IPin,public IMediaPosition,public IMediaSeeking{public:
HRESULT __stdcall QueryInterface(const IID& id,void** out){++queries;if(mode==2)return -9;if(&id==&IID_IMediaPosition)*out=(IMediaPosition*)this;else if(&id==&IID_IMediaSeeking)*out=(IMediaSeeking*)this;else{bad=1;return -8;}return 0;}
ULONG __stdcall Release(){++releases;return 1;}HRESULT __stdcall ConnectedTo(IPin** out){++connects;if(mode==1)return -7;*out=(IPin*)this;return 0;}
ULONG __stdcall AddRef(){return 0;}
HRESULT __stdcall NonDelegatingQueryInterface(const IID& riid, void** ppv){return 0;}
ULONG __stdcall NonDelegatingAddRef(){return 0;}
ULONG __stdcall NonDelegatingRelease(){return 0;}
void v3(){}
void v4(){}
void v5(){}
HRESULT __stdcall GetTypeInfoCount(unsigned int* pctinfo){return 0;}
void v6(){}
HRESULT __stdcall get_Duration(REFTIME* plength){return 0;}
HRESULT __stdcall get_CurrentPosition(REFTIME* pllTime){return 0;}
HRESULT __stdcall get_StopTime(REFTIME* pllTime){return 0;}
HRESULT __stdcall put_StopTime(REFTIME llTime){return 0;}
HRESULT __stdcall get_PrerollTime(REFTIME* pllTime){return 0;}
HRESULT __stdcall put_PrerollTime(REFTIME llTime){return 0;}
HRESULT __stdcall put_Rate(double dRate){return 0;}
HRESULT __stdcall get_Rate(double* pdRate){return 0;}
HRESULT __stdcall CanSeekForward(LONG* pCanSeekForward){return 0;}
HRESULT __stdcall CanSeekBackward(LONG* pCanSeekBackward){return 0;}
HRESULT __stdcall GetCapabilities(DWORD* pCapabilities){return 0;}
HRESULT __stdcall CheckCapabilities(DWORD* pCapabilities){return 0;}
HRESULT __stdcall IsFormatSupported(const GUID* pFormat){return 0;}
HRESULT __stdcall QueryPreferredFormat(GUID* pFormat){return 0;}
HRESULT __stdcall GetTimeFormat(GUID* pFormat){return 0;}
HRESULT __stdcall IsUsingTimeFormat(const GUID* pFormat){return 0;}
HRESULT __stdcall SetTimeFormat(const GUID* pFormat){return 0;}
HRESULT __stdcall GetDuration(LONGLONG* pDuration){return 0;}
HRESULT __stdcall GetStopPosition(LONGLONG* pStop){return 0;}
HRESULT __stdcall GetCurrentPosition(LONGLONG* pCurrent){return 0;}
HRESULT __stdcall ConvertTimeFormat(LONGLONG* pTarget, const GUID* pTargetFormat, LONGLONG Source, const GUID* pSourceFormat){return 0;}
HRESULT __stdcall SetPositions(LONGLONG* pCurrent, DWORD dwCurrentFlags, LONGLONG* pStop, DWORD dwStopFlags){return 0;}
HRESULT __stdcall GetPositions(LONGLONG* pCurrent, LONGLONG* pStop){return 0;}
HRESULT __stdcall GetAvailable(LONGLONG* pEarliest, LONGLONG* pLatest){return 0;}
HRESULT __stdcall SetRate(double dRate){return 0;}
HRESULT __stdcall GetRate(double* pdRate){return 0;}
HRESULT __stdcall GetPreroll(LONGLONG* pllPreroll){return 0;}
HRESULT __stdcall put_CurrentPosition(REFTIME value){++calls;seen=value;return mode==3?-13:17;}
};
class Fixture:public CPosPassThru{public:Fixture(IPin* p):CPosPassThru(0,0,0,p){}HRESULT __stdcall QueryInterface(const IID& riid, void** ppv){return 0;}
ULONG __stdcall AddRef(){return 0;}
ULONG __stdcall Release(){return 0;}
HRESULT __stdcall NonDelegatingQueryInterface(const IID& riid, void** ppv){return 0;}
ULONG __stdcall NonDelegatingAddRef(){return 0;}
ULONG __stdcall NonDelegatingRelease(){return 0;}
void v3(){}
void v4(){}
void v5(){}
HRESULT __stdcall ConnectedTo(IPin** pPin){return 0;}
HRESULT __stdcall GetTypeInfoCount(unsigned int* pctinfo){return 0;}
void v6(){}
HRESULT __stdcall get_Duration(REFTIME* plength){return 0;}
HRESULT __stdcall get_CurrentPosition(REFTIME* pllTime){return 0;}
HRESULT __stdcall get_StopTime(REFTIME* pllTime){return 0;}
HRESULT __stdcall put_StopTime(REFTIME llTime){return 0;}
HRESULT __stdcall get_PrerollTime(REFTIME* pllTime){return 0;}
HRESULT __stdcall put_PrerollTime(REFTIME llTime){return 0;}
HRESULT __stdcall put_Rate(double dRate){return 0;}
HRESULT __stdcall get_Rate(double* pdRate){return 0;}
HRESULT __stdcall CanSeekForward(LONG* pCanSeekForward){return 0;}
HRESULT __stdcall CanSeekBackward(LONG* pCanSeekBackward){return 0;}
HRESULT __stdcall GetCapabilities(DWORD* pCapabilities){return 0;}
HRESULT __stdcall CheckCapabilities(DWORD* pCapabilities){return 0;}
HRESULT __stdcall IsFormatSupported(const GUID* pFormat){return 0;}
HRESULT __stdcall QueryPreferredFormat(GUID* pFormat){return 0;}
HRESULT __stdcall GetTimeFormat(GUID* pFormat){return 0;}
HRESULT __stdcall IsUsingTimeFormat(const GUID* pFormat){return 0;}
HRESULT __stdcall SetTimeFormat(const GUID* pFormat){return 0;}
HRESULT __stdcall GetDuration(LONGLONG* pDuration){return 0;}
HRESULT __stdcall GetStopPosition(LONGLONG* pStop){return 0;}
HRESULT __stdcall GetCurrentPosition(LONGLONG* pCurrent){return 0;}
HRESULT __stdcall ConvertTimeFormat(LONGLONG* pTarget, const GUID* pTargetFormat, LONGLONG Source, const GUID* pSourceFormat){return 0;}
HRESULT __stdcall SetPositions(LONGLONG* pCurrent, DWORD dwCurrentFlags, LONGLONG* pStop, DWORD dwStopFlags){return 0;}
HRESULT __stdcall GetPositions(LONGLONG* pCurrent, LONGLONG* pStop){return 0;}
HRESULT __stdcall GetAvailable(LONGLONG* pEarliest, LONGLONG* pLatest){return 0;}
HRESULT __stdcall SetRate(double dRate){return 0;}
HRESULT __stdcall GetRate(double* pdRate){return 0;}
HRESULT __stdcall GetPreroll(LONGLONG* pllPreroll){return 0;}
};
int main(){Peer peer;Fixture fixture((IPin*)&peer);bool ok=true;for(mode=0;mode<4;++mode){queries=connects=releases=calls=bad=0;double argument=1.25;HRESULT hr=fixture.put_CurrentPosition(argument);if(mode==1)ok=ok&&hr<0&&connects==1&&!queries&&!calls&&!releases;else if(mode==2)ok=ok&&hr<0&&connects==1&&queries==1&&!calls&&releases==1;else ok=ok&&hr==(mode==3?-13:17)&&connects==1&&queries==1&&calls==1&&releases==2&&seen==argument;ok=ok&&!bad;}puts(ok?"00cad8f0_TEST PASS":"00cad8f0_TEST FAIL");return ok?0:1;}