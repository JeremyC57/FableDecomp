// CWorldMap::PostLoadTopology(long) -- after a map's topology loads: flag its heights changed, seed passability, rebuild edges, tell the engine
// Leaf: ?PostLoadTopology@CWorldMap@@UAEXJ@Z
// Retail: Fable.exe @ 0x004FF440, 75 bytes, `ret 4`. PDB ?PostLoadTopology@CWorldMap@@UAEXJ@Z (primary vtable
// 0x01244AEC slot 7).
// Callees: CMap::HeightAreaChanged(const C2DBoxI*) 0x0081E810 (null = the whole map), CWorldMap::
// InitPassabilityFromThemesIfNecessary(const C2DBoxI&) 0x0063E2E0 (landed), GFGetTime() 0x009E1BC0 (__fastcall, the
// double result is dropped: `fstp st(0)`), CWorldMap::UpdateAllEdges() 0x0063DFD0 (landed),
// CTBaseSingleton<CEngineManager>::Get() 0x00436FB0 (static __fastcall), CEngineManager::SetMapAsLoaded(long)
// 0x00640400 (manifest label "RemoveMap" is a propagation: it walks the engine list calling CIEngine slot +0xB0, the
// third of the AddMap +0xA8 / RemoveMap +0xAC (0x006403D0) / SetMapAsLoaded +0xB0 / SetMapAsUnloaded +0xB4 (0x00640430,
// called from UnloadLevel) quartet).
// Retail CWorldMap (engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport vectors / CArrays and
// the map member are 12 bytes, so a local model is used): +0x20 Maps (CArray<CMapInfo>, PDB +0x24); CMapInfo (0x48
// bytes): +0x00 Area, +0x10 PMap.
// Shape: the map's byte offset (map * 0x48) is computed once into edi (`lea ; shl 3`) and Maps.Start re-read after
// the first call; the CArray operator[] is STLport's `*(begin() + n)` (a plain `Start[n]` swaps the SIB base and index
// of the first load: `[eax+edi+10h]` for retail's `[edi+eax+10h]`); the engine singleton is fetched after the map argument is pushed (`Get()->SetMapAsLoaded(map)`).
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };

class CMap {
public:
    void HeightAreaChanged(const C2DBoxI* area);    // 0x0081E810
};

class CMapInfo {
public:
    C2DBoxI       Area;                         // +0x00
    CMap*         PMap;                         // +0x10 CCountedPointer<CMap>::Data
    void*         PMapInfo;                     // +0x14 CCountedPointer<CMap>::Info
    unsigned char _rest[0x30];                  // +0x18 .. +0x48
};

template <class T> class CArray {
public:
    T* Start;                                   // +0x00
    T* Finish;                                  // +0x04
    T* EndOfStorage;                            // +0x08
    T* begin() { return Start; }
    T& operator[](unsigned long i) { return *(begin() + i); }
};

class CEngineManager {
public:
    void SetMapAsLoaded(long map);              // 0x00640400
};
template <class T> class CTBaseSingleton {
public:
    static T* __fastcall Get();                 // 0x00436FB0 (CEngineManager)
};

double __fastcall GFGetTime();                  // 0x009E1BC0

class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06();
    virtual void PostLoadTopology(long map);    // slot 7
    unsigned char    _pad04[0x1c];
    CArray<CMapInfo> Maps;                      // +0x20 (PDB +0x24)
    void InitPassabilityFromThemesIfNecessary(const C2DBoxI& area);    // 0x0063E2E0
    void UpdateAllEdges();                      // 0x0063DFD0
};

void CWorldMap::PostLoadTopology(long map)
{
    Maps[map].PMap->HeightAreaChanged(0);
    InitPassabilityFromThemesIfNecessary(Maps[map].Area);
    GFGetTime();
    UpdateAllEdges();
    CTBaseSingleton<CEngineManager>::Get()->SetMapAsLoaded(map);
}