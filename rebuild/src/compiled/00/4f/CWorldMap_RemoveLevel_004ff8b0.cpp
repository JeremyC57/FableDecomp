// CWorldMap::RemoveLevel(long) -- unload a map if loaded, clear its placement cells, remove it from the engine and mark the slot unused
// Leaf: ?RemoveLevel@CWorldMap@@UAEXJ@Z
// Retail: Fable.exe @ 0x004FF8B0, 71 bytes, `ret 4`. PDB ?RemoveLevel@CWorldMap@@UAEXJ@Z (primary vtable 0x01244AEC
// slot 5).
// Callees: virtual slot 10 IsMapLoaded(long) const 0x004FC050 (+0x28), CWorldMap::UnloadLevel(long) 0x004FEDE0,
// CWorldMap::ChangeMapPlacementValuesTo(long, long) 0x004FCAC0 (landed), CTBaseSingleton<CEngineManager>::Get()
// 0x00436FB0 (static __fastcall), CEngineManager::RemoveMap(long) 0x006403D0 (CIEngine slot +0xAC per engine).
// Retail CWorldMap (engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport vectors / CArrays and
// the map member are 12 bytes, so a local model is used): +0x20 Maps (CArray<CMapInfo>, PDB +0x24); CMapInfo (0x48
// bytes): +0x24 Used.
// Shape: the map record is bound to a reference up front (ebx, computed before the first call) and its Used byte is
// cleared last, after the engine call; placement values are reset to map 0.
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class CMap;

class CMapInfo {
public:
    C2DBoxI       Area;                         // +0x00
    CMap*         PMap;                         // +0x10 CCountedPointer<CMap>::Data
    void*         PMapInfo;                     // +0x14 CCountedPointer<CMap>::Info
    void*         LevelName;                    // +0x18
    void*         LevelScriptName;              // +0x1C
    unsigned long MapUID;                       // +0x20
    bool          Used;                         // +0x24
    bool          Placed;                       // +0x25
    bool          Activated;                    // +0x26
    bool          Loading;                      // +0x27
    unsigned char _rest[0x20];                  // +0x28 .. +0x48
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
    void RemoveMap(long map);                   // 0x006403D0
};
template <class T> class CTBaseSingleton {
public:
    static T* __fastcall Get();                 // 0x00436FB0 (CEngineManager)
};

class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void RemoveLevel(long map);         // slot 5
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool IsMapLoaded(long map) const;   // slot 10 (+0x28) 0x004FC050
    unsigned char    _pad04[0x1c];
    CArray<CMapInfo> Maps;                      // +0x20 (PDB +0x24)
    void UnloadLevel(long map);                 // 0x004FEDE0
    void ChangeMapPlacementValuesTo(long from, long to);   // 0x004FCAC0
};

void CWorldMap::RemoveLevel(long map)
{
    CMapInfo& info = Maps[map];
    if (IsMapLoaded(map))
        UnloadLevel(map);
    ChangeMapPlacementValuesTo(map, 0);
    CTBaseSingleton<CEngineManager>::Get()->RemoveMap(map);
    info.Used = false;
}