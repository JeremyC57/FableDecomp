#include <cstdio>
// Behaviour of CTCHeroSpecialMovement::IsAvailableForSprint (0x007E0700): never while sprinting is disabled; a running
// sprint continues while frames remain; a new sprint needs more frames than fps * recharge seconds.
long g_FramesPerSecond = 15;
bool g_SprintEnabled = true;
struct CHeroSpecialMovementDef { float MinSprintRechargeSeconds; };
struct CTCHeroSpecialMovement_Retail {
    bool Sprinting;
    float SprintFramesRemaining;
    CHeroSpecialMovementDef* PHeroSpecialMovementDef;
    bool IsAvailableForSprint() const
    {
        float minimumFrames = g_FramesPerSecond * PHeroSpecialMovementDef->MinSprintRechargeSeconds;
        if (!g_SprintEnabled)
            return false;
        if (Sprinting && SprintFramesRemaining > 0.0f)
            return true;
        if (SprintFramesRemaining > minimumFrames)
            return true;
        return false;
    }
};
int main()
{
    CHeroSpecialMovementDef def; def.MinSprintRechargeSeconds = 2.0f;   // 30 frames
    CTCHeroSpecialMovement_Retail m; m.PHeroSpecialMovementDef = &def;
    m.Sprinting = true; m.SprintFramesRemaining = 5.0f;
    if (!m.IsAvailableForSprint()) { std::printf("007e0700_TEST FAIL\n"); return 1; }
    m.Sprinting = false;
    if (m.IsAvailableForSprint()) { std::printf("007e0700_TEST FAIL\n"); return 1; }
    m.SprintFramesRemaining = 31.0f;
    if (!m.IsAvailableForSprint()) { std::printf("007e0700_TEST FAIL\n"); return 1; }
    m.SprintFramesRemaining = 30.0f;
    if (m.IsAvailableForSprint()) { std::printf("007e0700_TEST FAIL\n"); return 1; }
    m.Sprinting = true; m.SprintFramesRemaining = 0.0f;
    if (m.IsAvailableForSprint()) { std::printf("007e0700_TEST FAIL\n"); return 1; }
    g_SprintEnabled = false; m.SprintFramesRemaining = 100.0f;
    if (m.IsAvailableForSprint()) { std::printf("007e0700_TEST FAIL\n"); return 1; }
    std::printf("007e0700_TEST PASS\n");
    return 0;
}
