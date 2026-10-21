// Behaviour test for CCreatureAction_CastBattleCharge::Initialise (0x008CD5A0): names the cast animation, adds the
// CTCBattleCharge component, aims it; an override sets distance + facing, an AI owner gets the level, and a live
// target thing is handed over (a dead or missing one is not).
#include <cstdio>
#include <cstring>
#include <string>

struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up, Forward; };
struct CPhysics { CRightHandedSet Last; int Calls; void SetFacingAnglesFromRHSet(const CRightHandedSet& s) { Last = s; ++Calls; } };
struct CThing;
struct CTCBattleCharge { float Distance; long Level; CThing* PTarget; CThing* Aim; bool Z; void SetPTarget(CThing* t, bool z) { Aim = t; Z = z; } };
struct CThing {
    unsigned char m_state; bool Dead; CPhysics Physics; CTCBattleCharge TC; std::string Added;
    CTCBattleCharge* AddTC(const std::string& name) { Added = name; return &TC; }
};
struct CAction {
    CThing* PCreature; CThing* PItemRemoved; CThing* PTarget; std::string Anim; bool BaseInit;
    bool Override; unsigned char Dir[12]; float Distance; long Level; bool IsZTargetting;
    void Initialise()
    {
        Anim = "ST_BATTLE_CHARGE_CAST";
        BaseInit = true;
        CTCBattleCharge* tc = PCreature->AddTC("CTCBattleCharge");
        tc->SetPTarget(PTarget, IsZTargetting);
        if (Override) {
            tc->Distance = Distance;
            CRightHandedSet set;
            set.Up.X = 0; set.Up.Y = 0; set.Up.Z = 1;
            std::memcpy(&set.Forward.X, Dir, 4); std::memcpy(&set.Forward.Y, Dir + 4, 4); std::memcpy(&set.Forward.Z, Dir + 8, 4);
            PCreature->Physics.SetFacingAnglesFromRHSet(set);
        }
        if (PCreature->m_state == 1)
            tc->Level = Level;
        CThing* target = PItemRemoved;
        if (target && !target->Dead)
            tc->PTarget = target;
    }
};

int main()
{
    CThing me, foe, item;
    std::memset(&me.TC, 0, sizeof me.TC); me.Physics.Calls = 0; me.m_state = 1; item.Dead = false;
    CAction a; a.PCreature = &me; a.PItemRemoved = &item; a.PTarget = &foe; a.BaseInit = false;
    a.Override = true; float d[3] = { 0.5f, -1.0f, 2.0f }; std::memcpy(a.Dir, d, 12);
    a.Distance = 7.5f; a.Level = 3; a.IsZTargetting = true;
    a.Initialise();
    bool ok = a.Anim == "ST_BATTLE_CHARGE_CAST" && a.BaseInit && me.Added == "CTCBattleCharge" && me.TC.Aim == &foe && me.TC.Z
        && me.TC.Distance == 7.5f && me.Physics.Calls == 1 && me.Physics.Last.Up.Z == 1.0f && me.Physics.Last.Forward.Y == -1.0f
        && me.TC.Level == 3 && me.TC.PTarget == &item;
    CThing me2; std::memset(&me2.TC, 0, sizeof me2.TC); me2.Physics.Calls = 0; me2.m_state = 0; item.Dead = true;
    CAction b = a; b.PCreature = &me2; b.Override = false; b.IsZTargetting = false;
    b.Initialise();
    ok = ok && me2.Physics.Calls == 0 && me2.TC.Distance == 0 && me2.TC.Level == 0 && me2.TC.PTarget == 0 && !me2.TC.Z;
    if (!ok) { std::printf("008cd5a0_TEST FAIL\n"); return 1; }
    std::printf("008cd5a0_TEST PASS\n");
    return 0;
}