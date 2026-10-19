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
#include <stdlib.h>
#include <new>
#ifdef _MSC_VER
#define __declspec(x) FIXTURE_DECLSPEC_##x
#define FIXTURE_DECLSPEC_novtable __declspec(novtable)
#define FIXTURE_DECLSPEC_dllimport
#endif

// DirectShow CBasePin wide-object-name constructor, native CAB370.
// SDK abstract novtable interfaces preserved; no production vtable emitted.
// Genuine SDK CBaseObject is distinct from Fable engine CBaseObject.
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
struct CBaseObject {CBaseObject(const wchar_t*);~CBaseObject();private:CBaseObject(const CBaseObject&);void operator=(const CBaseObject&);}; // release build: debug object-name/cookie fields absent
struct __declspec(novtable) CUnknown:INonDelegatingUnknown,CBaseObject {IUnknown* const m_pUnknown;volatile LONG m_cRef;CUnknown(const wchar_t*,IUnknown*);virtual ~CUnknown(){};HRESULT __stdcall NonDelegatingQueryInterface(const GUID&,void**);ULONG __stdcall NonDelegatingAddRef();ULONG __stdcall NonDelegatingRelease();};
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
 CBasePin(const wchar_t*,CBaseFilter*,CCritSec*,HRESULT*,const wchar_t*,PIN_DIRECTION); //CAB370
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
CBasePin::CBasePin(const wchar_t* name,CBaseFilter* filter,CCritSec* lock,HRESULT* hr,const wchar_t* pin,PIN_DIRECTION dir)
 :CUnknown(name,0),m_pName(0),m_Connected(0),m_dir(dir),m_pLock(lock),m_bRunTimeError(false),m_bCanReconnectWhenActive(false),m_bTryMyTypesFirst(false),m_pFilter(filter),m_pQSink(0),m_TypeVersion(1),m_tStart(),m_tStop(SDK_MAX_TIME),m_dRate(1.0)
{if(pin){int length=lstrlenW(pin)+1;int bytes=length*sizeof(wchar_t);m_pName=new wchar_t[length];if(m_pName)memcpy(m_pName,pin,bytes);}}



static int lastMethod,objects,mediaObjects,failMedia,failAllocation,lengthCalls;static void* lastThis;
static const wchar_t* expectedName;
void* operator new(size_t n) throw(std::bad_alloc){if(failAllocation){failAllocation=0;throw std::bad_alloc();}void* p=malloc(n?n:1);if(!p)throw std::bad_alloc();return p;}
void operator delete(void* p) throw(){free(p);}
#ifndef _MSC_VER
void* operator new[](size_t n) throw(std::bad_alloc){return ::operator new(n);}
void operator delete[](void* p) throw(){::operator delete(p);}
#endif
extern "C" int __stdcall lstrlenW(const wchar_t* s){++lengthCalls;int n=0;while(s[n])++n;return n;}
CBaseObject::CBaseObject(const wchar_t* n){assert(n==expectedName);++objects;}
CBaseObject::~CBaseObject(){--objects;}
CUnknown::CUnknown(const wchar_t* n,IUnknown* u):CBaseObject(n),m_pUnknown(u?u:reinterpret_cast<IUnknown*>(this)),m_cRef(0){}
CMediaType::CMediaType(){if(failMedia)throw 19;memset((AM_MEDIA_TYPE*)this,0,sizeof(AM_MEDIA_TYPE));bFixedSizeSamples=1;lSampleSize=1;++mediaObjects;}
CMediaType::~CMediaType(){--mediaObjects;}
CBasePin::~CBasePin(){::operator delete(m_pName);}
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

struct FixturePin:CBasePin {
 FixturePin(const wchar_t* n,CBaseFilter* f,CCritSec* l,HRESULT* h,const wchar_t* p,PIN_DIRECTION d):CBasePin(n,f,l,h,p,d){}
 HRESULT CheckMediaType(const CMediaType*){return 0;}
 HRESULT __stdcall BeginFlush(){return 0;}
 HRESULT __stdcall EndFlush(){return 0;}
};
int main(){
#ifdef _MSC_VER
 assert(sizeof(CBasePin)==0x98&&sizeof(CMediaType)==0x48);
#endif
 wchar_t name[]={L'o',L'b',L'j',0},pin[]={L'p',L'i',L'n',0},empty[]={0};expectedName=name;
 CBaseFilter* filter=reinterpret_cast<CBaseFilter*>(size_t(0x1234));CCritSec* lock=reinterpret_cast<CCritSec*>(size_t(0x5678));
 for(int mode=0;mode<3;++mode){HRESULT hr=-77;const wchar_t* input=mode==0?0:mode==1?empty:pin;lengthCalls=0;
  {FixturePin p(name,filter,lock,&hr,input,mode==2?PINDIR_OUTPUT:PINDIR_INPUT);
   assert(hr==-77&&objects==1&&mediaObjects==1&&lengthCalls==(input?1:0));
   assert(p.m_pFilter==filter&&p.m_pLock==lock&&!p.m_Connected&&!p.m_pQSink&&p.m_TypeVersion==1);
   assert(!p.m_bRunTimeError&&!p.m_bCanReconnectWhenActive&&!p.m_bTryMyTypesFirst);
   assert(p.m_tStart.m_time==0&&p.m_tStop.m_time==SDK_MAX_TIME&&p.m_dRate==1.0);
   assert(p.m_dir==(mode==2?PINDIR_OUTPUT:PINDIR_INPUT));
   assert(!p.m_cRef&&p.m_pUnknown==reinterpret_cast<IUnknown*>(static_cast<CUnknown*>(&p)));
   assert(p.m_mt.bFixedSizeSamples==1&&p.m_mt.lSampleSize==1&&!p.m_mt.pUnk&&!p.m_mt.cbFormat);
   if(input){assert(p.m_pName!=input);int k=0;do{assert(p.m_pName[k]==input[k]);}while(input[k++]);}else assert(!p.m_pName);
  }assert(!objects&&!mediaObjects);
 }
 failMedia=1;try{FixturePin p(name,filter,lock,0,pin,PINDIR_INPUT);assert(false);}catch(int n){assert(n==19);}failMedia=0;assert(!objects&&!mediaObjects);
 failAllocation=1;try{FixturePin p(name,filter,lock,0,pin,PINDIR_INPUT);assert(false);}catch(const std::bad_alloc&){}assert(!objects&&!mediaObjects);
 puts("CAB370 PASS");return 0;
}
