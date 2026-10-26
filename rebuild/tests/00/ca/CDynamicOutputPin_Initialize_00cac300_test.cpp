#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
#define __int64 long long
#endif
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <new>
#ifdef _MSC_VER
#define __declspec(x) FIXTURE_DECLSPEC_##x
#define FIXTURE_DECLSPEC_novtable __declspec(novtable)
#define FIXTURE_DECLSPEC_dllimport
#else
#define __declspec(x)
#define __stdcall
#define __int64 long long
#endif
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <new>

// CDynamicOutputPin constructor, not CEngineScreenEffectRenderer::Initialize.
// Native DirectShow ABI projection from Microsoft's amfilter.h/combase.h/wxutil.h.
// Public reference copies and URL listing retained in scratch/probe/directshow_*.
// Full SDK headers are absent from the repository; genuine interfaces below preserve
// all 26 primary, 18 IPin, 5 IQualityControl and 4 IPinFlowControl slots.
#include <stddef.h>
typedef long HRESULT;typedef unsigned long ULONG;typedef unsigned long DWORD;typedef int BOOL;typedef void* HANDLE;typedef __int64 REFERENCE_TIME;
struct GUID {unsigned long Data1;unsigned short Data2,Data3;unsigned char Data4[8];};
struct __declspec(novtable) IUnknown {virtual HRESULT __stdcall QueryInterface(const GUID&,void**)=0;virtual ULONG __stdcall AddRef()=0;virtual ULONG __stdcall Release()=0;};
struct __declspec(novtable) INonDelegatingUnknown {virtual HRESULT __stdcall NonDelegatingQueryInterface(const GUID&,void**)=0;virtual ULONG __stdcall NonDelegatingAddRef()=0;virtual ULONG __stdcall NonDelegatingRelease()=0;};
class CBaseObject { public:CBaseObject(const char*);~CBaseObject();private:CBaseObject(const CBaseObject&);void operator=(const CBaseObject&);}; // release build: debug object-name/cookie fields absent
struct __declspec(novtable) CUnknown:INonDelegatingUnknown,CBaseObject {IUnknown* const m_pUnknown;volatile long m_cRef;CUnknown(const char*,IUnknown*);virtual ~CUnknown(){};HRESULT __stdcall NonDelegatingQueryInterface(const GUID&,void**);ULONG __stdcall NonDelegatingAddRef();ULONG __stdcall NonDelegatingRelease();};
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
struct __declspec(novtable) CBasePin:CUnknown,IPin,IQualityControl {
 wchar_t* m_pName;IPin* m_Connected;PIN_DIRECTION m_dir;CCritSec* m_pLock;bool m_bRunTimeError,m_bCanReconnectWhenActive,m_bTryMyTypesFirst;
 CBaseFilter* m_pFilter;IQualityControl* m_pQSink;long m_TypeVersion;CMediaType m_mt;REFERENCE_TIME m_tStart,m_tStop;double m_dRate;
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
struct __declspec(novtable) CBaseOutputPin:CBasePin {
 IMemAllocator* m_pAllocator;IMemInputPin* m_pInputPin;
 CBaseOutputPin(const char* name,CBaseFilter* filter,CCritSec* lock,HRESULT* hr,const wchar_t* pin):CBasePin(name,filter,lock,hr,pin,PINDIR_OUTPUT),m_pAllocator(0),m_pInputPin(0){}
 HRESULT CompleteConnect(IPin*);virtual HRESULT DecideAllocator(IMemInputPin*,IMemAllocator**);virtual HRESULT DecideBufferSize(IMemAllocator*,ALLOCATOR_PROPERTIES*)=0;
 virtual HRESULT GetDeliveryBuffer(IMediaSample**,REFERENCE_TIME*,REFERENCE_TIME*,DWORD);virtual HRESULT Deliver(IMediaSample*);virtual HRESULT InitAllocator(IMemAllocator**);
 HRESULT CheckConnect(IPin*);HRESULT BreakConnect();HRESULT Active();HRESULT Inactive();HRESULT __stdcall EndOfStream();virtual HRESULT DeliverEndOfStream();HRESULT __stdcall BeginFlush();HRESULT __stdcall EndFlush();virtual HRESULT DeliverBeginFlush();virtual HRESULT DeliverEndFlush();virtual HRESULT DeliverNewSegment(REFERENCE_TIME,REFERENCE_TIME,double);
};
struct CDynamicOutputPin:CBaseOutputPin,IPinFlowControl {
 CDynamicOutputPin(const char*,CBaseFilter*,CCritSec*,HRESULT*,const wchar_t*);~CDynamicOutputPin();UNKNOWN_METHODS
 HRESULT __stdcall NonDelegatingQueryInterface(const GUID&,void**);HRESULT __stdcall Disconnect();HRESULT __stdcall Block(DWORD,HANDLE);
 HRESULT DeliverBeginFlush();HRESULT DeliverEndFlush();HRESULT Inactive();HRESULT Active();HRESULT CompleteConnect(IPin*);
 virtual HRESULT StartUsingOutputPin();virtual void StopUsingOutputPin();virtual bool StreamingThreadUsingOutputPin();
 enum BLOCK_STATE {NOT_BLOCKED,PENDING,BLOCKED};
 CCritSec m_BlockStateLock;HANDLE m_hUnblockOutputPinEvent,m_hNotifyCallerPinBlockedEvent;BLOCK_STATE m_BlockState;DWORD m_dwBlockCallerThreadID,m_dwNumOutstandingOutputPinUsers;HANDLE m_hStopEvent;IGraphConfig* m_pGraphConfig;BOOL m_bPinUsesReadOnlyAllocator;
 HRESULT Initialize(){m_hUnblockOutputPinEvent=CreateEventW(0,1,1,0);if(!m_hUnblockOutputPinEvent)return AmGetLastErrorToHResult();SetReconnectWhenActive(true);return 0;}
};
#ifdef _MSC_VER
typedef char Base98[sizeof(CBasePin)==0x98?1:-1];typedef char OutputA0[sizeof(CBaseOutputPin)==0xa0?1:-1];typedef char LockA4[offsetof(CDynamicOutputPin,m_BlockStateLock)==0xa4?1:-1];typedef char EventBC[offsetof(CDynamicOutputPin,m_hUnblockOutputPinEvent)==0xbc?1:-1];typedef char DynamicE0[sizeof(CDynamicOutputPin)==0xe0?1:-1];
#endif
CDynamicOutputPin::CDynamicOutputPin(const char* name,CBaseFilter* filter,CCritSec* lock,HRESULT* phr,const wchar_t* pin)
 :CBaseOutputPin(name,filter,lock,phr,pin),m_hStopEvent(0),m_pGraphConfig(0),m_bPinUsesReadOnlyAllocator(0),m_BlockState(NOT_BLOCKED),m_hUnblockOutputPinEvent(0),m_hNotifyCallerPinBlockedEvent(0),m_dwBlockCallerThreadID(0),m_dwNumOutstandingOutputPinUsers(0)
{HRESULT hr=Initialize();if(hr<0){*phr=hr;return;}}



static int baseCtor,baseDtor,critCtor,critDtor,eventCalls,errorCalls,objects,lastMethod;static void* lastThis;
static int scenario;static CBasePin* observed;static const char* expectedName;static CBaseFilter* expectedFilter;static CCritSec* expectedLock;static HRESULT* expectedHR;static const wchar_t* expectedPin;
CBaseObject::CBaseObject(const char*){++objects;}CBaseObject::~CBaseObject(){--objects;}
CUnknown::CUnknown(const char* name,IUnknown* unknown):CBaseObject(name),m_pUnknown(unknown),m_cRef(0){}
CMediaType::CMediaType(){memset(static_cast<AM_MEDIA_TYPE*>(this),0,sizeof(AM_MEDIA_TYPE));}CMediaType::~CMediaType(){}
CBasePin::CBasePin(const char* name,CBaseFilter* filter,CCritSec* lock,HRESULT* hr,const wchar_t* pin,PIN_DIRECTION dir):CUnknown(name,0){++baseCtor;observed=this;assert(name==expectedName&&filter==expectedFilter&&lock==expectedLock&&hr==expectedHR&&pin==expectedPin&&dir==PINDIR_OUTPUT);m_pFilter=filter;m_pLock=lock;m_dir=dir;m_bCanReconnectWhenActive=false;}
CBasePin::~CBasePin(){++baseDtor;}
CDynamicOutputPin::~CDynamicOutputPin(){}
extern "C" void __stdcall InitializeCriticalSection(CRITICAL_SECTION* p){++critCtor;assert(p==&static_cast<CDynamicOutputPin*>(observed)->m_BlockStateLock.m_CritSec);if(scenario==3)throw 31;}
extern "C" void __stdcall DeleteCriticalSection(CRITICAL_SECTION* p){++critDtor;assert(p==&static_cast<CDynamicOutputPin*>(observed)->m_BlockStateLock.m_CritSec);}
extern "C" HANDLE __stdcall CreateEventW(void* security,BOOL manual,BOOL signalled,const wchar_t* name){++eventCalls;assert(!security&&manual==1&&signalled==1&&!name);CDynamicOutputPin* p=static_cast<CDynamicOutputPin*>(observed);assert(!p->m_pAllocator&&!p->m_pInputPin&&!p->m_hUnblockOutputPinEvent&&!p->m_hNotifyCallerPinBlockedEvent&&!p->m_BlockState&&!p->m_dwBlockCallerThreadID&&!p->m_dwNumOutstandingOutputPinUsers&&!p->m_hStopEvent&&!p->m_pGraphConfig&&!p->m_bPinUsesReadOnlyAllocator&&!p->m_bCanReconnectWhenActive);if(scenario==4)throw 41;return scenario==0?reinterpret_cast<HANDLE>(size_t(0x1234)):0;}
HRESULT AmGetLastErrorToHResult(){++errorCalls;return scenario==1?-777:11;}
HRESULT __stdcall CUnknown::NonDelegatingQueryInterface(const GUID&,void**){lastThis=this;lastMethod=1;return 1;}
ULONG __stdcall CUnknown::NonDelegatingAddRef(){lastThis=this;lastMethod=2;return 2;}
ULONG __stdcall CUnknown::NonDelegatingRelease(){lastThis=this;lastMethod=3;return 3;}
HRESULT __stdcall CBasePin::QueryInterface(const GUID&,void**){lastThis=this;lastMethod=4;return 4;}
ULONG __stdcall CBasePin::AddRef(){lastThis=this;lastMethod=5;return 5;}
ULONG __stdcall CBasePin::Release(){lastThis=this;lastMethod=6;return 6;}
HRESULT __stdcall CBasePin::NonDelegatingQueryInterface(const GUID&,void**){lastThis=this;lastMethod=7;return 7;}
ULONG __stdcall CBasePin::NonDelegatingRelease(){lastThis=this;lastMethod=8;return 8;}
ULONG __stdcall CBasePin::NonDelegatingAddRef(){lastThis=this;lastMethod=9;return 9;}
HRESULT __stdcall CBasePin::Connect(IPin*,const AM_MEDIA_TYPE*){lastThis=this;lastMethod=10;return 10;}
HRESULT __stdcall CBasePin::ReceiveConnection(IPin*,const AM_MEDIA_TYPE*){lastThis=this;lastMethod=11;return 11;}
HRESULT __stdcall CBasePin::Disconnect(){lastThis=this;lastMethod=12;return 12;}
HRESULT __stdcall CBasePin::ConnectedTo(IPin**){lastThis=this;lastMethod=13;return 13;}
HRESULT __stdcall CBasePin::ConnectionMediaType(AM_MEDIA_TYPE*){lastThis=this;lastMethod=14;return 14;}
HRESULT __stdcall CBasePin::QueryPinInfo(PIN_INFO*){lastThis=this;lastMethod=15;return 15;}
HRESULT __stdcall CBasePin::QueryDirection(PIN_DIRECTION*){lastThis=this;lastMethod=16;return 16;}
HRESULT __stdcall CBasePin::QueryId(wchar_t**){lastThis=this;lastMethod=17;return 17;}
HRESULT __stdcall CBasePin::QueryAccept(const AM_MEDIA_TYPE*){lastThis=this;lastMethod=18;return 18;}
HRESULT __stdcall CBasePin::EnumMediaTypes(IEnumMediaTypes**){lastThis=this;lastMethod=19;return 19;}
HRESULT __stdcall CBasePin::QueryInternalConnections(IPin**,ULONG*){lastThis=this;lastMethod=20;return 20;}
HRESULT __stdcall CBasePin::EndOfStream(){lastThis=this;lastMethod=21;return 21;}
HRESULT __stdcall CBasePin::NewSegment(REFERENCE_TIME,REFERENCE_TIME,double){lastThis=this;lastMethod=22;return 22;}
HRESULT __stdcall CBasePin::Notify(IBaseFilter*,Quality){lastThis=this;lastMethod=23;return 23;}
HRESULT __stdcall CBasePin::SetSink(IQualityControl*){lastThis=this;lastMethod=24;return 24;}
long CBasePin::GetMediaTypeVersion(){lastThis=this;lastMethod=25;return 25;}
HRESULT CBasePin::Active(){lastThis=this;lastMethod=26;return 26;}
HRESULT CBasePin::Inactive(){lastThis=this;lastMethod=27;return 27;}
HRESULT CBasePin::Run(REFERENCE_TIME){lastThis=this;lastMethod=28;return 28;}
HRESULT CBasePin::SetMediaType(const CMediaType*){lastThis=this;lastMethod=29;return 29;}
HRESULT CBasePin::CheckConnect(IPin*){lastThis=this;lastMethod=30;return 30;}
HRESULT CBasePin::BreakConnect(){lastThis=this;lastMethod=31;return 31;}
HRESULT CBasePin::CompleteConnect(IPin*){lastThis=this;lastMethod=32;return 32;}
HRESULT CBasePin::GetMediaType(int,CMediaType*){lastThis=this;lastMethod=33;return 33;}
HRESULT CBaseOutputPin::CompleteConnect(IPin*){lastThis=this;lastMethod=34;return 34;}
HRESULT CBaseOutputPin::DecideAllocator(IMemInputPin*,IMemAllocator**){lastThis=this;lastMethod=35;return 35;}
HRESULT CBaseOutputPin::GetDeliveryBuffer(IMediaSample**,REFERENCE_TIME*,REFERENCE_TIME*,DWORD){lastThis=this;lastMethod=36;return 36;}
HRESULT CBaseOutputPin::Deliver(IMediaSample*){lastThis=this;lastMethod=37;return 37;}
HRESULT CBaseOutputPin::InitAllocator(IMemAllocator**){lastThis=this;lastMethod=38;return 38;}
HRESULT CBaseOutputPin::CheckConnect(IPin*){lastThis=this;lastMethod=39;return 39;}
HRESULT CBaseOutputPin::BreakConnect(){lastThis=this;lastMethod=40;return 40;}
HRESULT CBaseOutputPin::Active(){lastThis=this;lastMethod=41;return 41;}
HRESULT CBaseOutputPin::Inactive(){lastThis=this;lastMethod=42;return 42;}
HRESULT __stdcall CBaseOutputPin::EndOfStream(){lastThis=this;lastMethod=43;return 43;}
HRESULT CBaseOutputPin::DeliverEndOfStream(){lastThis=this;lastMethod=44;return 44;}
HRESULT __stdcall CBaseOutputPin::BeginFlush(){lastThis=this;lastMethod=45;return 45;}
HRESULT __stdcall CBaseOutputPin::EndFlush(){lastThis=this;lastMethod=46;return 46;}
HRESULT CBaseOutputPin::DeliverBeginFlush(){lastThis=this;lastMethod=47;return 47;}
HRESULT CBaseOutputPin::DeliverEndFlush(){lastThis=this;lastMethod=48;return 48;}
HRESULT CBaseOutputPin::DeliverNewSegment(REFERENCE_TIME,REFERENCE_TIME,double){lastThis=this;lastMethod=49;return 49;}
HRESULT __stdcall CDynamicOutputPin::QueryInterface(const GUID&,void**){lastThis=this;lastMethod=50;return 50;}
ULONG __stdcall CDynamicOutputPin::AddRef(){lastThis=this;lastMethod=51;return 51;}
ULONG __stdcall CDynamicOutputPin::Release(){lastThis=this;lastMethod=52;return 52;}
HRESULT __stdcall CDynamicOutputPin::NonDelegatingQueryInterface(const GUID&,void**){lastThis=this;lastMethod=53;return 53;}
HRESULT __stdcall CDynamicOutputPin::Disconnect(){lastThis=this;lastMethod=54;return 54;}
HRESULT __stdcall CDynamicOutputPin::Block(DWORD,HANDLE){lastThis=this;lastMethod=55;return 55;}
HRESULT CDynamicOutputPin::DeliverBeginFlush(){lastThis=this;lastMethod=56;return 56;}
HRESULT CDynamicOutputPin::DeliverEndFlush(){lastThis=this;lastMethod=57;return 57;}
HRESULT CDynamicOutputPin::Inactive(){lastThis=this;lastMethod=58;return 58;}
HRESULT CDynamicOutputPin::Active(){lastThis=this;lastMethod=59;return 59;}
HRESULT CDynamicOutputPin::CompleteConnect(IPin*){lastThis=this;lastMethod=60;return 60;}
HRESULT CDynamicOutputPin::StartUsingOutputPin(){lastThis=this;lastMethod=61;return 61;}
void CDynamicOutputPin::StopUsingOutputPin(){lastThis=this;lastMethod=62;}
bool CDynamicOutputPin::StreamingThreadUsingOutputPin(){lastThis=this;lastMethod=63;return true;}

struct FixturePin:CDynamicOutputPin {FixturePin(const char* name,CBaseFilter* filter,CCritSec* lock,HRESULT* hr,const wchar_t* pin):CDynamicOutputPin(name,filter,lock,hr,pin){}HRESULT CheckMediaType(const CMediaType*){return 0;}HRESULT DecideBufferSize(IMemAllocator*,ALLOCATOR_PROPERTIES*){return 0;}};
int main(){char name[]="native dynamic pin";wchar_t pinName[]={L'p',0};expectedName=name;expectedPin=pinName;expectedFilter=reinterpret_cast<CBaseFilter*>(size_t(0x3456));expectedLock=0;
 #ifdef _MSC_VER
 const int scenarioCount=3; // /GX assumes extern-C Win32 APIs cannot throw C++ exceptions.
#else
 const int scenarioCount=5; // host-only injected exceptions additionally exercise cleanup.
#endif
 for(scenario=0;scenario<scenarioCount;++scenario){baseCtor=baseDtor=critCtor=critDtor=eventCalls=errorCalls=objects=0;HRESULT status=47;expectedHR=&status;bool caught=false;
 try{FixturePin pin(name,expectedFilter,expectedLock,&status,pinName);assert(scenario<3);assert(baseCtor==1&&critCtor==1&&eventCalls==1&&!baseDtor&&!critDtor&&objects==1);assert(pin.m_bCanReconnectWhenActive==(scenario==0));assert(pin.m_hUnblockOutputPinEvent==(scenario==0?reinterpret_cast<HANDLE>(size_t(0x1234)):0));assert(status==(scenario==1?-777:47));assert(errorCalls==(scenario==0?0:1));
 IPin* ip=&pin;IQualityControl* iq=&pin;IPinFlowControl* flow=&pin;INonDelegatingUnknown* unknown=&pin;GUID guid;memset(&guid,0,sizeof(guid));void* out=0;
 ip->Disconnect();assert(lastThis==static_cast<CDynamicOutputPin*>(&pin)&&lastMethod==54);
 iq->Notify(0,Quality());assert(lastThis==static_cast<CBasePin*>(&pin)&&lastMethod==23);
 flow->Block(0,0);assert(lastThis==static_cast<CDynamicOutputPin*>(&pin)&&lastMethod==55);
 unknown->NonDelegatingQueryInterface(guid,&out);assert(lastThis==static_cast<CDynamicOutputPin*>(&pin)&&lastMethod==53);
#ifdef _MSC_VER
 assert(reinterpret_cast<char*>(ip)-reinterpret_cast<char*>(&pin)==0xc);assert(reinterpret_cast<char*>(iq)-reinterpret_cast<char*>(&pin)==0x10);assert(reinterpret_cast<char*>(flow)-reinterpret_cast<char*>(&pin)==0xa0);
#endif
 }catch(int code){caught=true;assert(code==(scenario==3?31:41));}
 assert(caught==(scenario>=3));assert(baseCtor==1&&baseDtor==1&&!objects);assert(critCtor==1&&critDtor==(scenario==3?0:1));assert(eventCalls==(scenario==3?0:1));if(scenario>=3)assert(status==47&&!errorCalls);
 }
 puts("00cac300_TEST PASS");return 0;}