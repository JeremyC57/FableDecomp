#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifndef _MSC_VER
#define FABLE_PORTABLE
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#define __int64 long long
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
// 00c11230 CSoundEventManager::CheckExisting
// Descriptive library names; original libpk names absent from all supplied PDB name sets.
// Flags: /EHsc
// Leaf: ?CheckExisting@CSoundEventManager@@QAEHPAVCSoundSample@@JJJJK@Z
#define FABLE_RETAIL_CSOUNDEVENTMANAGER_CHECK_EXISTING_PASS23
#define FABLE_RETAIL_CSOUNDEVENTGROUP
#include "libpk/CSoundEventManager.h"
#include "libpk/CSoundEventGroup.h"
#include "libpk/CSampleRef.h"
int CSoundEventManager::CheckExisting(CSoundSample* sample,long a,long b,long c,long force,unsigned long priority)
{
 CSampleRef key;
 key.Bind(sample,a,b,c);
 CSoundEvent* event=ListeningEvents.FindByKey(&key);
 key.Release();
 if(!event)return 1;
 if(!event->IsAudible(this)){
  event->Stop(this);
  return 1;
 }
 if((event->Pending||force)&&priority>=event->Priority){
  event->RestartPlayback(this);
  return 1;
 }
 return 0;
}

