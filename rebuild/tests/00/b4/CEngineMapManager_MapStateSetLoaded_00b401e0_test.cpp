#include <cstdio>
static int g_loaded = 0, g_dtors = 0; static long g_index = -1;
struct CCharString { void* m_pStringData; ~CCharString() { ++g_dtors; } };
struct SMapInfo { long m_kind; long m_id; CCharString m_name; unsigned char _rest[0x14]; };
struct CWorldMapProvider { virtual void v00() { } virtual SMapInfo GetMapInfo(long index) { g_index = index; SMapInfo i; i.m_id = 100 + index; i.m_name.m_pStringData = 0; return i; } };
struct SEngineState { unsigned char _pad[0x19c]; CWorldMapProvider* m_pWorldMaps; }; static CWorldMapProvider g_prov; static SEngineState g_state; static SEngineState* g_pEngineState = &g_state;
struct CEngineMap { void OnLoaded() { ++g_loaded; } };
struct SMapState { CEngineMap* m_pMap; long m_a; long m_infoId; };
struct CEngineMapManager { unsigned char _pad[0x20]; SMapState** m_ppStates;
    void MapState_SetLoaded(long index) { SMapState* state = m_ppStates[index]; SMapInfo info = g_pEngineState->m_pWorldMaps->GetMapInfo(index); state->m_infoId = info.m_id; state->m_pMap->OnLoaded(); } };
int main() { g_state.m_pWorldMaps = &g_prov; CEngineMap map; SMapState st = { &map, 0, 0 }; SMapState* table[3] = { 0, 0, &st }; CEngineMapManager m; m.m_ppStates = table; m.MapState_SetLoaded(2);
    if (g_index != 2 || st.m_infoId != 102 || g_loaded != 1 || g_dtors < 1) { std::printf("00b401e0_TEST FAIL\n"); return 1; } std::printf("00b401e0_TEST PASS\n"); return 0; }