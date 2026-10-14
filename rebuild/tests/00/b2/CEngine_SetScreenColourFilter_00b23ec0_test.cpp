#include <cstdio>
struct SScreenColourFilter { float m_values[8]; };
struct SScreenEffectState { unsigned char _pad[0x0c]; unsigned char m_filterLocked; unsigned char _pad0d[3]; SScreenColourFilter m_filter; };
static SScreenEffectState g_state; static SScreenEffectState* g_pScreenEffectState = &g_state;
struct CEngine { void SetScreenColourFilter(const SScreenColourFilter& f) { if (!g_pScreenEffectState->m_filterLocked) g_pScreenEffectState->m_filter = f; } };
int main() { SScreenColourFilter f; for (int i = 0; i < 8; ++i) { f.m_values[i] = (float)i; g_state.m_filter.m_values[i] = -1.0f; } g_state.m_filterLocked = 1; CEngine eng; eng.SetScreenColourFilter(f); if (g_state.m_filter.m_values[7] != -1.0f) { std::printf("00b23ec0_TEST FAIL\n"); return 1; }
    g_state.m_filterLocked = 0; eng.SetScreenColourFilter(f); if (g_state.m_filter.m_values[7] != 7.0f || g_state.m_filter.m_values[0] != 0.0f) { std::printf("00b23ec0_TEST FAIL\n"); return 1; } std::printf("00b23ec0_TEST PASS\n"); return 0; }