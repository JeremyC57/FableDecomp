// NScript::CQ_HobbeCaveScript::CGranny::OnPredicateFail() -- Record the matching entity death and clear its reason code.
// Leaf: ?OnPredicateFail@CGranny@CQ_HobbeCaveScript@NScript@@EAEXXZ
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: own entity allocation/binding00d8fc60 stores012d59e4; selected slot5 is original PDB OnPredicateFail. Actual CScriptThing vtable01238c8c slot18 =004aaaf0 MsgIsKilledBy, const ref-string signature. Original typed fields checked against own writes.
// Layout: real polymorphic CScriptThing and script/entity bases, original parent fields and named long ReasonForFailure. Unknown uncalled virtual names are descriptive. Shared handle headers aliased.
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
typedef unsigned long RetailUID;
#else
typedef unsigned __int64 RetailUID;
#endif
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString():PStringData(0){}CCharString(const char*,long=-1);~CCharString();bool FastCompare(const CCharString&)const;};
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
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript{class CQ_HobbeCaveScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:class CGranny;bool DoneIntro,SavedBeforeCaves,BratInCutscene,MissionSucceeded,MissionFailed,AlarmTriggered,KilledSideRoomHobbes,ReleaseBrat,BratReturned;CTimer BanditComment;long GotTreasure,BanditHitCount;bool KingGruntedOrders;long HobbesToSpawn;bool InCutscene,QuestStartScreened,ReachedHobbeCave,HobbesCloseby,NymphAngry;long ReasonForFailure,WaitForJamesReleased;};class CQ_HobbeCaveScript::CGranny:public CEntityScriptBase{public:CQ_HobbeCaveScript* ParentClass;CQ_SunnyvaleMasterData* PMasterData;bool IntroducedMyself;long TimesTalkedTo;bool WaitingOutside;private:virtual void OnPredicateFail();};void CQ_HobbeCaveScript::CGranny::OnPredicateFail(){if(Me.MsgIsKilledBy("")){ParentClass->ReasonForFailure=0;ParentClass->MissionFailed=true;}}}
#ifdef _MSC_VER
typedef char ExactOwnerSize[sizeof(NScript::CQ_HobbeCaveScript::CGranny)==40?1:-1];typedef char ExactParentSize[sizeof(NScript::CQ_HobbeCaveScript)==120?1:-1];
#endif