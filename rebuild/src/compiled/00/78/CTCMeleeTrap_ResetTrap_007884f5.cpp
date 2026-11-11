// CTCMeleeTrap::ResetTrap() -- stop the continuous animation flag, fetch the owner's appearance def and, when it has an animation named by the global "TRAP_RESET" string (any group), start that animation without looping
// Leaf: ?ResetTrap@CTCMeleeTrap@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007884F5, 96 bytes, size-optimised TU, plain `ret`. Ego_r.pdb / FableWin.pdb
//   ?ResetTrap@CTCMeleeTrap@@MAEXXZ (protected virtual; vtable 0x0126B884 slot 31).
// Callees (PDB spellings): CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2 (on the thing's base def read in place),
//   CAnimationSet::AnimExists 0x005DC2E0, CTCMeleeTrap::InitialiseAnimation(const CCharString&, bool) 0x0078809B, the def's virtual
//   release (slot 1).
// Globals: the anonymous-namespace CCharString STrapReset @ 0x013BADFC ("TRAP_RESET"; dynamic initializer 0x0121F057).
// Filter temporary: CAnimFilterNoGroup (vtable 0x0122FFDC; base CAnimFilterBase 0x0122FFCC).
// Layout: engine/CTCMeleeTrap.h (PlayingContinuous +0x60); CThing PBaseDef +0x70; CAppearanceDef Animation +0x34.
// Name fix (agentNF12 2026-11-11): moved off the local CAppearanceDef / CDefPointer / CDefClassBase / CThing / CTCMeleeTrap stand-ins
// (and a same-TU stand-in GetSubDef body) onto the shared headers (new engine/CTCMeleeTrap.h, CTCTrapBase.h retail opt-in).
// 0x0042B0A2 binds GetSubDef<CAppearanceDef> (was ?GetSubDef@CDefClassBase@@QAE_NAAUCAppearanceDefPointer@@@Z), 0x005DC2E0
// CAnimationSet::AnimExists (was FindAnimationByName); the row's own symbol is the PDB protected virtual MAEXXZ (was QAEXXZ).
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
#include "engine/CTCMeleeTrap.h"

namespace { extern CCharString STrapReset; }                  // 0x013BADFC ("TRAP_RESET")

void CTCMeleeTrap::ResetTrap()
{
    CDefPointer<const CAppearanceDef> appearance;
    PlayingContinuous = false;
    ParentThing->PeekBaseDef().GetSubDef(&appearance);
    if (appearance->Animation.AnimExists(STrapReset, CAnimFilterNoGroup()))
        InitialiseAnimation(STrapReset, false);
}
