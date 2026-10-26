// Behaviour test for 0x008baf20 CCreatureAction_CastUnholyPowerSpell::Initialise: find-or-add the CTCUnholyPower component (type 0x86), arm it, and give it
// the action's thing only when that thing is alive.
#include <cstdio>
#include <cstring>
class CThing;
struct CPtr { CThing* item; CPtr() : item(0) { } CPtr(const CPtr& o) : item(o.item) { } CPtr& operator=(const CPtr& o) { item = o.item; return *this; } CThing* GetPItem() const { return item; } };
struct CTC { CPtr PTarget; unsigned char m_bArmed; void SetTarget(CPtr t) { PTarget = t; } };
class CThing { public:
    bool dead; bool has; CTC tc; int added; char addedName[32];
    bool GetInterface(CTC** out, long type) { if (type == 0x86 && has) { *out = &tc; return true; } return false; }
    CTC* AddTC(const char* name, bool, void*) { ++added; std::strcpy(addedName, name); has = true; return &tc; }
};
inline bool IS_THING_ALIVE(const CPtr& p) { if (p.GetPItem() != 0) { if (!p.GetPItem()->dead) return true; } return false; }
struct CAction {
    CPtr PCreature; CPtr PItemRemoved; int baseInit;
    CAction() : baseInit(0) { }
    void BaseInitialise() { ++baseInit; }
    void Initialise()
    {
        BaseInitialise();
        CTC* tc;
        if (!PCreature.GetPItem()->GetInterface(&tc, 0x86))
            tc = PCreature.GetPItem()->AddTC("CTCUnholyPower", true, 0);
        tc->m_bArmed = 1;
        CPtr thing(PItemRemoved);
        if (IS_THING_ALIVE(thing))
            tc->SetTarget(thing);
    }
};
int main()
{
    CThing creature; creature.dead = false; creature.has = false; creature.added = 0; creature.tc.m_bArmed = 0;
    CThing victim; victim.dead = false;
    CAction a; a.PCreature.item = &creature; a.PItemRemoved.item = &victim;
    a.Initialise();
    int ok = a.baseInit == 1 && creature.added == 1 && std::strcmp(creature.addedName, "CTCUnholyPower") == 0 && creature.tc.m_bArmed == 1 && creature.tc.PTarget.item == &victim;
    creature.tc.PTarget.item = 0; victim.dead = true;
    a.Initialise();
    ok = ok && creature.added == 1 && creature.tc.PTarget.item == 0;
    if (!ok) { std::printf("008baf20_TEST FAIL\n"); return 1; }
    std::printf("008baf20_TEST PASS\n");
    return 0;
}