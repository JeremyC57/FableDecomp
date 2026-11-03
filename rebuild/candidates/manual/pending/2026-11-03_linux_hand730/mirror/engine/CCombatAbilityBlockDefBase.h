#ifndef FABLE_ENGINE_CCOMBATABILITYBLOCKDEFBASE_H
#define FABLE_ENGINE_CCOMBATABILITYBLOCKDEFBASE_H
// Original PDB type and retail layout evidence: Codex pass 11 row 0x007355F0 NAMING_EVIDENCE.md.
#include "engine/CSubDefClassBase.h"
#include "engine/CCombatAbilityBlockingData.h"
class CCombatAbilityBlockDefBase:public CSubDefClassBase {public:CCombatAbilityBlockingData CombatAbility;};
FABLE_STATIC_ASSERT(offsetof(CCombatAbilityBlockDefBase,CombatAbility)==0x28);
#endif
