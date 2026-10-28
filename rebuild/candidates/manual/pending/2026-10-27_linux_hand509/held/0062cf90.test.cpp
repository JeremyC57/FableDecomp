// Behaviour test for 0062cf90: CWorld::EACreatureBlock -- the gates (player mode 0x2F, either thing-manager pause flag,
// spell mode, not the main player, a dead creature, no CTCCombat, already blocking), the block veto (an unusable
// primary-slot weapon, overridden by CTCBerserk::CanBerserkBlock when the creature has one), then SetBlocking only when
// the StartBlocking action was accepted.
#include <cstdio>

struct Combat { bool blocking; int setCalls; bool IsBlocking() const { return blocking; } void SetBlocking() { ++setCalls; blocking = true; } };
struct Weapon { bool usable; };
struct Berserk { bool canBlock; };
struct Creature {
    bool alive;
    Combat* combat;                 // CTCCombat (0x30), or none
    bool hasCarrying;               // CTCCarrying (0x46)
    Weapon* primaryWeapon;          // the primary-slot thing's CTCWeapon (0x48); null = empty slot / no weapon TC
    Berserk* berserk;               // CTCBerserk (0x84), or none
    bool acceptsAction;
    int actions;
    bool SetCurrentAction() { ++actions; return acceptsAction; }
};
struct World {
    bool mode2F, nonScriptedPause, allPause, spellMode, mainPlayer;
    Creature* creature;
    void EACreatureBlock()
    {
        if (mode2F) return;
        Creature* c = creature;
        if (nonScriptedPause) return;
        if (allPause) return;
        if (spellMode) return;
        if (!mainPlayer) return;
        if (!(c && c->alive)) return;
        if (!c->combat) return;
        Combat* combat = c->combat;
        if (combat->IsBlocking()) return;
        bool canBlock = true;
        if (c->hasCarrying) {
            if (c->primaryWeapon && !c->primaryWeapon->usable)
                canBlock = false;
            if (c->berserk)
                canBlock = c->berserk->canBlock;
            if (!canBlock) return;
        }
        if (!combat->IsBlocking() && c->SetCurrentAction())
            combat->SetBlocking();
    }
};

static bool run(World w, Creature c, Combat* combat, int wantActions, int wantSet)
{
    w.creature = &c;
    c.combat = combat;
    if (combat) { combat->setCalls = 0; combat->blocking = false; }
    w.EACreatureBlock();
    return c.actions == wantActions && (!combat || combat->setCalls == wantSet);
}

int main()
{
    World base = { false, false, false, false, true, 0 };
    Combat combat = { false, 0 };
    Weapon bad = { false }, good = { true };
    Berserk yes = { true }, no = { false };
    Creature c0 = { true, 0, false, 0, 0, true, 0 };
    bool ok = run(base, c0, &combat, 1, 1);                              // plain block
    World w = base; w.mode2F = true;           ok = ok && run(w, c0, &combat, 0, 0);
    w = base; w.nonScriptedPause = true;       ok = ok && run(w, c0, &combat, 0, 0);
    w = base; w.allPause = true;               ok = ok && run(w, c0, &combat, 0, 0);
    w = base; w.spellMode = true;              ok = ok && run(w, c0, &combat, 0, 0);
    w = base; w.mainPlayer = false;            ok = ok && run(w, c0, &combat, 0, 0);
    Creature c = c0; c.alive = false;          ok = ok && run(base, c, &combat, 0, 0);
    ok = ok && run(base, c0, 0, 0, 0);                                   // no CTCCombat
    c = c0; c.hasCarrying = true; c.primaryWeapon = &bad;               ok = ok && run(base, c, &combat, 0, 0);
    c = c0; c.hasCarrying = true; c.primaryWeapon = &good;              ok = ok && run(base, c, &combat, 1, 1);
    c = c0; c.hasCarrying = true; c.primaryWeapon = &bad; c.berserk = &yes; ok = ok && run(base, c, &combat, 1, 1);
    c = c0; c.hasCarrying = true; c.primaryWeapon = &good; c.berserk = &no; ok = ok && run(base, c, &combat, 0, 0);
    c = c0; c.berserk = &no;                   ok = ok && run(base, c, &combat, 1, 1);   // berserk only read under carrying
    c = c0; c.acceptsAction = false;           ok = ok && run(base, c, &combat, 1, 0);   // action refused: no SetBlocking
    if (!ok) { std::printf("0062cf90_TEST FAIL\n"); return 1; }
    std::printf("0062cf90_TEST PASS\n");
    return 0;
}
