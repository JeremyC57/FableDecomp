// CCreatureAction_ThrowShotObject::IsToFaceTarget() const -- a thrown shot always faces its target: returns true
// Leaf: ?IsToFaceTarget@CCreatureAction_ThrowShotObject@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00956720, 3 bytes: `mov al,1 ; ret` (thiscall, this unused).
// Identity: FableWin.pdb 0x02BF6990 ?IsToFaceTarget@CCreatureAction_ThrowShotObject@@UBE_NXZ (Ego_r.pdb has no record: inlined / folded there);
//   retail vftable 0x012944D4 slot 73 (+0x124) = 0x00956720, the slot of CCombatActionBase::IsToFaceTarget in the Ego_r class-record order
//   (70 CCreatureActionBase slots, then CCombatActionBase's own eleven: GetNextPos 70 .. IsToFaceTarget 73). No landed row references it
//   (only the vtable).
// NF175 (2026-11-20, q11895): re-spelled from the free __fastcall stand-in ?CCreatureAction_ThrowShotObject_IsToFaceTarget@@YI_NXZ onto the shared
//   engine/CCreatureAction_ThrowShotObject.h (_CLASS mode, the define block of the landed 0x00956960 Clone) with the NF175 opt-ins
//   CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS and CCREATUREACTION_THROWSHOTOBJECT_FACETARGET.
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
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_COPYCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_CLASS
#define FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_CLONE
#define FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_INITIALISE
#define FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_FACETARGET
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_ThrowShotObject.h"

bool CCreatureAction_ThrowShotObject::IsToFaceTarget() const
{
    return true;
}
