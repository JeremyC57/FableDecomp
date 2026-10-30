// NScript::CV_MurderTwistScript::CMurderTwistBrother::OnPredicateFail() -- erase the matching magic-rune entity on predicate failure.
// Leaf: ?OnPredicateFail@CMurderTwistBrother@CV_MurderTwistScript@NScript@@EAEXXZ
// Identity: proven own named binding00ea8943 ->original class factory ->vtable012ef2dc slot5; original PDB privatevirtual EAE method. Actual entity20 Game/Me prefix, parent/master+20/+24, original BrainState28 where present; parent exact script96 with ItemsFound72/State76/Event80 and typed vector84.
// Model amendment: original protected CBaseClassNonCopyable ctor/dtor; genuine protected _List_base/rebound-node proxy and _Vector_base/end-storage proxy. Existing active vector APIs/bodies preserved verbatim; unused map/tree definitions removed.
// Layout: real typed script/level/handle storage; original CScriptThing nonvirtual operator== forwards protected virtual IsEqualTo(constThing&)const slot78 through original PImp.Data; base default IsEqualTo returns false as own004aa8a0. Original game const RemoveThing108/EntitySetAsKillable517; genuine STLport vector begin/end/erase/push_back/_M_insert_overflow. Counted-info unsigned refcount and copy constructor retain actual addref. Unused virtual slots are descriptive projections; no fabricated active APIs. Shared handle headers aliased; fixtures alone expose protected storage.
#define CScriptThing CScriptThing_HeaderLayout
#include "engine/CScriptThing.h"
#undef CScriptThing
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
class CBaseClassNonCopyable:public CBaseClass{protected:CBaseClassNonCopyable();virtual ~CBaseClassNonCopyable();};
class CCharStringData;class CCharString{public:CCharString();CCharString(const CCharString&);CCharString(const char*,long=-1);~CCharString();CCharStringData* PStringData;bool FastCompare(const CCharString&)const;};
class CCPPointerInfo{public:unsigned long RefCount;void(__fastcall* DeleteFunc)(void*);void* Data;};template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;CCountedPointer():Data(0),Info(0){}CCountedPointer(const CCountedPointer& x):Data(x.Data),Info(x.Info){if(Info)++Info->RefCount;}~CCountedPointer();CCountedPointer& operator=(const CCountedPointer&);T* operator->()const{return Data;}T& operator*()const{return *Data;}};
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
class CScriptThing:public CBaseClass{public:CCountedPointer<CScriptThing> PImp;bool operator==(const CScriptThing& other)const{return PImp->IsEqualTo(*other.PImp);}protected:virtual void UnknownThingSlot1(){}virtual void UnknownThingSlot2(){}virtual void UnknownThingSlot3(){}virtual void UnknownThingSlot4(){}virtual void UnknownThingSlot5(){}virtual void UnknownThingSlot6(){}virtual void UnknownThingSlot7(){}virtual void UnknownThingSlot8(){}virtual void UnknownThingSlot9(){}virtual void UnknownThingSlot10(){}virtual void UnknownThingSlot11(){}virtual void UnknownThingSlot12(){}virtual void UnknownThingSlot13(){}virtual void UnknownThingSlot14(){}virtual void UnknownThingSlot15(){}virtual void UnknownThingSlot16(){}virtual void UnknownThingSlot17(){}virtual void UnknownThingSlot18(){}virtual void UnknownThingSlot19(){}virtual void UnknownThingSlot20(){}virtual void UnknownThingSlot21(){}virtual void UnknownThingSlot22(){}virtual void UnknownThingSlot23(){}virtual void UnknownThingSlot24(){}virtual void UnknownThingSlot25(){}virtual void UnknownThingSlot26(){}virtual void UnknownThingSlot27(){}virtual void UnknownThingSlot28(){}virtual void UnknownThingSlot29(){}virtual void UnknownThingSlot30(){}virtual void UnknownThingSlot31(){}virtual void UnknownThingSlot32(){}virtual void UnknownThingSlot33(){}virtual void UnknownThingSlot34(){}virtual void UnknownThingSlot35(){}virtual void UnknownThingSlot36(){}virtual void UnknownThingSlot37(){}virtual void UnknownThingSlot38(){}virtual void UnknownThingSlot39(){}virtual void UnknownThingSlot40(){}virtual void UnknownThingSlot41(){}virtual void UnknownThingSlot42(){}virtual void UnknownThingSlot43(){}virtual void UnknownThingSlot44(){}virtual void UnknownThingSlot45(){}virtual void UnknownThingSlot46(){}virtual void UnknownThingSlot47(){}virtual void UnknownThingSlot48(){}virtual void UnknownThingSlot49(){}virtual void UnknownThingSlot50(){}virtual void UnknownThingSlot51(){}virtual void UnknownThingSlot52(){}virtual void UnknownThingSlot53(){}virtual void UnknownThingSlot54(){}virtual void UnknownThingSlot55(){}virtual void UnknownThingSlot56(){}virtual void UnknownThingSlot57(){}virtual void UnknownThingSlot58(){}virtual void UnknownThingSlot59(){}virtual void UnknownThingSlot60(){}virtual void UnknownThingSlot61(){}virtual void UnknownThingSlot62(){}virtual void UnknownThingSlot63(){}virtual void UnknownThingSlot64(){}virtual void UnknownThingSlot65(){}virtual void UnknownThingSlot66(){}virtual void UnknownThingSlot67(){}virtual void UnknownThingSlot68(){}virtual void UnknownThingSlot69(){}virtual void UnknownThingSlot70(){}virtual void UnknownThingSlot71(){}virtual void UnknownThingSlot72(){}virtual void UnknownThingSlot73(){}virtual void UnknownThingSlot74(){}virtual void UnknownThingSlot75(){}virtual void UnknownThingSlot76(){}virtual void UnknownThingSlot77(){}virtual bool IsEqualTo(const CScriptThing&)const{return false;}};
namespace NScript{class CRuntimeScriptedEntityPersist{public:class CPersistInfo;CVectorMap<RetailUID,CPersistInfo> PersistScripts;};class CQ_SunnyvaleMasterData;}
class CScriptBase:public CBaseClassNonCopyable{public:CScriptBase();virtual ~CScriptBase();
std::list<CCountedPointer<CSpawnedFuncBase> > Threads;
CVectorMap<RetailUID,CCountedPointer<CActiveEntityScriptBase> > EntityScripts;
CVectorMap<CCharString,CCountedPointer<CEntityScriptBindingBase>,CCompareString<CCountedPointer<CEntityScriptBindingBase> > > EntityScriptBindings;
unsigned long LastWorldUpdateFrame;CSpawnedFuncBase* PActiveThread;NScript::CRuntimeScriptedEntityPersist RuntimeScriptedEntityPersist;
virtual void RegisterMain(){}virtual void Main(){}virtual void Init(){}virtual void OnPersist(CPersistContext&){}
};
class CGameScriptInterfaceBase;

