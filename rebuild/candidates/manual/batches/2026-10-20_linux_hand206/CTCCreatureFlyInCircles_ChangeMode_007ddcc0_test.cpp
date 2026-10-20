// Behaviour test for CTCCreatureFlyInCircles::ChangeMode (0x007DDCC0): hover (0) -> "HOVER_INTO_FLY", fly (1) -> "FLY_INTO_HOVER";
// on acceptance the mode flips, the transition flags are set and the speed delta is (target - speed) / duration.
#include <cstdio>
#include <cstring>
static int g_strDtors = 0, g_actionDtors = 0, g_actions = 0;
struct CCharString {
    const char* PStringData;
    CCharString() : PStringData("") { }
    ~CCharString() { ++g_strDtors; }
    CCharString& operator=(const char* text) { PStringData = text; return *this; }
};
struct CThingCreatureBase;
static long g_duration = 10;
struct CCombatActionBase {
    virtual void v00() { }
    long TotalTime;
    const char* Anim;
    ~CCombatActionBase() { ++g_actionDtors; }
    long GetTotalTime() const { return TotalTime; }
};
struct CCreatureAction_DragonFlight : CCombatActionBase {
    unsigned char FaceTarget;
    float         TurnRate;
    CCreatureAction_DragonFlight(CThingCreatureBase* owner, const CCharString& animName) { TotalTime = g_duration; Anim = animName.PStringData; ++g_actions; }
};
struct CThingCreatureBase {
    bool Accept; const char* LastAnim;
    bool SetCurrentAction(CCombatActionBase* action) { LastAnim = action->Anim; return Accept; }
};
struct CTCCreatureFlyInCircles {
    virtual void v00() { }
    CThingCreatureBase* Creature;
    float Speed, DeltaSpeed;
    long  SpeedChangeTicks, Mode;
    bool  Transitioning, ModeChangeQueued;
    void ChangeMode();
};

void CTCCreatureFlyInCircles::ChangeMode()
{
    CCharString animation;
    long newMode = 0;
    float targetSpeed = 0.0f;
    switch (Mode) {
    case 2:
        return;
    case 1:
        animation = "FLY_INTO_HOVER";
        newMode = 0;
        targetSpeed = 1.0f;
        break;
    case 0:
        animation = "HOVER_INTO_FLY";
        newMode = 1;
        targetSpeed = 3.0f;
        break;
    }
    CCreatureAction_DragonFlight action(Creature, animation);
    if (Creature->SetCurrentAction(&action)) {
        Transitioning = true;
        ModeChangeQueued = false;
        Mode = newMode;
        long time = action.GetTotalTime();
        DeltaSpeed = (targetSpeed - Speed) / time;
        SpeedChangeTicks = time;
    }
}

int main()
{
    int bad = 0;
    CThingCreatureBase dragon; dragon.Accept = true; dragon.LastAnim = 0;
    CTCCreatureFlyInCircles f; f.Creature = &dragon; f.Speed = 1.0f; f.DeltaSpeed = 0; f.SpeedChangeTicks = 0;
    f.Mode = 0; f.Transitioning = false; f.ModeChangeQueued = true;
    f.ChangeMode();
    float expect = (3.0f - 1.0f) / 10;
    if (std::strcmp(dragon.LastAnim, "HOVER_INTO_FLY") || f.Mode != 1 || !f.Transitioning || f.ModeChangeQueued
        || f.DeltaSpeed != expect || f.SpeedChangeTicks != 10 || g_actionDtors != 1 || g_strDtors != 1) bad |= 1;
    f.Speed = 3.0f; g_duration = 4; f.ModeChangeQueued = true;
    f.ChangeMode();
    expect = (1.0f - 3.0f) / 4;
    if (std::strcmp(dragon.LastAnim, "FLY_INTO_HOVER") || f.Mode != 0 || f.DeltaSpeed != expect || f.SpeedChangeTicks != 4 || f.ModeChangeQueued) bad |= 2;
    dragon.Accept = false; f.Transitioning = false;
    f.ChangeMode();                                  // refused: nothing changes, temporaries still destroyed
    if (f.Mode != 0 || f.Transitioning || g_actionDtors != 3 || g_strDtors != 3) bad |= 4;
    f.Mode = 2; int before = g_actions;
    f.ChangeMode();                                  // mode 2: no action at all
    if (g_actions != before || g_strDtors != 4 || f.Mode != 2) bad |= 8;
    f.Mode = 7; dragon.Accept = true; f.Speed = 2.0f; g_duration = 2;
    f.ChangeMode();                                  // unknown mode: empty animation, mode 0, target 0
    if (dragon.LastAnim[0] || f.Mode != 0 || f.DeltaSpeed != (0.0f - 2.0f) / 2) bad |= 16;
    if (bad) { std::printf("007ddcc0_TEST FAIL\n"); return 1; }
    std::printf("007ddcc0_TEST PASS\n");
    return 0;
}
