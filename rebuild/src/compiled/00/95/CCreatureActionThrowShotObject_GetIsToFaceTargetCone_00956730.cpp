// CCreatureAction_ThrowShotObject::GetIsToFaceTargetCone() const -- the facing cone of a thrown shot: 1.0
// Leaf: ?GetIsToFaceTargetCone@CCreatureAction_ThrowShotObject@@UBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00956730, 7 bytes: `fld dword ptr [0x0122DED8] ; ret` (the float constant 1.0f; thiscall, this unused).
// Identity: FableWin.pdb 0x02BF69A0 ?GetIsToFaceTargetCone@CCreatureAction_ThrowShotObject@@UBEMXZ (Ego_r.pdb has no record); retail vftable
//   0x012944D4 slot 78 (+0x138) = 0x00956730, CCombatActionBase::GetIsToFaceTargetCone's slot in the Ego_r class-record order. No landed row
//   references it (only the vtable).
// NF175 (2026-11-20, q11896): re-spelled from the free __fastcall stand-in ?CCreatureAction_ThrowShotObject__GetIsToFaceTargetCone@@YIMPAX@Z
//   (which read an extern g_FloatOne for the constant) onto the shared engine/CCreatureAction_ThrowShotObject.h (_CLASS mode, 0x00956960's define
//   block) with the NF175 opt-ins CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS and CCREATUREACTION_THROWSHOTOBJECT_FACETARGET; the constant is the
//   literal 1.0f (retail 0x0122DED8 holds 1.0f).
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

float CCreatureAction_ThrowShotObject::GetIsToFaceTargetCone() const
{
    return 1.0f;
}