class CScriptDataBase;template<class T>class CLevelScriptBase:public CScriptBase{public:CGameScriptInterfaceBase* Game;T* MasterScriptData;CLevelScriptBase(CScriptDataBase* data,CGameScriptInterfaceBase& game):Game(&game),MasterScriptData((T*)data){}};class CEntityScriptBase{public:CGameScriptInterfaceBase* Game;CScriptThing Me;CEntityScriptBase(CGameScriptInterfaceBase&,const CScriptThing&);virtual ~CEntityScriptBase(){}virtual void UnknownEntitySlot1(){}virtual void Init(){}virtual void UnknownEntitySlot3(){}virtual void OnPersist(CPersistContext&){}virtual void OnPredicateFail(){};};
namespace NScript{class CV_MurderTwistScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:long MurderTwistItemsFound,MurderTwistState,MurderTwistEvent;std::vector<CScriptThing> MagicRuneRumbleList;CV_MurderTwistScript(CScriptDataBase*,CGameScriptInterfaceBase&);class CMurderTwistBrother:public CEntityScriptBase{public:CV_MurderTwistScript* ParentClass;CQ_SunnyvaleMasterData* PMasterData;long BrainState;CMurderTwistBrother(CV_MurderTwistScript&,CScriptDataBase*,const CScriptThing&);private:virtual void OnPredicateFail();};};
void CV_MurderTwistScript::CMurderTwistBrother::OnPredicateFail(){for(CScriptThing* i=ParentClass->MagicRuneRumbleList.begin();i!=ParentClass->MagicRuneRumbleList.end();++i){if(Me==*i){ParentClass->MagicRuneRumbleList.erase(i);break;}}}
}
#ifdef _MSC_VER
typedef char ParentSize[sizeof(NScript::CV_MurderTwistScript)==96?1:-1];typedef char EntitySize[sizeof(NScript::CV_MurderTwistScript::CMurderTwistBrother)==32?1:-1];
#endif