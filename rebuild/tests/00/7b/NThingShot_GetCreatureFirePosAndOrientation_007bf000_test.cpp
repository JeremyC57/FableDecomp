// Behaviour test for NThingShot::GetCreatureFirePosAndOrientation (0x007BF000): the same choice of fire point over a host model.
#include <cstdio>
#include <cstring>

struct Vec { float X, Y, Z; };
struct RHSet { Vec Up, Forward; };
struct Appearance {
    const char* helper;      // the one helper point this mesh has (0 = none)
    Vec helperPos;
    bool hasDummy; Vec dummyPos; RHSet dummySet;
    bool GetHelperPoint(const char* name, Vec& pos) const
    {
        if (!helper || std::strcmp(helper, name)) return false;
        pos = helperPos; return true;
    }
    bool GetDummyObject(const char* name, Vec& pos, RHSet& set) const
    {
        if (!hasDummy || std::strcmp(name, "SHOT_CREATION_DUMMY")) return false;
        pos = dummyPos; set = dummySet; return true;
    }
};
struct Action { bool hasStrikeHelper; const char* strikeHelper; };
struct Thing {
    bool isCreature; const Action* action; const Appearance* appearance; RHSet rh; Vec facing; float radius; Vec pos; float headZ;
    const Thing* primary; bool projectileWeapon;
};

static void GetCreatureFirePosAndOrientation(const Thing& creature, Vec& pos, RHSet& orientation)
{
    const Thing* source = &creature;
    const Thing* weapon = creature.primary;
    if (weapon && weapon->projectileWeapon)
        source = weapon;
    const Appearance* appearance = source->appearance;
    if (source->isCreature) {
        const Action* action = source->action;
        if (action && action->hasStrikeHelper) {
            bool found = appearance->GetHelperPoint(action->strikeHelper, pos);
            if (found) { orientation = source->rh; return; }
        }
    }
    if (appearance->GetDummyObject("SHOT_CREATION_DUMMY", pos, orientation))
        return;
    if (appearance->GetHelperPoint("SHOT_CREATION_POINT", pos)) { orientation = source->rh; return; }
    float height = 0.2f;
    if (creature.isCreature)
        height = creature.headZ;
    pos = creature.pos;
    pos.Z = height;
    if (pos.Z < height + 0.2f)
        pos.Z = height + 0.2f;
    Vec facing = creature.facing;
    Vec up = { 0.0f, 0.0f, 1.0f };
    orientation.Up = up; orientation.Forward = facing;
    pos.X += facing.X * creature.radius; pos.Y += facing.Y * creature.radius; pos.Z += facing.Z * creature.radius;
}

int main()
{
    int fails = 0;
    Appearance plain = { 0, { 0, 0, 0 }, false, { 0, 0, 0 }, { { 0, 0, 0 }, { 0, 0, 0 } } };
    Thing hero = { true, 0, &plain, { { 0, 0, 1 }, { 1, 0, 0 } }, { 0.0f, 1.0f, 0.0f }, 0.5f, { 10.0f, 20.0f, 0.0f }, 1.5f, 0, false };
    Vec pos; RHSet set;
    GetCreatureFirePosAndOrientation(hero, pos, set);                // fallback: head height + 0.2 along the facing
    if (pos.X != 10.0f || pos.Y != 20.5f || !(pos.Z > 1.65f && pos.Z < 1.75f) || set.Up.Z != 1.0f || set.Forward.Y != 1.0f) ++fails;
    Appearance point = { "SHOT_CREATION_POINT", { 1.0f, 2.0f, 3.0f }, false, { 0, 0, 0 }, { { 0, 0, 0 }, { 0, 0, 0 } } };
    hero.appearance = &point;
    GetCreatureFirePosAndOrientation(hero, pos, set);
    if (pos.Z != 3.0f || set.Forward.X != 1.0f) ++fails;
    Appearance dummy = { 0, { 0, 0, 0 }, true, { 4.0f, 5.0f, 6.0f }, { { 0, 1, 0 }, { 0, 0, 1 } } };
    hero.appearance = &dummy;
    GetCreatureFirePosAndOrientation(hero, pos, set);
    if (pos.X != 4.0f || set.Up.Y != 1.0f) ++fails;
    Action strike = { true, "HAND_R" };
    Appearance hand = { "HAND_R", { 7.0f, 8.0f, 9.0f }, true, { 4.0f, 5.0f, 6.0f }, { { 0, 1, 0 }, { 0, 0, 1 } } };
    hero.action = &strike; hero.appearance = &hand;
    GetCreatureFirePosAndOrientation(hero, pos, set);                // the strike helper wins
    if (pos.X != 7.0f || set.Forward.X != 1.0f) ++fails;
    Appearance bowMesh = { "SHOT_CREATION_POINT", { 11.0f, 0, 0 }, false, { 0, 0, 0 }, { { 0, 0, 0 }, { 0, 0, 0 } } };
    Thing bow = { false, 0, &bowMesh, { { 0, 0, 1 }, { 0, 1, 0 } }, { 0, 0, 0 }, 0.0f, { 0, 0, 0 }, 0.0f, 0, true };
    hero.primary = &bow;
    GetCreatureFirePosAndOrientation(hero, pos, set);                // a carried projectile weapon is the source
    if (pos.X != 11.0f || set.Forward.Y != 1.0f) ++fails;
    if (fails) { std::printf("007bf000_TEST FAIL %d\n", fails); return 1; }
    std::printf("007bf000_TEST PASS\n");
    return 0;
}