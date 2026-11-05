// CGameScriptInterface::GetAllowedToFollow(CScriptThing thing) -- script query: true when the script thing resolves no thing, else the thing's CTCAIScratchpad::AllowedToFollow flag (read through CThing::GetTC<CTCAIScratchpad>); the by-value CScriptThing is destroyed inline
// Leaf: ?GetAllowedToFollow@CGameScriptInterface@@UAE_NVCScriptThing@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008A6390, 240 bytes, thiscall, `ret 0Ch` (the 12-byte CScriptThing by value).
// Identity (proven): CGameScriptInterface vtable 0x01260F0C slot 833; Ego_r.pdb ?GetAllowedToFollow@CGameScriptInterface@@UAE_NVCScriptThing@@@Z
//   (donor 0x008E4920) and the manifest label agree; it reads the bool at +0x6C of interface 0xAA = Ego_r.pdb
//   CTCAIScratchpad::AllowedToFollow (no STL member before +0x7C, so the PDB offset is retail's).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound 0x0040F020, CBaseClass::~CBaseClass() 0x0099A2E0, operator
//   delete 0x00BFE9BC; the pointee's virtual GetThing (slot 11, +0x2C).
// Shape (family K032, representative 0x008A60D0): the landed J018 getter CGameScriptInterface::GetDisableHeadLooking 0x008A6BF0
//   with `return true` for an unresolved thing (`mov al,1`) and a direct member read (no CTCAIScratchpad getter of this
//   flag is in the PDB name sets). Header debt: engine/CScriptThing.h / CGameScriptInterface.h model the vptr as data.
#include <stddef.h>
#define FABLE_RETAIL_STLP_BITSET_INDEX
#include "engine/CVectorMap.h"

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CTCBase;

#include "engine/C3DVector.h"
class CCharStringData;

enum EScriptingStateGroups { EScriptingStateGroups_Force32 = 0x7fffffff };
class CTCAIScratchpad {                                      // Ego_r.pdb members (= retail offsets up to +0x7C)
public:
    void*                 __vfptr_and_CTCBase[3];             // +0x00 CTCBase (vptr, ParentThing, flags)
    unsigned long         ThankingPhrase;                     // +0x0C
    unsigned long         IgnoringPhrase;                     // +0x10
    C3DVector             WanderCentrePoint;                  // +0x14
    float                 WanderMinDistance;                  // +0x20
    float                 WanderMaxDistance;                  // +0x24
    long                  GossipCounter;                      // +0x28
    long                  MaxGossipPhrase;                    // +0x2C
    unsigned long         WarningPhrase;                      // +0x30
    unsigned long         BeerRequestPhrase;                  // +0x34
    EScriptingStateGroups ScriptingStateGroup;                // +0x38
    float                 MaxHeroReactionDistance;            // +0x3C
    long                  ActionFrequency;                    // +0x40
    float                 ActionFrequencyVariation;           // +0x44
    CCharString           Action;                             // +0x48
    bool                  FaceHeroForAction;                  // +0x4C
    CCharString           TargetName;                         // +0x50
    float                 FollowDistance;                     // +0x54
    bool                  AttackHeroOnSight;                  // +0x58
    long                  TimeToSpendHarassingHero;           // +0x5C
    float                 CombatNearbyEnemyFleeingBreakOffRange;   // +0x60
    float                 CombatNearbyBreakOffRange;          // +0x64
    bool                  StealStealableItems;                // +0x68
    bool                  RecoverStealableItems;              // +0x69
    bool                  TakeStealableItemToRandomDestination;    // +0x6A
    bool                  KillSelfAndStealableItemAfterReachingDestination;   // +0x6B
    bool                  AllowedToFollow;                    // +0x6C
    CCharString           TableName;                          // +0x70
    CCharString           SeatName;                           // +0x74
    bool                  DisableHeadLooking;                 // +0x78
    bool                  IsPushableByHero;                   // +0x79
    bool                  LookForFiniteTime;                  // +0x7A
    bool                  AvoidRegionExits;                   // +0x7B
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
inline CThing* CScriptThing::GetPThing() const { return PImp.Data == 0 ? 0 : PImp.Data->GetPThing(); }

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

bool CGameScriptInterface::GetAllowedToFollow(CScriptThing thing)
{
    if (!thing.GetPThing())
        return true;
    CTCAIScratchpad* tc;
    thing.GetPThing()->GetTC(TCI_AI_SCRATCHPAD, &tc);
    return tc->AllowedToFollow;
}