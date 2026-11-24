// CCombatActionBase::GetActionMovementVector() const -- the base combat action moves along +Y: C3DVector(0, 1, 0)
// Leaf: ?GetActionMovementVector@CCombatActionBase@@UBE?AVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062EA80, 16 bytes: `mov eax,[esp+4] ; mov [eax],0 ; mov [eax+4],3F800000h ; mov [eax+8],0 ; ret 4` (hidden result pointer) (thiscall, this unused).
// Identity: FableWin.pdb 0x01D91750 ?GetActionMovementVector@CCombatActionBase@@UBE?AVC3DVector@@XZ (public virtual const; the header-inline CCombatActionBase body, which Ego_r / Ego_d do not record) +
//   retail CCombatActionBase vftable 0x01259E44 slot 75 (+0x12C) = 0x0062EA80, which the landed vtable emitters (0x00955620 and the CCreatureAction
//   class rows) bind under this spelling. One PDB + the retail slot (nf228 ruling: the second source is the retail vftable / body).
// NF261 (2026-11-22, q12740): re-spelled from a row-local non-virtual non-const `CCombatActionBase::GetActionMovementVector()` model (no Leaf line)
//   onto the shared engine/CCombatActionBase.h (MELEE branch + MELEE_OWN_VIRTUALS, the define block of the landed slot rows 0x0062EAC0 / 0x0062EAD0).
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

C3DVector CCombatActionBase::GetActionMovementVector() const
{
    return C3DVector(0.0f, 1.0f, 0.0f);
}
