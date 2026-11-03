#ifndef FABLE_ENGINE_CCOMBATANTINFO_H
#define FABLE_ENGINE_CCOMBATANTINFO_H
// Original PDB type and retail layout evidence: Codex pass 11 row 0x007355F0 NAMING_EVIDENCE.md.
#include "engine/CAttackingCombatantInfo.h"
#include "engine/CDefendingCombatantInfo.h"
struct CCombatantInfo {CThingCreatureBase* Parent;CAttackingCombatantInfo AttackerInfo;CDefendingCombatantInfo DefenderInfo;};
FABLE_STATIC_ASSERT(sizeof(CCombatantInfo)==0x70);
#endif
