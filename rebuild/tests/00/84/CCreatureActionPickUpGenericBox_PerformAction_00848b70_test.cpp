// Behaviour test for 0x00848B70 CCreatureAction_PickUpGenericBox::PerformAction: the same logic through a small copy of the model.
#include <cstdio>

enum { TCI_ENTITY_MODE_MANAGER = 0x31, TCI_CARRYING = 0x46, TCI_CARRYABLE = 0x47, TCI_WEAPON = 0x48 };

class CCarryableDef { public: int refs; long ActiveCarrySlot; };
struct DefRef {                                   // CDefPointer<const CCarryableDef> stand-in for the test: counted reference
    CCarryableDef* p;
    DefRef() : p(0) {}
    ~DefRef() { if (p) --p->refs; }
};
class CTCCarrying;
class CTCCarryable { public: CTCCarrying* PCarryingTC; };
class CThing { public:
    unsigned char Type; bool Dead; bool weapon; bool killed; bool killedImmediately;
    CCarryableDef* def; CTCCarryable carryable; CTCCarrying* carrying; struct Modes* modes;
    CThing() : Type(0), Dead(false), weapon(false), killed(false), killedImmediately(false), def(0), carrying(0), modes(0) { carryable.PCarryingTC = 0; }
    bool IsDead() const { return Dead; }
    unsigned long GetType() const { return Type; }
    bool HasTC(int id) const { return id == TCI_WEAPON && weapon; }
    void Kill(bool immediately) { killed = true; killedImmediately = immediately; }
    bool GetSubDef(DefRef* out) { out->p = def; ++def->refs; return true; }
};
class CTCCarrying { public:
    CThing* primary; CThing* put; long putSlot; bool putImmediately; int removes; bool removeArg;
    CTCCarrying() : primary(0), put(0), putSlot(-1), putImmediately(false), removes(0), removeArg(true) {}
    CThing* RemoveThingFromPrimarySlot(bool b) { ++removes; removeArg = b; CThing* t = primary; primary = 0; return t; }
    void PutThingInCarrySlot(CThing& thing, long slot, bool immediately) { put = &thing; putSlot = slot; putImmediately = immediately; }
};
struct Modes { unsigned long added; int count; Modes() : added(0), count(0) {} bool AddMode(unsigned long m) { added = m; ++count; return true; } };

struct Action {
    CThing* PCreature; CThing* PThingToPickUp; int finished;
    Action() : PCreature(0), PThingToPickUp(0), finished(0) {}
    void FinishAction() { ++finished; }
    void PerformAction()
    {
        if (PThingToPickUp != 0 && !PThingToPickUp->IsDead()) {
            const CTCCarryable* ptc_carryable = &PThingToPickUp->carryable;
            if (ptc_carryable->PCarryingTC == 0) {
                CTCCarrying* ptc_carrying = PCreature->carrying;
                CThing* held = ptc_carrying->RemoveThingFromPrimarySlot(false);
                if (held && !held->IsDead() && PCreature->GetType() == 1 && held->HasTC(TCI_WEAPON))
                    held->Kill(true);
                DefRef def;
                PThingToPickUp->GetSubDef(&def);
                ptc_carrying->PutThingInCarrySlot(*PThingToPickUp, def.p->ActiveCarrySlot, true);
                PCreature->modes->AddMode(0x22);
            } else {
                FinishAction();
            }
        } else {
            FinishAction();
        }
    }
};

static int fail(const char* why) { (void)why; std::printf("00848b70_TEST FAIL\n"); return 1; }

int main()
{
    CCarryableDef def; def.refs = 1; def.ActiveCarrySlot = 3;
    // 1: no box -> FinishAction only
    {
        CTCCarrying carrying; Modes modes; CThing creature; creature.carrying = &carrying; creature.modes = &modes;
        Action a; a.PCreature = &creature;
        a.PerformAction();
        if (a.finished != 1 || carrying.removes != 0 || carrying.put != 0 || modes.count != 0) return fail("null box");
    }
    // 2: dead box -> FinishAction only
    {
        CTCCarrying carrying; Modes modes; CThing creature; creature.carrying = &carrying; creature.modes = &modes;
        CThing box; box.def = &def; box.Dead = true;
        Action a; a.PCreature = &creature; a.PThingToPickUp = &box;
        a.PerformAction();
        if (a.finished != 1 || carrying.removes != 0 || carrying.put != 0 || modes.count != 0) return fail("dead box");
    }
    // 3: box already carried by someone -> FinishAction only
    {
        CTCCarrying carrying, other; Modes modes; CThing creature; creature.carrying = &carrying; creature.modes = &modes;
        CThing box; box.def = &def; box.carryable.PCarryingTC = &other;
        Action a; a.PCreature = &creature; a.PThingToPickUp = &box;
        a.PerformAction();
        if (a.finished != 1 || carrying.removes != 0 || carrying.put != 0 || modes.count != 0) return fail("carried box");
    }
    // 4: free box, empty hand -> picked up into the def's slot, mode 0x22 added, def reference released
    {
        CTCCarrying carrying; Modes modes; CThing creature; creature.carrying = &carrying; creature.modes = &modes; creature.Type = 1;
        CThing box; box.def = &def;
        Action a; a.PCreature = &creature; a.PThingToPickUp = &box;
        a.PerformAction();
        if (a.finished != 0 || carrying.removes != 1 || carrying.removeArg != false) return fail("pickup: remove");
        if (carrying.put != &box || carrying.putSlot != 3 || !carrying.putImmediately) return fail("pickup: put");
        if (modes.count != 1 || modes.added != 0x22) return fail("pickup: mode");
        if (def.refs != 1) return fail("pickup: def reference");
    }
    // 5: the held-thing kill rule: alive weapon in a type-1 creature's hand is killed, nothing else is
    {
        static const struct { unsigned char type; bool dead; bool weapon; bool expectKill; } cases[] = {
            { 1, false, true,  true  },
            { 1, false, false, false },
            { 1, true,  true,  false },
            { 2, false, true,  false },
            { 0, false, true,  false },
        };
        for (int i = 0; i < int(sizeof(cases) / sizeof(cases[0])); ++i) {
            CTCCarrying carrying; Modes modes; CThing creature; creature.carrying = &carrying; creature.modes = &modes;
            creature.Type = cases[i].type;
            CThing held; held.Dead = cases[i].dead; held.weapon = cases[i].weapon; carrying.primary = &held;
            CThing box; box.def = &def;
            Action a; a.PCreature = &creature; a.PThingToPickUp = &box;
            a.PerformAction();
            if (held.killed != cases[i].expectKill) return fail("kill rule");
            if (cases[i].expectKill && !held.killedImmediately) return fail("kill immediately");
            if (a.finished != 0 || carrying.put != &box || carrying.putSlot != 3 || modes.added != 0x22 || def.refs != 1) return fail("pickup after held");
        }
    }
    std::printf("00848b70_TEST PASS\n");
    return 0;
}