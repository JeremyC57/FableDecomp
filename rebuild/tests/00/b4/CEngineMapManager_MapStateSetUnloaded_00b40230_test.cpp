#include <cstdio>
static int g_unloaded = 0;
struct CEngineMap { void OnUnloaded() { ++g_unloaded; } };
struct SMapState { CEngineMap* m_pMap; long m_a; long m_infoId; };
struct CEngineMapManager { unsigned char _pad[0x20]; SMapState** m_ppStates; void MapState_SetUnloaded(long i) { SMapState* s = m_ppStates[i]; if (s->m_pMap) s->m_pMap->OnUnloaded(); s->m_infoId = 0; } };
int main() { CEngineMap map; SMapState a = { &map, 0, 5 }, b = { 0, 0, 6 }; SMapState* table[2] = { &a, &b }; CEngineMapManager m; m.m_ppStates = table;
    m.MapState_SetUnloaded(0); if (g_unloaded != 1 || a.m_infoId != 0) { std::printf("00b40230_TEST FAIL\n"); return 1; } m.MapState_SetUnloaded(1); if (g_unloaded != 1 || b.m_infoId != 0) { std::printf("00b40230_TEST FAIL\n"); return 1; } std::printf("00b40230_TEST PASS\n"); return 0; }