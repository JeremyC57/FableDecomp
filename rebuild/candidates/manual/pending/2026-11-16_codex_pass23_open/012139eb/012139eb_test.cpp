// Self-contained shared-header constructor behavior and generated-unwind test.
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#if !defined(_MSC_VER) || defined(_CPPUNWIND)
#define TEST_UNWIND
#endif
inline void* operator new(size_t,void* p) throw(){return p;}
inline void operator delete(void*,void*) throw(){}
// 00c2cbd0 CEventScript::CEventScript
// Leaf: ??0CEventScript@@QAE@PAUCScriptOwner@@@Z
// Flags: /EHsc
// Descriptive libpk identities; the catalog's quest-script label is false.
#define FABLE_RETAIL_LIBPK_EVENT_SCRIPT_CTOR_PASS23
#include "libpk/CEventScript.h"
CEventScript::CEventScript(CScriptOwner* owner):CScriptRoot(owner)
{
    m_pOwner->m_registry.Add(this);
}


static CScriptOwner* owners;
static CEventScript* active;
static int redirect_tree,redirect_flags,throw_stage,stage,trace[12],used;
static int root_dt,tree_dt,base_dt,removes,adds;
static void* script_live_vptr;
static void* flags_live_vptr;
static void* container_live_vptr;
static void* script_dead_vptr;
static void* flags_dead_vptr;
static void* container_dead_vptr;
static void check(bool ok){if(!ok){fputs("constructor fixture check failed\n",stderr);exit(1);}}
static void* vptr(const void* p){void* q;memcpy(&q,p,sizeof(q));return q;}
static void record(int n){check(used<12);trace[used++]=n;}
static void may_throw(int n){
#ifdef TEST_UNWIND
 if(throw_stage==n)throw n;
#else
 check(throw_stage==0);
#endif
}
CScriptRegistry::CScriptRegistry():PKCTRList<CEventScript>(0){}
static void registry_add(void* registry,void* item){
 check(stage==2&&item==active);
 CScriptOwner* expected=&owners[redirect_flags?2:(redirect_tree?1:0)];
 check(registry==&expected->m_registry&&active->m_pOwner==expected);
 check(active->m_flags.m_maskB==0&&active->m_flags.m_valueB==0);
 check(active->m_flags.m_maskA==0&&active->m_flags.m_valueA==0);
 check(active->m_events.m_tree.Count==0);
 check(active->m_events.m_tree.Root==&active->m_events.m_tree.Sentinel);
 check(vptr(active)==script_live_vptr&&vptr(&active->m_flags)==flags_live_vptr);
 record(3);++adds;stage=3;may_throw(3);
}
static void registry_remove(void* registry,void* item){
 check(registry==&active->m_pOwner->m_registry&&item==active);
 record(4);++removes;
}
template<class T> void PKCTRList<T>::Add(T* p){registry_add(this,p);}
template<class T> void PKCTRList<T>::Remove(T* p){registry_remove(this,p);}
template<class T> void PKCTRList<T>::MorphToArray(){check(false);}
template<class T> void PKCTRList<T>::MorphToTree(){check(false);}
template<class T> T* PKCTRList<T>::Find(const T*){check(false);return 0;}
template<class T> void PKCTRList<T>::ForEach(void*){check(false);}
template<class T> T* PKCTRList<T>::GetFirst(){check(false);return 0;}
template<class T> T* PKCTRList<T>::GetLast(){check(false);return 0;}
template<class T> T* PKCTRList<T>::GetItemByIndex(long){check(false);return 0;}
template<class T> void PKCTRList<T>::Reserve(long){check(false);}
template<class T> long PKCTRList<T>::GetCount(){check(false);return 0;}
#define COMPARATORS(K) \
 int K::IsEqual(const void*,const void*)const{check(false);return 0;} \
 int K::IsLess(const void*,const void*)const{check(false);return 0;} \
 int K::IsKeyEqual(const void*,const void*)const{check(false);return 0;} \
 int K::IsKeyLess(const void*,const void*)const{check(false);return 0;}
COMPARATORS(CScriptRegistry)
COMPARATORS(CEventContainer)
void CEventContainer::TransferAllFrom(PKCTRList<CEvent>*){check(false);}
#define CONTAINER_METHODS(K) \
 int K::IsArray(){check(false);return 0;} \
 int K::IsTree(){check(false);return 0;} \
 void K::MorphToArray(void*){check(false);} \
 void K::MorphToTree(void*){check(false);} \
 void K::Reserve(long){check(false);} \
 void K::Add(void*,void*){check(false);} \
 void K::Remove(void*,void*){check(false);} \
 void* K::Find(void*,const void*){check(false);return 0;} \
 void* K::FindByKey(void*,const void*){check(false);return 0;} \
 void* K::GetItemByIndex(long){check(false);return 0;} \
 long K::GetCount(){check(false);return 0;} \
 void* K::GetFirst(){check(false);return 0;} \
 void* K::GetLast(){check(false);return 0;} \
 void K::ForEach(void*){check(false);}
