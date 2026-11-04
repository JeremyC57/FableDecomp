// Behaviour test for CTCSpecialAbilities::SetupDrainLifeSpell() (0x0072B260): a model of the row -- false unless the creature
// has a targeting component whose target is a live creature that is drainable and either an enemy or targeted while the
// creature's player is z-targeting; on success a missing CTCDrainLife component is added once and the call returns true.
#include <stdio.h>
#include <string.h>
struct Thing {
    bool hasTargeting, dead, isCreature, drainable, zTargeting, hasDrainLife;
    Thing* target; int enemyOf; int adds; char added[32];
};
static bool AreThingsEnemies(const Thing& a, const Thing& b) { return b.enemyOf == 1 && &a != &b; }
static void AddTC(Thing& t, const char* name) { ++t.adds; strcpy(t.added, name); t.hasDrainLife = true; }
static bool SetupDrainLifeSpell(Thing& creature)
{
    if (creature.hasTargeting) {
        Thing* target = creature.target;
        if (target && !target->dead && target->isCreature) {
            if (AreThingsEnemies(creature, *target) || creature.zTargeting) {
                if (target->drainable) {
                    if (!creature.hasDrainLife)
                        AddTC(creature, "CTCDrainLife");
                    return true;
                }
            }
        }
    }
    return false;
}
int main()
{
    Thing c; memset(&c, 0, sizeof c); Thing t; memset(&t, 0, sizeof t);
    c.hasTargeting = true; c.target = &t; t.isCreature = true; t.drainable = true; t.enemyOf = 1;
    int ok = SetupDrainLifeSpell(c) && c.adds == 1 && strcmp(c.added, "CTCDrainLife") == 0;
    ok = ok && SetupDrainLifeSpell(c) && c.adds == 1;
    t.dead = true; ok = ok && !SetupDrainLifeSpell(c); t.dead = false;
    t.enemyOf = 0; ok = ok && !SetupDrainLifeSpell(c);
    c.zTargeting = true; ok = ok && SetupDrainLifeSpell(c);
    t.drainable = false; ok = ok && !SetupDrainLifeSpell(c); t.drainable = true;
    t.isCreature = false; ok = ok && !SetupDrainLifeSpell(c); t.isCreature = true;
    c.target = 0; ok = ok && !SetupDrainLifeSpell(c);
    c.target = &t; c.hasTargeting = false; ok = ok && !SetupDrainLifeSpell(c);
    printf(ok ? "0072b260_TEST PASS\n" : "0072b260_TEST FAIL\n");
    return ok ? 0 : 1;
}