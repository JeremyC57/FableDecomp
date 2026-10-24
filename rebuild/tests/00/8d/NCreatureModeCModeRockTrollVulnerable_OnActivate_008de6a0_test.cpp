#include <cstdio>
// Behaviour test for NCreatureMode::CModeRockTrollVulnerable::OnActivate (retail 0x008DE6A0): the recovery count is five
// seconds of frames; base activation; the troll's animation is suspended and it starts the troll-vulnerable-into action.
struct CCreatureActionBase { int Kind; };
struct CThingCreatureBase {
    int suspends;
    int actions;
    void SetSuspendAnimation(bool suspend, long reason) { if (suspend && reason == 0) ++suspends; }
    void SetCurrentAction(const CCreatureActionBase& action) { actions += action.Kind; }
};
struct CCreatureAction_TrollVulnerableInto : CCreatureActionBase {
    CCreatureAction_TrollVulnerableInto(CThingCreatureBase* owner) { Kind = owner ? 1 : 100; }
};
static long g_FramesPerSecond = 30;
static int g_baseActivations;
struct CModeBase {
    void OnActivate() { ++g_baseActivations; }
};
struct CModeRockTrollVulnerable : CModeBase {
    long RecoveryCount;
    CThingCreatureBase* PCreature;
    void OnActivate()
    {
        RecoveryCount = (long)(g_FramesPerSecond * 5.0f);
        CModeBase::OnActivate();
        CThingCreatureBase* creature = PCreature;
        creature->SetSuspendAnimation(true, 0);
        creature->SetCurrentAction(CCreatureAction_TrollVulnerableInto(PCreature));
    }
};

int main()
{
    CThingCreatureBase troll;
    troll.suspends = 0; troll.actions = 0;
    CModeRockTrollVulnerable m;
    m.PCreature = &troll; m.RecoveryCount = 0;
    g_baseActivations = 0;
    m.OnActivate();
    if (m.RecoveryCount != 150 || g_baseActivations != 1 || troll.suspends != 1 || troll.actions != 1) {
        std::printf("008de6a0_TEST FAIL\n");
        return 1;
    }
    std::printf("008de6a0_TEST PASS\n");
    return 0;
}