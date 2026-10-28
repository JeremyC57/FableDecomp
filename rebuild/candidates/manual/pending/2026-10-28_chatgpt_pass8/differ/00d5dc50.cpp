// NScript::CQ_GuildTrainingWillScript::CQ_GuildTrainingWillScript(CScriptDataBase* data,CGameScriptInterfaceBase& game) -- Initialize script ownership and its empty vector.
// Leaf: ??0CQ_GuildTrainingWillScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Identity: own script-info registration00cd62fe literalQ_GuildTrainingWill -> factory -> constructor00d5dc50/vtable012d1038; exact original PDB signature.
// Layout: real typed CScriptBase64/CLevelScriptBase72, original named fields and STLport containers; PC allocation108. Handle headers included under aliases to restore original inheritance. Only vector default initialization emits stores; scalar progress state is left for Init.
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
class CCharStringData;class CCharString{public:CCharStringData* PStringData;bool FastCompare(const CCharString&)const;};
class CCPPointerInfo;template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;};
class CEntityScriptBindingBase;class CActiveEntityScriptBase;class CSpawnedFuncBase;
namespace std{
template<class A,class B>struct pair{A first;B second;};template<class T>class allocator{};
struct _List_node_base{_List_node_base* _M_next;_List_node_base* _M_prev;};template<class T,class A=allocator<T> >class list{public:_List_node_base* _M_node;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A& a,V p):A(a),_M_data(p){}};
template<class T,class A>class _Vector_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;_Vector_base(const A& a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:vector():_Vector_base<T,A>(A()){}T* begin(){return this->_M_start;}T* end(){return this->_M_finish;}};
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
class CScriptBase:public CBaseClassNonCopyable{public:CScriptBase();
std::list<CCountedPointer<CSpawnedFuncBase> > Threads;
CVectorMap<RetailUID,CCountedPointer<CActiveEntityScriptBase> > EntityScripts;
CVectorMap<CCharString,CCountedPointer<CEntityScriptBindingBase>,CCompareString<CCountedPointer<CEntityScriptBindingBase> > > EntityScriptBindings;
unsigned long LastWorldUpdateFrame;CSpawnedFuncBase* PActiveThread;NScript::CRuntimeScriptedEntityPersist RuntimeScriptedEntityPersist;
virtual void RegisterMain(){}virtual void Main(){}virtual void Init(){}virtual void OnPersist(CPersistContext&){}
};
class CGameScriptInterfaceBase;class CScriptDataBase;
template<class T>class CLevelScriptBase:public CScriptBase{public:CGameScriptInterfaceBase* Game;T* MasterScriptData;CLevelScriptBase(CScriptDataBase* data,CGameScriptInterfaceBase& game):Game(&game),MasterScriptData((T*)data){} };
namespace std{
template<class T>struct less{};template<class T>struct _Select1st{};
struct _Rb_tree_node_base{int _M_color;_Rb_tree_node_base* _M_parent;_Rb_tree_node_base* _M_left;_Rb_tree_node_base* _M_right;};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
template<class T,class A>class _Rb_tree_base{public:_STLP_alloc_proxy<_Rb_tree_node<T>*,T,A> _M_header;};
template<class K,class V,class S,class C,class A>class _Rb_tree:public _Rb_tree_base<V,A>{public:unsigned int _M_node_count;C _M_key_compare;};
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map{public:_Rb_tree<K,pair<const K,V>,_Select1st<pair<const K,V> >,C,A> _M_t;};
}
class CTimer{public:long TimerIndex;};
namespace NScript{class CQ_GuildTrainingWillScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:long TargetsHit,TutorialState,TotalTrainingDummiesCounter,GenericTutorialCounter,HeroHit;bool TestFinished,BanditsDefeated;std::vector<CScriptThing> DummyWizardsVector;CQ_GuildTrainingWillScript(CScriptDataBase* data,CGameScriptInterfaceBase& game);virtual void RegisterMain();virtual void Main();virtual void Init();virtual void OnPersist(CPersistContext&);static CScriptBase* __fastcall Alloc(CScriptDataBase*,CGameScriptInterfaceBase&);};
CQ_GuildTrainingWillScript::CQ_GuildTrainingWillScript(CScriptDataBase* data,CGameScriptInterfaceBase& game):CLevelScriptBase<CQ_SunnyvaleMasterData>(data,game){}
}
#ifdef _MSC_VER
typedef char ExactBaseSize[sizeof(CScriptBase)==64?1:-1];typedef char ExactOwnerSize[sizeof(NScript::CQ_GuildTrainingWillScript)==108?1:-1];
#endif
