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
#define private public
// 00c11520 CSoundEventManager::SetEventPosition
// Descriptive libpk names; native slot27 and position-field behavior, no original PDB coverage.
// Flags: /EHsc
// Leaf: ?SetEventPosition@CSoundEventManager@@UAEXJABVC3DVector@@@Z
#define FABLE_RETAIL_CSOUNDEVENTMANAGER_STREAMED_PASS23
#define FABLE_RETAIL_CSOUNDEVENTMANAGER_EVENT_POSITION_PASS23
#define FABLE_RETAIL_CSOUNDEVENTGROUP
#include "libpk/CSoundEventManager.h"
#include "libpk/CSoundEventGroup.h"
#include "libpk/CSoundPosition.h"
#include "libpk/PKCProtectionInstance.h"
extern PKCCriticalSection g_lock_0143bb14;
void CSoundEventManager::SetEventPosition(long key,const C3DVector& position)
{
 PKCProtectionInstance lock(&g_lock_0143bb14);
 CSoundEvent* const& event=m_events.FindInChildThenSelf(key);
 if(event){
  CSoundPosition data;
  data.Relative=event->GetPositionFlag(this);
  data.Position=position;
  event->Set3DPosition(this,data);
 }
}

#undef private
PKCCriticalSection g_lock_0143bb14;
PKCCriticalSection::PKCCriticalSection(){}PKCCriticalSection::~PKCCriticalSection(){}
static CSoundEventManager* Manager;
static CSoundEvent* Selected;
static C3DVector* PositionInput;
static CSoundPosition* Packet;
static long ExpectedKey;
static bool Relative,Mutate;
static int Trace[20],Used,ThrowAt,Released;
static void Step(int n){Trace[Used++]=n;
#ifndef _MSC_VER
 if(ThrowAt==n)throw n;
#endif
}
PKCProtectionInstance::PKCProtectionInstance(PKCCriticalSection*p):m_pSection(p),m_tick(123){assert(p==&g_lock_0143bb14);Step(1);}
PKCProtectionInstance::~PKCProtectionInstance(){assert(m_pSection==&g_lock_0143bb14&&m_tick==123);++Released;Step(6);}
CSoundEvent* CSoundEventSet::FindInChildThenSelf(long key){assert(this==&Manager->m_events&&key==ExpectedKey);Step(2);return Selected;}
CSoundPosition::CSoundPosition(){Position.X=Position.Y=Position.Z=0;Relative=false;Packet=this;Step(3);}
bool CSoundEvent::GetPositionFlag(CSoundEventManager*p){assert(this==Selected&&p==Manager);Step(4);if(Mutate){PositionInput->X+=4;PositionInput->Y+=8;PositionInput->Z+=16;}return Relative;}
void CSoundEvent::Set3DPosition(CSoundEventManager*p,const CSoundPosition&value){const CSoundPosition*data=&value;assert(this==Selected&&p==Manager&&data==Packet);assert(data->Relative==Relative);assert(data->Position.X==PositionInput->X&&data->Position.Y==PositionInput->Y&&data->Position.Z==PositionInput->Z);Step(5);}

