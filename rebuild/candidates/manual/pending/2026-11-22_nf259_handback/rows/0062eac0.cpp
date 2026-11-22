// CCombatActionBase::GetIsToFaceTargetCone() const -- the default facing cone of a combat action: 1/9
// Leaf: ?GetIsToFaceTargetCone@CCombatActionBase@@UBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062EAC0, 7 bytes: `fld dword ptr [0x01259D18] ; ret` (thiscall, this unused). 0x01259D18 holds the
//   float 0x3DE38E39 (= 1/9, rounded) and has a second reader (0x00B4F215), so it is a pooled float literal (`__real@3de38e39`), not a
//   named global.
// Identity: Ego_r.pdb 0x00545E10 + FableWin.pdb 0x01D918A0 ?GetIsToFaceTargetCone@CCombatActionBase@@UBEMXZ (public virtual const); retail
//   CCombatActionBase vftable 0x01259E44 slot 78 (+0x138) = 0x0062EAC0, which the landed vtable emitters (0x00955620 and the nf255 / nf257
//   CCreatureAction class rows) bind under this spelling.
// NF259 (2026-11-22, q12730): re-spelled from the free extern "C" stand-in _CCombatActionBase_GetIsToFaceTargetCone (which read the made-up
//   extern g_value_01259d18) onto the shared engine/CCombatActionBase.h (MELEE branch + MELEE_OWN_VIRTUALS, the landed sibling 0x00956730's defines).
// Inferred: the wording of the 1/9 literal (`1.0f / 9.0f`) is unsourced; only its value (0x3DE38E39, retail 0x01259D18) is retail's.
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

float CCombatActionBase::GetIsToFaceTargetCone() const
{
    return 1.0f / 9.0f;
}
