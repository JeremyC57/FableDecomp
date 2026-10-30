// NScript::CV_MayorsInvitationScript::OnPersist(CPersistContext&) -- transfer six original progress fields in script order.
// Leaf: ?OnPersist@CV_MayorsInvitationScript@NScript@@UAEXAAVCPersistContext@@@Z
// Identity: proven original field-name literals in own body reconcile original named PDB owner layout and public virtual OnPersist signature; actual script virtualslot4. PATH_ID/STAGE_IN_PATH are original nested enums of CV_ExposeMayorScript (PDB template Transfer qualifiers), zero enumerator names explicitly descriptive. Exact bool/long/enum reference Transfer<T>(constchar*,T&,constT&) APIs.
// Model amendment: original protected CBaseClassNonCopyable ctor/dtor; genuine protected _List_base/rebound-node proxy and _Vector_base/end-storage proxy. Existing active vector APIs/bodies preserved verbatim; unused map/tree definitions removed.
// Layout: accepted original typed CScriptBase64/CLevelScriptBase72, original own fields and full typed handle inheritance; retail size108. CCountedPointer specialmembers retained; actual CCharString ctor(constchar*,long) and non-POD members, true context44 typed prefix. Unused original interfaces omitted explicitly; shared handle headers aliased. Own flag/scalar offsets reconcile donor20-byte larger CScriptBase.
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

class CScriptDataBase;template<class T>class CLevelScriptBase:public CScriptBase{public:CGameScriptInterfaceBase* Game;T* MasterScriptData;CLevelScriptBase(CScriptDataBase* data,CGameScriptInterfaceBase& game):Game(&game),MasterScriptData((T*)data){}};
#ifndef _MSC_VER
#define __cdecl
#endif
class CDefinitionManager;class CSymbolMap;class CStringParser;class CMemoryDataInputStream;class CDataOutputStream;class CPersistContext{public:CDefinitionManager* PDefinitionManager;CSymbolMap* PSymbolMap;CStringParser* PParser;void (__cdecl *ErrorCallback)(CCharString*);CCharString* PStringToClean;bool UseDefaults,UseCRCCheck,SingleTransfer,Finished;enum EMode{EMode_DescriptiveZero=0};EMode Mode;bool Binary;CCharString* PStringToSaveTo;CMemoryDataInputStream* PLoadStream;CDataOutputStream* PSaveStream;template<class T>void Transfer(const char*,T&,const T& fallback=T());};
namespace NScript{class CV_MayorsInvitationScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:bool MissionSucceeded,MissionFailed,MissionOver,HeroAbortedQuest,HeroLearnedSecret,HeroReturnedFromParanoidQuest,HeroHasDefeatedThunder,HeroHasLostToThunder,QuestCardGiven;long SecretsLearned,WhichQuest,MonitorThunderHeroDuelRunning;CScriptThing destinationHSP;CV_MayorsInvitationScript(CScriptDataBase*,CGameScriptInterfaceBase&);virtual void OnPersist(CPersistContext&);};void CV_MayorsInvitationScript::OnPersist(CPersistContext& context){context.Transfer("WhichQuest",WhichQuest);context.Transfer("SecretsLearned",SecretsLearned);context.Transfer("HeroLearnedSecret",HeroLearnedSecret);context.Transfer("HeroHasDefeatedThunder",HeroHasDefeatedThunder);context.Transfer("HeroHasLostToThunder",HeroHasLostToThunder);context.Transfer("HeroReturnedFromParanoidQuest",HeroReturnedFromParanoidQuest);}}
#ifdef _MSC_VER
typedef char BaseSize[sizeof(CScriptBase)==64?1:-1];typedef char ContextSize[sizeof(CPersistContext)==44?1:-1];typedef char OwnerSize[sizeof(NScript::CV_MayorsInvitationScript)==108?1:-1];
#endif