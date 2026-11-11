// CTCMeleeTrap::TriggerTrap() -- play the trap's "TRAP_TRIGGER" animation and hand the trigger / reset animation durations (the reset one only when the appearance has a "TRAP_RESET" animation, else 0) to the base trap's transition timing
// Leaf: ?TriggerTrap@CTCMeleeTrap@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00788406, 239 bytes, size-optimised TU, plain `ret`. Ego_r.pdb / FableWin.pdb
//   ?TriggerTrap@CTCMeleeTrap@@MAEXXZ (protected virtual; vtable 0x0126B884 slot 30).
// Callees (PDB spellings): CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2 (on the thing's base def read in place),
//   CTCMeleeTrap::InitialiseAnimation(const CCharString&, bool) 0x0078809B, CAnimationSet::GetAnim 0x005DC340 / AnimExists 0x005DC2E0,
//   CTCBase::GetWorld 0x00686A70, CMeshDataBank::PeekAnimation 0x00A242C0 (through CWorld::GetMeshBank, PMainMeshBank +0x3C),
//   CTCTrapBase::SetTransitionParams(float, float) 0x00787C35, the def's virtual release (slot 1).
// Globals: the anonymous-namespace CCharStrings STrapTrigger @ 0x013BADF8 ("TRAP_TRIGGER") and STrapReset @ 0x013BADFC ("TRAP_RESET").
// Filter temporaries: CAnimFilterNoGroup (vtable 0x0122FFDC; base CAnimFilterBase 0x0122FFCC).
// Layout: CThing PBaseDef +0x70; CAppearanceDef Animation +0x34; C3DAnimationInfo Duration +0x10.
// Name fix (agentNF12 2026-11-11): moved off the local CDefPointer / CDefClassBase / CThing / CWorld / CMeshDataBank / C3DAnimationInfo /
// CTCMeleeTrap stand-ins (and a same-TU stand-in GetSubDef body) onto the shared headers (new engine/CTCMeleeTrap.h, CTCTrapBase.h
// retail opt-in). 0x0042B0A2 binds GetSubDef<CAppearanceDef> (was ?GetSubDef@CDefClassBase@@QBE_NPAV?$CDefPointer@UCMeleeTrapDef@@@@@Z),
// 0x005DC340 GetAnim (was CAnimationTable::GetAnimation), 0x00686A70 ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ (was QAE), 0x00A242C0
// CMeshDataBank::PeekAnimation (was GetAnimation), 0x00787C35 CTCTrapBase::SetTransitionParams (was CTCMeleeTrap::SetStrikeTimes);
// the row's own symbol is the PDB protected virtual MAEXXZ (was QAEXXZ).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCTRAPBASE
#define FABLE_RETAIL_CANIMATIONSET_ANIMEXISTS
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESH_BANK
#define FABLE_RETAIL_CMESHDATABANK_PEEKANIMATION
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefClassBase.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefPointer.h"
#include "engine/CAppearanceDef.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterNoGroup.h"
#include "engine/CDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DAnimationInfo.h"
#include "engine/CTCMeleeTrap.h"

namespace {
extern CCharString STrapTrigger;                               // 0x013BADF8 ("TRAP_TRIGGER")
extern CCharString STrapReset;                                 // 0x013BADFC ("TRAP_RESET")
}

void CTCMeleeTrap::TriggerTrap()
{
    CDefPointer<const CAppearanceDef> appearance;
    ParentThing->PeekBaseDef().GetSubDef(&appearance);
    InitialiseAnimation(STrapTrigger, false);
    const CAnimationEntry& trigger = appearance->Animation.GetAnim(STrapTrigger, 0, CAnimFilterNoGroup());
    float triggerTime = GetWorld().GetMeshBank()->PeekAnimation(trigger.BankIndex)->Duration;
    float resetTime = 0.0f;
    if (appearance->Animation.AnimExists(STrapReset, CAnimFilterNoGroup())) {
        const CAnimationEntry& reset = appearance->Animation.GetAnim(STrapReset, 0, CAnimFilterNoGroup());
        resetTime = GetWorld().GetMeshBank()->PeekAnimation(reset.BankIndex)->Duration;
    }
    SetTransitionParams(triggerTime, resetTime);
}
