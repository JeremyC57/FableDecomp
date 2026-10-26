// CEngineMapManager::MapState_SetUnloaded(long index) -- run the map's unloaded hook when the state entry still has a map, then clear the entry's info id
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B40230, length 33 bytes.
// Callees: the map's unloaded hook @ 0x00BDBFF0 (on the map object at +0 of the state entry). Shape: the entry pointer stays in esi across the call, the
// id store at +8 is an immediate zero. The chunk label "CEngine::MapState_SetUnloaded" names the method; `this` is the map manager (pointer table at +0x20).
class CEngineMap { public:
    void OnUnloaded();                             // 0x00BDBFF0
};

struct SMapState {
    CEngineMap* m_pMap;                            // +0x00
    long        m_a;                               // +0x04
    long        m_infoId;                          // +0x08
};

struct CEngineMapManager {
    unsigned char _pad[0x20];
    SMapState**   m_ppStates;                      // +0x20
    void MapState_SetUnloaded(long index);
};

void CEngineMapManager::MapState_SetUnloaded(long index)
{
    SMapState* state = m_ppStates[index];
    if (state->m_pMap)
        state->m_pMap->OnUnloaded();
    state->m_infoId = 0;
}