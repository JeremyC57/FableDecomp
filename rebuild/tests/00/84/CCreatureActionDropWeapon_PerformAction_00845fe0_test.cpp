// behaviour test for 0x00845fe0 CCreatureAction_DropWeapon::PerformAction():
// a creature with a carrying component drops the weapon in its primary slot (if any), then a creature with a mode
// manager leaves mode 4; without a carrying component nothing happens
#include <cstdio>
struct Thing { int id; };
struct Carrying { Thing* primary; };
struct Modes { int removed; Modes() : removed(-1) {} void RemoveMode(unsigned long m) { removed = (int)m; } };
struct Creature { Carrying* carrying; Modes* modes; };
static int gDrops = 0; static Thing* gDropped = 0;
static void DropWeapon(Thing& weapon, Carrying&, Creature&) { ++gDrops; gDropped = &weapon; }
static void PerformAction(Creature* c)
{
    Carrying* carrying = c->carrying;
    if (carrying) {
        Thing* weapon = carrying->primary;
        if (weapon)
            DropWeapon(*weapon, *carrying, *c);
        Modes* modes = c->modes;
        if (modes)
            modes->RemoveMode(4);
    }
}
int main()
{
    int fails = 0;
    Thing sword = { 1 }; Carrying carry = { &sword }; Modes modes;
    Creature c = { &carry, &modes };
    PerformAction(&c);
    if (gDrops != 1 || gDropped != &sword || modes.removed != 4) fails |= 1;
    Carrying empty = { 0 }; Modes m2; Creature c2 = { &empty, &m2 };
    PerformAction(&c2);
    if (gDrops != 1 || m2.removed != 4) fails |= 2;
    Modes m3; Creature c3 = { 0, &m3 };
    PerformAction(&c3);
    if (m3.removed != -1) fails |= 4;
    if (fails) { std::printf("00845fe0_TEST FAIL %d\n", fails); return 1; }
    std::printf("00845fe0_TEST PASS\n");
    return 0;
}