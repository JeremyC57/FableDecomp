// NScript::CQ_BountyHuntScript::WatchForHostage2Killed(CScriptThing thing) -- Watch the hostage death message until cancellation.
// Leaf: ?WatchForHostage2Killed@CQ_BountyHuntScript@NScript@@QAEXVCScriptThing@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: Own CBH_Hostage2 Init builds spawned member callback00d16c85 with literal WatchForHostage2Killed and ParentClass., passes parent+20; original PDB CQ_BountyHuntScript public method(CScriptThing by value). Real flag85 reconciles typed script base. CScriptThing MsgIsKilledBy const slot18, Game NewScriptFrame const slot7, CScriptBase IsActiveThreadTerminating00cb7940.
// Layout: real typed script/level/container/handle ancestry; parent Game64 and flags84/85. Original CScriptThing vtable and counted-pointer destructor/copy semantics, no synthetic dispatch. Unused virtual slots descriptive. Shared headers aliased for real hierarchy.
#define CScriptThing CScriptThing_HeaderLayout
#include "engine/CScriptThing.h"
#undef CScriptThing
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
typedef unsigned long RetailUID;
#else
typedef unsigned __int64 RetailUID;
#endif
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString(const char*,long=-1);~CCharString();bool FastCompare(const CCharString&)const;};
#include "engine/CCountedPointer.h"

class CEntityScriptBindingBase;class CActiveEntityScriptBase;class CSpawnedFuncBase;
#include "engine/CArray.h"
template<class K,class V>class CKeyPairCompareLess{public:bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b)const{return a.first<b.first;}};
template<class V>class CCompareString{public:bool operator()(const std::pair<CCharString,V>& a,const std::pair<CCharString,V>& b)const{return a.first.FastCompare(b.first);}};
template<class K,class V,class Cmp=CKeyPairCompareLess<K,V> >class CVectorMap{public:CArray<std::pair<K,V> > Data;Cmp Compare;bool Dirty;__forceinline void Sort(){if(Dirty){std::sort(Data.begin(),Data.end(),Compare);Dirty=false;}}};

class CPersistContext;class CBaseObjectPointer;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
class CScriptThing:public CBaseClass{public:CCountedPointer<CScriptThing> PImp;CScriptThing(){}CScriptThing(const CScriptThing& s): CBaseClass(s), PImp(s.PImp){}virtual ~CScriptThing(){}virtual void UnknownSlot1(){}virtual void UnknownSlot2(){}virtual void UnknownSlot3(){}virtual void UnknownSlot4(){}virtual void UnknownSlot5(){}virtual void UnknownSlot6(){}virtual void UnknownSlot7(){}virtual void UnknownSlot8(){}virtual void UnknownSlot9(){}virtual void UnknownSlot10(){}virtual void UnknownSlot11(){}virtual void UnknownSlot12(){}virtual void UnknownSlot13(){}virtual void UnknownSlot14(){}virtual void UnknownSlot15(){}virtual void UnknownSlot16(){}virtual void UnknownSlot17(){}virtual bool MsgIsKilledBy(const CCharString& who)const{if(!PImp.Data)return false;return PImp->MsgIsKilledBy(who);}};
namespace NScript{class CRuntimeScriptedEntityPersist{public:class CPersistInfo;CVectorMap<RetailUID,CPersistInfo> PersistScripts;};class CQ_SunnyvaleMasterData;}
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CGameScriptInterfaceBase;
class CGameScriptInterfaceBase:public CBaseClassNonCopyable{public:virtual void UnknownSlot1(){}virtual void UnknownSlot2(){}virtual void UnknownSlot3(){}virtual void UnknownSlot4(){}virtual void UnknownSlot5(){}virtual void UnknownSlot6(){}virtual void NewScriptFrame()const=0;};
namespace NScript{class CQ_BountyHuntScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:bool QuestFailed;bool QuestSucceeded;bool QuestOver;bool QuestStarted;bool FishermanDeactivated;bool VillagerKilled;bool VillagerIntroDone;bool LeaderKilled;bool Deputy1Killed;bool Deputy2Killed;bool Deputy1IntroComplete;bool Deputy2IntroComplete;bool Hostage1Killed;bool Hostage2Killed;void WatchForHostage2Killed(CScriptThing);};void CQ_BountyHuntScript::WatchForHostage2Killed(CScriptThing thing){while(!thing.MsgIsKilledBy("")){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;}
if(IsActiveThreadTerminating())return;Hostage2Killed=true;
return;
}}
#ifdef _MSC_VER
typedef char FlagOffset[offsetof(NScript::CQ_BountyHuntScript,Hostage2Killed)==85?1:-1];
#endif