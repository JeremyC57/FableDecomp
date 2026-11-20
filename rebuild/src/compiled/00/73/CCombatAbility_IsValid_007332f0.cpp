// CCombatAbility::IsValid(const CCombatAbilityData& combat_ability, unsigned long* prandom_seed) const -- an active ability stays valid; an idle one whose repeat countdown is off (RepeatTimer == -1) passes when the next value of the caller's random seed (mod 100) is below the data's percentage chance
// Leaf: ?IsValid@CCombatAbility@@QBE_NABVCCombatAbilityData@@PAK@Z
// Retail: Fable.exe @ 0x007332F0, 71 bytes, thiscall `ret 8`.
// Identity (proven): the manifest names the row CCombatAbility::IsValid with (CCombatAbilityData*, unsigned long*); both
//   PDBs have ?IsValid@CCombatAbility@@QBE_NABVCCombatAbilityData@@PAK@Z and Ego_r.pdb CCombatAbility is
//   {RepeatTimer +0, ActiveTimer +4}, which the body reads; CCombatAbilityData::PercentageChance is +4 (vptr first).
// Re-port (p25N, 2026-11-20): onto the shared engine/CCombatAbility.h / CCombatAbilityData.h (opt-in FABLE_RETAIL_CCOMBATABILITY_MEMBERS)
//   and Ego_d.pdb's text, for the same-TU copy in 0x00733CF0 UpdateGetHitCounterAttack. Ego_d (tc_combat.cpp, file id 3264): parameters
//   combat_ability / prandom_seed; lines 166 / 168 / 170 / 172 / 174 = 15 / 4 / 29 / 4 / 2 bytes at /Od, this text the same -- one 29-byte
//   condition at 170, so `RepeatTimer == -1` in place (an IsUsable() call is 15 bytes alone) and the seed stepped by GFRandom(100, seed)
//   (engine/GFRandom.h; Ego_r.pdb ?GFRandom@@YIKKPAK@Z: seed * 0x24A1 + 0x24DF, GFROR13 0x00497890, remainder by the range).
// Body: IsActive() (0x00733090, same TU, expanded: ActiveTimer >= 0) -> true; else RepeatTimer == -1 and the roll (unsigned `div`, `jae`)
//   below PercentageChance -> true; otherwise false.
// Define block: the tc_combat.cpp rows' common block (0x00733CF0 UpdateGetHitCounterAttack and its TU-mates), so that every same-TU copy
//   compiles from the same text.
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_GETHITCOUNTERATTACK
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITY_MEMBERS
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_STLP_MULTIMAP_BEGIN_END
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_STLP_NOT1
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_NENTITYEVENTS_CISHITEVENTHITBY
#include "engine/ECombatAbilityInterfaceType.h"
#include "engine/CTCCombat.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatAbilityData.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_EventFilters.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/GFRandom.h"

// Fidelity: same-TU strong definition of 0x00733090 (retail TU-mate)
bool CCombatAbility::IsActive() const
{
    return ActiveTimer >= 0;
}

bool CCombatAbility::IsValid(const CCombatAbilityData& combat_ability, unsigned long* prandom_seed) const
{
    if (IsActive())
        return true;
    if (RepeatTimer == -1 && GFRandom(100, prandom_seed) < (unsigned long)combat_ability.PercentageChance)
        return true;
    return false;
}
