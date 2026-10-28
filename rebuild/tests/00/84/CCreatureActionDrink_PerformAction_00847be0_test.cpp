// behaviour test for 0x00847BE0 CCreatureAction_Drink::PerformAction(): a live drink whose base def has a CBonusItemDef
// sub-def applies that bonus through the drinker's CTCHeroStats; no sub-def, no stats, a dead or missing drink: nothing
#include <cstdio>
struct BonusDef { int amount; };
struct BaseDef { const BonusDef* bonus; bool GetSubDef(const BonusDef** out) const { if (!bonus) return false; *out = bonus; return true; } };
struct Stats { int applied; int total; void ApplyBonusItemEffects(const BonusDef& d) { ++applied; total += d.amount; } };
struct Thing {
    bool dead; const BaseDef* def; Stats* stats;
    bool IsDead() const { return dead; }
    const BaseDef& PeekBaseDef() const { return *def; }
    bool GetTC(Stats** out) { if (!stats) return false; *out = stats; return true; }
};
struct Action {
    Thing* PCreature; Thing* PThingToDrink;
    void PerformAction()
    {
        if (PThingToDrink && !PThingToDrink->IsDead()) {
            const BonusDef* bonus;
            Stats* stats;
            if (PThingToDrink->PeekBaseDef().GetSubDef(&bonus) && PCreature->GetTC(&stats))
                stats->ApplyBonusItemEffects(*bonus);
        }
    }
};
int main()
{
    int fails = 0;
    BonusDef bonus = { 25 };
    BaseDef potion = { &bonus }, water = { 0 };
    Stats s = { 0, 0 };
    Thing hero = { false, &water, &s };
    Thing drink = { false, &potion, 0 };
    Action a = { &hero, &drink };
    a.PerformAction();
    if (s.applied != 1 || s.total != 25) fails |= 1;
    drink.def = &water; a.PerformAction();
    drink.def = &potion; drink.dead = true; a.PerformAction();
    drink.dead = false; hero.stats = 0; a.PerformAction();
    a.PThingToDrink = 0; a.PerformAction();
    if (s.applied != 1) fails |= 2;
    if (fails) { std::printf("00847be0_TEST FAIL %d\n", fails); return 1; }
    std::printf("00847be0_TEST PASS\n");
    return 0;
}