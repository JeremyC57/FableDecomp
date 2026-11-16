// Self-contained actual shared classes and source; external operations are controlled.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define FABLE_PORTABLE
#define __fastcall
#define __stdcall
#define __cdecl
#define __int64 long long
#define __declspec(x)
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_CCODA_HOST_ALLOCATION_PASS23
#define private public
#define protected public
// 00c3f800 CSalPlatform::CreateSwVoices
// Flags: /EHsc
// Leaf: ?CreateSwVoices@CSalPlatform@@AAE_NXZ
#define FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
#define FABLE_RETAIL_PASS22_CCODACONTEXT_CREATE
#include "engine/CSalPlatform.h"
#include "engine/CSalSw2DVoice.h"
#include "engine/CSalSw3DVoice.h"
#include "engine/CodaCaps.h"
// Layout: native array strides250/248; genuine six-channel history and gain arrays.
FABLE_STATIC_ASSERT(sizeof(CSalSw2DVoice)==0x250);
FABLE_STATIC_ASSERT(sizeof(CSalSw3DVoice)==0x248);
// SDK2.10 changes this method's bool result into an int error code. Native caller tests AL.
bool CSalPlatform::CreateSwVoices()
{
 if(g_SalCaps.SW2DVoice.nVoices>0){
  m_Sw2DVoiceMem=new CSalSw2DVoice[g_SalCaps.SW2DVoice.nVoices];
  if(!m_Sw2DVoiceMem){
   g_SalCaps.SW2DVoice.nVoices=0;
   g_SalCaps.SW2DVoice.nFreeVoices=0;
   g_SalCaps.SW3DVoice.nVoices=0;
   g_SalCaps.SW3DVoice.nFreeVoices=0;
   return false;
  }
  m_Sw2DVoices.CreateStatic(m_Sw2DVoiceMem,sizeof(CSalSw2DVoice),g_SalCaps.SW2DVoice.nVoices);
 }
 if(g_SalCaps.SW3DVoice.nVoices>0){
  m_Sw3DVoiceMem=new CSalSw3DVoice[g_SalCaps.SW3DVoice.nVoices];
  if(!m_Sw3DVoiceMem){
   g_SalCaps.SW3DVoice.nVoices=0;
   g_SalCaps.SW3DVoice.nFreeVoices=0;
   return false;
  }
  m_Sw3DVoices.CreateStatic(m_Sw3DVoiceMem,sizeof(CSalSw3DVoice),g_SalCaps.SW3DVoice.nVoices);
 }
 return true;
}


