// CCombatActionBase::GetIsToFaceTargetMinDist() const -- the default minimum facing distance of a combat action: 0.5
// Leaf: ?GetIsToFaceTargetMinDist@CCombatActionBase@@UBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062EAD0, 7 bytes: `fld dword ptr [0x0122F59C] ; ret` (thiscall, this unused). 0x0122F59C holds 0.5f
//   and is read by many functions (the pooled literal `__real@3f000000`).
// Identity: Ego_r.pdb 0x00545E20 + FableWin.pdb 0x01D918C0 ?GetIsToFaceTargetMinDist@CCombatActionBase@@UBEMXZ (public virtual const); retail
//   CCombatActionBase vftable 0x01259E44 slot 79 (+0x13C) = 0x0062EAD0, bound under this spelling by the landed vtable emitters.
// NF259 (2026-11-22, q12730): re-spelled from the free __fastcall stand-in
//   ?CCombatActionBase_GetIsToFaceTargetMinDist@@YIMPAUCAIStateGroup_EnemyCreatureBase@@@Z (which read the made-up extern
//   g_CombatMinDistance_0122f59c) onto the shared engine/CCombatActionBase.h (MELEE branch + MELEE_OWN_VIRTUALS, as 0x00956730).
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
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"

float CCombatActionBase::GetIsToFaceTargetMinDist() const
{
    return 0.5f;
}
