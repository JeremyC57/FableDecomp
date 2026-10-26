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