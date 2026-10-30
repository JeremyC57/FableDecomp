// NScript::CQ_BreakSiegeScript::OnPersist(CPersistContext& context) -- Transfer persistent script state with zero defaults.
// Leaf: ?OnPersist@CQ_BreakSiegeScript@NScript@@UAEXAAVCPersistContext@@@Z
// Identity: Original PDB OnPersist and unique own persist literals; own vtable0x12cab18 slot4, constructor stores at0xd1cc28,0xd23ca7. All12 own strings and typed original owner fields mapped in order. Transfer<bool>004045c0, Transfer<long>00410be0.
// Layout: real typed CScriptBase64/CLevelScriptBase72 with original fields and STLport containers. Shared handle headers aliased for real polymorphic hierarchy. CPersistContext API-only projection, no storage used.
#define CScriptThing CScriptThing_HeaderLayout
#include "engine/CScriptThing.h"
#undef CScriptThing
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
typedef unsigned long RetailUID;
#else
typedef unsigned __int64 RetailUID;
#endif
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable:public CBaseClass{protected:CBaseClassNonCopyable(){}~CBaseClassNonCopyable(){}private:CBaseClassNonCopyable(const CBaseClassNonCopyable&);CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);};
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
class CScriptThing:public CBaseClass{public:CCountedPointer<CScriptThing> PImp;};
namespace NScript{class CRuntimeScriptedEntityPersist{public:class CPersistInfo;CVectorMap<RetailUID,CPersistInfo> PersistScripts;};class CQ_SunnyvaleMasterData;}
class CScriptBase:public CBaseClassNonCopyable{public:
std::list<CCountedPointer<CSpawnedFuncBase> > Threads;
CVectorMap<RetailUID,CCountedPointer<CActiveEntityScriptBase> > EntityScripts;
CVectorMap<CCharString,CCountedPointer<CEntityScriptBindingBase>,CCompareString<CCountedPointer<CEntityScriptBindingBase> > > EntityScriptBindings;
unsigned long LastWorldUpdateFrame;CSpawnedFuncBase* PActiveThread;NScript::CRuntimeScriptedEntityPersist RuntimeScriptedEntityPersist;
virtual void RegisterMain(){}virtual void Main(){}virtual void Init(){}virtual void OnPersist(CPersistContext&){}
};
class CGameScriptInterfaceBase;
template<class T>class CLevelScriptBase:public CScriptBase{public:CGameScriptInterfaceBase* Game;T* MasterScriptData;};
class CPersistContext{public:template<class T>void Transfer(const char*,T&,const T&);};class CTimer{public:long TimerIndex;};
namespace NScript{class CQ_BreakSiegeScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:bool MissionSucceeded;bool MissionFailed;bool BanditDemandsKnown;bool ChiefRefusesDemands;bool BanditLeaderFree;bool BanditSiegeStart;bool HasFirstWaveAttacked;bool StartSecondWave;bool BanditsAttackHero;bool HeroLetInTown;bool GuardFollowingHero;bool EndCutsceneStarts;bool EndCutsceneDone;bool AttackedBanditLeader;bool QuestStartScreened;long ReasonForFailure;virtual void OnPersist(CPersistContext&);};void CQ_BreakSiegeScript::OnPersist(CPersistContext& context){context.Transfer("BanditDemandsKnown",BanditDemandsKnown,false);context.Transfer("ChiefRefusesDemands",ChiefRefusesDemands,false);context.Transfer("BanditLeaderFree",BanditLeaderFree,false);context.Transfer("BanditSiegeStart",BanditSiegeStart,false);context.Transfer("BanditsAttackHero",BanditsAttackHero,false);context.Transfer("HasFirstWaveAttacked",HasFirstWaveAttacked,false);context.Transfer("StartSecondWave",StartSecondWave,false);context.Transfer("HeroLetInTown",HeroLetInTown,false);context.Transfer("GuardFollowingHero",GuardFollowingHero,false);context.Transfer("EndCutsceneStarts",EndCutsceneStarts,false);context.Transfer("EndCutsceneDone",EndCutsceneDone,false);context.Transfer("QuestStartScreened",QuestStartScreened,false);}}