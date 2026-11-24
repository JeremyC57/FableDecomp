#ifndef FABLE_ENGINE_CMELEECOMBATABILITYDEF_H
#define FABLE_ENGINE_CMELEECOMBATABILITYDEF_H
#ifdef FABLE_RETAIL_CDRAWABILITYBUTTONS_WEAPONABILITIES_PASS27
#include "engine/CParentDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/EWeaponClass.h"
#include "engine/CHeroAbilityStatPackage.h"
#include "stlport/fable_containers.h"
// Ego_r and FableWin field lists, retail RTTI and virtual table 0x01234D5C.
class CMeleeCombatAbilityDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    CDefString FlourishAnimName;
    CDefString FlourishResponseAnimName;
    float DamageMultiplier;
    bool Flourish;
    bool BlockCounter;
    long AbilityButtonGraphicOverride;
    std::vector<EWeaponClass> ValidWeaponClasses;
    bool Decapitate;
    long KnockDownEffects;
    bool EvadeAllHits;
    bool CauseRecoil;
    CHeroAbilityStatPackage StatsNeeded;
    long ComboMultiplierLowerBound;
    long ComboMultiplierUpperBound;
};
FABLE_STATIC_ASSERT(sizeof(CMeleeCombatAbilityDef) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CMeleeCombatAbilityDef, AbilityButtonGraphicOverride) == 0x4c);
#endif
#endif