#undef private
#undef protected
static void check(bool good){if(!good){fputs("CreateSwVoices assertion failed\n",stderr);exit(1);}}
_CCAPS g_SalCaps;
static CSalPlatform* active;
static int alloc_calls,free_calls,fail_alloc,registered2,registered3,created2,created3,destroyed2,destroyed3,throw_kind,throw_index;
static void* raw_blocks[2];static unsigned int raw_sizes[2];
extern "C" void* __cdecl CodaMalloc(unsigned int n){check(alloc_calls<2);int i=alloc_calls++;raw_sizes[i]=n;if(alloc_calls==fail_alloc){raw_blocks[i]=0;return 0;}raw_blocks[i]=malloc(n);check(raw_blocks[i]!=0);return raw_blocks[i];}
extern "C" void __cdecl CodaFree(void* p){if(!p)return;int found=-1;for(int i=0;i<alloc_calls;++i)if(raw_blocks[i]==p)found=i;check(found>=0);raw_blocks[found]=0;++free_calls;free(p);}
CCodaObjectBase::~CCodaObjectBase(){}
CCodaObject::~CCodaObject(){}
CSalVoice::~CSalVoice(){}
CSalBaseVoice::~CSalBaseVoice(){}
CSalSwBaseVoice::~CSalSwBaseVoice(){}
CSalSwVoice::~CSalSwVoice(){}
CSalSwBuffer::~CSalSwBuffer(){}
CSalIIRFilter::CSalIIRFilter():m_iA(0),m_iB(0),m_iZ(0){}
CSalSw2DVoice::CSalSw2DVoice(){
#ifndef _MSC_VER
 if(throw_kind==2&&created2==throw_index)throw 29;
#endif
 ++created2;
}
CSalSw3DVoice::CSalSw3DVoice(){
#ifndef _MSC_VER
 if(throw_kind==3&&created3==throw_index)throw 29;
#endif
 ++created3;
}
CSalSw2DVoice::~CSalSw2DVoice(){++destroyed2;}
CSalSw3DVoice::~CSalSw3DVoice(){++destroyed3;}
void CCodaObjectList::CreateStatic(CCodaObject* first,unsigned int size,unsigned int count){
 check(first&&count);
 if(this==&active->m_Sw2DVoices){check(first==active->m_Sw2DVoiceMem&&size==sizeof(CSalSw2DVoice)&&count==(unsigned)created2&&created3==0);++registered2;}
 else{check(this==&active->m_Sw3DVoices&&first==active->m_Sw3DVoiceMem&&size==sizeof(CSalSw3DVoice)&&count==(unsigned)created3);++registered3;}
 m_pObjects=first;m_pFree=first;m_nObjects=count;m_nFree=count;
}
static void reset(){alloc_calls=free_calls=registered2=registered3=created2=created3=destroyed2=destroyed3=0;raw_blocks[0]=raw_blocks[1]=0;}
static void clear_lists(CSalPlatform& p){p.m_Sw2DVoiceMem=0;p.m_Sw3DVoiceMem=0;p.m_Sw2DVoices.m_pObjects=p.m_Sw2DVoices.m_pFree=0;p.m_Sw3DVoices.m_pObjects=p.m_Sw3DVoices.m_pFree=0;p.m_Sw2DVoices.m_nObjects=p.m_Sw2DVoices.m_nFree=0;p.m_Sw3DVoices.m_nObjects=p.m_Sw3DVoices.m_nFree=0;}
static void release_arrays(CSalPlatform& p){delete[] p.m_Sw2DVoiceMem;delete[] p.m_Sw3DVoiceMem;p.m_Sw2DVoiceMem=0;p.m_Sw3DVoiceMem=0;check(destroyed2==created2&&destroyed3==created3);for(int i=0;i<alloc_calls;++i)check(raw_blocks[i]==0);}
int main(){int cases=0;const int counts[]={-1,0,1,2,4};
 for(int aa=0;aa<5;++aa)for(int bb=0;bb<5;++bb)for(int failure=0;failure<3;++failure){
  CSalPlatform p;clear_lists(p);active=&p;reset();fail_alloc=failure;throw_kind=0;memset(&g_SalCaps,0,sizeof(g_SalCaps));int a=counts[aa],b=counts[bb];g_SalCaps.SW2DVoice.nVoices=a;g_SalCaps.SW2DVoice.nFreeVoices=71;g_SalCaps.SW3DVoice.nVoices=b;g_SalCaps.SW3DVoice.nFreeVoices=73;g_SalCaps.HW2DVoice.nVoices=101;g_SalCaps.HW3DVoice.nVoices=103;
  bool result=p.CreateSwVoices();bool bad2=a>0&&failure==1;bool bad3=!bad2&&b>0&&failure==(a>0?2:1);
  check(result==(!bad2&&!bad3));check(registered2==(a>0&&!bad2?1:0)&&registered3==(b>0&&!bad2&&!bad3?1:0));
  check(created2==(a>0&&!bad2?a:0)&&created3==(b>0&&!bad2&&!bad3?b:0));
  check(g_SalCaps.SW2DVoice.nVoices==(bad2?0:a)&&g_SalCaps.SW2DVoice.nFreeVoices==(bad2?0:71));
  check(g_SalCaps.SW3DVoice.nVoices==((bad2||bad3)?0:b)&&g_SalCaps.SW3DVoice.nFreeVoices==((bad2||bad3)?0:73));
  check(g_SalCaps.HW2DVoice.nVoices==101&&g_SalCaps.HW3DVoice.nVoices==103);release_arrays(p);++cases;
 }
 check(cases==75);
#ifndef _MSC_VER
 for(int kind=2;kind<=3;++kind)for(int index=0;index<3;++index){CSalPlatform p;clear_lists(p);active=&p;reset();fail_alloc=0;throw_kind=kind;throw_index=index;memset(&g_SalCaps,0,sizeof(g_SalCaps));g_SalCaps.SW2DVoice.nVoices=3;g_SalCaps.SW3DVoice.nVoices=3;bool caught=false;try{p.CreateSwVoices();}catch(int e){caught=e==29;}check(caught&&free_calls==1&&g_SalCaps.SW2DVoice.nVoices==3&&g_SalCaps.SW3DVoice.nVoices==3);if(kind==2)check(created2==index&&destroyed2==index&&registered2==0&&created3==0);else check(created2==3&&registered2==1&&created3==index&&destroyed3==index&&registered3==0);release_arrays(p);}
#endif
 puts("01215240_TEST PASS");return 0;
}

