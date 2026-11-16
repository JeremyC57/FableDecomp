#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __int64 long long
#define FABLE_PORTABLE
#endif
#define __declspec(x)
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
// 00ca97a0 CDynamicOutputPin::Disconnect
// Flags: /EHsc
// Leaf: ?Disconnect@CDynamicOutputPin@@UAGJXZ
// Original SDK amfilter.cpp body; native IPin vtable slot5 and EH parent verified.
#define FABLE_RETAIL_CDYNAMICOUTPUTPIN_PASS23
#include "rebuild_abi.h"
#include "engine/CDynamicOutputPin.h"
long __stdcall CBasePin::DisconnectInternal()
{
    if(m_Connected) {
        long hr=BreakConnect();
        if(hr<0)return hr;
        m_Connected->Release();
        m_Connected=0;
        return S_OK;
    } else {
        return S_FALSE;
    }
}
long __stdcall CDynamicOutputPin::Disconnect()
{
    CAutoLock cObjectLock(m_pLock);
    return DisconnectInternal();
}

#undef __declspec
static void* ExpectedLock;
static int LockDepth,Enters,Leaves,Breaks,Releases,FailPoint;
static long BreakResult;
static char Events[16];static unsigned EventCount;
static IPin* Replacement;static IPin* Released;
static void Event(char n){assert(EventCount<15);Events[EventCount++]=n;Events[EventCount]=0;}
extern "C" void __stdcall EnterCriticalSection(CRITICAL_SECTION* p){assert((void*)p==ExpectedLock&&!LockDepth);++LockDepth;++Enters;Event('E');}
extern "C" void __stdcall LeaveCriticalSection(CRITICAL_SECTION* p){assert((void*)p==ExpectedLock&&LockDepth==1);--LockDepth;++Leaves;Event('L');}
struct FixtureException {};
static void MaybeThrow(int point){
#if !defined(_MSC_VER)||defined(_CPPUNWIND)
 if(FailPoint==point)throw FixtureException();
#else
 assert(!FailPoint);(void)point;
#endif
}
CMyBaseObject::CMyBaseObject(const unsigned short*){}
CMyBaseObject::~CMyBaseObject(){}
CUnknown::CUnknown(const unsigned short* name,IUnknown* unk):CMyBaseObject(name),m_pUnknown(unk),m_cRef(0){}
CUnknown::~CUnknown(){}
long __stdcall CUnknown::NonDelegatingQueryInterface(const GUID&,void**){abort();}
unsigned long __stdcall CUnknown::NonDelegatingAddRef(){abort();}
unsigned long __stdcall CUnknown::NonDelegatingRelease(){abort();}
CMediaType::CMediaType(){memset(static_cast<_AMMediaType*>(this),0,sizeof(_AMMediaType));}
CMediaType::~CMediaType(){}
CBasePin::CBasePin(const unsigned short* name,CBaseFilter* filter,CCritSec* lock,long* result,const unsigned short*,PIN_DIRECTION direction):CUnknown(name,0),m_pName(0),m_Connected(0),m_dir(direction),m_pLock(lock),m_bRunTimeError(false),m_bCanReconnectWhenActive(false),m_bTryMyTypesFirst(false),m_pFilter(filter),m_pQSink(0),m_TypeVersion(0),m_dRate(1.0){if(result)*result=0;}
CBasePin::~CBasePin(){}
CBaseOutputPin::CBaseOutputPin(const unsigned short* name,CBaseFilter* filter,CCritSec* lock,long* result,const unsigned short* pinname):CBasePin(name,filter,lock,result,pinname,PINDIR_OUTPUT),m_pAllocator(0),m_pInputPin(0){}
CDynamicOutputPin::CDynamicOutputPin(const unsigned short* name,CBaseFilter* filter,CCritSec* lock,long* result,const unsigned short* pinname):CBaseOutputPin(name,filter,lock,result,pinname),m_hUnblockOutputPinEvent(0),m_hNotifyCallerPinBlockedEvent(0),m_BlockState(NOT_BLOCKED),m_dwBlockCallerThreadID(0),m_dwNumOutstandingOutputPinUsers(0),m_hStopEvent(0),m_pGraphConfig(0),m_bPinUsesReadOnlyAllocator(0){}
CDynamicOutputPin::~CDynamicOutputPin(){}
long __stdcall CBasePin::QueryInterface(const GUID&,void**){abort();}
unsigned long __stdcall CBasePin::AddRef(){abort();}
unsigned long __stdcall CBasePin::Release(){abort();}
long __stdcall CBasePin::NonDelegatingQueryInterface(const GUID&,void**){abort();}
unsigned long __stdcall CBasePin::NonDelegatingRelease(){abort();}
unsigned long __stdcall CBasePin::NonDelegatingAddRef(){abort();}
long __stdcall CBasePin::Connect(IPin*,const _AMMediaType*){abort();}
long __stdcall CBasePin::ReceiveConnection(IPin*,const _AMMediaType*){abort();}
long __stdcall CBasePin::Disconnect(){abort();}
long __stdcall CBasePin::ConnectedTo(IPin**){abort();}
long __stdcall CBasePin::ConnectionMediaType(_AMMediaType*){abort();}
long __stdcall CBasePin::QueryPinInfo(_PinInfo*){abort();}
long __stdcall CBasePin::QueryDirection(PIN_DIRECTION*){abort();}
long __stdcall CBasePin::QueryId(unsigned short**){abort();}
long __stdcall CBasePin::QueryAccept(const _AMMediaType*){abort();}
long __stdcall CBasePin::EnumMediaTypes(IEnumMediaTypes**){abort();}
long __stdcall CBasePin::QueryInternalConnections(IPin**,unsigned long*){abort();}
long __stdcall CBasePin::EndOfStream(){abort();}
long __stdcall CBasePin::NewSegment(REFERENCE_TIME,REFERENCE_TIME,double){abort();}
long __stdcall CBasePin::Notify(IBaseFilter*,Quality){abort();}
long __stdcall CBasePin::SetSink(IQualityControl*){abort();}
long CBasePin::GetMediaTypeVersion(){abort();}
long CBasePin::Active(){abort();}
long CBasePin::Inactive(){abort();}
long CBasePin::Run(REFERENCE_TIME){abort();}
long CBasePin::SetMediaType(const CMediaType*){abort();}
long CBasePin::CheckConnect(IPin*){abort();}
long CBasePin::BreakConnect(){abort();}
long CBasePin::CompleteConnect(IPin*){abort();}
long CBasePin::GetMediaType(int,CMediaType*){abort();}
long CBaseOutputPin::CompleteConnect(IPin*){abort();}
long CBaseOutputPin::DecideAllocator(IMemInputPin*,IMemAllocator**){abort();}
long CBaseOutputPin::GetDeliveryBuffer(IMediaSample**,REFERENCE_TIME*,REFERENCE_TIME*,unsigned long){abort();}
long CBaseOutputPin::Deliver(IMediaSample*){abort();}
long CBaseOutputPin::InitAllocator(IMemAllocator**){abort();}
long CBaseOutputPin::CheckConnect(IPin*){abort();}
long CBaseOutputPin::BreakConnect(){abort();}
long CBaseOutputPin::Active(){abort();}
long CBaseOutputPin::Inactive(){abort();}
long __stdcall CBaseOutputPin::EndOfStream(){abort();}
long CBaseOutputPin::DeliverEndOfStream(){abort();}
long __stdcall CBaseOutputPin::BeginFlush(){abort();}
long __stdcall CBaseOutputPin::EndFlush(){abort();}
long CBaseOutputPin::DeliverBeginFlush(){abort();}
long CBaseOutputPin::DeliverEndFlush(){abort();}
long CBaseOutputPin::DeliverNewSegment(REFERENCE_TIME,REFERENCE_TIME,double){abort();}
long __stdcall CDynamicOutputPin::QueryInterface(const GUID&,void**){abort();}
unsigned long __stdcall CDynamicOutputPin::AddRef(){abort();}
unsigned long __stdcall CDynamicOutputPin::Release(){abort();}
long __stdcall CDynamicOutputPin::NonDelegatingQueryInterface(const GUID&,void**){abort();}
long __stdcall CDynamicOutputPin::Block(unsigned long,void*){abort();}
void CDynamicOutputPin::SetConfigInfo(IGraphConfig*,void*){abort();}
long CDynamicOutputPin::DeliverBeginFlush(){abort();}
long CDynamicOutputPin::DeliverEndFlush(){abort();}
long CDynamicOutputPin::Inactive(){abort();}
long CDynamicOutputPin::Active(){abort();}
long CDynamicOutputPin::CompleteConnect(IPin*){abort();}
long CDynamicOutputPin::StartUsingOutputPin(){abort();}
void CDynamicOutputPin::StopUsingOutputPin(){abort();}
bool CDynamicOutputPin::StreamingThreadUsingOutputPin(){abort();}

