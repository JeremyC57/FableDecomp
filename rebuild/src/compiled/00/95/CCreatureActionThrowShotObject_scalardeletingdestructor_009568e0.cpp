// CCreatureAction_ThrowShotObject::`scalar deleting destructor'(unsigned int flags) -- the implicit destructor (a direct call to
// ~CCombatActionBase 0x0062E900, no own vtable store), then free the object when bit 0 is set
// Leaf: ??_GCCreatureAction_ThrowShotObject@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009568E0, 30 bytes, thiscall, `ret 4`; vftable 0x012944D4 slot 0 (rtti-vtables.tsv).
// Name (NF181, 2026-11-20, q11897): FableWin.pdb 0x02BF69B0 ??_GCCreatureAction_ThrowShotObject@@UAEPAXI@Z + the retail vftable
//   slot 0; was the free __fastcall stand-in CCreatureAction_ThrowShotObject_scalar_deleting_destructor on a row-local model.
// Fidelity: scaffolding, not retail (nf86 / nf88 pattern): the deleting destructor is a compiler-emitted COMDAT that VC7.1 emits with
//   the vtable, i.e. wherever the header's inline constructor (engine/CCreatureAction_ThrowShotObject.h, _CLASS opt-in) is expanded.
//   The anonymous-namespace helper below expands it (placement new), so this object emits ??_7CCreatureAction_ThrowShotObject@@6B@ and
//   ??_G pick-any, as the landed 0x009551A0 / 0x009566C0 / 0x00956960 objects already do (same macro set, so the COMDAT copies are
//   identical). Internal to this TU; never called.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_CLASS
#define FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_CLONE
#define FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_INITIALISE
#include <new>
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_ThrowShotObject.h"

namespace {
void ReferenceThrowShotObjectVtable(void* where, CThingCreatureBase& creature, CThing& thing)
{
    new (where) CCreatureAction_ThrowShotObject(creature, thing);
}
}
