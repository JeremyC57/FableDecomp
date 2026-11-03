#ifndef FABLE_ENGINE_CCOMBATABILITYUSEPROJECTILEWEAPONDEF_H
#define FABLE_ENGINE_CCOMBATABILITYUSEPROJECTILEWEAPONDEF_H
// Original PDB type and retail layout evidence: Codex pass 11 row 0x007355F0 NAMING_EVIDENCE.md.
#include "engine/CSubDefClassBase.h"
#include "engine/CCombatAbilityProjectileAttackData.h"
class CCombatAbilityUseProjectileWeaponDef:public CSubDefClassBase {public:CCombatAbilityProjectileAttackData CombatAbility;};
FABLE_STATIC_ASSERT(offsetof(CCombatAbilityUseProjectileWeaponDef,CombatAbility)==0x28);
#endif
