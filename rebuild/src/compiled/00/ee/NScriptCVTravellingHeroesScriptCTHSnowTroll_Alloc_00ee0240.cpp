// NScript::CV_TravellingHeroesScript::CTH_SnowTroll::Alloc(parent,data,me) -- allocate the original entity script and return its counted pointer.
// Leaf: ?Alloc@CTH_SnowTroll@CV_TravellingHeroesScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Identity: proven own named binding registration00edef50 ->factory00ee0240 ->original constructor00ee02c0 and exact PDB owner/Alloc/constructor prototypes. Public CCountedPointer<CEntityScriptBase>(T*) and static fastcall DeleteData(void*) are PDB-proven; own controlblock stores callback00cdee00, which consumes ECX and deletes through entity virtual dtor.
// Model amendment: original protected CBaseClassNonCopyable ctor/dtor; genuine protected _List_base/rebound-node proxy and _Vector_base/end-storage proxy. Existing active vector APIs/bodies preserved verbatim; unused map/tree definitions removed.
// Proven original constructor revision: FableWin nameset ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z declares public (unsignedlong,void(__fastcall*)(void*),void*); exact3-field constructor now used by ordinary new-expression. Earlier claim that no original controlblock ctor was proven was incorrect. This is not the prior unproven2arg signature. Own allocation and field stores corroborate argument roles; donor address used only asnameset.
// Layout: actual entity20 Game/Me prefix +original parent/master fields and typed own members, PCsize52. True counted pointer8 and controlblock12 RefCount/fastcallDeleteFunc/Data; no invented constructor/helper signatures for controlblock. Source uses the original PDB-proven three-argument controlblock constructor with the observed field initialization order; no invented helper or constructor signature. Non-POD default/copy/dtor declarations preserve native class-return hidden result ABI; genuine original virtual Entity destructor and correct out-of-line constructor target. Parent typed level72/script64 prefix retained, unused parent suffix explicitly omitted. Shared handle headers aliased.
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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable:public CBaseClass{protected:CBaseClassNonCopyable();virtual ~CBaseClassNonCopyable();};
class CCharStringData;class CCharString{public:CCharString();CCharString(const CCharString&);CCharString(const char*,long=-1);~CCharString();CCharStringData* PStringData;bool FastCompare(const CCharString&)const;};
class CCPPointerInfo{public:unsigned long RefCount;void(__fastcall *DeleteFunc)(void*);void* Data;CCPPointerInfo(unsigned long refs,void(__fastcall *deleter)(void*),void* data):RefCount(refs),DeleteFunc(deleter),Data(data){}};template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;CCountedPointer();CCountedPointer(const CCountedPointer&);~CCountedPointer();CCountedPointer& operator=(const CCountedPointer&);static void __fastcall DeleteData(void*);CCountedPointer(T* data):Data(data){if(Data)Info=new CCPPointerInfo(1,&DeleteData,Data);else Info=0;}};

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

class CScriptDataBase;template<class T>class CLevelScriptBase:public CScriptBase{public:CGameScriptInterfaceBase* Game;T* MasterScriptData;CLevelScriptBase(CScriptDataBase* data,CGameScriptInterfaceBase& game):Game(&game),MasterScriptData((T*)data){}};class CEntityScriptBase{public:CGameScriptInterfaceBase* Game;CScriptThing Me;CEntityScriptBase(CGameScriptInterfaceBase&,const CScriptThing&);virtual ~CEntityScriptBase();};class CWideString;
namespace NScript{class CV_TravellingHeroesScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:CV_TravellingHeroesScript(CScriptDataBase*,CGameScriptInterfaceBase&);class CTH_SnowTroll:public CEntityScriptBase{public:CV_TravellingHeroesScript* ParentClass;CQ_SunnyvaleMasterData* PMasterData;CScriptThing Entrance,Exit;CTH_SnowTroll(CV_TravellingHeroesScript&,CScriptDataBase*,const CScriptThing&);static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_TravellingHeroesScript&,CScriptDataBase*,const CScriptThing&);};};CCountedPointer<CEntityScriptBase> __fastcall CV_TravellingHeroesScript::CTH_SnowTroll::Alloc(CV_TravellingHeroesScript& parent,CScriptDataBase* data,const CScriptThing& me){return CCountedPointer<CEntityScriptBase>(new CTH_SnowTroll(parent,data,me));}}
#ifdef _MSC_VER
typedef char EntitySize[sizeof(NScript::CV_TravellingHeroesScript::CTH_SnowTroll)==52?1:-1];typedef char InfoSize[sizeof(CCPPointerInfo)==12?1:-1];
#endif