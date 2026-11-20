// CCombatAbility::SetAsActive(const CCombatAbilityData& combat_ability) -- start the active countdown: ConstantFPS * SecondsDuration frames
// Leaf: ?SetAsActive@CCombatAbility@@QAEXABVCCombatAbilityData@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007330A0, 28 bytes (fild ConstantFPS 0x01375550 ; fmul [data+8] ; __ftol2 ; store +4 ; `ret 4`). NEW
//   catalogue row: int3 pad before (0x0073309B..9F) and after (0x007330BC..BF); no E8/E9 caller and no absolute reference (an
//   unreferenced out-of-line copy: 0x00733CF0 expands it at 0x00733DEB).
// PDB: Ego_r.pdb 0x0074B250 + FableWin.pdb ?SetAsActive@CCombatAbility@@QAEXABVCCombatAbilityData@@@Z (Ego_r order as 0x00733090's).
//   Ego_d.pdb: tc_combat.cpp line 219, 28 bytes at /Od (this text: 28); parameter combat_ability.
// Callees: the inline CMainGameComponent::GetConstantFPS() (FableWin.pdb ?GetConstantFPS@CMainGameComponent@@SIJXZ; the static
//   ?ConstantFPS@CMainGameComponent@@1JA read in place), __ftol2.
// Layout: engine/CCombatAbility.h (ActiveTimer +4); engine/CCombatAbilityData.h (SecondsDuration +8).
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

void CCombatAbility::SetAsActive(const CCombatAbilityData& combat_ability)
{
    ActiveTimer = (long)(CMainGameComponent::GetConstantFPS() * combat_ability.SecondsDuration);
}