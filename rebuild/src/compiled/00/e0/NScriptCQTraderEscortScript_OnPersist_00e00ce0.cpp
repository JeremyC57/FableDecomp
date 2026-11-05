// NScript::CQ_TraderEscortScript::OnPersist(CPersistContext& context) -- Transfer persistent state with zero defaults.
// Leaf: ?OnPersist@CQ_TraderEscortScript@NScript@@UAEXAAVCPersistContext@@@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: original PDB member signatures and unique persist field names; own vtable012df16c slot4, constructed at00e006a8/00e0af86. Field strings and typed helper callsites independently identify this owner; donor addresses not used.
// Layout: original field types after typed72-byte level-script base or20-byte entity base; PC owner size124. Shared handle headers aliased for true original bases. CPersistContext is an API-only projection, no storage used.
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
typedef unsigned long RetailUID;
#else
typedef unsigned __int64 RetailUID;
#endif
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CCharStringData;class CCharString{public:CCharStringData* PStringData;bool FastCompare(const CCharString&)const;};
class CCPPointerInfo;
#include "engine/CCountedPointer.h"

class CEntityScriptBindingBase;class CActiveEntityScriptBase;class CSpawnedFuncBase;
#include "engine/CArray.h"
#include "engine/CVectorMap.h"
template<class V>class CCompareString{public:bool operator()(const std::pair<CCharString,V>& a,const std::pair<CCharString,V>& b)const{return a.first.FastCompare(b.first);}};

class CPersistContext;class CBaseObjectPointer;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
namespace NScript{class CRuntimeScriptedEntityPersist{public:class CPersistInfo;CVectorMap<RetailUID,CPersistInfo> PersistScripts;};class CQ_SunnyvaleMasterData;}
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CGameScriptInterfaceBase;
class CPersistContext{public:template<class T>void Transfer(const char*,T&,const T&);};class CTimer{public:long TimerIndex;};
namespace NScript{class CQ_TraderEscortScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:CTimer CommentTimer;long BalverinesToSurpriseHeroNeeded,TradersStillAliveCounter;bool InfectedTraderCanGetUp,TradersShouldBeScared,IntroFinished,FollowInfoGiven,EndStarted,SavedInMiddle,SavedNearEnd;long IncubationTimeLow,IncubationTimeMedium,IncubationTimeHigh,TraderPickpocketedCount;bool MissionSucceeded,MissionFailed,QuestStartScreened,GoneWrongWay,ShownBalverine,ShownEarthTroll;long TimesBantered;CCharString TraderToTalk;virtual void OnPersist(CPersistContext&);};void CQ_TraderEscortScript::OnPersist(CPersistContext& context){context.Transfer("IntroFinished",IntroFinished,false);context.Transfer("SavedInMiddle",SavedInMiddle,false);context.Transfer("SavedNearEnd",SavedNearEnd,false);context.Transfer("EndStarted",EndStarted,false);context.Transfer("QuestStartScreened",QuestStartScreened,false);context.Transfer("ShownBalverine",ShownBalverine,false);context.Transfer("ShownEarthTroll",ShownEarthTroll,false);}}
#ifdef _MSC_VER
typedef char ExactOwnerSize[sizeof(NScript::CQ_TraderEscortScript)==124?1:-1];
#endif