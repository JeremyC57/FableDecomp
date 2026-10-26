// CEngineMapManager::MapState_SetLoaded(long index) -- fetch the map's info record (id + name) from the world-map provider for `index`, store its id in the map's state and run the map's loaded hook; the record's name is destroyed afterwards
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B401E0, length 67 bytes.
// Callees: virtual slot 1 of the provider at +0x19c of the engine state @ 0x01436E8C (hidden-pointer result, the index), the map's loaded hook @ 0x00BDBFE0
// (on the map object at +0 of the state entry), ~CCharString @ 0x0099EAE0 (on the record's name at +4: the record's inline destructor). Shape: the map
// state table at +0x20 holds pointers to 12-byte entries {map, ?, id}; the 0x20-byte frame holds the returned record (its id read from +4, its name destroyed at +8);
// the chunk label "CEngine::MapState_SetLoaded" names the method; `this` is the map manager (pointer table at +0x20).
class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};

struct SMapInfo {                                  // returned by value (0x20 bytes): a word, the id, the name and 0x14 bytes of other fields
    long          m_kind;                          // +0x00
    long          m_id;                            // +0x04
    CCharString   m_name;                          // +0x08
    unsigned char _rest[0x14];                     // +0x0c
    ~SMapInfo() { }
};

struct CWorldMapProvider {
    virtual void v00();
    virtual SMapInfo GetMapInfo(long index);       // slot 1 (hidden result pointer, then the index)
};

struct SEngineState_01436e8c {
    unsigned char      _pad[0x19c];
    CWorldMapProvider* m_pWorldMaps;               // +0x19c
};
extern SEngineState_01436e8c* g_pEngineState_01436e8c;   // 0x01436E8C

class CEngineMap { public:
    void OnLoaded();                               // 0x00BDBFE0
};

struct SMapState {
    CEngineMap* m_pMap;                            // +0x00
    long        m_a;                               // +0x04
    long        m_infoId;                          // +0x08
};

struct CEngineMapManager {
    unsigned char _pad[0x20];
    SMapState**   m_ppStates;                      // +0x20
    void MapState_SetLoaded(long index);
};

void CEngineMapManager::MapState_SetLoaded(long index)
{
    SMapState* state = m_ppStates[index];
    SMapInfo info = g_pEngineState_01436e8c->m_pWorldMaps->GetMapInfo(index);
    state->m_infoId = info.m_id;
    state->m_pMap->OnLoaded();
}