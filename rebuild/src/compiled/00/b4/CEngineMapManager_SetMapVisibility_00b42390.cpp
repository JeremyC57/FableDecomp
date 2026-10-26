// CEngineMapManager::SetMapVisibility(long index, bool visible) -- forward to the state entry at `index`: SetMapVisibility(&m_visibleMaps, visible) on the entry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B42390, length 25 bytes.
// Callees: CEngineMap::SetMapVisibility(list<CEngineMap*>*, bool) @ 0x00B41DB0 (on the state entry `m_ppStates[index]` itself; the visible-map
// list member at +0x2c is passed by address). Shape: the bool is pushed first (right to left), the list address second; `ret 8`.
class CEngineMap;

struct CRetailMapList {                            // VC6 Dinkumware list<CEngineMap*>: one sentinel pointer
    void* head;
};

struct SMapState {                                 // the state entry (its map pointer first); the callee runs on the ENTRY
    CEngineMap* m_pMap;                            // +0x00
    void SetMapVisibility(CRetailMapList* visibleMaps, bool visible);   // 0x00B41DB0 (manifest "CEngineMap::SetMapVisibility")
};

struct CEngineMapManager {
    unsigned char  _pad[0x20];
    SMapState**    m_ppStates;                     // +0x20
    unsigned char  _pad24[8];
    CRetailMapList m_visibleMaps;                  // +0x2c
    void SetMapVisibility(long index, bool visible);
};

void CEngineMapManager::SetMapVisibility(long index, bool visible)
{
    m_ppStates[index]->SetMapVisibility(&m_visibleMaps, visible);
}