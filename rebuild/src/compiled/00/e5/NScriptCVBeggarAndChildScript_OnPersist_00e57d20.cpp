// NScript::CV_BeggarAndChildScript::OnPersist(CPersistContext& context) -- Transfer persistent script state with zero defaults.
// Leaf: ?OnPersist@CV_BeggarAndChildScript@NScript@@UAEXAAVCPersistContext@@@Z
// Identity: Original PDB OnPersist and unique own persist literals; own vtable0x12e65e0 slot4, constructor stores at0xe57bc8,0xe5d0d6. All12 own strings and typed original owner fields mapped in order. Transfer<bool>004045c0, Transfer<long>00410be0.
// Layout: real typed CScriptBase64/CLevelScriptBase72 with original fields and STLport containers. Shared handle headers aliased for real polymorphic hierarchy. CPersistContext API-only projection, no storage used.
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
namespace NScript{class CV_BeggarAndChildScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:bool BeggarHit;bool BeggarLeft;bool BullyLeft;bool BullyHit;bool TaughtBelch;bool TaughtSneer;bool TaughtThanks;bool TaughtBattleCry;bool BullyHasTaunted;long BelchedAtBeggar;bool BelchedAtBully;bool QuestCardGiven;bool ExpressionTutorialShown;CTimer TauntTimer;CTimer BeggarReplyTimer;virtual void OnPersist(CPersistContext&);};void CV_BeggarAndChildScript::OnPersist(CPersistContext& context){context.Transfer("BeggarHit",BeggarHit,false);context.Transfer("BeggarLeft",BeggarLeft,false);context.Transfer("BullyLeft",BullyLeft,false);context.Transfer("BullyHit",BullyHit,false);context.Transfer("TaughtBelch",TaughtBelch,false);context.Transfer("TaughtSneer",TaughtSneer,false);context.Transfer("TaughtThanks",TaughtThanks,false);context.Transfer("TaughtBattleCry",TaughtBattleCry,false);context.Transfer("BullyHasTaunted",BullyHasTaunted,false);context.Transfer("BelchedAtBeggar",BelchedAtBeggar,0L);context.Transfer("BelchedAtBully",BelchedAtBully,false);context.Transfer("QuestCardGiven",QuestCardGiven,false);}}