// Behaviour test for CEnvironment::RestoreDefaultWeightAllChannels (0x006B3750): for priorities 1..4 and an unforced theme, sets the default weight on that entry of all six channels.
#include <cstdio>
struct Entry { float Weight; bool Flag; int Calls; void SetDefaultThemeWeight(float w, bool f) { Weight = w; Flag = f; ++Calls; } };
struct Stack { Entry* Begin; Entry* End; Entry* Cap; };
class CEnvironment { public:
    bool ForcedEnvironmentTheme;
    Stack* PriorityStackChannels;
    void RestoreDefaultWeightAllChannels(int priority, float weight)
    {
        if (ForcedEnvironmentTheme) return;
        if (priority < 1 || priority >= 5) return;
        for (int channel = 0; channel < 6; ++channel)
            PriorityStackChannels[channel].Begin[priority].SetDefaultThemeWeight(weight, true);
    }
};
int main()
{
    Entry entries[6][5]; Stack stacks[6];
    for (int c = 0; c < 6; ++c) { stacks[c].Begin = entries[c]; for (int p = 0; p < 5; ++p) { entries[c][p].Calls = 0; entries[c][p].Weight = 0.0f; entries[c][p].Flag = false; } }
    CEnvironment e; e.ForcedEnvironmentTheme = false; e.PriorityStackChannels = stacks;
    e.RestoreDefaultWeightAllChannels(2, 0.75f);
    e.RestoreDefaultWeightAllChannels(0, 1.0f);
    e.RestoreDefaultWeightAllChannels(5, 1.0f);
    for (int c = 0; c < 6; ++c)
        for (int p = 0; p < 5; ++p)
            if (entries[c][p].Calls != (p == 2 ? 1 : 0) || (p == 2 && (entries[c][p].Weight != 0.75f || !entries[c][p].Flag))) { std::printf("006b3750_TEST FAIL\n"); return 1; }
    e.ForcedEnvironmentTheme = true;
    e.RestoreDefaultWeightAllChannels(2, 0.1f);
    if (entries[0][2].Calls != 1) { std::printf("006b3750_TEST FAIL\n"); return 1; }
    std::printf("006b3750_TEST PASS\n");
    return 0;
}