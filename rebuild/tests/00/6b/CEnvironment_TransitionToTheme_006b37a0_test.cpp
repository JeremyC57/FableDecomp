#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

static int g_calls = 0; static long g_theme = -5; static float g_duration = 0; static void* g_entry = 0; static long g_classIndex = 0;
class CDefinitionManager { public: long GetDefClassIndexFromGlobalIndex(long globalIndex) { return g_classIndex; } };
struct CEnvironmentThemePriorityStackEntry { long pad; void StartThemeTransition(long theme, float duration) { ++g_calls; g_theme = theme; g_duration = duration; g_entry = this; } };
struct CEnvironmentThemePriorityStack { CEnvironmentThemePriorityStackEntry* m_pBegin; CEnvironmentThemePriorityStackEntry* m_pEnd; CEnvironmentThemePriorityStackEntry* m_pCapacity; };
struct SThemeRecord { long m_a, m_b, m_c; };
class CEnvironment { public: void* __vftable; CDefinitionManager* DefinitionManager; unsigned char _pad0[0x3e - 0x8]; bool ForcedEnvironmentTheme; unsigned char _pad1[0x48 - 0x3f]; CEnvironmentThemePriorityStack* PriorityStackChannels; unsigned char _pad2[0x1954 - 0x4c]; SThemeRecord* m_pThemesBegin; SThemeRecord* m_pThemesEnd;
    void TransitionToTheme(long globalThemeIndex, float duration) {
        if (ForcedEnvironmentTheme) return;
        long theme = DefinitionManager->GetDefClassIndexFromGlobalIndex(globalThemeIndex) - 1;
        if (theme < 0) return;
        if (theme >= m_pThemesEnd - m_pThemesBegin) return;
        PriorityStackChannels[0].m_pBegin[0].StartThemeTransition(theme, duration); } };
int main()
{
    CEnvironment* e = (CEnvironment*)std::malloc(sizeof(CEnvironment)); std::memset(e, 0, sizeof(*e));
    CDefinitionManager dm; e->DefinitionManager = &dm; CEnvironmentThemePriorityStackEntry ents[2]; CEnvironmentThemePriorityStack ch[2]; ch[0].m_pBegin = ents; ch[1].m_pBegin = ents + 1; e->PriorityStackChannels = ch;
    SThemeRecord themes[3]; e->m_pThemesBegin = themes; e->m_pThemesEnd = themes + 3;
    e->ForcedEnvironmentTheme = true; g_classIndex = 2; e->TransitionToTheme(10, 1.5f); bool ok = g_calls == 0;
    e->ForcedEnvironmentTheme = false; g_classIndex = 0; e->TransitionToTheme(10, 1.5f); ok = ok && g_calls == 0;
    g_classIndex = 4; e->TransitionToTheme(10, 1.5f); ok = ok && g_calls == 0;
    g_classIndex = 3; e->TransitionToTheme(10, 1.5f); ok = ok && g_calls == 1 && g_theme == 2 && g_duration == 1.5f && g_entry == ents;
    std::free(e);
    if (!ok) { std::printf("006b37a0_TEST FAIL\n"); return 1; }
    std::printf("006b37a0_TEST PASS\n"); return 0;
}