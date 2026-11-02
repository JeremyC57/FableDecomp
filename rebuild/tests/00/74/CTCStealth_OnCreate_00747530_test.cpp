// Behaviour test for CTCStealth::OnCreate (0x00747530): the stealth flags are cleared, the four timers set to -1, the
// sneaking speed derived last.
#include <cstdio>
class CTCBase {
public:
    virtual ~CTCBase() { }
    virtual void OnCreate() { }
};
class CTCStealth : public CTCBase {
public:
    virtual void OnCreate();
    int speedCalls; bool flagsClearedBeforeSpeed;
    void InitialiseSneakSpeed()
    {
        ++speedCalls;
        flagsClearedBeforeSpeed = !Viewable && !Smellable && !Hearable && !Sneaking && FrameStartedWalking == -1 && InDangerCoolOffTimer == -1;
        MaxSneakingSpeed = 0.5f;
    }
    void ResetStealthVariables();
    bool Viewable, Smellable, Hearable, Sneaking;
    float MaxSneakingSpeed;
    long FrameStartedWalking, FrameStartedSneakingAndMoving, SneakingAndMovingDeedHandle, RunningAwayFromCombatTimer, InDangerCoolOffTimer;
};
void CTCStealth::ResetStealthVariables()
{
    Viewable = false;
    Smellable = false;
    Hearable = false;
    Sneaking = false;
    FrameStartedWalking = -1;
    FrameStartedSneakingAndMoving = -1;
    RunningAwayFromCombatTimer = -1;
    InDangerCoolOffTimer = -1;
    InitialiseSneakSpeed();
}
void CTCStealth::OnCreate()
{
    ResetStealthVariables();
}
int main()
{
    CTCStealth s;
    s.speedCalls = 0; s.flagsClearedBeforeSpeed = false;
    s.Viewable = s.Smellable = s.Hearable = s.Sneaking = true;
    s.FrameStartedWalking = s.FrameStartedSneakingAndMoving = s.RunningAwayFromCombatTimer = s.InDangerCoolOffTimer = 12;
    s.SneakingAndMovingDeedHandle = 99; s.MaxSneakingSpeed = 0.0f;
    CTCBase* b = &s;
    b->OnCreate();
    if (s.speedCalls != 1 || !s.flagsClearedBeforeSpeed || s.FrameStartedSneakingAndMoving != -1 || s.RunningAwayFromCombatTimer != -1
        || s.SneakingAndMovingDeedHandle != 99 || s.MaxSneakingSpeed < 0.49f) {
        std::printf("00747530_TEST FAIL\n"); return 1;
    }
    std::printf("00747530_TEST PASS\n");
    return 0;
}