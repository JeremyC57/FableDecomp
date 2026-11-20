// CModeStandBase::OnActivate() -- base activate, look up the stand/slow-walk/walk/jog/run/sprint animations (stand falls back to DEFAULT, missing gaits to stand/run), refresh the player's movement speeds or measure the walk/jog/run speeds, then restart the animation when the creature is idle
// Leaf: ?OnActivate@CModeStandBase@@UAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Grade: RELOCATION_MATCH -- engine/CCountedPointer.h model plus the ternary GetInterface store (landed 0x007A0490 form), which fixes the zero-register choice around the ST_RUN/ST_SPRINT temporaries
// Retail: Fable.exe (TLC, VC7.1) @ 0x008621A0, 1504 bytes (virtual, `ret`).
// Callees: CTCCreatureModeManager::CModeBase::OnActivate 0x0073AAC0, CModeBase::GetWorld 0x0073A680, CWorld::GetWorldSeed
//   0x0049D810 (world +8 + 0x1613C), own virtual slot 4 GetAnimFilter (+0x10), NCreatureMode::GetRandomAnimationFromCreature 0x00861E60
//   (__fastcall, result in ecx, name in edx) and NCreatureMode::GetMovementSpeedFromAnimation 0x00861950 (__fastcall; Ego_r / FableWin), CCharString 0x0099EBF0 / ~0x0099EAE0,
//   operator new 0x00BFEA1A, CThingCreatureBase::GetAnimFromName 0x00662A00, CAnimEntryLite(const CAnimationEntry&) 0x0073A9B0,
//   CCountedPointer<CAnimEntryLite>::Reset 0x00863090 / operator= 0x00754F10 (out of line), operator delete 0x00BFE9BC,
//   CThingPlayerCreature::InitialiseMovementSpeeds 0x006A9A00, CVectorMap LowerBound 0x0040F020 (component 0x5a),
//   0x0070B460 (component -> animate thing), CAnimateThing::SetAsFinished 0x0070BE80, IsPerformingAnyAction 0x006622A0,
//   own virtual slot 2 AnimUpdate (+8).
// Data: "ST_IDLE_SUBTLE" 0x01276DBC, "ST_SLOW_WALK" 0x0125D14C, "ST_WALK" 0x0125A990, "ST_JOG" 0x0125D144, "ST_RUN"
//   0x0125A998, "ST_SPRINT" 0x0125D138, "DEFAULT" 0x01230554, 0.0f 0x0122DEDC.
// Layout: Ego_r.pdb CModeStandBase (engine/CModeStandBase.h, FABLE_RETAIL_CMODESTANDBASE: the retail class over the shared nested
//   CTCCreatureModeManager::CModeBase): MaxRunSpeed +0x28, MaxJogSpeed +0x2c, MaxWalkSpeed +0x30, counted-pointer animations +0x38..+0x64.
// Naming (NF128, 2026-11-20; q6453 / q6455 / q6454): moved off the local CModeBase / CModeStandBase / CThingCreatureBase / CWorld /
//   CTCAnimate stand-ins onto engine/CModeStandBase.h (FABLE_RETAIL_CMODESTANDBASE + _ACTIVATE), the shared nested
//   CTCCreatureModeManager::CModeBase (VIRTUALS: slot 4 GetAnimFilter, slot 2 AnimUpdate, GetWorld), engine/CThing.h, engine/CThingCreatureBase.h,
//   engine/CThingPlayerCreature.h, engine/CWorld.h (GetWorldSeed, FLAT_GETMESHBANK), engine/CTCAnimationComplex.h and engine/CAnimateThing.h;
//   the two NCreatureMode helpers take their Ego_r / FableWin signatures. RELOCATION_MATCH in one form.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_MOVEMENTSPEEDS
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_ACTIVATE
#define FABLE_RETAIL_CWORLD_FLAT_GETMESHBANK
#define FABLE_RETAIL_CANIMATETHING_SETASFINISHED
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CWorld.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CCountedPointer.h"
#include "engine/NAnimationEntryBodge_CAnimEntryLite.h"
#include "engine/CModeStandBase.h"
#include "engine/CAnimateThing.h"
#include "engine/CTCAnimationComplex.h"