CONTAINER_METHODS(PKCContainer_00c17b70)
CONTAINER_METHODS(PKCTRRedblackTree)
PKCContainer_00c17b70::~PKCContainer_00c17b70(){
 check(this==&active->m_events.m_tree);record(6);++base_dt;
}
PKCTRRedblackTree::PKCTRRedblackTree(PKCTRAllocator* shared)
 :PAllocator(shared),POwnedAllocator(0),Root(&Sentinel),Count(0)
{
 check(shared==0&&stage==0&&this==&active->m_events.m_tree);
 check(active->m_pOwner==&owners[0]);
 Sentinel.Left=Sentinel.Right=&Sentinel;Sentinel.Parent=0;
 Sentinel.Colour=0;Sentinel.Key=0;
 script_live_vptr=vptr(active);container_live_vptr=vptr(&active->m_events);
 if(redirect_tree)active->m_pOwner=&owners[1];
 record(1);stage=1;may_throw(1);
}
PKCTRRedblackTree::~PKCTRRedblackTree(){
 check(this==&active->m_events.m_tree&&stage>=1);
 check(Sentinel.Left==&Sentinel&&Sentinel.Right==&Sentinel&&Root==&Sentinel);
 record(5);++tree_dt;
}
CFlagOverride::CFlagOverride():m_maskB(0),m_valueB(0),m_maskA(0),m_valueA(0){
 check(this==&active->m_flags&&stage==1);
 check(active->m_events.m_tree.Count==0&&tree_dt==0&&base_dt==0);
 flags_live_vptr=vptr(this);
 if(redirect_flags)active->m_pOwner=&owners[2];
 record(2);stage=2;may_throw(2);
}
void CFlagOverride::LoadPrimaryFlags(void*,bool){check(false);}
void CFlagOverride::LoadSecondaryFlags(void*,bool){check(false);}
void CEventScript::Register(){check(false);}
CEvent* CEventScript::CreateEvent(CEventTarget*,void*,void*){check(false);return 0;}
void* CEventScript::CreateInstance(void*,void*,const CEventPosition*,void*,void*){check(false);return 0;}
long CEventScript::GetEventCount(){check(false);return 0;}
CEvent* CEventScript::GetEventByIndex(long){check(false);return 0;}
CFlagOverrideBase* CEventScript::GetFlagOverride(){return &m_flags;}
CEvent* CEventScript::CreateEventFromParameters(void*,void*,void*,void*,void*,void*){check(false);return 0;}
CEventScript::~CEventScript(){++root_dt;m_pOwner->m_registry.Remove(this);}
static void verify_trace(int throwing){
 int want[6],n=0;
 want[n++]=1;
 if(throwing!=1)want[n++]=2;
 if(throwing!=1&&throwing!=2)want[n++]=3;
 if(!throwing)want[n++]=4;
 if(throwing!=1)want[n++]=5;
 want[n++]=6;
 check(used==n);for(int j=0;j<n;++j)check(trace[j]==want[j]);
}
int main(){
 int cases=0;CScriptOwner local_owners[3];owners=local_owners;
 for(int fill=0;fill!=8;++fill)
 for(redirect_tree=0;redirect_tree!=2;++redirect_tree)
 for(redirect_flags=0;redirect_flags!=2;++redirect_flags){
#ifdef TEST_UNWIND
  const int stages=4;
#else
  const int stages=1;
#endif
  for(throw_stage=0;throw_stage!=stages;++throw_stage){
   union Storage{void* align;unsigned char bytes[sizeof(CEventScript)+32];} storage;
   const unsigned char value=(unsigned char)(0x31+fill*17);
   memset(storage.bytes,value,sizeof(storage.bytes));
   for(int k=0;k!=3;++k)memset(owners[k].UnrecoveredPrefix,value,16);
   active=reinterpret_cast<CEventScript*>(storage.bytes+16);
   stage=used=root_dt=tree_dt=base_dt=removes=adds=0;
   bool caught=false;
#ifdef TEST_UNWIND
   try{new(active)CEventScript(&owners[0]);}
   catch(int n){caught=(n==throw_stage);}
#else
   new(active)CEventScript(&owners[0]);
#endif
   if(throw_stage){
    check(caught&&root_dt==0&&removes==0);
    check(adds==(throw_stage==3?1:0));
   }else{
    check(!caught&&adds==1&&used==3&&tree_dt==0&&base_dt==0);
    check(active->m_pOwner==&owners[redirect_flags?2:(redirect_tree?1:0)]);
    check(active->GetFlagOverride()==&active->m_flags);
    active->~CEventScript();check(root_dt==1&&removes==1);
   }
   check(tree_dt==(throw_stage==1?0:1)&&base_dt==1);
   verify_trace(throw_stage);
   // Object lifetime has ended: inspect only unsigned-char storage, never call it.
   void* dead_script=vptr(storage.bytes+16);
   void* dead_container=vptr(storage.bytes+16+offsetof(CEventScript,m_events));
   check(dead_script!=script_live_vptr&&dead_container!=container_live_vptr);
   if(!cases){script_dead_vptr=dead_script;container_dead_vptr=dead_container;}
   check(script_dead_vptr==dead_script&&container_dead_vptr==dead_container);
   if(throw_stage!=1){
    void* dead_flags=vptr(storage.bytes+16+offsetof(CEventScript,m_flags));
    check(dead_flags!=flags_live_vptr);
    if(!flags_dead_vptr)flags_dead_vptr=dead_flags;
    check(dead_flags==flags_dead_vptr);
   }
   for(int b=0;b!=16;++b){
    check(storage.bytes[b]==value&&storage.bytes[sizeof(CEventScript)+16+b]==value);
    for(int k=0;k!=3;++k)check(owners[k].UnrecoveredPrefix[b]==value);
   }
   ++cases;
  }
 }
#ifdef TEST_UNWIND
 check(cases==128);
#else
 check(cases==32);
#endif
 puts("012139eb_TEST PASS");return 0;
}
