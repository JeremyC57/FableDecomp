// NScript::CQ_GuildTrainingWillScript::RegisterMain() -- Register the named entity factory then finish local and game binding setup.
// Leaf: ?RegisterMain@CQ_GuildTrainingWillScript@NScript@@UAEXXZ
// Identity: own binding literalTheRealGuildmaster and factory00d5dde0; original PDB parent/nested factory names and ctor signature; binding vtable012d1068 slots0 GetEntityScriptName/1 CreateActiveEntityScript. Actual game PostAddScriptedEntities slot64.
// Layout: original typed script base and class-template binding28 with CCharString+4,parent+8,member-function pointer+12,factory+16,bool+20,Flags+24. No extra binding destructor/base slot. Parent declaration omits unused own suffix; nested class is a static Alloc API projection only. Shared handle headers aliased.
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
class CBaseClassNonCopyable:public CBaseClass{protected:CBaseClassNonCopyable(){}~CBaseClassNonCopyable(){}private:CBaseClassNonCopyable(const CBaseClassNonCopyable&);CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);};
class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString():PStringData(0){}CCharString(const char*,long=-1);CCharString(const CCharString&);~CCharString();bool FastCompare(const CCharString&)const;};
class CCPPointerInfo;template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;CCountedPointer();CCountedPointer(const CCountedPointer&);~CCountedPointer();};
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
void AddEntityScriptBinding(CEntityScriptBindingBase*);void PostAddScriptedEntities();virtual void RegisterMain(){}virtual void Main(){}virtual void Init(){}virtual void OnPersist(CPersistContext&){}
};
class CGameScriptInterfaceBase;
template<class T>class CLevelScriptBase:public CScriptBase{public:CGameScriptInterfaceBase* Game;T* MasterScriptData;};
class CPersistContext{public:template<class T>void Transfer(const char*,T&,const T&);};class CTimer{public:long TimerIndex;};
class CEntityScriptBase{public:virtual ~CEntityScriptBase(){}CGameScriptInterfaceBase* Game;CScriptThing Me;};class CScriptDataBase;
class CEntityScriptBindingBase{public:virtual const CCharString& GetEntityScriptName()const=0;protected:virtual CCountedPointer<CActiveEntityScriptBase> CreateActiveEntityScript(const CScriptThing&,RetailUID)=0;};
template<class T>class CEntityScriptBinding:public CEntityScriptBindingBase{public:typedef CCountedPointer<CEntityScriptBase>(__fastcall *TAllocFunc)(T&,CScriptDataBase*,const CScriptThing&);CCharString EntityScriptName;T* PParentScript;void(T::*Func)(CScriptThing);TAllocFunc ClassAllocFunc;bool ClassAllocValid;unsigned long Flags;CEntityScriptBinding(const CCharString& name,T* parent,TAllocFunc alloc,unsigned long flags):EntityScriptName(name),PParentScript(parent),ClassAllocFunc(alloc),ClassAllocValid(true),Flags(flags){}virtual const CCharString& GetEntityScriptName()const{return EntityScriptName;}protected:virtual CCountedPointer<CActiveEntityScriptBase> CreateActiveEntityScript(const CScriptThing&,RetailUID);};
class CGameScriptInterfaceBase:public CBaseClassNonCopyable{public:virtual void UnknownSlot1(){}virtual void UnknownSlot2(){}virtual void UnknownSlot3(){}virtual void UnknownSlot4(){}virtual void UnknownSlot5(){}virtual void UnknownSlot6(){}virtual void UnknownSlot7(){}virtual void UnknownSlot8(){}virtual void UnknownSlot9(){}virtual void UnknownSlot10(){}virtual void UnknownSlot11(){}virtual void UnknownSlot12(){}virtual void UnknownSlot13(){}virtual void UnknownSlot14(){}virtual void UnknownSlot15(){}virtual void UnknownSlot16(){}virtual void UnknownSlot17(){}virtual void UnknownSlot18(){}virtual void UnknownSlot19(){}virtual void UnknownSlot20(){}virtual void UnknownSlot21(){}virtual void UnknownSlot22(){}virtual void UnknownSlot23(){}virtual void UnknownSlot24(){}virtual void UnknownSlot25(){}virtual void UnknownSlot26(){}virtual void UnknownSlot27(){}virtual void UnknownSlot28(){}virtual void UnknownSlot29(){}virtual void UnknownSlot30(){}virtual void UnknownSlot31(){}virtual void UnknownSlot32(){}virtual void UnknownSlot33(){}virtual void UnknownSlot34(){}virtual void UnknownSlot35(){}virtual void UnknownSlot36(){}virtual void UnknownSlot37(){}virtual void UnknownSlot38(){}virtual void UnknownSlot39(){}virtual void UnknownSlot40(){}virtual void UnknownSlot41(){}virtual void UnknownSlot42(){}virtual void UnknownSlot43(){}virtual void UnknownSlot44(){}virtual void UnknownSlot45(){}virtual void UnknownSlot46(){}virtual void UnknownSlot47(){}virtual void UnknownSlot48(){}virtual void UnknownSlot49(){}virtual void UnknownSlot50(){}virtual void UnknownSlot51(){}virtual void UnknownSlot52(){}virtual void UnknownSlot53(){}virtual void UnknownSlot54(){}virtual void UnknownSlot55(){}virtual void UnknownSlot56(){}virtual void UnknownSlot57(){}virtual void UnknownSlot58(){}virtual void UnknownSlot59(){}virtual void UnknownSlot60(){}virtual void UnknownSlot61(){}virtual void UnknownSlot62(){}virtual void UnknownSlot63(){}virtual void PostAddScriptedEntities()const=0;};
namespace NScript{class CQ_GuildTrainingWillScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:class CTheRealGuildmaster:public CEntityScriptBase{public:static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CQ_GuildTrainingWillScript&,CScriptDataBase*,const CScriptThing&);};virtual void RegisterMain();};void CQ_GuildTrainingWillScript::RegisterMain(){AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingWillScript>("TheRealGuildmaster",this,CTheRealGuildmaster::Alloc,1));PostAddScriptedEntities();Game->PostAddScriptedEntities();}}
#ifdef _MSC_VER
typedef char ExactBindingSize[sizeof(CEntityScriptBinding<NScript::CQ_GuildTrainingWillScript>)==28?1:-1];
#endif