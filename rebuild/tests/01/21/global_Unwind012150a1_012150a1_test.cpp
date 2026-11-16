// 012150a1 actual shared CCodaContext parent and generated-array cleanup behavior.
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define FABLE_PORTABLE
#endif
#define private public
#define protected public
#define FABLE_RETAIL_PASS22_CCODACONTEXT_CREATE
#define FABLE_RETAIL_CCODA_HOST_ALLOCATION_PASS23
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#include "engine/CCodaContext.h"
#include "engine/CCoda2DSource.h"
#include "engine/CCoda3DSource.h"
#include "engine/CCodaListener.h"
#include "engine/CodaEnvironment.h"
#include "engine/CSalPlatform.h"
static bool bad;static int events[64],nevents,allocAttempt,failAllocation,throwType=-1,throwElement;
static int allocTypes[4],requestedCounts[4],constructed[4],destroyed[4],attempted[4],freed[4];static void*raw[4];static size_t rawSize[4];
static const size_t Strides[4]={sizeof(CCoda2DSource),sizeof(CCoda3DSource),sizeof(CCodaListener),sizeof(CCodaEnvironment)};
static CCodaObjectList*Lists[4];static unsigned int ListStrides[4];
static void Event(int x){if(nevents>=64)bad=true;else events[nevents++]=x;}
struct CtorFailure{};
static void Construct(int t){++attempted[t];
#if !defined(_MSC_VER) || defined(_CPPUNWIND)
 if(t==throwType&&attempted[t]==throwElement)throw CtorFailure();
#endif
 ++constructed[t];Event(200+t);
}
static void Destruct(int t){++destroyed[t];}
extern "C" void* __cdecl CodaMalloc(unsigned int n){int attempt=++allocAttempt;if(attempt>4){bad=true;return 0;}int t=allocTypes[attempt-1];Event(100+t);if(n<(size_t)requestedCounts[t]*Strides[t]||n>(size_t)requestedCounts[t]*Strides[t]+16)bad=true;if(attempt==failAllocation)return 0;raw[t]=malloc(n);rawSize[t]=n;return raw[t];}
extern "C" void __cdecl CodaFree(void*p){if(!p)return;for(int t=0;t<4;++t)if(raw[t]==p){Event(400+t);++freed[t];free(p);raw[t]=0;return;}bad=true;}
CCodaObjectBase::~CCodaObjectBase(){} CCodaObject::~CCodaObject(){} CCodaSource::~CCodaSource(){}
CCoda2DSource::CCoda2DSource(){Construct(0);} CCoda2DSource::~CCoda2DSource(){Destruct(0);}
CCoda3DSource::CCoda3DSource(){Construct(1);} CCoda3DSource::~CCoda3DSource(){Destruct(1);}
CCodaListener::CCodaListener(){Construct(2);} CCodaListener::~CCodaListener(){Destruct(2);}
CCodaEnvironment::CCodaEnvironment(){Construct(3);} CCodaEnvironment::~CCodaEnvironment(){Destruct(3);}
#define UNUSED_QUERY(owner,name) bool owner::name()const{abort();return false;}
UNUSED_QUERY(CCodaObject,IsBuffer) UNUSED_QUERY(CCodaObject,IsContext) UNUSED_QUERY(CCodaObject,IsCreated)
UNUSED_QUERY(CCodaObject,IsEnvironment) UNUSED_QUERY(CCodaObject,IsEAX) UNUSED_QUERY(CCodaObject,IsListener)
UNUSED_QUERY(CCodaObject,IsSource) UNUSED_QUERY(CCodaObject,IsReverb) UNUSED_QUERY(CCodaObject,IsVoice)
UNUSED_QUERY(CCodaSource,IsSource) UNUSED_QUERY(CCodaSource,Is3D) UNUSED_QUERY(CCodaSource,IsCreated)
UNUSED_QUERY(CCoda3DSource,Is3D)
bool CCodaSource::Create(unsigned int){abort();return false;}
#undef UNUSED_QUERY
void CCodaObjectList::CreateStatic(CCodaObject*p,unsigned int stride,unsigned int count){int t=0;while(t<4&&Lists[t]!=this)++t;if(t==4){bad=true;return;}Event(300+t);if(!p||stride!=Strides[t]||count!=(unsigned int)requestedCounts[t])bad=true;m_pObjects=p;m_pFree=p;m_nFree=(int)count;m_nObjects=count;ListStrides[t]=stride;}
static const CRESOURCEDESC*expectedResources;static const CPLATFORMDESC*expectedPlatform;static CSalPlatform**expectedOut;static CSalPlatform platform;static int platformError;
int CSalContext::Create(const CRESOURCEDESC*r,const CPLATFORMDESC*p,CSalPlatform**out){Event(1);if(r!=expectedResources||p!=expectedPlatform||out!=expectedOut)bad=true;*out=&platform;return platformError;}
CCodaContext CODA;int CODA_Error;static int callbackError;
static void FABLE_STDCALL Callback(int n){Event(81);callbackError=n;}
static void FABLE_STDCALL InstanceCallback(int){bad=true;}
// The EAX field is opaque to this method: the marker is compared only, never used as an object.

