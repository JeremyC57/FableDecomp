// CCombatActionBase::AllowsCombinationAnims() const -- the base combat action never allows combination animations
// Leaf: ?AllowsCombinationAnims@CCombatActionBase@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062EA10, 3 bytes: `xor al,al ; ret` (thiscall, this unused).
// Identity: FableWin.pdb 0x01D916B0 ?AllowsCombinationAnims@CCombatActionBase@@UBE_NXZ (public virtual const: the override of the protected
//   CCreatureActionBase::AllowsCombinationAnims; the header-inline CCombatActionBase body, which Ego_r / Ego_d do not record) + retail
//   CCombatActionBase vftable 0x01259E44 slot 66 (+0x108) = 0x0062EA10 (CCreatureActionBase's own 0x01259D24 slot 66 is 0x00692890). One PDB +
//   the retail slot + the body (nf228 ruling; nf261 ruling 3 for the seven sibling slot rows 0x0062EA40 .. 0x0062EAB0).
// NF264 (2026-11-22, q12750): re-spelled from the free stand-in `bool __fastcall CCombatActionBase_AllowsCombinationAnims()` (no Leaf line) onto
//   the shared engine/CCombatActionBase.h (the sibling slot rows' MELEE define block + the new declaration-only opt-in
//   FABLE_RETAIL_CCOMBATACTIONBASE_ALLOWSCOMBINATIONANIMS).
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
#define FABLE_RETAIL_CCOMBATACTIONBASE_ALLOWSCOMBINATIONANIMS
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"

bool CCombatActionBase::AllowsCombinationAnims() const
{
    return false;
}
