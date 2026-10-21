#include <cstdio>
// Behaviour copy of CCreatureAction_SummonerSheatheWeapon::PerformAction (0x008CB780): move the live weapon to the sheathe slot, base PerformAction, hand the combat component the creature def's attack style (counted def released afterwards).
struct Def { long refs; bool destroyed; };
struct Weapon { bool alive; };
struct Carrying { Weapon* primary; Weapon* removed; Weapon* placed; long slot; bool now; };
struct Combat { const Def* style; };
struct Creature { Carrying* carrying; Combat* combat; long attackStyleIndex; Def* defTable; };
static void PerformAction(Creature& c, bool& baseRan)
{
    if (c.carrying) {
        Weapon* w = c.carrying->primary;
        if (w && w->alive) { c.carrying->removed = w; c.carrying->placed = w; c.carrying->slot = 77; c.carrying->now = true; }
    }
    baseRan = true;
    Def* style = &c.defTable[c.attackStyleIndex]; ++style->refs;   // GetDef<CAttackStyleDef> hands over a reference
    c.combat->style = style;
    if (--style->refs == 0) style->destroyed = true;                // the CDefPointer local's release
}
int main()
{
    Weapon sword = { true }; Carrying carry = { &sword, 0, 0, 0, false }; Combat combat = { 0 };
    Def defs[2] = { { 1, false }, { 0, false } };
    Creature c = { &carry, &combat, 0, defs }; bool base = false;
    PerformAction(c, base);
    bool ok = carry.removed == &sword && carry.placed == &sword && carry.slot == 77 && carry.now && base && combat.style == &defs[0] && defs[0].refs == 1 && !defs[0].destroyed;
    c.attackStyleIndex = 1; sword.alive = false; carry.removed = 0;
    PerformAction(c, base);
    ok = ok && carry.removed == 0 && combat.style == &defs[1] && defs[1].destroyed;
    std::printf(ok ? "008cb780_TEST PASS\n" : "008cb780_TEST FAIL\n");
    return ok ? 0 : 1;
}