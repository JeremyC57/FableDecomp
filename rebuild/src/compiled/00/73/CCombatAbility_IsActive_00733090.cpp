// CCombatAbility::IsActive() const -- the ability's active countdown is running (ActiveTimer >= 0)
// Leaf: ?IsActive@CCombatAbility@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00733090, 11 bytes (`mov edx,[ecx+4] ; xor eax,eax ; test edx,edx ; setge al ; ret`). NEW catalogue row:
//   int3 pad before (0x00733081..8F, after the 0x00733080 `ret`) and after (0x0073309B..9F); no E8/E9 caller and no absolute reference
//   (an unreferenced out-of-line copy: its callers 0x007332F0 / 0x00733CF0 expand it).
// PDB: Ego_r.pdb 0x0074B240 + FableWin.pdb ?IsActive@CCombatAbility@@QBE_NXZ; Ego_r order IsActive 0x0074B240 / SetAsActive 0x0074B250 /
//   SetAsFinished 0x0074B270 / Reset 0x0074B290 = retail 0x00733090 / 0x007330A0 / 0x007330C0 / 0x007330E0. Ego_d.pdb: tc_combat.cpp
//   (file id 3264) line 194, 14 bytes at /Od (this text: 14).
// Layout: engine/CCombatAbility.h (Ego_r.pdb: RepeatTimer +0, ActiveTimer +4).
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

bool CCombatAbility::IsActive() const
{
    return ActiveTimer >= 0;
}