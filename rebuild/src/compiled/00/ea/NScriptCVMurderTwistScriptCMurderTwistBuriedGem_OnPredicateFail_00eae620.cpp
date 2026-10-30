// NScript::CV_MurderTwistScript::CMurderTwistBuriedGem::OnPredicateFail() -- erase the matching magic-rune entity on predicate failure.
// Leaf: ?OnPredicateFail@CMurderTwistBuriedGem@CV_MurderTwistScript@NScript@@EAEXXZ
// Identity: proven own named binding00ea8ad3 ->original class factory ->vtable012ef7c8 slot5; original PDB privatevirtual EAE method. Actual entity20 Game/Me prefix, parent/master+20/+24, original BrainState28 where present; parent exact script96 with ItemsFound72/State76/Event80 and typed vector84.
// Model amendment: original protected CBaseClassNonCopyable ctor/dtor; genuine protected _List_base/rebound-node proxy and _Vector_base/end-storage proxy. Existing active vector APIs/bodies preserved verbatim; unused map/tree definitions removed.
// Layout: real typed script/level/handle storage; original CScriptThing nonvirtual operator== forwards protected virtual IsEqualTo(constThing&)const slot78 through original PImp.Data; base default IsEqualTo returns false as own004aa8a0. Original game const RemoveThing108/EntitySetAsKillable517; genuine STLport vector begin/end/erase/push_back/_M_insert_overflow. Counted-info unsigned refcount and copy constructor retain actual addref. Unused virtual slots are descriptive projections; no fabricated active APIs. Shared handle headers aliased; fixtures alone expose protected storage.
#ifndef _MSC_VER
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void*p){return p;}
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
typedef unsigned long RetailUID;
#else
typedef unsigned __int64 RetailUID;
#endif
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CCharStringData;class CCharString{public:CCharString();CCharString(const CCharString&);CCharString(const char*,long=-1);~CCharString();CCharStringData* PStringData;bool FastCompare(const CCharString&)const;};
#include "engine/CCountedPointer.h"

class CEntityScriptBindingBase;class CActiveEntityScriptBase;class CSpawnedFuncBase;
namespace std{
struct __false_type{__false_type(){}};template<class A,class B>struct pair{A first;B second;};template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
struct _List_node_base{_List_node_base* _M_next;_List_node_base* _M_prev;};template<class T>struct _List_node:public _List_node_base{T _M_data;};template<class T,class A>class _List_base{protected:typedef _List_node<T> _Node;typedef allocator<_Node> _NodeAlloc;_STLP_alloc_proxy<_Node*,_Node,_NodeAlloc> _M_node;};template<class T,class A=allocator<T> >class list:protected _List_base<T,A>{};

template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:T* erase(T*);void push_back(const T& value){if(this->_M_finish!=this->_M_end_of_storage._M_data){new((void*)this->_M_finish)T(value);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,value,__false_type(),1,true);}protected:void _M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool);public:T* begin(){return this->_M_start;}T* end(){return this->_M_finish;}};
template<class Iter,class Compare>void __fastcall sort(Iter first,Iter last,Compare comp);
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
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline bool CScriptThing::operator==(const CScriptThing& other) const {return PImp->IsEqualTo(*other.PImp);}
inline bool CScriptThing::IsEqualTo(const CScriptThing&) const {return false;}
namespace NScript{class CRuntimeScriptedEntityPersist{public:class CPersistInfo;CVectorMap<RetailUID,CPersistInfo> PersistScripts;};class CQ_SunnyvaleMasterData;}
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CGameScriptInterfaceBase;

class CScriptDataBase;class CEntityScriptBase{public:CGameScriptInterfaceBase* Game;CScriptThing Me;CEntityScriptBase(CGameScriptInterfaceBase&,const CScriptThing&);virtual ~CEntityScriptBase(){}virtual void UnknownEntitySlot1(){}virtual void Init(){}virtual void UnknownEntitySlot3(){}virtual void OnPersist(CPersistContext&){}virtual void OnPredicateFail(){};};
namespace NScript{class CV_MurderTwistScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:long MurderTwistItemsFound,MurderTwistState,MurderTwistEvent;std::vector<CScriptThing> MagicRuneRumbleList;CV_MurderTwistScript(CScriptDataBase*,CGameScriptInterfaceBase&);class CMurderTwistBuriedGem:public CEntityScriptBase{public:CV_MurderTwistScript* ParentClass;CQ_SunnyvaleMasterData* PMasterData;CMurderTwistBuriedGem(CV_MurderTwistScript&,CScriptDataBase*,const CScriptThing&);private:virtual void OnPredicateFail();};};
void CV_MurderTwistScript::CMurderTwistBuriedGem::OnPredicateFail(){for(CScriptThing* i=ParentClass->MagicRuneRumbleList.begin();i!=ParentClass->MagicRuneRumbleList.end();++i){if(Me==*i){ParentClass->MagicRuneRumbleList.erase(i);break;}}}
}
#ifdef _MSC_VER
typedef char ParentSize[sizeof(NScript::CV_MurderTwistScript)==96?1:-1];typedef char EntitySize[sizeof(NScript::CV_MurderTwistScript::CMurderTwistBuriedGem)==28?1:-1];
#endif