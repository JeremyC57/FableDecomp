// CCreatureAction_GetUpFromSitting::CCreatureAction_GetUpFromSitting(CThingCreatureBase& creature, const CCharString& animation, long sittingMode,
//   bool removeOnPerformAction) -- base CCombatActionBase(creature, 50, CSoundPair()), SittingMode / RemoveOnPerformAction, own vtable, then
//   the animation handed to CCombatActionBase::SetCombatAnimation
// Leaf: ??0CCreatureAction_GetUpFromSitting@@QAE@AAVCThingCreatureBase@@ABVCCharString@@J_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00860150, 82 bytes, thiscall `ret 10h`: CCombatActionBase ctor 0x00858030, vtable 0x01276C2C,
//   SetCombatAnimation 0x008574D0. A COMDAT of retail's NCreatureMode::GetUpFromSitting TU (0x0085FF10, which calls it on a temporary);
//   also called by CGameScriptInterface::EntitySetAsSittingOnFloor 0x008A8C60.
// Names: Ego_r.pdb / FableWin.pdb ??0CCreatureAction_GetUpFromSitting@@QAE@AAVCThingCreatureBase@@ABVCCharString@@J_N@Z (creature by reference;
//   engine/CCreatureAction_GetUpFromSitting.h); both landed callers bind this spelling.
// NF86 (round 7, q11102 / q11120): was the pointer-parameter stand-in leaf ??0..@@QAE@PAVCThingCreatureBase@@ABVCCharString@@J_N@Z over local
//   CSoundPair / CCombatActionBase / CCreatureAction_GetUpFromSitting models (a strong definition). Now the ONE shared inline body from the
//   header (NF86 opt-in FABLE_RETAIL_CCREATUREACTION_GETUPFROMSITTING_CTOR_INLINE, the body NF84 wrote in 0x0085FF10), so this object and
//   0x0085FF10's both carry it as a pick-any COMDAT (selection 2; comdat.py) and no object defines a nodup copy.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_SETCOMBATANIMATION
#define FABLE_RETAIL_CCREATUREACTION_GETUPFROMSITTING_CTOR_INLINE
#define FABLE_RETAIL_CCREATUREACTION_GETUPFROMSITTING_RTTI_DECLS   // NF327 (2026-11-24, NF323 Q3): Clone in its PDB access (UBE), one unit with 0x00860290
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_GetUpFromSitting.h"

namespace {
// Fidelity: scaffolding, not retail: the smallest out-of-line use of the header's inline constructor (a temporary passed by reference, the form
// retail's GetUpFromSitting 0x0085FF10 calls out of line), so this object emits the constructor as a pick-any COMDAT. Internal to this TU.
bool ReferenceGetUpFromSittingCtor(CThingCreatureBase& creature, const CCharString& animation)
{
    return creature.IsFreeToPerformAction(CCreatureAction_GetUpFromSitting(creature, animation, 0, false));
}
}
