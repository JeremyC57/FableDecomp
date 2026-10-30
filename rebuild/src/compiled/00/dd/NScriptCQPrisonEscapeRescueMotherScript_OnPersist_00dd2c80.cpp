// NScript::CQ_PrisonEscapeRescueMotherScript::OnPersist(CPersistContext& context) -- Transfer persistent state with zero defaults.
// Leaf: ?OnPersist@CQ_PrisonEscapeRescueMotherScript@NScript@@UAEXAAVCPersistContext@@@Z
// Identity: original PDB member signatures and unique persist field names; own vtable012db86c slot4, constructed at00dd26e0. Field strings and typed helper callsites independently identify this owner; donor addresses not used.
// Layout: original field types after typed72-byte level-script base or20-byte entity base; PC owner size128. Shared handle headers aliased for true original bases. CPersistContext is an API-only projection, no storage used.
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
typedef unsigned long RetailUID;
#else
typedef unsigned __int64 RetailUID;
#endif
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CCharStringData;class CCharString{public:CCharStringData* PStringData;bool FastCompare(const CCharString&)const;};
class CCPPointerInfo;template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;};
class CEntityScriptBindingBase;class CActiveEntityScriptBase;class CSpawnedFuncBase;
namespace std{
template<class T>class allocator{};template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
struct _List_node_base{_List_node_base* _M_next;_List_node_base* _M_prev;};template<class T>struct _List_node:public _List_node_base{T _M_data;};
template<class T,class A>class _List_base{protected:typedef _List_node<T> _Node;typedef allocator<_Node> _NodeAlloc;_STLP_alloc_proxy<_Node*,_Node,_NodeAlloc> _M_node;};
template<class T,class A=allocator<T> >class list:protected _List_base<T,A>{};
template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:T* begin(){return this->_M_start;}T* end(){return this->_M_finish;}};
template<class Iter,class Compare>void __fastcall sort(Iter,Iter,Compare);
}
template<class T>class CArray:public std::vector<T>{};
template<class K,class V>class CKeyPairCompareLess{public:bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b)const{return a.first<b.first;}};
template<class V>class CCompareString{public:bool operator()(const std::pair<CCharString,V>& a,const std::pair<CCharString,V>& b)const{return a.first.FastCompare(b.first);}};
template<class K,class V,class Cmp=CKeyPairCompareLess<K,V> >class CVectorMap{public:CArray<std::pair<K,V> > Data;Cmp Compare;bool Dirty;__forceinline void Sort(){if(Dirty){std::sort(Data.begin(),Data.end(),Compare);Dirty=false;}}};

class CPersistContext;class CBaseObjectPointer;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
namespace NScript{class CRuntimeScriptedEntityPersist{public:class CPersistInfo;CVectorMap<RetailUID,CPersistInfo> PersistScripts;};class CQ_SunnyvaleMasterData;}
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CGameScriptInterfaceBase;
class CPersistContext{public:template<class T>void Transfer(const char*,T&,const T&);};class CTimer{public:long TimerIndex;};
namespace NScript{class CQ_PrisonEscapeRescueMotherScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:long CurrentTask,TasksDone[3],TargetNumber[3];bool HeroAgreesToTask,HeroHasFoundWeapons,MotherCellDoorOpen,MotherFollowing,MotherDied,WatchingHero,PrisonCutsceneFinished,RaceStarting;long YearsPassedInCell,PrisonerConversationIndex,ConversationIndex;bool HeroEscapedCell,AlarmSounded,MotherRescuedFromPrisonAutoSaveCheckPoint;CTimer RunningTimer;virtual void OnPersist(CPersistContext&);};void CQ_PrisonEscapeRescueMotherScript::OnPersist(CPersistContext& context){context.Transfer("CurrentTask",CurrentTask,0L);context.Transfer("HeroHasFoundWeapons",HeroHasFoundWeapons,false);context.Transfer("MotherRescuedFromPrisonAutoSaveCheckPoint",MotherRescuedFromPrisonAutoSaveCheckPoint,false);}}
#ifdef _MSC_VER
typedef char ExactOwnerSize[sizeof(NScript::CQ_PrisonEscapeRescueMotherScript)==128?1:-1];
#endif