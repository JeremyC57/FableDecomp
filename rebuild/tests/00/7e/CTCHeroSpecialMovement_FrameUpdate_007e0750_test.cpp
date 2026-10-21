#include <cstdio>
// Behaviour test for CTCHeroSpecialMovement::FrameUpdate (0x007E0750): same body as src/, with the player, input
// queue and availability predicates stubbed.
struct CGamePlayerInterface {
    bool runHeld, sprintHeld; long lastPlayer;
    bool IsGameActionEventInQueue(long action, long buttonEventType, long playerNumber)
    {
        lastPlayer = playerNumber;
        if (buttonEventType != 2) return false;
        return action == 0x1f ? runHeld : action == 0x1e ? sprintHeld : false;
    }
};
struct CPlayer {
    CGamePlayerInterface* m_pInterface;
    bool m_controlsLocked;
    long GetPlayerNumber() const { return 3; }
};
struct CThing {
    CPlayer* player; bool canRun, canSprint;
    CPlayer* GetPlayer() { return player; }
    const CPlayer* GetPlayer() const { return player; }
};
bool IsAvailableForHeroRun(CThing* thing) { return thing->canRun; }
bool IsAvailableForHeroSprint(CThing* thing) { return thing->canSprint; }
struct CHeroSpecialMovementDef {
    float TotalTimeToSprintSeconds;
    float MinimumNumberOfSecondsLeftToBeAllowedToSprint;
    float TotalTimeToFullyRestoreSprintBarSeconds;
    float NoSecondsToRunBeforeSprintStarts;
};
long g_FramesPerSecond = 10;
bool g_bHeroRunOption = true;
inline void ClampLow(float& value, float lo) { if (value < lo) value = lo; }
inline void Raise(float& value, float hi) { if (hi > value) value = hi; }
struct CTCHeroSpecialMovement {
    CThing* ParentThing;
    bool Sprinting;
    bool Running;
    float SprintFramesRemaining;
    long RunTimer;
    const CHeroSpecialMovementDef* PHeroSpecialMovementDef;
    const CThing& GetThing() const { return *ParentThing; }
    void SetSprinting(bool sprinting)
    {
        if (sprinting) {
            if (!Sprinting && IsAvailableForHeroSprint(ParentThing))
                Sprinting = true;
        } else if (Sprinting)
            Sprinting = false;
    }
    void SetRunning(bool running)
    {
        if (running) {
            if (!Running && IsAvailableForHeroRun(ParentThing))
                Running = true;
        } else if (Running)
            Running = false;
    }
    void FrameUpdate();
};
void CTCHeroSpecialMovement::FrameUpdate()
{
    CGamePlayerInterface* input = ParentThing->GetPlayer()->m_pInterface;
    if (g_bHeroRunOption) {
        SetRunning(input->IsGameActionEventInQueue(0x1f, 2, GetThing().GetPlayer()->GetPlayerNumber())
                   && !GetThing().GetPlayer()->m_controlsLocked && IsAvailableForHeroRun(ParentThing));
        if (Running) {
            if (!Sprinting) {
                if (++RunTimer > (long)(g_FramesPerSecond * PHeroSpecialMovementDef->NoSecondsToRunBeforeSprintStarts))
                    SetSprinting(true);
            } else
                RunTimer = 0;
        } else
            SetSprinting(false);
    } else {
        SetSprinting(input->IsGameActionEventInQueue(0x1e, 2, GetThing().GetPlayer()->GetPlayerNumber())
                     && IsAvailableForHeroSprint(ParentThing));
    }
    if (Sprinting) {
        SprintFramesRemaining -= 1.0f;
        ClampLow(SprintFramesRemaining, 0.0f);
        if (SprintFramesRemaining == 0.0f || !IsAvailableForHeroSprint(ParentThing))
            SetSprinting(false);
        if (Sprinting)
            return;
    }
    SprintFramesRemaining += PHeroSpecialMovementDef->TotalTimeToFullyRestoreSprintBarSeconds
                             / PHeroSpecialMovementDef->TotalTimeToSprintSeconds;
    Raise(SprintFramesRemaining, g_FramesPerSecond * PHeroSpecialMovementDef->TotalTimeToSprintSeconds);
}

int main()
{
    int fails = 0;
    CGamePlayerInterface input = { false, false, -1 };
    CPlayer player = { &input, false };
    CThing hero = { &player, true, true };
    CHeroSpecialMovementDef def = { 2.0f, 0.5f, 4.0f, 0.3f };
    CTCHeroSpecialMovement tc = { &hero, false, false, 5.0f, 0, &def };
    // run option on: run held -> running, timer counts to 0.3 * 10 = 3 frames, then sprint starts
    input.runHeld = true;
    tc.FrameUpdate();
    if (!tc.Running || tc.Sprinting || tc.RunTimer != 1 || input.lastPlayer != 3) ++fails;
    if (tc.SprintFramesRemaining != 20.0f) ++fails;          // 5 + 4/2 < 20 -> raised to FPS * total
    tc.FrameUpdate(); tc.FrameUpdate();
    if (tc.Sprinting || tc.RunTimer != 3) ++fails;
    tc.FrameUpdate();                                        // 4 > 3: sprint
    if (!tc.Sprinting || tc.SprintFramesRemaining != 23.0f) ++fails;   // 20 + 2 + 2 (refill is a max, not a cap) - 1
    tc.FrameUpdate();                                        // sprinting: timer reset, bar drains
    if (tc.RunTimer != 0 || !tc.Sprinting || tc.SprintFramesRemaining != 22.0f) ++fails;
    tc.SprintFramesRemaining = 0.5f;
    tc.FrameUpdate();                                        // drains below 0 -> clamped, sprint ends, refills
    if (tc.Sprinting || tc.SprintFramesRemaining != 20.0f) ++fails;
    // controls locked: running stops, sprint cleared
    player.m_controlsLocked = true; tc.Sprinting = true; tc.SprintFramesRemaining = 30.0f;
    tc.FrameUpdate();
    if (tc.Running || tc.Sprinting || tc.SprintFramesRemaining != 32.0f) ++fails;   // 30 + 2 > 20: kept
    player.m_controlsLocked = false;
    // run option off: sprint button sprints directly when available
    g_bHeroRunOption = false; input.runHeld = false; input.sprintHeld = true; tc.SprintFramesRemaining = 10.0f;
    tc.FrameUpdate();
    if (!tc.Sprinting || tc.SprintFramesRemaining != 9.0f) ++fails;
    hero.canSprint = false;
    tc.FrameUpdate();                                        // not available any more: sprint ends
    if (tc.Sprinting) ++fails;
    if (fails) { std::printf("007e0750_TEST FAIL\n"); return 1; }
    std::printf("007e0750_TEST PASS\n");
    return 0;
}