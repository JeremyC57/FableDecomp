// Behaviour test for 0x008C7600 CCreatureAction_IceTrollIcicleAssaultInto::Initialise: animation, base Initialise, boss phase
// bump only with a boss component, and the icicle mode only with a mode manager and a live target.
#include <cstdio>
#include <cstring>
struct CThing;
struct CTCBoss { int phase; void IncrementBossPhase() { ++phase; } };
struct CMode { CThing* owner; CThing* target; CMode(CThing* o, CThing* t) : owner(o), target(t) { } };
struct CTCCreatureModeManager { CMode* last; int count; void AddMode(CMode* m) { delete last; last = m; ++count; } };
struct CThing {
    bool dead; bool hasBoss; bool hasModes; CTCBoss boss; CTCCreatureModeManager modes;
    bool GetBoss(CTCBoss** out) { if (!hasBoss) return false; *out = &boss; return true; }
    bool GetModes(CTCCreatureModeManager** out) { if (!hasModes) return false; *out = &modes; return true; }
};
struct CPtr { CThing* item; CThing* GetPItem() const { return item; } };
struct CAction {
    CPtr PCreature; CPtr PTarget; char anim[32]; int baseInit;
    void SetAnimationName(const char* s) { std::strcpy(anim, s); }
    void Initialise()
    {
        SetAnimationName("ICICLE_ASSAULT_INTO");
        ++baseInit;
        CTCBoss* boss;
        if (PCreature.GetPItem()->GetBoss(&boss))
            boss->IncrementBossPhase();
        CTCCreatureModeManager* modeManager;
        if (PCreature.GetPItem()->GetModes(&modeManager) && PTarget.GetPItem() && !PTarget.GetPItem()->dead)
            modeManager->AddMode(new CMode(PCreature.GetPItem(), PTarget.GetPItem()));
    }
};
int main()
{
    CThing troll; troll.dead = false; troll.hasBoss = true; troll.hasModes = true; troll.boss.phase = 0; troll.modes.last = 0; troll.modes.count = 0;
    CThing hero; hero.dead = false;
    CAction a; a.PCreature.item = &troll; a.PTarget.item = &hero; a.baseInit = 0;
    a.Initialise();
    int ok = std::strcmp(a.anim, "ICICLE_ASSAULT_INTO") == 0 && a.baseInit == 1 && troll.boss.phase == 1 && troll.modes.count == 1
          && troll.modes.last->owner == &troll && troll.modes.last->target == &hero;
    hero.dead = true; troll.hasBoss = false;
    a.Initialise();
    ok = ok && troll.boss.phase == 1 && troll.modes.count == 1;
    a.PTarget.item = 0; hero.dead = false;
    a.Initialise();
    ok = ok && troll.modes.count == 1;
    delete troll.modes.last;
    if (!ok) { std::printf("008c7600_TEST FAIL\n"); return 1; }
    std::printf("008c7600_TEST PASS\n");
    return 0;
}