static CSoundEventManager* Manager;
static CSoundEvent* Selected;
static const CSampleRef* Key;
static long A,B,C;
static bool Audible;
static int Trace[20],Used,Destructed,ThrowAt;
static void Step(int step){Trace[Used++]=step;
#ifndef _MSC_VER
 if(step==ThrowAt)throw step;
#endif
}
CSampleRef::CSampleRef():Sample(0),Key1(0),Key2(0),Key3(0){Key=this;Step(1);}
CSampleRef::~CSampleRef(){assert(Key==this);++Destructed;Step(8);}
void CSampleRef::Bind(CSoundSample* p,long a,long b,long c){assert(this==Key&&!p&&a==A&&b==B&&c==C);Step(2);Sample=p;Key1=a;Key2=b;Key3=c;}
void CSampleRef::Release(){assert(this==Key);Step(4);Sample=0;Key1=Key2=Key3=0;}
bool CSoundEvent::IsAudible(CSoundEventManager* p){assert(this==Selected&&p==Manager);Step(5);return Audible;}
void CSoundEvent::Stop(CSoundEventManager*p){assert(this==Selected&&p==Manager);Step(6);}
void CSoundEvent::RestartPlayback(CSoundEventManager*p){assert(this==Selected&&p==Manager);Step(7);}
CSoundEventList::CSoundEventList():PKCTRList<CSoundEvent>(0){}
CSoundEvent* CSoundEventList::FindByKey(const CSampleRef* key){assert(this==&Manager->ListeningEvents&&key==Key&&key->Sample==0&&key->Key1==A&&key->Key2==B&&key->Key3==C);Step(3);return Selected;}
int CSoundEventList::IsEqual(const void*,const void*)const{abort();}
int CSoundEventList::IsLess(const void*,const void*)const{abort();}
int CSoundEventList::IsKeyEqual(const void*,const void*)const{abort();}
int CSoundEventList::IsKeyLess(const void*,const void*)const{abort();}
void CSoundEventList::TransferAllFrom(CSoundEventList*){abort();}
void CHandleBasedMemoryPool_012b4098::MoveResource(void*,long,void*,void*,unsigned long){abort();}
int CHandleBasedMemoryPool_012b4098::CanMoveResource(void*,long){abort();}
void CHandleBasedMemoryPool_012b4098::ValidateResource(void*){abort();}
template<class T> void PKCTRList<T>::MorphToArray(){abort();}
template<class T> void PKCTRList<T>::MorphToTree(){abort();}
template<class T> T* PKCTRList<T>::Find(const T*){abort();}
template<class T> void PKCTRList<T>::Remove(T*){abort();}
template<class T> void PKCTRList<T>::Add(T*){abort();}
template<class T> void PKCTRList<T>::ForEach(void*){abort();}
template<class T> T* PKCTRList<T>::GetFirst(){abort();}
template<class T> T* PKCTRList<T>::GetLast(){abort();}
template<class T> T* PKCTRList<T>::GetItemByIndex(long){abort();}
template<class T> void PKCTRList<T>::Reserve(long){abort();}
template<class T> long PKCTRList<T>::GetCount(){abort();}
long* CSoundEvent::GetIndexSlot(){abort();}
void CSoundEvent::ResetLevel(CSoundEventManager*){abort();}
void CSoundEvent::SetUserGain(CSoundEventManager*,float){abort();}
void CSoundEvent::SetPlaybackParameter(CSoundEventManager*,float){abort();}
void CSoundEvent::ResetPlaybackState(CSoundEventManager*){abort();}
void CSoundEvent::Set3DPosition(CSoundEventManager*,const CSoundPosition&){abort();}
void CSoundEvent::SetPositionParameters(CSoundEventManager*,float,float){abort();}
void CSoundEvent::ApplyPan(CSoundEventManager*,float){abort();}
bool CSoundEvent::GetPositionFlag(CSoundEventManager*){abort();}
static void Reset(){Used=Destructed=ThrowAt=0;Key=0;}
int main(){
 CSoundEventManager manager;Manager=&manager;CSoundEvent event;
 memset(manager.OpaqueManagerPrefix,0xa5,sizeof manager.OpaqueManagerPrefix);memset(manager.OpaqueManagerMiddle,0x5a,sizeof manager.OpaqueManagerMiddle);
 const unsigned long priorities[]={0UL,1UL,2UL,0x7fffffffUL,0xffffffffUL};unsigned cases=0;
 for(int found=0;found<2;++found)for(int audible=0;audible<2;++audible)for(int pending=-1;pending<=1;++pending)for(int force=-1;force<=1;++force)for(int ep=0;ep<5;++ep)for(int rp=0;rp<5;++rp)for(int seed=0;seed<3;++seed){
  Reset();Selected=found?&event:0;Audible=audible!=0;event.Priority=priorities[ep];event.Pending=pending;A=seed-1;B=seed*7;C=-seed*13;
  int result=manager.CheckExisting(0,A,B,C,force,priorities[rp]);
  bool pass=!found||!audible||((pending!=0||force!=0)&&priorities[rp]>=priorities[ep]);assert(result==(pass?1:0)&&Destructed==1);assert(event.Priority==priorities[ep]&&event.Pending==pending);
  int expected[]={1,2,3,4,5,0,8};int size=7;
  if(!found){expected[4]=8;size=5;}else if(!audible)expected[5]=6;else if(pass)expected[5]=7;else{expected[5]=8;size=6;}
  assert(Used==size);for(int n=0;n<size;++n)assert(Trace[n]==expected[n]);
  for(unsigned n=0;n<sizeof manager.OpaqueManagerPrefix;++n)assert(manager.OpaqueManagerPrefix[n]==0xa5);
  for(unsigned n=0;n<sizeof manager.OpaqueManagerMiddle;++n)assert(manager.OpaqueManagerMiddle[n]==0x5a);
  ++cases;
 }
 assert(cases==2700);
#ifndef _MSC_VER
 for(int site=1;site<=7;++site){
  Reset();Selected=&event;Audible=site!=6;event.Pending=1;event.Priority=1;A=1;B=2;C=3;ThrowAt=site;bool caught=false;
  try{manager.CheckExisting(0,A,B,C,1,2);}catch(int value){caught=true;assert(value==site);}
  assert(caught&&Destructed==(site==1?0:1));assert(Trace[Used-1]==(site==1?1:8));
 }
#endif
 puts("012114f4_TEST PASS");return 0;
}