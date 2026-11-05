// NScript::CQ_BreakSiegeScript::OnPersist(CPersistContext& context) -- Transfer persistent script state with zero defaults.
// Leaf: ?OnPersist@CQ_BreakSiegeScript@NScript@@UAEXAAVCPersistContext@@@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: Original PDB OnPersist and unique own persist literals; own vtable0x12cab18 slot4, constructor stores at0xd1cc28,0xd23ca7. All12 own strings and typed original owner fields mapped in order. Transfer<bool>004045c0, Transfer<long>00410be0.
// Layout: real typed CScriptBase64/CLevelScriptBase72 with original fields and STLport containers. Shared handle headers aliased for real polymorphic hierarchy. CPersistContext API-only projection, no storage used.
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
namespace NScript{class CQ_BreakSiegeScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:bool MissionSucceeded;bool MissionFailed;bool BanditDemandsKnown;bool ChiefRefusesDemands;bool BanditLeaderFree;bool BanditSiegeStart;bool HasFirstWaveAttacked;bool StartSecondWave;bool BanditsAttackHero;bool HeroLetInTown;bool GuardFollowingHero;bool EndCutsceneStarts;bool EndCutsceneDone;bool AttackedBanditLeader;bool QuestStartScreened;long ReasonForFailure;virtual void OnPersist(CPersistContext&);};void CQ_BreakSiegeScript::OnPersist(CPersistContext& context){context.Transfer("BanditDemandsKnown",BanditDemandsKnown,false);context.Transfer("ChiefRefusesDemands",ChiefRefusesDemands,false);context.Transfer("BanditLeaderFree",BanditLeaderFree,false);context.Transfer("BanditSiegeStart",BanditSiegeStart,false);context.Transfer("BanditsAttackHero",BanditsAttackHero,false);context.Transfer("HasFirstWaveAttacked",HasFirstWaveAttacked,false);context.Transfer("StartSecondWave",StartSecondWave,false);context.Transfer("HeroLetInTown",HeroLetInTown,false);context.Transfer("GuardFollowingHero",GuardFollowingHero,false);context.Transfer("EndCutsceneStarts",EndCutsceneStarts,false);context.Transfer("EndCutsceneDone",EndCutsceneDone,false);context.Transfer("QuestStartScreened",QuestStartScreened,false);}}