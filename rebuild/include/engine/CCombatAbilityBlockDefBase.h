#ifndef FABLE_ENGINE_CCOMBATABILITYBLOCKDEFBASE_H
#define FABLE_ENGINE_CCOMBATABILITYBLOCKDEFBASE_H
// Original PDB type and retail layout evidence: Codex pass 11 row 0x007355F0 NAMING_EVIDENCE.md.
#include "engine/CSubDefClassBase.h"
#include "engine/CCombatAbilityBlockingData.h"
#ifdef FABLE_RETAIL_CCOMBATABILITYBLOCKDEFBASE_TRANSFER
// Retail opt-in (chunk 1018, row 0x00465C54): Ego_r.pdb ?Transfer@CCombatAbilityBlockDefBase@@UAEXAAVCPersistContext@@@Z, vtable slot 18
// of CCombatAbilityBlockDefBase (0x01232E04, rtti-vtables.tsv). Declaration only (no layout change: the base already has slot 18).
class CPersistContext;
class CCombatAbilityBlockDefBase:public CSubDefClassBase {public:virtual void Transfer(CPersistContext&);CCombatAbilityBlockingData CombatAbility;};
#else
class CCombatAbilityBlockDefBase:public CSubDefClassBase {public:CCombatAbilityBlockingData CombatAbility;};
#endif
FABLE_STATIC_ASSERT(offsetof(CCombatAbilityBlockDefBase,CombatAbility)==0x28);
#endif
