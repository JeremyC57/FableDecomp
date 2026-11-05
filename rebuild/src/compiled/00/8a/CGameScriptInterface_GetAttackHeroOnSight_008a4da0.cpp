// CGameScriptInterface::GetAttackHeroOnSight(CScriptThing thing) -- script query: false when the script thing resolves no thing, else the thing's CTCAIScratchpad::AttackHeroOnSight flag (read through CThing::GetTC<CTCAIScratchpad>); the by-value CScriptThing is destroyed inline
// Leaf: ?GetAttackHeroOnSight@CGameScriptInterface@@UAE_NVCScriptThing@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A4DA0, 240 bytes, thiscall, `ret 0Ch` (the 12-byte CScriptThing by value).
// Identity (proven): CGameScriptInterface::GetAttackHeroOnSight(CScriptThing); evidence: CGameScriptInterface vtable slot 809 (ghidra_out/cgsi_slots_aligned.tsv) holds this body as ?GetAttackHeroOnSight@CGameScriptInterface@@UAE_NVCScriptThing@@@Z, which FableWin.pdb and Ego_r.pdb hold; it reads the bool at +0x58 of interface 0xAA, Ego_r.pdb CTCAIScratchpad::AttackHeroOnSight (the PDB getter ?GetAttackHeroOnSight@CTCAIScratchpad@@QBE_NXZ; the landed GetActionFrequency 0x008A3D20 reads ActionFrequency +0x40 of the same 0xAA)
// Label: the manifest's mangled "?GetAttackHeroOnSight@CGameScriptInterface@@UAE_NVCScriptThing@@@Z" is this row's own CGSI slot name (family J018: six
//   CTCAIScratchpad flag getters differing only in the field offset; retail has no identical-code folding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> >::LowerBound(const ETCInterfaceType&) 0x0040F020
//   (Ego_r.pdb names it; the catalog's bindings there are stand-ins), CBaseClass::~CBaseClass() 0x0099A2E0 (the CScriptThing
//   destructor's base call), operator delete 0x00BFE9BC; the pointee's virtual GetThing (slot 11, +0x2C).
// Shape: `if (!thing.GetPThing()) return false; CTCAIScratchpad* tc; thing.GetPThing()->GetTC(&tc); return tc->GetAttackHeroOnSight();`
//   -- CScriptThing::GetThing forwards to the counted pimpl; CThing::GetTC<T>(T**) (Ego_r.pdb
//   ??$GetTC@VCTCAIScratchpad@@@CThing@@QAE_NPAPAVCTCAIScratchpad@@@Z) tests UsingComponents through the STLport bitset
//   reference proxy (`test ch, 4`) and looks the type up through a map pointer (LowerBound on a key copy, the end / greater
//   check against the constant); its result is ignored, so a missing component reads the unset local (retail's stale load).
// Layout: retail CThing UsingComponents (bitset<274>) +0x20, Components (CVectorMap, 0x10) +0x44; CScriptThing = vptr
//   (vtable 0x01238C8C, RTTI .?AVCScriptThing@@) + CCountedPointer<CScriptThing> PImp; CTCAIScratchpad has no STL member
//   before +0x7C, so its Ego_r.pdb offsets are retail's (AttackHeroOnSight +0x58). The engine CScriptThing / CGameScriptInterface
//   headers model the vptr as a data member, so both are local here (no CThing / CTCAIScratchpad header exists).
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
    bool GetAttackHeroOnSight() const { return AttackHeroOnSight; }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
inline CThing* CScriptThing::GetPThing() const { return PImp.Data == 0 ? 0 : PImp.Data->GetPThing(); }

class CGameScriptInterface {
public:
    virtual ~CGameScriptInterface();
    virtual bool GetAttackHeroOnSight(CScriptThing thing);
};

bool CGameScriptInterface::GetAttackHeroOnSight(CScriptThing thing)
{
    if (!thing.GetPThing())
        return false;
    CTCAIScratchpad* tc;
    thing.GetPThing()->GetTC(TCI_AI_SCRATCHPAD, &tc);
    return tc->GetAttackHeroOnSight();
}