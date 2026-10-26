// Behaviour test for 006b3880: CEnvironment::TransitionToDefaultTheme range checks and the (channel, priority) entry it reaches (model copy).
#include <cstdio>
static int g_calls = 0; static void* g_last = 0; static float g_dur = 0.0f;
struct CEntry {
    unsigned char pad[0x30];
    void TransitionToDefaultTheme(float duration) { ++g_calls; g_last = this; g_dur = duration; }
};
template <class T> class CArray { public: T* first; T* last; T* end; T& operator[](long i) { return first[i]; } };
struct CStack { CArray<CEntry> Ents; };
class CEnvironment { public:
    bool ForcedEnvironmentTheme;
    CArray<CStack> PriorityStackChannels;
    void TransitionToDefaultTheme(long channel, long priority, float duration)
    {
        if (ForcedEnvironmentTheme) return;
        if (channel < 0 || channel >= 6) return;
        if (priority < 1 || priority >= 5) return;
        PriorityStackChannels[channel].Ents[priority].TransitionToDefaultTheme(duration);
    }
};
int main()
{
    static CEntry entries[6][5]; static CStack stacks[6];
    for (int c = 0; c < 6; ++c) { stacks[c].Ents.first = entries[c]; stacks[c].Ents.last = stacks[c].Ents.end = entries[c] + 5; }
    CEnvironment env; env.ForcedEnvironmentTheme = false;
    env.PriorityStackChannels.first = stacks; env.PriorityStackChannels.last = env.PriorityStackChannels.end = stacks + 6;
    int fail = 0;
    env.TransitionToDefaultTheme(2, 3, 1.5f);
    if (g_calls != 1 || g_last != &entries[2][3] || g_dur != 1.5f) fail = 1;
    env.TransitionToDefaultTheme(5, 1, 2.0f);
    if (g_calls != 2 || g_last != &entries[5][1]) fail = 1;
    env.TransitionToDefaultTheme(-1, 2, 1.0f);   // channel out of range
    env.TransitionToDefaultTheme(6, 2, 1.0f);
    env.TransitionToDefaultTheme(0, 0, 1.0f);    // priority 0 (the base layer) is never transitioned
    env.TransitionToDefaultTheme(0, 5, 1.0f);
    if (g_calls != 2) fail = 1;
    env.ForcedEnvironmentTheme = true;
    env.TransitionToDefaultTheme(1, 1, 1.0f);    // a forced theme blocks transitions
    if (g_calls != 2) fail = 1;
    if (fail) { std::printf("006b3880_TEST FAIL\n"); return 1; }
    std::printf("006b3880_TEST PASS\n");
    return 0;
}