void CSoundEventManager::AddToWorld(void*) { abort(); }
void CSoundEventManager::SetMember888(long) { abort(); }
void CSoundEventManager::SetWorldMode(bool) { abort(); }
void CSoundEventManager::SetMember7cc(long) { abort(); }
void* CSoundEventManager::GetWorldSelection() { abort(); }
void* CSoundEventManager::FindInWorld(void*) { abort(); }
void CSoundEventManager::SetMember884(unsigned char) { abort(); }
void CSoundEventManager::SetWorldGain(float) { abort(); }
float CSoundEventManager::GetWorldGain() { abort(); }
long CSoundEventManager::SetWorldEnabled(void*) { abort(); }
int CSoundEventManager::IsWorldRunning() { abort(); }
long CSoundEventManager::SetWorldState(void*) { abort(); }
void CSoundEventManager::SetListenerFrame(const CSoundPosition*,const CSoundPosition*,const CSoundPosition*) { abort(); }
void CSoundEventManager::SetListenerPosition(const CSoundPosition*) { abort(); }
int CSoundEventList::IsEqual(const void*,const void*)const { abort(); }
int CSoundEventList::IsLess(const void*,const void*)const { abort(); }
int CSoundEventList::IsKeyEqual(const void*,const void*)const { abort(); }
int CSoundEventList::IsKeyLess(const void*,const void*)const { abort(); }
void CSoundEventList::TransferAllFrom(CSoundEventList*) { abort(); }
CSoundEvent* CSoundEventList::FindByKey(const CSampleRef*) { abort(); }
int CSoundEventSet::IsEqual(const void*,const void*)const { abort(); }
int CSoundEventSet::IsLess(const void*,const void*)const { abort(); }
int CSoundEventSet::IsKeyEqual(const void*,const void*)const { abort(); }
int CSoundEventSet::IsKeyLess(const void*,const void*)const { abort(); }
void CHandleBasedMemoryPool_012b4098::MoveResource(void* resource, long handle, void* from, void* to, unsigned long size) { abort(); }
int CHandleBasedMemoryPool_012b4098::CanMoveResource(void* resource, long handle) { abort(); }
void CHandleBasedMemoryPool_012b4098::ValidateResource(void* resource) { abort(); }
long* CSoundEvent::GetIndexSlot() { abort(); }
void CSoundEvent::ResetLevel(CSoundEventManager* manager) { abort(); }
void CSoundEvent::SetUserGain(CSoundEventManager* manager,float gain) { abort(); }
void CSoundEvent::SetPlaybackParameter(CSoundEventManager* manager,float value) { abort(); }
void CSoundEvent::ResetPlaybackState(CSoundEventManager* manager) { abort(); }
void CSoundEvent::SetPositionParameters(CSoundEventManager* manager,float first,float second) { abort(); }
void CSoundEvent::ApplyPan(CSoundEventManager* manager,float pan) { abort(); }
long CSoundEventManager::PlayStreamed(CStreamSoundBank*,long,unsigned long,const Input*){abort();}
long CSoundEventManager::PlaySample(void*,long,const Input*){abort();}
long CSoundEventManager::PlaySelectedResource(const void*,const Input*){abort();}
long CSoundEventManager::FindPlayingSelectedResource(const void*,const void*){abort();}
long CSoundEventManager::FindPlayingSample(const void*,void*){abort();}
void CSoundEventManager::StopAllEvents(){abort();}
void CSoundEventManager::StopEventsWithField24(void*){abort();}
void CSoundEventManager::StopEventsWithField10(void*){abort();}
void CSoundEventManager::ResetEvent(long){abort();}
void CSoundEventManager::ResetEventPlayback(long){abort();}
void CSoundEventManager::SetEventPan(long,float){abort();}
void CSoundEventManager::SetEventGain(long,float){abort();}
void CSoundEventManager::SetEventPlaybackParameter(long,float){abort();}
template<class T>void PKCTRList<T>::MorphToArray(){abort();}
template<class T>void PKCTRList<T>::MorphToTree(){abort();}
template<class T>T*PKCTRList<T>::Find(const T*){abort();return 0;}
template<class T>void PKCTRList<T>::Remove(T*){abort();}
template<class T>void PKCTRList<T>::Add(T*){abort();}
template<class T>void PKCTRList<T>::ForEach(void*){abort();}
template<class T>T*PKCTRList<T>::GetFirst(){abort();return 0;}
template<class T>T*PKCTRList<T>::GetLast(){abort();return 0;}
template<class T>T*PKCTRList<T>::GetItemByIndex(long){abort();return 0;}
template<class T>void PKCTRList<T>::Reserve(long){abort();}
template<class T>long PKCTRList<T>::GetCount(){abort();return 0;}
CSoundEventList::CSoundEventList():PKCTRList<CSoundEvent>(0){}
CSoundEventSet::CSoundEventSet():PKCTRList<CPooledResource>(0){}
int main(){
 CSoundEventManager manager;CSoundEvent event;Manager=&manager;C3DVector input;PositionInput=&input;CSoundEventManager* base=&manager;long keys[]={-1,0,1,10000};unsigned cases=0;

#ifdef _MSC_VER
 assert(sizeof(CSoundEvent)==0xb4&&sizeof(CSoundPosition)==16);
 assert((char*)&manager.m_events-(char*)&manager==0x2c);
#endif
 for(int found=0;found<2;++found)for(int flag=0;flag<2;++flag)for(int mutation=0;mutation<2;++mutation)for(int key=0;key<4;++key)for(int seed=0;seed<8;++seed){
  Used=ThrowAt=Released=0;Packet=0;Selected=found?&event:0;Relative=flag!=0;Mutate=mutation!=0;ExpectedKey=keys[key];input.X=(float)seed;input.Y=-2.0f*seed;input.Z=.5f*seed;
  base->SetEventPosition(ExpectedKey,input);assert(Released==1);
  int want[]={1,2,3,4,5,6};int count=6;if(!found){want[2]=6;count=3;}assert(Used==count);for(int n=0;n<count;++n)assert(Trace[n]==want[n]);
  assert(input.X==seed+(found&&mutation?4:0));assert(input.Y==-2*seed+(found&&mutation?8:0));assert(input.Z==.5f*seed+(found&&mutation?16:0));++cases;
 }
 assert(cases==256);
#ifndef _MSC_VER
 for(int site=1;site<=5;++site){
  Used=Released=0;ThrowAt=site;Selected=&event;Relative=true;Mutate=false;ExpectedKey=1;input.X=input.Y=input.Z=1;bool caught=false;
  try{base->SetEventPosition(1,input);}catch(int n){caught=true;assert(n==site);}
  assert(caught&&Released==(site==1?0:1));assert(Trace[Used-1]==(site==1?1:6));
 }
#endif
 puts("0121154e_TEST PASS");return 0;
}