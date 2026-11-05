// NScript::CQ_TraderConflictGoodScript::CQ_TraderConflictGoodScript(CScriptDataBase* data,CGameScriptInterfaceBase& game) -- Initialize script ownership and its empty vector.
// Leaf: ??0CQ_TraderConflictGoodScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: own script-info registration00cd7750 literalQ_TraderConflictGood -> factory -> constructor00dfa0b0/vtable012deac8; exact original PDB signature.
// Layout: real typed CScriptBase64/CLevelScriptBase72, original named fields and STLport containers; PC allocation104. Handle headers included under aliases to restore original inheritance. Only vector default initialization emits stores; scalar progress state is left for Init.
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
class CGameScriptInterfaceBase;class CScriptDataBase;
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
namespace NScript{class CQ_TraderConflictGoodScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:std::vector<CScriptThing> AllCreatures;long TradersFollowing,TradersReachedTeleporter,BanditSecurityLinesSaid;bool FirstRegionDead,EnteredNewRegion,IntroDone,OutroStart,OutroDone,OpenedCage,CommentedOnBanditCostume,MissionSucceeded;CQ_TraderConflictGoodScript(CScriptDataBase* data,CGameScriptInterfaceBase& game);virtual void RegisterMain();virtual void Main();virtual void Init();virtual void OnPersist(CPersistContext&);static CScriptBase* __fastcall Alloc(CScriptDataBase*,CGameScriptInterfaceBase&);};
CQ_TraderConflictGoodScript::CQ_TraderConflictGoodScript(CScriptDataBase* data,CGameScriptInterfaceBase& game):CLevelScriptBase<CQ_SunnyvaleMasterData>(data,game){}
}
#ifdef _MSC_VER
typedef char ExactBaseSize[sizeof(CScriptBase)==64?1:-1];typedef char ExactOwnerSize[sizeof(NScript::CQ_TraderConflictGoodScript)==104?1:-1];
#endif