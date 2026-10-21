// Behaviour test for CCreatureAction_CastWillEnflameSpell::Initialise (0x008BA220): the owner's enflame component is
// reused when present (else added by name), activated and aimed; an AI owner gets the level; a live target is handed over.
#include <cstdio>
#include <string>

struct CThing;
struct CTCWillEnflame { bool Active; long Level; CThing* PTarget; CThing* Aim; bool Z; void SetPTarget(CThing* t, bool z) { Aim = t; Z = z; } };
struct CThing {
    unsigned char m_state; bool Dead; bool Has; CTCWillEnflame TC; int Added;
    bool GetInterface(CTCWillEnflame** out) { if (!Has) return false; *out = &TC; return true; }
    CTCWillEnflame* AddTC(const std::string& name) { if (name == "CTCWillEnflame") { ++Added; Has = true; } return &TC; }
};
struct CAction {
    CThing* PCreature; CThing* PItemRemoved; CThing* PTarget; int BaseInit; long Level; bool IsZTargetting;
    void Initialise()
    {
        ++BaseInit;
        CTCWillEnflame* tc;
        if (!PCreature->GetInterface(&tc))
            tc = PCreature->AddTC("CTCWillEnflame");
        tc->Active = true;
        tc->SetPTarget(PTarget, IsZTargetting);
        if (PCreature->m_state == 1)
            tc->Level = Level;
        CThing* target = PItemRemoved;
        if (target && !target->Dead)
            tc->PTarget = target;
    }
};

int main()
{
    CThing me = { 1, false, false, { false, 0, 0, 0, false }, 0 };
    CThing foe = { 0, false, false, { false, 0, 0, 0, false }, 0 };
    CAction a = { &me, &foe, &foe, 0, 4, true };
    a.Initialise();
    bool ok = me.Added == 1 && me.TC.Active && me.TC.Aim == &foe && me.TC.Z && me.TC.Level == 4 && me.TC.PTarget == &foe;
    me.m_state = 0; me.TC.Level = 0; me.TC.PTarget = 0; foe.Dead = true; a.Level = 9;
    a.Initialise();
    ok = ok && me.Added == 1 && me.TC.Level == 0 && me.TC.PTarget == 0 && a.BaseInit == 2;
    if (!ok) { std::printf("008ba220_TEST FAIL\n"); return 1; }
    std::printf("008ba220_TEST PASS\n");
    return 0;
}