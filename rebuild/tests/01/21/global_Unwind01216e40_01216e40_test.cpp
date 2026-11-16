#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __int64 long long
#define FABLE_PORTABLE
#endif
// Test-only replacement for imported API declarations; real SDK classes remain.
#define __declspec(x)
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
// 00caa860 CBaseMediaFilter::SetSyncSource -- original parent for unwind01216e00
// Flags: /EHsc
// Leaf: ?SetSyncSource@CBaseMediaFilter@@UAGJPAUIReferenceClock@@@Z
// Retail: parent00caa860,109bytes. SDK source and native EH/vtable evidence.
#define FABLE_RETAIL_CBASEMEDIAFILTER_PASS23
#include "rebuild_abi.h"
#include "engine/CBaseMediaFilter.h"
long __stdcall CBaseMediaFilter::SetSyncSource(IReferenceClock* pClock)
{
    CAutoLock cObjectLock(m_pLock);
    if(pClock)pClock->AddRef();
    if(m_pClock)m_pClock->Release();
    m_pClock=pClock;
    return NOERROR;
}

// 00caa8d0 CBaseMediaFilter::GetSyncSource -- original parent for unwind01216e20
// Flags: /EHsc
// Leaf: ?GetSyncSource@CBaseMediaFilter@@UAGJPAPAUIReferenceClock@@@Z
// Retail: parent00caa8d0,130bytes. SDK source and native EH/vtable evidence.
#define FABLE_RETAIL_CBASEMEDIAFILTER_PASS23
#include "rebuild_abi.h"
#include "engine/CBaseMediaFilter.h"
long __stdcall CBaseMediaFilter::GetSyncSource(IReferenceClock** pClock)
{
    CheckPointer(pClock,E_POINTER);
    CAutoLock cObjectLock(m_pLock);
    if(m_pClock)m_pClock->AddRef();
    *pClock=m_pClock;
    return NOERROR;
}

// 00caa9c0 CBaseMediaFilter::Run -- original parent for unwind01216e40
// Flags: /EHsc
// Leaf: ?Run@CBaseMediaFilter@@UAGJ_J@Z
// Retail: parent00caa9c0,148bytes. SDK source and native EH/vtable evidence.
#define FABLE_RETAIL_CBASEMEDIAFILTER_PASS23
#include "rebuild_abi.h"
#include "engine/CBaseMediaFilter.h"
long __stdcall CBaseMediaFilter::Run(REFERENCE_TIME tStart)
{
    CAutoLock cObjectLock(m_pLock);
    m_tStart=tStart;
    if(m_State==State_Stopped){
        long hr=Pause();
        if(hr<0)return hr;
    }
    m_State=State_Running;
    return S_OK;
}

#undef __declspec
static void* ExpectedLock;
static int LockDepth,Enters,Leaves,PauseCalls,FailPoint;
static long PauseResult;
static unsigned int Events[12],EventCount;
static void Event(unsigned int n){assert(EventCount<12);Events[EventCount++]=n;}
extern "C" void __stdcall EnterCriticalSection(CRITICAL_SECTION* p){assert((void*)p==ExpectedLock);++LockDepth;++Enters;Event(1);}
extern "C" void __stdcall LeaveCriticalSection(CRITICAL_SECTION* p){assert((void*)p==ExpectedLock && LockDepth==1);--LockDepth;++Leaves;Event(2);}
static void Reset(){assert(!LockDepth);Enters=Leaves=PauseCalls=FailPoint=0;EventCount=0;PauseResult=0;}
struct FixtureException {};
static void MaybeThrow(int n){
#if !defined(_MSC_VER) || defined(_CPPUNWIND)
 if(FailPoint==n)throw FixtureException();
#else
 assert(FailPoint==0);(void)n;
#endif
}
CMyBaseObject::CMyBaseObject(const unsigned short*){}
CMyBaseObject::~CMyBaseObject(){}
CUnknown::CUnknown(const unsigned short* n,IUnknown* p):CMyBaseObject(n),m_pUnknown(p),m_cRef(0){}
CUnknown::~CUnknown(){}
long __stdcall CUnknown::NonDelegatingQueryInterface(const GUID&,void**){abort();}
unsigned long __stdcall CUnknown::NonDelegatingAddRef(){abort();}
unsigned long __stdcall CUnknown::NonDelegatingRelease(){abort();}
CBaseMediaFilter::CBaseMediaFilter(const unsigned short* n,IUnknown* p,CCritSec* lock,const GUID& id):
 CUnknown(n,p),m_State(State_Stopped),m_pClock(0),m_clsid(id),m_pLock(lock){}
