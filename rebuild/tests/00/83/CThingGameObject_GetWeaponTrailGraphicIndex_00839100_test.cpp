// Behaviour test for 0x00839100 CThingGameObject::GetWeaponTrailGraphicIndex: the row's selection logic over a model of the
// things and components it reads (STLport tree with the real _M_decrement / _M_find walks, bool color). Every branch is
// reachable: the carrier swap (carryable with PCarryingTC -> GetPCarriedByThing), the creature path (current action null or
// IsKnockdownStrike / IsVitalStrike / IsDecapitatingAttack), the non-creature path (this thing's CTCTrapBase
// IsCurrentTrapToCauseKnockdown), the wielder's berserk / combat flourish, the reverse augmentation walk and the type-0 find.
#include <cstdio>
struct NodeBase { bool _M_color; NodeBase* _M_parent; NodeBase* _M_left; NodeBase* _M_right; };
struct Node : NodeBase { long Key; long Attack; long Knockdown; };
static NodeBase* Decrement(NodeBase* n)
{
    if (n->_M_color == false && n->_M_parent->_M_parent == n) return n->_M_right;
    if (n->_M_left) { NodeBase* y = n->_M_left; while (y->_M_right) y = y->_M_right; return y; }
    NodeBase* y = n->_M_parent;
    while (n == y->_M_left) { n = y; y = y->_M_parent; }
    return y;
}
static NodeBase* Find(NodeBase* header, long key)
{
    NodeBase* y = header; NodeBase* x = header->_M_parent;
    while (x) { if (!(((Node*)x)->Key < key)) { y = x; x = x->_M_left; } else x = x->_M_right; }
    return (y == header || key < ((Node*)y)->Key) ? header : y;
}
struct Action { bool knockdown, vital, decapitating; };
struct Thing;
struct Carryable { Thing* carrier; bool carryingTC; };
struct Trap { bool knockdownTrap; };
struct Berserk { long attack, knockdown; };
struct Combat { bool flourishValid, inFlourish; };
struct TrailSet { long Attack; long Knockdown; }; // CWeaponTrailGraphicSet (Ego_r)
struct Def { TrailSet FlourishSpecialWeaponTrail; NodeBase* trails; };
struct Thing {
    unsigned long typeFlags; Def* def; Carryable* carryable; Trap* trap; Berserk* berserk; Combat* combat;
    unsigned long* augmentations; Action* action;
};
static long GetWeaponTrailGraphicIndex(const Thing* self)
{
    if (!self->def) return -1;
    const Def* def = self->def;
    const Thing* wielder = self;
    if (self->carryable && self->carryable->carryingTC) wielder = self->carryable->carrier;
    bool special = false;
    if (wielder->typeFlags & 8) {
        const Action* a = wielder->action;
        if (a && (a->knockdown || a->vital || a->decapitating)) special = true;
    } else if (self->trap && self->trap->knockdownTrap)
        special = true;
    if (wielder->berserk) return special ? wielder->berserk->knockdown : wielder->berserk->attack;
    if (wielder->combat && wielder->combat->flourishValid && !wielder->combat->inFlourish) {
        long g = special ? def->FlourishSpecialWeaponTrail.Knockdown : def->FlourishSpecialWeaponTrail.Attack;
        if (g > 0) return g;
    }
    if (self->augmentations) {
        NodeBase* end = def->trails->_M_left;
        for (NodeBase* it = def->trails; it != end; it = Decrement(it)) {
            Node* n = (Node*)Decrement(it);
            if (n->Key && (*self->augmentations & n->Key) == (unsigned long)n->Key)
                return special ? n->Knockdown : n->Attack;
        }
    }
    NodeBase* e = Find(def->trails, 0);
    if (e != def->trails) return special ? ((Node*)e)->Knockdown : ((Node*)e)->Attack;
    return -1;
}
int main()
{
    NodeBase header = { false, 0, 0, 0 };
    Node n0, n2, n4;
    n2._M_color = true; n2._M_parent = &header; n2._M_left = &n0; n2._M_right = &n4; n2.Key = 2; n2.Attack = 20; n2.Knockdown = 21;
    n0._M_color = false; n0._M_parent = &n2; n0._M_left = 0; n0._M_right = 0; n0.Key = 0; n0.Attack = 10; n0.Knockdown = 11;
    n4._M_color = false; n4._M_parent = &n2; n4._M_left = 0; n4._M_right = 0; n4.Key = 4; n4.Attack = 40; n4.Knockdown = 41;
    header._M_parent = &n2; header._M_left = &n0; header._M_right = &n4;
    Def def = { { 7, 8 }, &header };
    unsigned long augBits = 0;
    Action action = { false, false, false };
    Trap trap = { false };
    Berserk berserk = { 50, 51 };
    Combat combat = { false, false };
    Thing hero = { 8, 0, 0, 0, 0, 0, 0, 0 };                       // creature carrier
    Carryable carry = { &hero, false };
    Thing weapon = { 0, &def, &carry, &trap, 0, 0, &augBits, 0 };  // the weapon object itself
    int bad = 0;
    augBits = 4 | 2; if (GetWeaponTrailGraphicIndex(&weapon) != 40) bad |= 1;         // reverse walk: highest key first
    augBits = 2; if (GetWeaponTrailGraphicIndex(&weapon) != 20) bad |= 2;
    augBits = 0; if (GetWeaponTrailGraphicIndex(&weapon) != 10) bad |= 4;             // type-0 entry
    trap.knockdownTrap = true; if (GetWeaponTrailGraphicIndex(&weapon) != 11) bad |= 8; // not carried: non-creature trap path
    carry.carryingTC = true; if (GetWeaponTrailGraphicIndex(&weapon) != 10) bad |= 16;  // carried: creature path, null action
    hero.action = &action; if (GetWeaponTrailGraphicIndex(&weapon) != 10) bad |= 32;    // action with no special strike
    action.knockdown = true; if (GetWeaponTrailGraphicIndex(&weapon) != 11) bad |= 64;
    action.knockdown = false; action.vital = true; if (GetWeaponTrailGraphicIndex(&weapon) != 11) bad |= 128;
    action.vital = false; action.decapitating = true; if (GetWeaponTrailGraphicIndex(&weapon) != 11) bad |= 256;
    action.decapitating = false;
    hero.combat = &combat; combat.flourishValid = true; if (GetWeaponTrailGraphicIndex(&weapon) != 7) bad |= 512;  // wielder's combat
    action.vital = true; if (GetWeaponTrailGraphicIndex(&weapon) != 8) bad |= 1024;
    combat.inFlourish = true; if (GetWeaponTrailGraphicIndex(&weapon) != 11) bad |= 2048;
    weapon.combat = &combat; combat.inFlourish = false; carry.carryingTC = false; trap.knockdownTrap = false;
    weapon.combat = 0;                                                                   // not carried: weapon's own (no) combat
    if (GetWeaponTrailGraphicIndex(&weapon) != 10) bad |= 4096;
    carry.carryingTC = true; hero.berserk = &berserk; if (GetWeaponTrailGraphicIndex(&weapon) != 51) bad |= 8192; // berserk wielder
    action.vital = false; if (GetWeaponTrailGraphicIndex(&weapon) != 50) bad |= 16384;
    hero.berserk = 0; hero.combat = 0; def.FlourishSpecialWeaponTrail.Attack = 0;
    header._M_parent = &n4; n4._M_parent = &header; header._M_left = &n4; header._M_right = &n4; n4._M_color = true;
    if (GetWeaponTrailGraphicIndex(&weapon) != -1) bad |= 32768;                         // no type-0 entry
    weapon.def = 0; if (GetWeaponTrailGraphicIndex(&weapon) != -1) bad |= 65536;          // no weapon def
    if (bad) { std::printf("00839100_TEST FAIL %d\n", bad); return 1; }
    std::printf("00839100_TEST PASS\n");
    return 0;
}