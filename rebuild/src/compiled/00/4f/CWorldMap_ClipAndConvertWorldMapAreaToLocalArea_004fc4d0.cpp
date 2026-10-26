// CWorldMap::ClipAndConvertWorldMapAreaToLocalArea(long, const C2DBoxI&) const -- clip a world-map box to one map's area and express it relative to that map's corner
// Leaf: ClipAndConvertWorldMapAreaToLocalArea
// Retail: Fable.exe @ 0x004FC4D0, 142 bytes, `ret 0Ch` (hidden C2DBoxI result). PDB
// ?ClipAndConvertWorldMapAreaToLocalArea@CWorldMap@@QBE?AVC2DBoxI@@JABV2@@Z
// Callee: C2DBoxI::ClipTo(const C2DBoxI&) 0x0049C810.
// Retail CWorldMap (the engine header engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport
// vectors / CArrays are 12 bytes and the map member is 12 bytes, so everything after +0x14 sits lower):
//   +0x20 Maps (CArray<CMapInfo>: start +0x20, finish +0x24; PDB +0x24), +0x2C Regions (vector<CRegion>, 0x58-byte
//   elements; PDB +0x34), +0x38 RegionTime, +0x44 PRegionGraph, +0x4C SoundThemesDisabled, +0x50 Granularity,
//   +0x54 GranularityShift (PDB +0x64), +0x58 MapPlacementPitch (PDB +0x68), +0x5C Area {TLX, TLY, BRX, BRY}
//   (PDB +0x6C), +0x6C MapPlacement (vector<unsigned short>, PDB +0x7C), +0x9C LoadedRegion (PDB +0xBC).
// CMapInfo (0x48 bytes, PDB = retail): +0x00 Area, +0x10 PMap, +0x18 LevelName, +0x1C LevelScriptName,
// +0x20 MapUID, +0x24 Used ... +0x40 IsSea.
// Shape: the box and a copy of the map's area are frame locals (memberwise copies through registers); the result is
// the clipped box shifted in place by the map corner (TLX, BRX, then TLY, BRY) and returned through the copy ctor.
class C2DBoxI { public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
    C2DBoxI(const C2DBoxI& o) : TLX(o.TLX), TLY(o.TLY), BRX(o.BRX), BRY(o.BRY) { }
    void ClipTo(const C2DBoxI& other);          // 0x0049C810
};

#pragma auto_inline(off)
// The real 0x0049C810 body (landed as rebuild/src/compiled/00/49/C2DBoxI_ClipTo_0049c810.cpp), defined above the
// caller: VC7.1 then knows it writes only `this`, keeps the map corner's TLX in edi across the call and builds the
// two copies with all loads first.
void C2DBoxI::ClipTo(const C2DBoxI& other)
{
    if (TLX < other.TLX) TLX = other.TLX; else if (TLX > other.BRX) TLX = other.BRX;
    if (TLY < other.TLY) TLY = other.TLY; else if (TLY > other.BRY) TLY = other.BRY;
    if (BRX < other.TLX) BRX = other.TLX; else if (BRX > other.BRX) BRX = other.BRX;
    if (BRY < other.TLY) BRY = other.TLY; else if (BRY > other.BRY) BRY = other.BRY;
}
#pragma auto_inline(on)

class CMapInfo { public:
    C2DBoxI       Area;                         // +0x00
    unsigned char _rest[0x38];                  // +0x10 .. +0x48
};

template <class T> class CArray { public:
    T* Start;                                   // +0x00
    T* Finish;                                  // +0x04
    T* EndOfStorage;                            // +0x08
    const T& operator[](unsigned long i) const { return Start[i]; }
};

class CWorldMap { public:
    unsigned char    _pad[0x20];
    CArray<CMapInfo> Maps;                      // +0x20
    C2DBoxI ClipAndConvertWorldMapAreaToLocalArea(long map, const C2DBoxI& area) const;
};

C2DBoxI CWorldMap::ClipAndConvertWorldMapAreaToLocalArea(long map, const C2DBoxI& area) const
{
    const CMapInfo& info = Maps[map];
    C2DBoxI box(area);
    C2DBoxI mapArea(info.Area);
    box.ClipTo(mapArea);
    box.TLX -= info.Area.TLX;
    box.BRX -= info.Area.TLX;
    box.TLY -= info.Area.TLY;
    box.BRY -= info.Area.TLY;
    return box;
}