CBaseMediaFilter::~CBaseMediaFilter(){assert(!m_pClock);}
long __stdcall CBaseMediaFilter::QueryInterface(const GUID&,void**){abort();}
unsigned long __stdcall CBaseMediaFilter::AddRef(){abort();}
unsigned long __stdcall CBaseMediaFilter::Release(){abort();}
long __stdcall CBaseMediaFilter::NonDelegatingQueryInterface(const GUID&,void**){abort();}
long __stdcall CBaseMediaFilter::GetClassID(GUID*){abort();}
long __stdcall CBaseMediaFilter::GetState(unsigned long,FILTER_STATE*){abort();}
long __stdcall CBaseMediaFilter::Stop(){abort();}
long __stdcall CBaseMediaFilter::Pause(){abort();}
long CBaseMediaFilter::StreamTime(CRefTime&){abort();}
class FixtureClock:public IReferenceClock {
public:
 unsigned long Refs;int Adds,Releases;
 FixtureClock():Refs(1),Adds(0),Releases(0){}
 long __stdcall QueryInterface(const GUID&,void**){abort();}
 unsigned long __stdcall AddRef(){assert(LockDepth==1);Event(3);MaybeThrow(1);++Adds;return ++Refs;}
 unsigned long __stdcall Release(){assert(LockDepth==1 && Refs);Event(4);MaybeThrow(2);++Releases;return --Refs;}
 long __stdcall GetTime(REFERENCE_TIME*){abort();}
 long __stdcall AdviseTime(REFERENCE_TIME,REFERENCE_TIME,unsigned long,unsigned long*){abort();}
 long __stdcall AdvisePeriodic(REFERENCE_TIME,REFERENCE_TIME,unsigned long,unsigned long*){abort();}
 long __stdcall Unadvise(unsigned long){abort();}
};
class FixtureFilter:public CBaseMediaFilter {
public:
 FixtureFilter(CCritSec* p,const GUID& id):CBaseMediaFilter(0,0,p,id){}
 void SetClock(IReferenceClock* p){m_pClock=p;}
 IReferenceClock* Clock()const{return m_pClock;}
 void SetState(FILTER_STATE s){m_State=s;}
 FILTER_STATE State()const{return m_State;}
 REFERENCE_TIME Start()const{return m_tStart;}
 long __stdcall Pause(){assert(LockDepth==1);++PauseCalls;Event(5);MaybeThrow(3);if(PauseResult>=0)m_State=State_Paused;return PauseResult;}
};
int main(){
 CCritSec lock;ExpectedLock=&lock;GUID id={0};FixtureFilter filter(&lock,id);unsigned int cases=0;
 for(int oldClock=0;oldClock<3;++oldClock)for(int newClock=0;newClock<3;++newClock)for(int refs=1;refs<=4;++refs){
  FixtureClock a,b;a.Refs=b.Refs=(unsigned long)refs;IReferenceClock* clocks[3]={0,&a,&b};Reset();filter.SetClock(clocks[oldClock]);
  assert(filter.SetSyncSource(clocks[newClock])==0 && filter.Clock()==clocks[newClock]);
  assert(a.Refs==(unsigned long)(refs+(newClock==1)-(oldClock==1)) && b.Refs==(unsigned long)(refs+(newClock==2)-(oldClock==2)));
  assert(a.Adds==(newClock==1) && b.Adds==(newClock==2) && a.Releases==(oldClock==1) && b.Releases==(oldClock==2));
  unsigned int k=0;assert(Events[k++]==1);if(newClock)assert(Events[k++]==3);if(oldClock)assert(Events[k++]==4);assert(Events[k++]==2 && k==EventCount);
  assert(!LockDepth && Enters==1 && Leaves==1);filter.SetClock(0);++cases;
 }
 for(int present=0;present<2;++present)for(int nullOutput=0;nullOutput<2;++nullOutput){
  FixtureClock clock;Reset();filter.SetClock(present?&clock:0);IReferenceClock* result=&clock;
  long hr=filter.GetSyncSource(nullOutput?0:&result);
  if(nullOutput){assert(hr==E_POINTER && !Enters && !Leaves && clock.Refs==1);}
  else{assert(hr==0 && result==(present?&clock:0) && clock.Refs==(unsigned long)(1+present) && Enters==1 && Leaves==1 && !LockDepth);}
  filter.SetClock(0);++cases;
 }
 for(int state=0;state<3;++state)for(int result=0;result<3;++result)for(int time=0;time<4;++time){
  Reset();filter.SetState((FILTER_STATE)state);PauseResult=result==0?0:result==1?1:-7;
  REFERENCE_TIME start=time==0?0:time==1?17:time==2?-29:(((REFERENCE_TIME)3<<32)+5);
  long hr=filter.Run(start);assert(filter.Start()==start && PauseCalls==(state==0));
  assert(hr==((state==0 && PauseResult<0)?PauseResult:0));
  assert(filter.State()==((state==0 && PauseResult<0)?State_Stopped:State_Running));
  assert(Enters==1 && Leaves==1 && LockDepth==0);++cases;
 }
 assert(cases==76);
#if !defined(_MSC_VER) || defined(_CPPUNWIND)
 for(int mode=1;mode<=4;++mode){
  FixtureClock a,b;Reset();filter.SetClock(&a);filter.SetState(State_Stopped);FailPoint=mode==4?3:mode==3?1:mode;bool caught=false;
  try{if(mode==4)filter.Run(31);else if(mode==3){IReferenceClock* out=0;filter.GetSyncSource(&out);}else filter.SetSyncSource(&b);}
  catch(const FixtureException&){caught=true;}
  assert(caught && Enters==1 && Leaves==1 && LockDepth==0);filter.SetClock(0);
 }
#endif
 puts("01216e40_TEST PASS");return 0;
}