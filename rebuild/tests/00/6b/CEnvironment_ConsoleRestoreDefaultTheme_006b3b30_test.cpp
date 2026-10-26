// Behaviour test for 006b3b30: CEnvironment::ConsoleRestoreDefaultTheme reads (channel, priority, duration) from the console args and transitions even while forced (model copy).
#include <cstdio>
static int g_calls = 0; static void* g_last = 0; static float g_dur = 0.0f; static bool g_forced_at_call = true;
class CEnvironment;
static CEnvironment* g_env = 0;
struct CEntry {
    unsigned char pad[0x30];
    void TransitionToDefaultTheme(float duration);
};
template <class T> class CArray { public: T* first; T* last; T* end; T& operator[](long i) { return first[i]; } };
struct CStack { CArray<CEntry> Ents; };
class CGenericVar { public: void* vt; long Type; void* ValString; long ValSLONG; unsigned long ValULONG; bool ValBool; float ValFloat; };
struct CVarVector { CGenericVar* first; CGenericVar* last; CGenericVar* end; const CGenericVar& operator[](unsigned int n) const { return first[n]; } };
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
    static void ConsoleRestoreDefaultTheme(CEnvironment& env, const CVarVector& args)
    {
        env.ForcedEnvironmentTheme = false;
        env.TransitionToDefaultTheme(args[0].ValSLONG, args[1].ValSLONG, args[2].ValFloat);
        env.ForcedEnvironmentTheme = true;
    }
};
void CEntry::TransitionToDefaultTheme(float duration) { ++g_calls; g_last = this; g_dur = duration; g_forced_at_call = g_env->ForcedEnvironmentTheme; }
int main()
{
    static CEntry entries[6][5]; static CStack stacks[6];
    for (int c = 0; c < 6; ++c) { stacks[c].Ents.first = entries[c]; stacks[c].Ents.last = stacks[c].Ents.end = entries[c] + 5; }
    CEnvironment env; env.ForcedEnvironmentTheme = true; g_env = &env;
    env.PriorityStackChannels.first = stacks; env.PriorityStackChannels.last = env.PriorityStackChannels.end = stacks + 6;
    CGenericVar v[3] = {};
    v[0].ValSLONG = 4; v[1].ValSLONG = 2; v[2].ValFloat = 3.25f;
    CVarVector args = { v, v + 3, v + 3 };
    int fail = 0;
    CEnvironment::ConsoleRestoreDefaultTheme(env, args);
    if (g_calls != 1 || g_last != &entries[4][2] || g_dur != 3.25f || g_forced_at_call) fail = 1;
    if (!env.ForcedEnvironmentTheme) fail = 1;          // the force flag is set again afterwards
    v[1].ValSLONG = 7;                                    // out-of-range priority: no transition, flag still set
    env.ForcedEnvironmentTheme = false;
    CEnvironment::ConsoleRestoreDefaultTheme(env, args);
    if (g_calls != 1 || !env.ForcedEnvironmentTheme) fail = 1;
    if (fail) { std::printf("006b3b30_TEST FAIL\n"); return 1; }
    std::printf("006b3b30_TEST PASS\n");
    return 0;
}