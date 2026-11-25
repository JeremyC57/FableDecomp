// CCreatureAction_CastWillLightningSpell::Clone() const -- `return new CCreatureAction_CastWillLightningSpell(*this);` with the implicit copy constructor inlined (CCombatActionBase copy constructor, vftable store, own members)
// Leaf: ?Clone@CCreatureAction_CastWillLightningSpell@@UBEPAVCCreatureActionBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BE910, 71 bytes: operator new(0x11C) 0x00BFEA1A, null test, CCombatActionBase(const CCombatActionBase&) 0x0062F3B0, C's vftable store, then 2 member copies;
//   a public const virtual __thiscall returning CCreatureActionBase* (0 when the allocation fails).
// Identity (NF425, 2026-11-25): self ?Clone@CCreatureAction_CastWillLightningSpell@@UBEPAVCCreatureActionBase@@XZ: RTTI placement +
//   Ego_r/FableWin; base copy constructor ??0CCombatActionBase@@QAE@ABV0@@Z 0x0062F3B0 (Ego_r/FableWin; MELEE_COPYCTOR / COPYCTOR
//   opt-in).
// Respell (NF425; NF427 header move): was the stand-in self ?Clone@CCreatureAction_CastWillLightningSpell@@QAEPAUCBase_0062f3b0@@XZ (non-virtual,
//   non-const, over a made-up base model); now the real virtual member, so the RTTI-placed name is defined.
// Header: engine/CCreatureAction_CastWillLightningSpell.h, opt-in FABLE_RETAIL_CCREATUREACTION_CASTWILLLIGHTNINGSPELL_VIRTUAL_MODEL
//   (NF427, 2026-11-25; generator NF322 genmodel.py as run by NF329, NF325 style: the polymorphic retail class, declarations only,
//   retail size; it declares every slot whose RTTI-port label names CCreatureAction_CastWillLightningSpell, so the emitted
//   ??_7CCreatureAction_CastWillLightningSpell@@6B@ names all 81 of retail's slots: 0 DIFF, 0 WRONG); needs FABLE_RETAIL_BASE_OBJECT,
//   FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_KRAKEN_ROAR; prelude: the row's NF425 block plus those
//   opt-ins.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_MAZE_DEFENSIVE_MELEE_PASS27
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_COPYCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_ONANIMATIONLOADED
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_CCREATUREACTION_CASTWILLLIGHTNINGSPELL_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCreatureAction_CastWillLightningSpell.h"

CCreatureActionBase* CCreatureAction_CastWillLightningSpell::Clone() const
{
    return new CCreatureAction_CastWillLightningSpell(*this);
}
