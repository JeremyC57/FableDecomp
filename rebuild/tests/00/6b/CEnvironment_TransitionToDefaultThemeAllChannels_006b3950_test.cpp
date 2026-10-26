#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

static int g_calls = 0; static void* g_entries[8]; static float g_duration = 0;
struct CEnvironmentThemePriorityStackEntry { unsigned char _body[0x30]; void TransitionToDefaultTheme(float duration) { g_entries[g_calls++] = this; g_duration = duration; } };
struct CEnvironmentThemePriorityStack { CEnvironmentThemePriorityStackEntry* m_pBegin; CEnvironmentThemePriorityStackEntry* m_pEnd; CEnvironmentThemePriorityStackEntry* m_pCapacity; };
class CEnvironment { public: unsigned char _pad0[0x3e]; bool ForcedEnvironmentTheme; unsigned char _pad1[0x48 - 0x3f]; CEnvironmentThemePriorityStack* PriorityStackChannels;
    void TransitionToDefaultThemeAllChannels(long priority, float duration) {
        if (ForcedEnvironmentTheme) return;
        if (priority < 1 || priority >= 5) return;
        for (long channel = 0; channel < 6; ++channel) PriorityStackChannels[channel].m_pBegin[priority].TransitionToDefaultTheme(duration); } };
int main()
{
    CEnvironment e; std::memset(&e, 0, sizeof(e)); CEnvironmentThemePriorityStackEntry ents[6][5]; CEnvironmentThemePriorityStack ch[6];
    for (int i = 0; i < 6; ++i) ch[i].m_pBegin = ents[i];
    e.PriorityStackChannels = ch; e.ForcedEnvironmentTheme = true;
    e.TransitionToDefaultThemeAllChannels(2, 0.5f); bool ok = g_calls == 0;
    e.ForcedEnvironmentTheme = false; e.TransitionToDefaultThemeAllChannels(0, 0.5f); e.TransitionToDefaultThemeAllChannels(5, 0.5f); ok = ok && g_calls == 0;
    e.TransitionToDefaultThemeAllChannels(3, 0.5f); ok = ok && g_calls == 6 && g_duration == 0.5f;
    for (int i = 0; i < 6; ++i) ok = ok && g_entries[i] == &ents[i][3];
    if (!ok) { std::printf("006b3950_TEST FAIL\n"); return 1; }
    std::printf("006b3950_TEST PASS\n"); return 0;
}