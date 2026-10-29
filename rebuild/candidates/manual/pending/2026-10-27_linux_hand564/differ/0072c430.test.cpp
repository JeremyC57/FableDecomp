// Behaviour test for CTCSpecialAbilities::IsAbilityAvailable(EHeroAbility) const @ 0x0072C430: the guards and the per-ability TC checks.
#include <stdio.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("check failed line %d\n", __LINE__); fails++; } } while (0)
typedef int EHeroAbility;
static int g_targetedCalls = 0;
class CTCBase { public: virtual ~CTCBase() {} };
class CTCInventoryAbilities : public CTCBase { public:
    bool Able[20];
    bool IsAbleToUseAbility(EHeroAbility ability, bool) const { return Able[ability]; }
};
class CTCTargetingBase : public CTCBase { public:
    virtual void* GetPTargetedThing() const { g_targetedCalls++; return 0; }
};
class CThing { public:
    bool Using[0x100]; const CTCBase* Slots[0x100];
    CThing() { for (int i = 0; i < 0x100; i++) { Using[i] = false; Slots[i] = 0; } }
    bool HasTC(int type) const { return Using[type]; }
    template <class T> bool PeekTC(const T** out, int type) const
    {
        if (HasTC(type)) { *out = (const T*)Slots[type]; return true; }
        return false;
    }
};
class CTCSpecialAbilities { public:
    CThing* ParentThing; CThing* Creature; bool AbilityAvailability[20]; bool AnyOk;
    bool IsAbleToUseAnyAbility(EHeroAbility) const { return AnyOk; }
    bool IsAbilityAvailable(EHeroAbility ability) const;
};
bool CTCSpecialAbilities::IsAbilityAvailable(EHeroAbility ability) const
{
    const CTCInventoryAbilities* inventoryAbilities;
    if (IsAbleToUseAnyAbility(ability) && AbilityAvailability[ability] && ParentThing->PeekTC(&inventoryAbilities, 0x6F) && inventoryAbilities->IsAbleToUseAbility(ability, true)) {
    const CTCTargetingBase* targeting;
    Creature->PeekTC(&targeting, 0x8);
    targeting->GetPTargetedThing();
    switch (ability) {
    case 2:  return !ParentThing->HasTC(0x92);
    case 1:  return !ParentThing->HasTC(0x96);
    case 3:  return !ParentThing->HasTC(0x95);
    case 5:  return !ParentThing->HasTC(0x9E);
    case 6:  return !ParentThing->HasTC(0x97);
    case 11: return !ParentThing->HasTC(0x94);
    case 8:  return !ParentThing->HasTC(0x84);
    case 12: return !ParentThing->HasTC(0xA8);
    case 17: return !ParentThing->HasTC(0x88);
    case 4:
    case 9:
    case 10: return true;
    case 13: return !ParentThing->HasTC(0xA9);
    case 14: return !ParentThing->HasTC(0x99);
    case 15: return !ParentThing->HasTC(0x9A);
    case 16: return !ParentThing->HasTC(0x9D);
    case 18: return !ParentThing->HasTC(0x85);
    case 19: return !ParentThing->HasTC(0x86);
    }
    }
    return false;
}
int main()
{
    static CThing hero, creature; CTCInventoryAbilities inv; CTCTargetingBase targeting;
    CTCSpecialAbilities sa; sa.ParentThing = &hero; sa.Creature = &creature; sa.AnyOk = true;
    for (int i = 0; i < 20; i++) { sa.AbilityAvailability[i] = true; inv.Able[i] = true; }
    creature.Using[0x8] = true; creature.Slots[0x8] = &targeting;
    for (int a = 1; a < 20; a++) CHECK(!sa.IsAbilityAvailable(a));        // no inventory abilities TC: never available
    hero.Using[0x6F] = true; hero.Slots[0x6F] = &inv;
    static const int tc[20] = { 0, 0x96, 0x92, 0x95, 0, 0x9E, 0x97, 0, 0x84, 0, 0, 0x94, 0xA8, 0xA9, 0x99, 0x9A, 0x9D, 0x88, 0x85, 0x86 };
    for (int a = 0; a < 20; a++) {
        bool want = (a == 4 || a == 9 || a == 10 || tc[a] != 0);
        CHECK(sa.IsAbilityAvailable(a) == want);                          // nothing active yet
        if (tc[a]) { hero.Using[tc[a]] = true; CHECK(!sa.IsAbilityAvailable(a)); hero.Using[tc[a]] = false; }
    }
    CHECK(!sa.IsAbilityAvailable(7));                                     // raise dead: no arm
    sa.AbilityAvailability[4] = false; CHECK(!sa.IsAbilityAvailable(4)); sa.AbilityAvailability[4] = true;
    inv.Able[9] = false; CHECK(!sa.IsAbilityAvailable(9)); inv.Able[9] = true;
    sa.AnyOk = false; CHECK(!sa.IsAbilityAvailable(10)); sa.AnyOk = true;
    CHECK(g_targetedCalls > 0);
    if (fails) { printf("0072c430_TEST FAIL\n"); return 1; }
    printf("0072c430_TEST PASS\n"); return 0;
}
