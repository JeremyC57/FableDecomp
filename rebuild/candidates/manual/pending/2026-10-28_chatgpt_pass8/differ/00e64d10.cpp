// NScript::CV_ChickenKickingScript::CChickenSign::Alloc(parent,data,me) -- allocate the original entity script and return its counted pointer.
// Leaf: ?Alloc@CChickenSign@CV_ChickenKickingScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Identity: proven own named binding registration00e62b4d ->factory00e64d10 ->original constructor00e64d90 and exact PDB owner/Alloc/constructor prototypes. Public CCountedPointer<CEntityScriptBase>(T*) and static fastcall DeleteData(void*) are PDB-proven; own controlblock stores callback00cdee00, which consumes ECX and deletes through entity virtual dtor.
// Layout: actual entity20 Game/Me prefix +original parent/master fields and typed own members, PCsize40. True counted pointer8 and controlblock12 RefCount/fastcallDeleteFunc/Data; no invented constructor/helper signatures for controlblock. Source directly initializes original fields before assigning Info. Non-POD default/copy/dtor declarations preserve native class-return hidden result ABI; genuine original virtual Entity destructor and correct out-of-line constructor target. Parent typed level72/script64 prefix retained, unused parent suffix explicitly omitted. Shared handle headers aliased.
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
class CBaseClass{public:virtual ~CBaseClass(){}};class CBaseClassNonCopyable:public CBaseClass{};
class CCharStringData;class CCharString{public:CCharString();CCharString(const CCharString&);CCharString(const char*,long=-1);~CCharString();CCharStringData* PStringData;bool FastCompare(const CCharString&)const;};
class CCPPointerInfo{public:unsigned long RefCount;void(__fastcall *DeleteFunc)(void*);void* Data;};template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;CCountedPointer();CCountedPointer(const CCountedPointer&);~CCountedPointer();CCountedPointer& operator=(const CCountedPointer&);static void __fastcall DeleteData(void*);CCountedPointer(T* data):Data(data){if(Data){CCPPointerInfo* info=new CCPPointerInfo;if(info){info->RefCount=1;info->DeleteFunc=&DeleteData;info->Data=Data;}Info=info;}else Info=0;}};

class CEntityScriptBindingBase;class CActiveEntityScriptBase;class CSpawnedFuncBase;
namespace std{
template<class A,class B>struct pair{A first;B second;};template<class T>class allocator{};
struct _List_node_base{_List_node_base* _M_next;_List_node_base* _M_prev;};template<class T,class A=allocator<T> >class list{public:_List_node_base* _M_node;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:T* begin(){return this->_M_start;}T* end(){return this->_M_finish;}};
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

namespace std{
template<class T>struct less{};template<class T>struct _Select1st{};
struct _Rb_tree_node_base{int _M_color;_Rb_tree_node_base* _M_parent;_Rb_tree_node_base* _M_left;_Rb_tree_node_base* _M_right;};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
template<class T,class A>class _Rb_tree_base{public:_STLP_alloc_proxy<_Rb_tree_node<T>*,T,A> _M_header;};
template<class K,class V,class S,class C,class A>class _Rb_tree:public _Rb_tree_base<V,A>{public:unsigned int _M_node_count;C _M_key_compare;};
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map{public:_Rb_tree<K,pair<const K,V>,_Select1st<pair<const K,V> >,C,A> _M_t;};
}
class CScriptDataBase;template<class T>class CLevelScriptBase:public CScriptBase{public:CGameScriptInterfaceBase* Game;T* MasterScriptData;CLevelScriptBase(CScriptDataBase* data,CGameScriptInterfaceBase& game):Game(&game),MasterScriptData((T*)data){}};class CEntityScriptBase{public:CGameScriptInterfaceBase* Game;CScriptThing Me;CEntityScriptBase(CGameScriptInterfaceBase&,const CScriptThing&);virtual ~CEntityScriptBase();};class CWideString;
namespace NScript{class CV_ChickenKickingScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:CV_ChickenKickingScript(CScriptDataBase*,CGameScriptInterfaceBase&);class CChickenSign:public CEntityScriptBase{public:CV_ChickenKickingScript* ParentClass;CQ_SunnyvaleMasterData* PMasterData;std::vector<CWideString> TextKeys;CChickenSign(CV_ChickenKickingScript&,CScriptDataBase*,const CScriptThing&);static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ChickenKickingScript&,CScriptDataBase*,const CScriptThing&);};};CCountedPointer<CEntityScriptBase> __fastcall CV_ChickenKickingScript::CChickenSign::Alloc(CV_ChickenKickingScript& parent,CScriptDataBase* data,const CScriptThing& me){return CCountedPointer<CEntityScriptBase>(new CChickenSign(parent,data,me));}}
#ifdef _MSC_VER
typedef char EntitySize[sizeof(NScript::CV_ChickenKickingScript::CChickenSign)==40?1:-1];typedef char InfoSize[sizeof(CCPPointerInfo)==12?1:-1];
#endif
