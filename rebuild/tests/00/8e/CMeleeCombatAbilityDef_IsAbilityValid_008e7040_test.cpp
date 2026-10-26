// 008e7040 behaviour test: CMeleeCombatAbilityDef::IsAbilityValid (match.cpp control flow, host stand-ins)
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
class CTCHeroStats { public: long Combo; long Str; long GetComboMultiplier() const { return Combo; } };
class CTCCombat { public: bool Melee, Flourish; bool IsInMelee() const { return Melee; } bool CanFlourish() const { return Flourish; } };
class CTCWeapon { public: long Class; long GetWeaponClass() const { return Class; } };
class CThing { public: bool Alive; CTCWeapon* Weapon; };
class CTCCarrying { public: CThing* Primary; CThing* GetThingInPrimarySlot() const { return Primary; } };
class CThingCreatureBase;
struct CCombatantInfo { std::map<int, CThingCreatureBase*> Attackers; };
class CThingCreatureBase { public:
    CTCHeroStats* Stats; CTCCarrying* Carrying; CTCCombat* Combat; unsigned long Flags6c;
    std::vector<std::string> Anims; CCombatantInfo* Info;
    bool GetStats(CTCHeroStats** o) const { *o = Stats; return Stats != 0; }
    bool GetCarrying(CTCCarrying** o) const { *o = Carrying; return Carrying != 0; }
    bool GetCombat(CTCCombat** o) const { *o = Combat; return Combat != 0; }
    bool HasAnimation(const std::string& n) const { return std::find(Anims.begin(), Anims.end(), n) != Anims.end(); }
};
static bool IS_THING_ALIVE(CThing* t) { return t != 0 && t->Alive; }
class CHeroAbilityStatPackage { public: long MinStr; bool IsMetBy(const CTCHeroStats* s) const { return s->Str >= MinStr; } };
class CMeleeCombatAbilityDef { public:
    std::string FlourishAnimName; bool Flourish; std::vector<long> ValidWeaponClasses;
    CHeroAbilityStatPackage StatsNeeded; long ComboMultiplierLowerBound, ComboMultiplierUpperBound;
    bool IsAbilityValid(CThingCreatureBase* creature)
    {
        CTCHeroStats* stats;
        if (creature->GetStats(&stats)) {
            long multiplier = stats->GetComboMultiplier();
            if (ComboMultiplierLowerBound != -1 && multiplier <= ComboMultiplierLowerBound)
                goto fail;
            if (ComboMultiplierUpperBound != -1 && multiplier > ComboMultiplierUpperBound)
                goto fail;
            if (!StatsNeeded.IsMetBy(stats))
                goto fail;
            if (!(creature->Flags6c & 8))
                goto fail;
            {
            CTCCarrying* carrying;
            if (!creature->GetCarrying(&carrying))
                goto fail;
            CTCCombat* combat;
            if (!creature->GetCombat(&combat))
                goto fail;
            if (Flourish) {
                if (!combat->IsInMelee())
                    goto fail;
                if (!combat->CanFlourish())
                    goto fail;
            }
            CThing* weapon = carrying->GetThingInPrimarySlot();
            long weaponClass = 0;
            if (IS_THING_ALIVE(weapon)) {
                if (weapon->Weapon)
                    weaponClass = weapon->Weapon->GetWeaponClass();
            }
            std::vector<long>::iterator last = ValidWeaponClasses.end();
            if (std::find(ValidWeaponClasses.begin(), last, weaponClass) == last)
                goto fail;
            if (FlourishAnimName.length() != 0) {
                CCombatantInfo* info = creature->Info;
                for (std::map<int, CThingCreatureBase*>::iterator it = info->Attackers.begin(); it != info->Attackers.end(); ++it) {
                    CThingCreatureBase* attacker = it->second;
                    if (!attacker->HasAnimation(FlourishAnimName))
                        goto fail;
                }
            }
            return true;
            }
        }
fail:
        return false;
    }
};
int main()
{
    CTCHeroStats st = { 5, 10 }; CTCCombat cb = { true, true }; CTCWeapon wp = { 3 }; CThing sword = { true, &wp };
    CTCCarrying car = { &sword }; CCombatantInfo info;
    CThingCreatureBase hero; hero.Stats = &st; hero.Carrying = &car; hero.Combat = &cb; hero.Flags6c = 8; hero.Info = &info;
    CThingCreatureBase foe; foe.Stats = 0; foe.Carrying = 0; foe.Combat = 0; foe.Flags6c = 0; foe.Info = 0; foe.Anims.push_back("Flourish1");
    info.Attackers[1] = &foe;
    CMeleeCombatAbilityDef d; d.Flourish = false; d.ValidWeaponClasses.push_back(3); d.StatsNeeded.MinStr = 8;
    d.ComboMultiplierLowerBound = -1; d.ComboMultiplierUpperBound = -1;
    CHECK(d.IsAbilityValid(&hero));
    d.ComboMultiplierLowerBound = 5; CHECK(!d.IsAbilityValid(&hero));
    d.ComboMultiplierLowerBound = 4; CHECK(d.IsAbilityValid(&hero));
    d.ComboMultiplierUpperBound = 4; CHECK(!d.IsAbilityValid(&hero));
    d.ComboMultiplierUpperBound = 5; CHECK(d.IsAbilityValid(&hero));
    d.StatsNeeded.MinStr = 11; CHECK(!d.IsAbilityValid(&hero)); d.StatsNeeded.MinStr = 8;
    hero.Flags6c = 0; CHECK(!d.IsAbilityValid(&hero)); hero.Flags6c = 8;
    hero.Carrying = 0; CHECK(!d.IsAbilityValid(&hero)); hero.Carrying = &car;
    hero.Combat = 0; CHECK(!d.IsAbilityValid(&hero)); hero.Combat = &cb;
    d.Flourish = true; cb.Melee = false; CHECK(!d.IsAbilityValid(&hero)); cb.Melee = true;
    cb.Flourish = false; CHECK(!d.IsAbilityValid(&hero)); cb.Flourish = true; CHECK(d.IsAbilityValid(&hero));
    wp.Class = 2; CHECK(!d.IsAbilityValid(&hero));
    sword.Alive = false; CHECK(!d.IsAbilityValid(&hero));      // class 0 without a live weapon
    d.ValidWeaponClasses.push_back(0); CHECK(d.IsAbilityValid(&hero));
    d.FlourishAnimName = "Flourish1"; CHECK(d.IsAbilityValid(&hero));
    d.FlourishAnimName = "Flourish2"; CHECK(!d.IsAbilityValid(&hero));
    hero.Stats = 0; d.FlourishAnimName = ""; CHECK(!d.IsAbilityValid(&hero));
    if (g_fail) return 1;
    printf("008e7040_TEST PASS\n");
    return 0;
}