class TestPin : public CDynamicOutputPin {
public:
 TestPin(CCritSec* lock):CDynamicOutputPin(0,0,lock,0,0){}
 void SetPeer(IPin* peer){m_Connected=peer;}
 IPin* Peer()const{return m_Connected;}
 long CheckMediaType(const CMediaType*){abort();}
 long DecideBufferSize(IMemAllocator*,_AllocatorProperties*){abort();}
 long BreakConnect(){assert(LockDepth==1);++Breaks;Event('B');MaybeThrow(1);if(Replacement)m_Connected=Replacement;return BreakResult;}
 unsigned long __stdcall Release(){assert(LockDepth==1);++Releases;Released=static_cast<IPin*>(this);Event('R');MaybeThrow(2);return 1;}
};
static void Reset(){assert(!LockDepth);Enters=Leaves=Breaks=Releases=FailPoint=0;BreakResult=0;EventCount=0;Events[0]=0;Replacement=Released=0;}
int main(){CCritSec lock;ExpectedLock=&lock;TestPin pin(&lock),peer(&lock),other(&lock);unsigned cases=0;
#ifdef _MSC_VER
 assert(sizeof(CBasePin)==0x98&&sizeof(CBaseOutputPin)==0xa0&&sizeof(CDynamicOutputPin)==0xe0);
 assert((char*)static_cast<IPin*>(&pin)-(char*)&pin==12);
#endif
 const long results[]={-2147467259L,-1L,0L,1L,7L};
 for(int repeat=0;repeat<17;++repeat)for(int connected=0;connected<2;++connected)for(int result=0;result<5;++result)for(int replace=0;replace<2;++replace){
  Reset();pin.SetPeer(connected?static_cast<IPin*>(&peer):0);BreakResult=results[result];Replacement=replace?static_cast<IPin*>(&other):0;
  long actual=static_cast<IPin*>(&pin)->Disconnect();
  assert(Enters==1&&Leaves==1&&!LockDepth&&Breaks==connected);
  if(!connected){assert(actual==S_FALSE&&!pin.Peer()&&!Releases&&!Released);assert(!strcmp(Events,"EL"));}
  else if(BreakResult<0){assert(actual==BreakResult&&pin.Peer()==(Replacement?Replacement:static_cast<IPin*>(&peer))&&!Releases&&!Released);assert(!strcmp(Events,"EBL"));}
  else {assert(actual==S_OK&&!pin.Peer()&&Releases==1&&Released==(Replacement?Replacement:static_cast<IPin*>(&peer)));assert(!strcmp(Events,"EBRL"));}
  ++cases;
 }
 assert(cases==340u);
#if !defined(_MSC_VER)||defined(_CPPUNWIND)
 for(int fault=1;fault<=2;++fault){Reset();pin.SetPeer(&peer);FailPoint=fault;bool caught=false;try{static_cast<IPin*>(&pin)->Disconnect();}catch(const FixtureException&){caught=true;}assert(caught&&Enters==1&&Leaves==1&&!LockDepth&&pin.Peer()==static_cast<IPin*>(&peer));assert(!strcmp(Events,fault==1?"EBL":"EBRL"));}
#endif
 puts("01216ce0_TEST PASS");return 0;
}