bool CCodaContext::Create(const CRESOURCEDESC* pcResources,const CPLATFORMDESC* pcPlatform)
{
    int nError=CSalContext::Create(pcResources,pcPlatform,&m_pSal);
    if(nError) {
        CODA.SetError(nError);
        return false;
    }
    if(pcResources) {
        m_nMax2DSources=pcResources->n2DSources;
        m_nMax3DSources=pcResources->n3DSources;
        m_nMaxListeners=pcResources->nListeners;
        m_nMaxEnvironments=pcResources->nEnvironments;
    }
    if(m_nMax2DSources>0) {
        m_2DSourceMem=new CCoda2DSource[m_nMax2DSources];
        if(!m_2DSourceMem)return false;
        m_2DSources.CreateStatic(m_2DSourceMem,sizeof(CCoda2DSource),m_nMax2DSources);
    }
    if(m_nMax3DSources>0) {
        m_3DSourceMem=new CCoda3DSource[m_nMax3DSources];
        if(!m_3DSourceMem)return false;
        m_3DSources.CreateStatic(m_3DSourceMem,sizeof(CCoda3DSource),m_nMax3DSources);
    }
    if(m_nMaxListeners>0) {
        m_ListenerMem=new CCodaListener[m_nMaxListeners];
        if(!m_ListenerMem)return false;
        m_Listeners.CreateStatic(m_ListenerMem,sizeof(CCodaListener),m_nMaxListeners);
    }
    if(m_nMaxEnvironments>0) {
        m_EnvironmentMem=new CCodaEnvironment[m_nMaxEnvironments];
        if(!m_EnvironmentMem)return false;
        m_Environments.CreateStatic(m_EnvironmentMem,sizeof(CCodaEnvironment),m_nMaxEnvironments);
    }
    m_EAX=0;
    return true;
}


