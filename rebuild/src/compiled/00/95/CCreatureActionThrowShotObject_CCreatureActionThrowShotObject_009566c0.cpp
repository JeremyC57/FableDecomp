// CCreatureAction_ThrowShotObject::CCreatureAction_ThrowShotObject(CThingCreatureBase&, CThing&) -- base CCombatActionBase(creature, thing, 150, GetSoundPair(creature, SWORD_SWING))
// Leaf: ??0CCreatureAction_ThrowShotObject@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009566C0, oracle length 87 bytes.
// Retail bytes:  sub esp,0Ch ; push esi ; push edi ; push -1 ; mov esi,ecx ; push offset "SWORD_SWING" ; lea ecx,[esp+10h] ; call CCharString::CCharString ; mov edi,[owner] ; lea eax,[esp+8] ; push eax ; mov edx,edi ; l
// Two-argument combat-action constructor whose sound pair comes from the protected static __fastcall
// CCombatActionBase::GetSoundPair @ 0x008561A0: the name literal converts implicitly to a CCharString temporary, the 8-byte CSoundPair
// result lands in a frame temporary through the hidden pointer and is handed by reference to the base constructor @ 0x00857F30 with
// the action code 150 (0x96); the string temporary dies after the base constructor, then this class's vtable (0x012944D4) is stored.
// NF145 (q11780 / q11785): Ego_r.pdb 0x0097B320 / FableWin.pdb 0x02BF6850 ??0CCreatureAction_ThrowShotObject@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z
//   (references; was the pointer form ...@PAVCThingCreatureBase@@PAVCThing@@@Z, which no landed row binds). The body is the header's inline
//   constructor (engine/CCreatureAction_ThrowShotObject.h, opt-in FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_CLASS, which retail expands
//   inside 0x009551A0); the row's local CCharString / CSoundPair / CCombatActionBase / CCreatureAction_ThrowShotObject models (with a
//   placeholder virtual v00) are gone, so this object's ??_7CCreatureAction_ThrowShotObject@@6B@ COMDAT is the header class's, as in
//   0x009551A0.
//   NF148 (2026-11-20, q11785, one vtable): also defines FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_CLONE / _INITIALISE (slots 11 / 12 =
//   0x00956960 / 0x008B66D0), so this vtable COMDAT is identical to the 0x009551A0 and 0x00956960 copies.
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
// Fidelity: scaffolding, not retail (nf86 / nf88 pattern, as 0x004371A0): a placement construction through the header's inline
// constructor with inline expansion switched off for this one function, so this object emits the body out of line as a pick-any COMDAT
// (retail's 0x009566C0). Internal to this TU; never called.
#pragma inline_depth(0)
void ReferenceThrowShotObjectCtor(void* where, CThingCreatureBase& creature, CThing& thing)
{
    new (where) CCreatureAction_ThrowShotObject(creature, thing);
}
#pragma inline_depth()
}
