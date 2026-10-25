// CWorldMap::IsMapLoaded(long) const -- is the map slot used, activated and holding its CMap?
// Leaf: ?IsMapLoaded@CWorldMap@@UBE_NJ@Z
// Retail: Fable.exe @ 0x004FC050, 47 bytes, `ret 4`. PDB ?IsMapLoaded@CWorldMap@@UBE_NJ@Z (primary vtable 0x01244AEC
// slot 10, +0x28).
// Retail CWorldMap (engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport vectors / CArrays and
// the map member are 12 bytes, so everything after +0x14 sits lower): +0x20 Maps (CArray<CMapInfo>, PDB +0x24).
// CMapInfo (0x48 bytes, PDB = retail): +0x00 Area, +0x10 PMap (CCountedPointer<CMap>), +0x24 Used, +0x25 Placed,
// +0x26 Activated.
// Shape: one `&&` chain returned as a bool gives the full-register `mov eax,1` / `xor eax,eax` exits.
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
    const T& operator[](unsigned long i) const { return Start[i]; }
};

class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool IsMapLoaded(long map) const;   // slot 10 (+0x28)
    unsigned char    _pad04[0x1c];
    CArray<CMapInfo> Maps;                      // +0x20 (PDB +0x24)
};

bool CWorldMap::IsMapLoaded(long map) const
{
    const CMapInfo& info = Maps[map];
    return info.Used && info.Activated && info.PMap;
}