static void Reset(){bad=false;nevents=0;allocAttempt=0;throwType=-1;throwElement=0;callbackError=0;memset(constructed,0,sizeof(constructed));memset(destroyed,0,sizeof(destroyed));memset(attempted,0,sizeof(attempted));memset(freed,0,sizeof(freed));memset(raw,0,sizeof(raw));memset(rawSize,0,sizeof(rawSize));}
static void Init(CCodaContext&e,const int*c,CCodaEAX*sentinel){e.m_pSal=0;e.m_2DSourceMem=0;e.m_3DSourceMem=0;e.m_ListenerMem=0;e.m_EnvironmentMem=0;e.m_EAX=sentinel;e.m_nMax2DSources=c[0];e.m_nMax3DSources=c[1];e.m_nMaxListeners=c[2];e.m_nMaxEnvironments=c[3];CCodaObjectList*l[4]={&e.m_2DSources,&e.m_3DSources,&e.m_Listeners,&e.m_Environments};for(int t=0;t<4;++t){l[t]->m_pObjects=0;l[t]->m_pFree=0;l[t]->m_nFree=0;l[t]->m_nObjects=0;Lists[t]=l[t];ListStrides[t]=0;}e.m_pfnErrorCallback=InstanceCallback;}
static void Cleanup(CCodaContext&e){delete[]e.m_2DSourceMem;delete[]e.m_3DSourceMem;delete[]e.m_ListenerMem;delete[]e.m_EnvironmentMem;}
static int Fail(int id,const char*s){printf("012150a1_TEST FAIL %d %s\n",id,s);return 1;}
int main(){
#ifdef _MSC_VER
 assert(sizeof(CCodaContext)==168 && Strides[0]==444 && Strides[1]==476 && Strides[2]==76 && Strides[3]==128);
#endif
 const int patterns[10][4]={{0,0,0,0},{-1,-1,-1,-1},{1,1,1,1},{2,2,2,2},{2,0,1,-1},{0,2,-1,1},{0,0,0,2},{1,-1,2,0},{0,1,0,0},{-1,0,0,1}};
 const int errors[3]={0,13,-7};CPLATFORMDESC p;memset(&p,0,sizeof p);char marker=0;CCodaEAX*sentinel=reinterpret_cast<CCodaEAX*>(&marker);int id=0;
 for(int pat=0;pat<10;++pat)for(int err=0;err<3;++err)for(int latch=0;latch<2;++latch)for(int hook=0;hook<2;++hook)for(int failure=0;failure<5;++failure)for(int provided=0;provided<2;++provided,++id){
  Reset();CCodaContext e;const int*initial=patterns[(pat+3)%10];Init(e,initial,sentinel);CRESOURCEDESC resource;resource.n2DSources=patterns[pat][0];resource.n3DSources=patterns[pat][1];resource.nListeners=patterns[pat][2];resource.nEnvironments=patterns[pat][3];expectedResources=provided?&resource:0;expectedPlatform=&p;expectedOut=&e.m_pSal;platformError=errors[err];CODA_Error=latch?42:0;CODA.m_pfnErrorCallback=hook?Callback:0;failAllocation=failure;
  const int*counts=provided?patterns[pat]:initial;int available=0;
  for(int t=0;t<4;++t){requestedCounts[t]=counts[t];if(counts[t]>0)allocTypes[available++]=t;}
  bool success=platformError==0;int expectCtor[4]={0,0,0,0};int wanted[64],nwanted=0;wanted[nwanted++]=1;int calls=0;
  if(platformError){if(!latch&&hook)wanted[nwanted++]=81;}
  else for(int t=0;t<4;++t)if(counts[t]>0){wanted[nwanted++]=100+t;++calls;if(calls==failure){success=false;break;}expectCtor[t]=counts[t];for(int j=0;j<counts[t];++j)wanted[nwanted++]=200+t;wanted[nwanted++]=300+t;}
  bool actual=e.Create(expectedResources,&p);
  if(actual!=success||bad||e.m_pSal!=&platform)return Fail(id,"result/boundaries");
  if(nevents!=nwanted)return Fail(id,"event count");
  for(int k=0;k<nwanted;++k)if(events[k]!=wanted[k])return Fail(id,"event order");
  if(allocAttempt!=(platformError?0:calls))return Fail(id,"allocation order");
  if(CODA_Error!=(latch?42:platformError)||callbackError!=((platformError&&!latch&&hook)?platformError:0))return Fail(id,"global error latch");
  int actualCounts[4]={e.m_nMax2DSources,e.m_nMax3DSources,e.m_nMaxListeners,e.m_nMaxEnvironments};CCodaObjectList*lists[4]={&e.m_2DSources,&e.m_3DSources,&e.m_Listeners,&e.m_Environments};CCodaObject*ptrs[4]={e.m_2DSourceMem,e.m_3DSourceMem,e.m_ListenerMem,e.m_EnvironmentMem};
  for(int t=0;t<4;++t){if(actualCounts[t]!=(platformError?initial[t]:counts[t]))return Fail(id,"limit copy");if(constructed[t]!=expectCtor[t]||destroyed[t]!=0||freed[t]!=0)return Fail(id,"normal constructor/lifetime");if((ptrs[t]!=0)!=(expectCtor[t]>0)||lists[t]->m_pObjects!=ptrs[t]||lists[t]->m_nObjects!=(unsigned int)expectCtor[t])return Fail(id,"static resource registration");}
  if(e.m_EAX!=(success?0:sentinel))return Fail(id,"success-only EAX reset");
  Cleanup(e);for(int t=0;t<4;++t)if(raw[t]||destroyed[t]!=constructed[t])return Fail(id,"fixture cleanup");
 }
 if(id!=1200)return Fail(id,"coverage");
#if !defined(_MSC_VER) || defined(_CPPUNWIND)
 for(int type=0;type<4;++type)for(int ordinal=1;ordinal<=2;++ordinal){
  Reset();CCodaContext e;Init(e,patterns[3],sentinel);CRESOURCEDESC r;r.n2DSources=r.n3DSources=r.nListeners=r.nEnvironments=2;expectedResources=&r;expectedPlatform=&p;expectedOut=&e.m_pSal;platformError=0;failAllocation=0;CODA_Error=0;CODA.m_pfnErrorCallback=0;throwType=type;throwElement=ordinal;for(int t=0;t<4;++t){allocTypes[t]=t;requestedCounts[t]=2;}
  bool caught=false;try{e.Create(&r,&p);}catch(const CtorFailure&){caught=true;}
  if(!caught||bad||allocAttempt!=type+1||e.m_EAX!=sentinel)return Fail(id,"constructor throw state");
  CCodaObjectList*lists[4]={&e.m_2DSources,&e.m_3DSources,&e.m_Listeners,&e.m_Environments};
  for(int t=0;t<4;++t){int complete=t<type?2:t==type?ordinal-1:0;if(constructed[t]!=complete||destroyed[t]!=(t==type?complete:0)||freed[t]!=(t==type?1:0)||lists[t]->m_nObjects!=(t<type?2u:0u))return Fail(id,"array unwind boundary");}
  Cleanup(e);for(int t=0;t<4;++t)if(raw[t]||destroyed[t]!=constructed[t])return Fail(id,"throw fixture cleanup");++id;
 }
#endif
 puts("012150a1_TEST PASS");return 0;
}