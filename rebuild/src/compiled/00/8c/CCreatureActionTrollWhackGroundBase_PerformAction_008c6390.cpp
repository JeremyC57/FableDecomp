// CCreatureAction_TrollWhackGroundBase::PerformAction() -- perform the troll's whack-ground attack: forward to WhackGround()
// Leaf: ?PerformAction@CCreatureAction_TrollWhackGroundBase@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C6390, 5 bytes: `jmp 0x008C61C0` (WhackGround, a tail call with `this` still in ecx).
// Name-fix 2026-11-12 (was the extern "C" free forwarder CCreatureAction_TrollWhackGroundBase_PerformAction + the stand-in
//   ForwardTarget_008c61c0): Ego_r.pdb + FableWin.pdb ?PerformAction@CCreatureAction_TrollWhackGroundBase@@UAEXXZ (public virtual);
//   rebuild/manifest/rtti-vtables.tsv CCreatureAction_TrollWhackGroundBase vtable 0x0128E0EC slot 3 = 0x008C6390 (slot 3 is
//   PerformAction in the sibling vtables, landed 0x008C63C0 / 0x008C6430); the callee 0x008C61C0 is the landed private
//   ?WhackGround@CCreatureAction_TrollWhackGroundBase@@AAEXXZ. Class from engine/CCreatureAction_TrollWhackGroundBase.h
//   (FABLE_RETAIL_CCREATUREACTION_TROLLWHACKGROUNDBASE_CLASS), with the include set of the landed WhackGround row.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_OWN_VIRTUALS
#define FABLE_RETAIL_CCREATUREACTION_TROLLWHACKGROUNDBASE_CLASS
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CDefPointer.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/NCreatureActionEvents.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_TrollWhackGroundBase.h"

void CCreatureAction_TrollWhackGroundBase::PerformAction()
{
    WhackGround();
}
