#include <cstdio>
struct CRetailMapList { void* head; }; static CRetailMapList* g_list = 0; static bool g_visible = false; static void* g_map = 0;
struct CEngineMap { int x; };
struct SMapState { CEngineMap* m_pMap; void SetMapVisibility(CRetailMapList* l, bool v) { g_map = this; g_list = l; g_visible = v; } };
struct CEngineMapManager { unsigned char _pad[0x20]; SMapState** m_ppStates; unsigned char _pad24[8]; CRetailMapList m_visibleMaps; void SetMapVisibility(long i, bool v) { m_ppStates[i]->SetMapVisibility(&m_visibleMaps, v); } };
int main() { CEngineMap map; SMapState s = { &map }; SMapState* table[2] = { 0, &s }; CEngineMapManager m; m.m_ppStates = table; m.SetMapVisibility(1, true); if (g_map != &s || g_list != &m.m_visibleMaps || !g_visible) { std::printf("00b42390_TEST FAIL\n"); return 1; } std::printf("00b42390_TEST PASS\n"); return 0; }
