// CCombatAbility::Reset() -- both countdowns off (ActiveTimer = RepeatTimer = -1)
// Leaf: ?Reset@CCombatAbility@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007330E0, 14 bytes (`mov dword [ecx+4],-1 ; mov dword [ecx],-1 ; ret`). NEW catalogue row: int3 pad
//   before (0x007330DB..DF) and after (0x007330EE..EF, then catalogued 0x007330F0); no E8/E9 caller and no absolute reference.
// PDB: Ego_r.pdb 0x0074B290 + FableWin.pdb ?Reset@CCombatAbility@@QAEXXZ (Ego_r order as 0x00733090's). Ego_d.pdb: tc_combat.cpp lines
//   231 / 232, 10 / 9 bytes at /Od (this text: 10 / 9 -- ActiveTimer first, as retail stores it).
// Layout: engine/CCombatAbility.h (RepeatTimer +0, ActiveTimer +4).
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

void CCombatAbility::Reset()
{
    ActiveTimer = -1;
    RepeatTimer = -1;
}