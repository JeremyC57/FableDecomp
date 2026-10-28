// NScript::CV_TravellingHeroesScript::Init() -- Reset the script progress flags.
// Leaf: ?Init@CV_TravellingHeroesScript@NScript@@UAEXXZ
// Identity: own script-info registration 00cd98f2->00ee40a0 stores factory for literal V_TravellingHeroes, whose own allocation/constructor stores vtable012f407c; target is slot3 Init. Original field names from named PDB class layout, independently checked against own offsets and setter uses.
// Layout: shared handle headers aliased to restore real CBaseObject/CBaseClass hierarchy and counted pointers. Original CScriptBase64 typed list/maps, CLevelScriptBase72 Game/MasterScriptData; own registry uses common master-data allocator00cdbd20. Flags at120; PC owner size140. No opaque base prefix.
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
class CBaseClass{public:virtual ~CBaseClass(){}};class CBaseClassNonCopyable:public CBaseClass{protected:CBaseClassNonCopyable(){}~CBaseClassNonCopyable(){}private:CBaseClassNonCopyable(const CBaseClassNonCopyable&);CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);};
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
class CBaseObject:public CBaseClass{public:CBaseObjectPointer* IntelligentPointer;};
class CScriptGameResourceObjectBase:public CBaseObject{};
class CScriptGameResourceObjectScriptedThingBase:public CScriptGameResourceObjectBase{public:CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;};
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
namespace std{
template<class T>struct less{};template<class T>struct _Select1st{};
struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base* _M_parent;_Rb_tree_node_base* _M_left;_Rb_tree_node_base* _M_right;};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
template<class T,class A>class _Rb_tree_base{protected:_STLP_alloc_proxy<_Rb_tree_node<T>*,_Rb_tree_node<T>,allocator<_Rb_tree_node<T> > > _M_header;};
template<class K,class V,class S,class C,class A>class _Rb_tree:protected _Rb_tree_base<V,A>{public:unsigned int _M_node_count;C _M_key_compare;};
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map{public:_Rb_tree<K,pair<const K,V>,_Select1st<pair<const K,V> >,C,A> _M_t;};
}
namespace NScript{class CV_TravellingHeroesScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:CScriptGameResourceObjectScriptedThingBase seh_Rival,seh_Girl,seh_Boy;bool ThunderKidsTalking,ThunderSceneStarted,ThunderSceneDone,MazeCutsceneDone,WhisperCutscenePlayed,SnowTrollSceneDone;std::map<CCharString,CCharString> csargs;virtual void Init();};
void CV_TravellingHeroesScript::Init(){WhisperCutscenePlayed=false;ThunderKidsTalking=false;ThunderSceneStarted=false;ThunderSceneDone=false;MazeCutsceneDone=false;SnowTrollSceneDone=false;}}
#ifdef _MSC_VER
typedef char ExactBaseSize[sizeof(CScriptBase)==64?1:-1];typedef char ExactOwnerSize[sizeof(NScript::CV_TravellingHeroesScript)==140?1:-1];
#endif