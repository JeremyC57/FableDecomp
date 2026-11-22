// CCreatureAction_DragonFireBeamInto::`scalar deleting destructor'(unsigned int flags) -- the implicit destructor (a direct call to
//   ~CCombatActionBase 0x0062E900, no own vtable store), then free the object when bit 0 is set; returns this
// Leaf: ??_GCCreatureAction_DragonFireBeamInto@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009539C0, thiscall, `ret 4`; vftable 0x01293E54 slot 0 (RTTI .?AVCCreatureAction_DragonFireBeamInto@@,
//   manifest rtti-vtables.tsv), its only reference.
// Name (agentNF255, 2026-11-22, q12720, with its ctor 0x009535B0): the member the compiler emits for the shared header class (FableWin.pdb
//   0x02BE21E0 ??_ECCreatureAction_DragonFireBeamInto@@UAEPAXI@Z is its vtable alias; ??1 implicit, FableWin 0x02BE2270); was the free
//   __fastcall stand-in CCreatureAction_DragonFireBeamInto_scalar_deleting_destructor (no Leaf line) on a row-local model.
// Fidelity: scaffolding, not retail (nf86 / nf88 pattern, the 0x009568E0 CCreatureAction_ThrowShotObject recipe): the deleting destructor is a
//   compiler-emitted COMDAT that VC7.1 emits with the vtable. The class's constructor is out of line (row 0x009535B0), so the anonymous-
//   namespace helper below expands the implicit copy constructor instead (its vtable store), and this object emits
//   ??_7CCreatureAction_DragonFireBeamInto@@6B@ and ??_G pick-any, as the ctor 0x009535B0 and Clone 0x00953CF0 objects do (the same
//   prelude text, so the COMDAT copies are one). Internal to this TU; never called.
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
#define FABLE_RETAIL_JACKDRAGON_HOVER_PASS27
#define FABLE_RETAIL_CCREATUREACTION_DRAGONFIREBEAMINTO_INITIALISE
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
#include "engine/CCreatureAction_DragonFireBeamInto.h"
#include <new>

namespace {
void ReferenceDragonFireBeamIntoVtable(void* where, const CCreatureAction_DragonFireBeamInto& other)
{
    new (where) CCreatureAction_DragonFireBeamInto(other);
}
}
