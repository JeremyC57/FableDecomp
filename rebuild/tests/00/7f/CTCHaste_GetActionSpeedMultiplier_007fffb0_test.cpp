#include <cstdio>

// Model of CTCHaste::GetActionSpeedMultiplier @ 0x007FFFB0.
static float g_ActionSpeedMultiplierOverride = -1.0f;
class CTCHaste { public:
    float Multiplier; bool ToTerminate; long PauseFrames;
    float GetActionSpeedMultiplier() const
    {
        if (PauseFrames >= 0) return 0.0f;
        if (g_ActionSpeedMultiplierOverride > 0.0f) return g_ActionSpeedMultiplierOverride;
        return Multiplier;
    }
};

int main()
{
    CTCHaste h; h.Multiplier = 1.5f; h.ToTerminate = false; h.PauseFrames = -1;
    if (h.GetActionSpeedMultiplier() != 1.5f) { std::printf("007fffb0_TEST FAIL\n"); return 1; }
    g_ActionSpeedMultiplierOverride = 3.0f;
    if (h.GetActionSpeedMultiplier() != 3.0f) { std::printf("007fffb0_TEST FAIL\n"); return 1; }
    h.PauseFrames = 0;
    if (h.GetActionSpeedMultiplier() != 0.0f) { std::printf("007fffb0_TEST FAIL\n"); return 1; }
    g_ActionSpeedMultiplierOverride = 0.0f; h.PauseFrames = -1;
    if (h.GetActionSpeedMultiplier() != 1.5f) { std::printf("007fffb0_TEST FAIL\n"); return 1; }
    std::printf("007fffb0_TEST PASS\n");
    return 0;
}