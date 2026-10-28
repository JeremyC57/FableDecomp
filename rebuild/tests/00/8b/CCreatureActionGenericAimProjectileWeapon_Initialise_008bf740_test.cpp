// behaviour test for 0x008BF740 CCreatureAction_GenericAimProjectileWeapon::Initialise(): set the "PROJECTILE_WEAPON_AIM"
// combat animation, put a live primary-slot projectile weapon into aiming, then run the combat Initialise
#include <cstdio>
#include <cstring>
static int gOrder = 0;
struct Weapon { bool projectile; int aimAt; bool IsProjectileWeapon() const { return projectile; } void SetAiming() { aimAt = ++gOrder; } };
struct Thing { bool dead; Weapon* weapon; bool IsDead() const { return dead; } bool GetTC(Weapon** out) { if (!weapon) return false; *out = weapon; return true; } };
struct Carrying { Thing* primary; Thing* GetThingInPrimarySlot() const { return primary; } };
struct Creature { Carrying* carrying; bool GetTC(Carrying** out) { if (!carrying) return false; *out = carrying; return true; } };
struct Action {
    Creature* PCreature; char anim[32]; int animAt, initAt;
    void SetCombatAnimation(const char* n) { std::strcpy(anim, n); animAt = ++gOrder; }
    void BaseInitialise() { initAt = ++gOrder; }
    void Initialise()
    {
        SetCombatAnimation("PROJECTILE_WEAPON_AIM");
        Carrying* carrying;
        if (PCreature->GetTC(&carrying)) {
            Thing* weapon = carrying->GetThingInPrimarySlot();
            Weapon* weaponTC;
            if (weapon && !weapon->IsDead() && weapon->GetTC(&weaponTC) && weaponTC->IsProjectileWeapon())
                weaponTC->SetAiming();
        }
        BaseInitialise();
    }
};
int main()
{
    int fails = 0;
    Weapon bow = { true, 0 };
    Thing bowThing = { false, &bow };
    Carrying car = { &bowThing };
    Creature hero = { &car };
    Action a; a.PCreature = &hero;
    a.Initialise();
    if (std::strcmp(a.anim, "PROJECTILE_WEAPON_AIM") != 0 || a.animAt != 1 || bow.aimAt != 2 || a.initAt != 3) fails |= 1;
    bow.aimAt = 0; bow.projectile = false; a.Initialise();
    bow.projectile = true; bowThing.dead = true; a.Initialise();
    bowThing.dead = false; car.primary = 0; a.Initialise();
    hero.carrying = 0; a.Initialise();
    if (bow.aimAt != 0 || a.initAt != 11) fails |= 2;
    if (fails) { std::printf("008bf740_TEST FAIL %d\n", fails); return 1; }
    std::printf("008bf740_TEST PASS\n");
    return 0;
}