bool CCodaObject::IsBuffer()const{abort();}
bool CCodaObject::IsContext()const{abort();}
bool CCodaObject::IsCreated()const{abort();}
bool CCodaObject::IsEnvironment()const{abort();}
bool CCodaObject::IsEAX()const{abort();}
bool CCodaObject::IsListener()const{abort();}
bool CCodaObject::IsSource()const{abort();}
bool CCodaObject::IsReverb()const{abort();}
bool CCodaObject::IsVoice()const{abort();}
int CSalVoice::Create(const CCodaSource *){abort();}
int CSalVoice::SetBuffer(const CCodaSource *){abort();}
bool CSalVoice::IsVoice() const{abort();}
bool CSalVoice::IsSoftware() const{abort();}
bool CSalVoice::Is3D() const{abort();}
void CSalVoice::Release(){abort();}
unsigned int CSalVoice::PlayBuffer(const CCodaSource *){abort();}
unsigned int CSalVoice::PlayQueue(CCodaSource *,CREFTIME ){abort();}
unsigned int CSalVoice::Update(CCodaSource *,CREFTIME ){abort();}
unsigned int CSalVoice::Resume(CREFTIME ){abort();}
bool CSalVoice::Stop(CREFTIME ){abort();}
void CSalVoice::ReleaseEnvelopes(CREFTIME ){abort();}
unsigned int CSalVoice::GetPlayCursor() const{abort();}
void CSalVoice::SetPlayCursor(unsigned int ){abort();}
int CSalVoice::EAXSet(const _GUID *,int ,void * ,int ){abort();}
int CSalVoice::EAXGet(const _GUID *,int ,void * ,int ){abort();}
bool CSalBaseVoice::IsCreated() const{abort();}
int CSalBaseVoice::Create(const CCodaSource *){abort();}
int CSalBaseVoice::SetBuffer(const CCodaSource *){abort();}
void CSalBaseVoice::Release(){abort();}
unsigned int CSalBaseVoice::PlayBuffer(const CCodaSource *){abort();}
unsigned int CSalBaseVoice::Resume(CREFTIME ){abort();}
void CSalBaseVoice::ReleaseEnvelopes(CREFTIME ){abort();}
int CSalBaseVoice::EAXSet(const _GUID *,int ,void * ,int ){abort();}
int CSalBaseVoice::EAXGet(const _GUID *,int ,void * ,int ){abort();}
void CSalBaseVoice::InitPlay(){abort();}
void CSalBaseVoice::SetPlayBuffer(const CSalBuffer &,bool ){abort();}
bool CSalSwBaseVoice::IsSoftware() const{abort();}
int CSalSwBaseVoice::Create(const CCodaSource *){abort();}
void CSalSwBaseVoice::Release(){abort();}
unsigned int CSalSwBaseVoice::PlayQueue(CCodaSource *,CREFTIME ){abort();}
bool CSalSwBaseVoice::Stop(CREFTIME ){abort();}
unsigned int CSalSwBaseVoice::Update(CCodaSource *,CREFTIME ){abort();}
unsigned int CSalSwBaseVoice::GetPlayCursor() const{abort();}
void CSalSwBaseVoice::SetPlayCursor(unsigned int ){abort();}
void CSalSwBaseVoice::InitPlay(){abort();}
void CSalSwBaseVoice::SetPlayBuffer(const CSalBuffer &,bool ){abort();}
unsigned int CSalSwBaseVoice::GetPlatformObjectSize(){abort();}
unsigned int CSalSwBaseVoice::GetPlatformBufferLoopStart(){abort();}
unsigned int CSalSwBaseVoice::GetPlatformBufferEnd(){abort();}
unsigned int CSalSwBaseVoice::InitParams(const CCodaSource *,const CODASOURCE &){abort();}
void CSalSwBaseVoice::ConvertFormat(CSalSwBuffer &,const void *,unsigned int ,unsigned int ){abort();}
void CSalSwBaseVoice::Mix(CSalSwBuffer &,unsigned int ,bool ){abort();}
void CSalSwBaseVoice::MixReverbInput(CSalReverb *,CSalSwBuffer &,unsigned int ){abort();}
void CSalSwBaseVoice::MixChannel(CSalSwBuffer &,unsigned int ,unsigned int ){abort();}
void CSalSwBaseVoice::StartRunOut(CREFTIME ){abort();}