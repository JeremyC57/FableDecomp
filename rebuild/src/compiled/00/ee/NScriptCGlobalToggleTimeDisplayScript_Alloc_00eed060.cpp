// NScript::CGlobal_ToggleTimeDisplayScript::Alloc(CScriptDataBase*,CGameScriptInterfaceBase&) -- allocate the original script with its game interface.
// Leaf: ?Alloc@CGlobal_ToggleTimeDisplayScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Identity: proven; own RegisterAllScripts registration00cda044 literalGlobal_ToggleTimeDisplay ->factory00eed060 ->ownvtable012f57a0. Exact PDB Alloc/data pointer/Game reference and public constructor(Game&) namesets. Data argument ignored.
// Model amendment: original protected CBaseClassNonCopyable ctor/dtor; genuine protected _List_base/rebound-node proxy and _Vector_base/end-storage proxy. Existing active vector APIs/bodies preserved verbatim; unused map/tree definitions removed.
// Layout: original typed CScriptBase64 (Threads list, EntityScripts/Bindings maps, runtime persist map) ->CLevelScriptNoMasterBase68 (Game+64), original own fields and PCsize72. No MasterScriptData here. Accepted main N228 shared typed prefix reused read-only; available handle headers aliased. Counted-pointer special members explicitly non-POD, original base ctor00cb8110/dtor00cbd510 external declarations. No fake flat vptr, extra callbacks or invented owner arguments.
#define CScriptThing CScriptThing_HeaderLayout
#include "engine/CScriptThing.h"
#undef CScriptThing
#define CScriptGameResourceObjectScriptedThingBase CScriptGameResourceObjectScriptedThingBase_HeaderLayout
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#undef CScriptGameResourceObjectScriptedThingBase
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
typedef unsigned long RetailUID;
#else
typedef unsigned __int64 RetailUID;
#endif
class CBaseClass{public:CBaseClass();virtual ~CBaseClass();};class CBaseClassNonCopyable:public CBaseClass{protected:CBaseClassNonCopyable();virtual ~CBaseClassNonCopyable();};
class CCharStringData;class CCharString{public:CCharStringData* PStringData;bool FastCompare(const CCharString&)const;};
class CCPPointerInfo;template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;CCountedPointer();CCountedPointer(const CCountedPointer&);~CCountedPointer();CCountedPointer& operator=(const CCountedPointer&);};
class CEntityScriptBindingBase;class CActiveEntityScriptBase;class CSpawnedFuncBase;
namespace std{
template<class A,class B>struct pair{A first;B second;};template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
struct _List_node_base{_List_node_base* _M_next;_List_node_base* _M_prev;};template<class T>struct _List_node:public _List_node_base{T _M_data;};template<class T,class A>class _List_base{protected:typedef _List_node<T> _Node;typedef allocator<_Node> _NodeAlloc;_STLP_alloc_proxy<_Node*,_Node,_NodeAlloc> _M_node;};template<class T,class A=allocator<T> >class list:protected _List_base<T,A>{};

template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{public:T* begin(){return this->_M_start;}T* end(){return this->_M_finish;}};
template<class Iter,class Compare>void __fastcall sort(Iter first,Iter last,Compare comp);
}
template<class T>class CArray:public std::vector<T>{};
template<class K,class V>class CKeyPairCompareLess{public:bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b)const{return a.first<b.first;}};
template<class V>class CCompareString{public:bool operator()(const std::pair<CCharString,V>& a,const std::pair<CCharString,V>& b)const{return a.first.FastCompare(b.first);}};
template<class K,class V,class Cmp=CKeyPairCompareLess<K,V> >class CVectorMap{public:CArray<std::pair<K,V> > Data;Cmp Compare;bool Dirty;__forceinline void Sort(){if(Dirty){std::sort(Data.begin(),Data.end(),Compare);Dirty=false;}}};

class CPersistContext;class CBaseObjectPointer;
class CBaseObject:public CBaseClass{public:CBaseObjectPointer* IntelligentPointer;};
class CScriptGameResourceObjectBase:public CBaseObject{};
class CScriptGameResourceObjectScriptedThingBase:public CScriptGameResourceObjectBase{public:CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;};
class CScriptThing:public CBaseClass{public:CCountedPointer<CScriptThing> PImp;};
namespace NScript{class CRuntimeScriptedEntityPersist{public:class CPersistInfo;CVectorMap<RetailUID,CPersistInfo> PersistScripts;};class CQ_SunnyvaleMasterData;}
class CScriptBase:public CBaseClassNonCopyable{public:CScriptBase();virtual ~CScriptBase();
std::list<CCountedPointer<CSpawnedFuncBase> > Threads;
CVectorMap<RetailUID,CCountedPointer<CActiveEntityScriptBase> > EntityScripts;
CVectorMap<CCharString,CCountedPointer<CEntityScriptBindingBase>,CCompareString<CCountedPointer<CEntityScriptBindingBase> > > EntityScriptBindings;
unsigned long LastWorldUpdateFrame;CSpawnedFuncBase* PActiveThread;NScript::CRuntimeScriptedEntityPersist RuntimeScriptedEntityPersist;
virtual void RegisterMain(){}virtual void Main(){}virtual void Init(){}virtual void OnPersist(CPersistContext&){}
};
class CGameScriptInterfaceBase;

class CScriptDataBase;class CLevelScriptNoMasterBase:public CScriptBase{public:CGameScriptInterfaceBase* Game;CLevelScriptNoMasterBase(CGameScriptInterfaceBase& game):Game(&game){}};
namespace NScript{class CGlobal_ToggleTimeDisplayScript:public CLevelScriptNoMasterBase{public:bool TimeDisplayActive;CGlobal_ToggleTimeDisplayScript(CGameScriptInterfaceBase& game):CLevelScriptNoMasterBase(game){}virtual void RegisterMain();virtual void Main();virtual void Init();virtual void OnPersist(CPersistContext&);static CScriptBase* __fastcall Alloc(CScriptDataBase*,CGameScriptInterfaceBase&);};CScriptBase* __fastcall CGlobal_ToggleTimeDisplayScript::Alloc(CScriptDataBase*,CGameScriptInterfaceBase& game){return new CGlobal_ToggleTimeDisplayScript(game);}}
#ifdef _MSC_VER
typedef char BaseSize[sizeof(CScriptBase)==64?1:-1];typedef char OwnerSize[sizeof(NScript::CGlobal_ToggleTimeDisplayScript)==72?1:-1];
#endif