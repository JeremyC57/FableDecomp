// CBasePin::CBasePin(const char* pObjectName, CBaseFilter* pFilter, CCritSec* pLock, HRESULT* phr, const wchar_t* pName, PIN_DIRECTION dir) -- DirectShow base-classes pin constructor (ANSI object name): CUnknown base, no name/connection, the direction, lock and filter stored, run-time-error/reconnect/try-types flags clear, no quality sink, type version 1, an empty media type, tStart 0, tStop MAX_TIME, rate 1.0; the pin name is copied into a new wide-string buffer when given
// Leaf: ??0CBasePin
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAC070, length 248 bytes (`ret 18h`, EH frame: state 0 after the CUnknown base,
// state 1 after the media type).
// Label correction: the manifest calls this "CEnginePrimitiveTypeInfo::InitializeEntry" (propagated); the body is amfilter.cpp
// CBasePin::CBasePin with a `const char*` object name -- its only callers construct CBaseInputPin / CBaseOutputPin (0x00CAC260
// landed as "CEntryRecord" stores m_pAllocator/m_bReadOnly/m_bFlushing/m_SampleProps of CBaseInputPin).
// Callees: CUnknown::CUnknown(const char*, IUnknown*) @ 0x00CACC80, CMediaType::CMediaType() @ 0x00CB1D00 (labelled "CEntry"),
// lstrlenW (import @ 0x0143FE48), operator new @ 0x00BFEA1A; the copy is the intrinsic memcpy (`rep movsd ; rep movsb`).
// Model: the landed wide-object-name twin 0x00CAB370 (same SDK class layout: m_pName +0x14, m_Connected +0x18, m_dir +0x1c,
// m_pLock +0x20, flags +0x24..+0x26, m_pFilter +0x28, m_pQSink +0x2c, m_TypeVersion +0x30, m_mt +0x34, m_tStart +0x80,
// m_tStop +0x88, m_dRate +0x90) with the ANSI CUnknown/CBaseObject constructors.
#include <stddef.h>
#ifndef _MSC_VER
#define __declspec(x)
#define __stdcall
typedef int LONG;typedef unsigned int ULONG;typedef long long REFERENCE_TIME;
#define SDK_MAX_TIME 0x7fffffffffffffffLL
#else
typedef long LONG;typedef unsigned long ULONG;typedef __int64 REFERENCE_TIME;
#define SDK_MAX_TIME 0x7fffffffffffffffI64
#endif
typedef LONG HRESULT;typedef ULONG DWORD;typedef int BOOL;typedef void* HANDLE;
struct GUID {ULONG Data1;unsigned short Data2,Data3;unsigned char Data4[8];};
struct __declspec(novtable) IUnknown {virtual HRESULT __stdcall QueryInterface(const GUID&,void**)=0;virtual ULONG __stdcall AddRef()=0;virtual ULONG __stdcall Release()=0;};
struct __declspec(novtable) INonDelegatingUnknown {virtual HRESULT __stdcall NonDelegatingQueryInterface(const GUID&,void**)=0;virtual ULONG __stdcall NonDelegatingAddRef()=0;virtual ULONG __stdcall NonDelegatingRelease()=0;};
struct CBaseObject {CBaseObject(const char*);~CBaseObject();private:CBaseObject(const CBaseObject&);void operator=(const CBaseObject&);}; // release build: debug object-name/cookie fields absent
struct __declspec(novtable) CUnknown:INonDelegatingUnknown,CBaseObject {IUnknown* const m_pUnknown;LONG m_cRef;CUnknown(const char*,IUnknown*); /*CACC80*/ virtual ~CUnknown(){};HRESULT __stdcall NonDelegatingQueryInterface(const GUID&,void**);ULONG __stdcall NonDelegatingAddRef();ULONG __stdcall NonDelegatingRelease();};
struct CBaseFilter;struct IBaseFilter;struct IMemAllocator;struct IMemInputPin;struct IMediaSample;struct IEnumMediaTypes;struct IGraphConfig;
enum PIN_DIRECTION {PINDIR_INPUT,PINDIR_OUTPUT};
struct AM_MEDIA_TYPE {GUID majortype,subtype;BOOL bFixedSizeSamples,bTemporalCompression;ULONG lSampleSize;GUID formattype;IUnknown* pUnk;ULONG cbFormat;unsigned char* pbFormat;};
struct CMediaType:AM_MEDIA_TYPE {CMediaType();~CMediaType();};
struct PIN_INFO;struct ALLOCATOR_PROPERTIES;
struct Quality {int Type;long Proportion;REFERENCE_TIME Late,TimeStamp;};
struct __declspec(novtable) IPin:IUnknown {
 virtual HRESULT __stdcall Connect(IPin*,const AM_MEDIA_TYPE*)=0;virtual HRESULT __stdcall ReceiveConnection(IPin*,const AM_MEDIA_TYPE*)=0;
 virtual HRESULT __stdcall Disconnect()=0;virtual HRESULT __stdcall ConnectedTo(IPin**)=0;virtual HRESULT __stdcall ConnectionMediaType(AM_MEDIA_TYPE*)=0;
 virtual HRESULT __stdcall QueryPinInfo(PIN_INFO*)=0;virtual HRESULT __stdcall QueryDirection(PIN_DIRECTION*)=0;virtual HRESULT __stdcall QueryId(wchar_t**)=0;
 virtual HRESULT __stdcall QueryAccept(const AM_MEDIA_TYPE*)=0;virtual HRESULT __stdcall EnumMediaTypes(IEnumMediaTypes**)=0;virtual HRESULT __stdcall QueryInternalConnections(IPin**,ULONG*)=0;
 virtual HRESULT __stdcall EndOfStream()=0;virtual HRESULT __stdcall BeginFlush()=0;virtual HRESULT __stdcall EndFlush()=0;virtual HRESULT __stdcall NewSegment(REFERENCE_TIME,REFERENCE_TIME,double)=0;
};
struct __declspec(novtable) IQualityControl:IUnknown {virtual HRESULT __stdcall Notify(IBaseFilter*,Quality)=0;virtual HRESULT __stdcall SetSink(IQualityControl*)=0;};
struct __declspec(novtable) IPinFlowControl:IUnknown {virtual HRESULT __stdcall Block(DWORD,HANDLE)=0;};
struct CRITICAL_SECTION {void* DebugInfo;long LockCount,RecursionCount;HANDLE OwningThread,LockSemaphore;size_t SpinCount;};
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(CRITICAL_SECTION*);
extern "C" __declspec(dllimport) HANDLE __stdcall CreateEventW(void*,BOOL,BOOL,const wchar_t*);
HRESULT AmGetLastErrorToHResult(); //CB0BC0 actual __cdecl
struct CCritSec {CRITICAL_SECTION m_CritSec;CCritSec(){InitializeCriticalSection(&m_CritSec);}~CCritSec(){DeleteCriticalSection(&m_CritSec);}};
#define UNKNOWN_METHODS HRESULT __stdcall QueryInterface(const GUID&,void**);ULONG __stdcall AddRef();ULONG __stdcall Release();
struct CRefTime {REFERENCE_TIME m_time;CRefTime():m_time(0){} CRefTime(REFERENCE_TIME t):m_time(t){}};
struct __declspec(novtable) CBasePin:CUnknown,IPin,IQualityControl {
 wchar_t* m_pName;IPin* m_Connected;PIN_DIRECTION m_dir;CCritSec* m_pLock;bool m_bRunTimeError,m_bCanReconnectWhenActive,m_bTryMyTypesFirst;
 CBaseFilter* m_pFilter;IQualityControl* m_pQSink;LONG m_TypeVersion;CMediaType m_mt;CRefTime m_tStart,m_tStop;double m_dRate;
 CBasePin(const char*,CBaseFilter*,CCritSec*,HRESULT*,const wchar_t*,PIN_DIRECTION); //CAC070
 virtual ~CBasePin();UNKNOWN_METHODS
 HRESULT __stdcall NonDelegatingQueryInterface(const GUID&,void**);ULONG __stdcall NonDelegatingRelease();ULONG __stdcall NonDelegatingAddRef();
 HRESULT __stdcall Connect(IPin*,const AM_MEDIA_TYPE*);HRESULT __stdcall ReceiveConnection(IPin*,const AM_MEDIA_TYPE*);HRESULT __stdcall Disconnect();HRESULT __stdcall ConnectedTo(IPin**);HRESULT __stdcall ConnectionMediaType(AM_MEDIA_TYPE*);
 HRESULT __stdcall QueryPinInfo(PIN_INFO*);HRESULT __stdcall QueryDirection(PIN_DIRECTION*);HRESULT __stdcall QueryId(wchar_t**);HRESULT __stdcall QueryAccept(const AM_MEDIA_TYPE*);HRESULT __stdcall EnumMediaTypes(IEnumMediaTypes**);HRESULT __stdcall QueryInternalConnections(IPin**,ULONG*);HRESULT __stdcall EndOfStream();HRESULT __stdcall NewSegment(REFERENCE_TIME,REFERENCE_TIME,double);
 HRESULT __stdcall Notify(IBaseFilter*,Quality);HRESULT __stdcall SetSink(IQualityControl*);
 virtual long GetMediaTypeVersion();virtual HRESULT Active();virtual HRESULT Inactive();virtual HRESULT Run(REFERENCE_TIME);virtual HRESULT CheckMediaType(const CMediaType*)=0;
 virtual HRESULT SetMediaType(const CMediaType*);virtual HRESULT CheckConnect(IPin*);virtual HRESULT BreakConnect();virtual HRESULT CompleteConnect(IPin*);virtual HRESULT GetMediaType(int,CMediaType*);
 void SetReconnectWhenActive(bool b){m_bCanReconnectWhenActive=b;}
};

#include <string.h>
#include <new>
extern "C" __declspec(dllimport) int __stdcall lstrlenW(const wchar_t*);
CBasePin::CBasePin(const char* name,CBaseFilter* filter,CCritSec* lock,HRESULT* hr,const wchar_t* pin,PIN_DIRECTION dir)
 :CUnknown(name,0),m_pName(0),m_Connected(0),m_dir(dir),m_pLock(lock),m_bRunTimeError(false),m_bCanReconnectWhenActive(false),m_bTryMyTypesFirst(false),m_pFilter(filter),m_pQSink(0),m_TypeVersion(1),m_tStart(),m_tStop(SDK_MAX_TIME),m_dRate(1.0)
{if(pin){int length=lstrlenW(pin)+1;int bytes=length*sizeof(wchar_t);m_pName=new wchar_t[length];if(m_pName)memcpy(m_pName,pin,bytes);}}