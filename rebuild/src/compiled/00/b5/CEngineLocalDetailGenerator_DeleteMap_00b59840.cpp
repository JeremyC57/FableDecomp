// CEngineLocalDetailGenerator::DeleteMap(CEngineMap* map) -- delete the map's local-detail cache map (its out-of-line destructor, then operator delete) and clear the pointer; `this` is unused
// Leaf: ?DeleteMap@CEngineLocalDetailGenerator@@QAEXPAVCEngineMap@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B59840, 41 bytes, thiscall, `ret 4`.
// Identity (proven, chunk c668): Ego_r + FableWin ?DeleteMap@CEngineLocalDetailGenerator@@QAEXPAVCEngineMap@@@Z; called by
//   ~CEngineMap 0x00B42A90 and CEngineWorldMap::DeleteMap 0x00B42B10 with ecx = [0x01436E88] and the map.
// Callees: CLocalDetailCacheMap::~CLocalDetailCacheMap 0x00BDED60 (Ego_r/FableWin ??1CLocalDetailCacheMap@@QAE@XZ, non-virtual;
//   chunk c671), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CEngineMap LocalDetailCacheMap +0x04.
// Shape: `if (p) { delete p; p = 0; }` keeps the clear inside the branch (before the join); the map stays in edi, the object in esi.
// Naming wave 16: landed as the descriptive CEngineResourceManager::DestroyMapStrip_00b59840(SMapEntry&) over a
//   N_00bded60::CPatchTesselationEdgeStrip stand-in; respelled on the PDB member (name-only: the bytes are unchanged).
void __cdecl operator delete(void* block);         // 0x00BFE9BC

class CLocalDetailCacheMap {
public:
    ~CLocalDetailCacheMap();                       // 0x00BDED60
};
class CEngineLandscapeMap;
class CEngineMap {
public:
    CEngineLandscapeMap*  LandscapeMap;            // +0x00
    CLocalDetailCacheMap* LocalDetailCacheMap;     // +0x04
};

class CEngineLocalDetailGenerator {
public:
    void DeleteMap(CEngineMap* map);
};

void CEngineLocalDetailGenerator::DeleteMap(CEngineMap* map)
{
    if (map->LocalDetailCacheMap) {
        delete map->LocalDetailCacheMap;
        map->LocalDetailCacheMap = 0;
    }
}