using NAnimationEntryBodge::CAnimEntryLite;
class CAnimFilterBase;
class CMeshDataBank;

namespace NCreatureMode {
CCountedPointer<CAnimEntryLite> __fastcall GetRandomAnimationFromCreature(const CCharString& name, const CThingCreatureBase& creature,
                                                                         const CAnimFilterBase& filter, unsigned long* seed);   // 0x00861E60
float __fastcall GetMovementSpeedFromAnimation(long anim, const CThingCreatureBase& creature, const CMeshDataBank& bank);   // 0x00861950
}
using NCreatureMode::GetRandomAnimationFromCreature;
using NCreatureMode::GetMovementSpeedFromAnimation;

void CModeStandBase::OnActivate()
{
    CTCCreatureModeManager::CModeBase::OnActivate();
    CThingCreatureBase& creature = (CThingCreatureBase&)ParentThing;
    PStandAnimation = GetRandomAnimationFromCreature("ST_IDLE_SUBTLE", creature, GetAnimFilter(), GetWorld().GetWorldSeed());
    PSlowWalkAnimation = GetRandomAnimationFromCreature("ST_SLOW_WALK", creature, GetAnimFilter(), GetWorld().GetWorldSeed());
    PWalkAnimation = GetRandomAnimationFromCreature("ST_WALK", creature, GetAnimFilter(), GetWorld().GetWorldSeed());
    PJogAnimation = GetRandomAnimationFromCreature("ST_JOG", creature, GetAnimFilter(), GetWorld().GetWorldSeed());
    PRunAnimation = GetRandomAnimationFromCreature("ST_RUN", creature, GetAnimFilter(), GetWorld().GetWorldSeed());
    PSprintAnimation = GetRandomAnimationFromCreature("ST_SPRINT", creature, GetAnimFilter(), GetWorld().GetWorldSeed());
    if (!PStandAnimation.Data)
        PStandAnimation.Reset(new CAnimEntryLite(creature.GetAnimFromName("DEFAULT", 0)));
    if (!PWalkAnimation.Data)
        PWalkAnimation = PStandAnimation;
    if (!PRunAnimation.Data)
        PRunAnimation = PStandAnimation;
    if (!PSlowWalkAnimation.Data)
        PSlowWalkAnimation = PStandAnimation;
    if (!PSprintAnimation.Data)
        PSprintAnimation = PRunAnimation;
    if (creature.GetType() == 2) {
        CThingPlayerCreature& player = (CThingPlayerCreature&)ParentThing;
        player.InitialiseMovementSpeeds();
        CTCAnimationComplex* animComplex;
        player.GetTC(TCI_ANIMATION, &animComplex);
        animComplex->GetAnimation().SetAsFinished();
    } else if (creature.GetType() == 1) {
        const CMeshDataBank& bank = *GetWorld().GetMeshBank();
        MaxWalkSpeed = !PWalkAnimation.Data ? 0.0f : GetMovementSpeedFromAnimation(PWalkAnimation.Data->BankIndex, creature, bank);
        MaxJogSpeed = !PJogAnimation.Data ? 0.0f : GetMovementSpeedFromAnimation(PJogAnimation.Data->BankIndex, creature, bank);
        MaxRunSpeed = !PRunAnimation.Data ? 0.0f : GetMovementSpeedFromAnimation(PRunAnimation.Data->BankIndex, creature, bank);
    }
    CTCAnimationComplex* animComplex;
    ParentThing.GetTC(TCI_ANIMATION, &animComplex);
    if (!creature.IsPerformingAnyAction()) {
        ForceAddNewAnims = true;
        AnimUpdate(animComplex->GetAnimation());
        ForceAddNewAnims = false;
    }
}
