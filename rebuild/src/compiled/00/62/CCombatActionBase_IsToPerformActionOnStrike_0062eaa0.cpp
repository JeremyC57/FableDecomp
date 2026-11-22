// CCombatActionBase::IsToPerformActionOnStrike() const -- the base combat action answers false
// Leaf: ?IsToPerformActionOnStrike@CCombatActionBase@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062EAA0, 3 bytes: `xor al,al ; ret` (thiscall, this unused).
// Identity: FableWin.pdb 0x01D91880 ?IsToPerformActionOnStrike@CCombatActionBase@@UBE_NXZ (public virtual const; the header-inline CCombatActionBase body, which Ego_r / Ego_d do not record) +
//   retail CCombatActionBase vftable 0x01259E44 slot 76 (+0x130) = 0x0062EAA0, which the landed vtable emitters (0x00955620 and the CCreatureAction
//   class rows) bind under this spelling. One PDB + the retail slot (nf228 ruling: the second source is the retail vftable / body).
// NF261 (2026-11-22, q12740): re-spelled from the free stand-in `bool __fastcall CCombatActionBase_IsToPerformActionOnStrike()` (no Leaf line) onto the shared
//   engine/CCombatActionBase.h (MELEE branch + MELEE_OWN_VIRTUALS, the define block of the landed slot rows 0x0062EAC0 / 0x0062EAD0).
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

bool CCombatActionBase::IsToPerformActionOnStrike() const
{
    return false;
}
