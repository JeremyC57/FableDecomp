// CWorldMap::GetAllLoadedMapsInArea(const C2DBoxI&, vector<unsigned long>&) const -- appends every loaded map whose area overlaps the box
// Retail 0x004FF310, 0x7B bytes (CWorldMap.cpp TU), vtable 0x01244AEC slot 21. Walks LoadedMaps (CArray<long>, retail
// +0x90/+0x94, PDB +0xAC) and tests Maps[n].Area (CMapInfo 72 bytes, Maps begin +0x20) with C2DBoxI::Overlapping
// 0x004FAB40; hits are appended with the inline STLport push_back (placement copy at `finish`, else
// _M_insert_overflow 0x00464ACE for one element at the end with an empty __true_type tag).
#include <new>
// ---- local CWorldMap model (retail offsets; the PDB header rebuild/include/engine/CWorldMap.h
// uses 16-byte STL containers, retail uses 12-byte STLport vectors, so every member after +0x14
// sits 4/8/12.. bytes lower in retail: Maps +0x20 (PDB +0x24), Granularity +0x50 (PDB +0x60),
// MapPlacementPitch +0x58 (PDB +0x68), Area +0x5C..+0x68 (PDB +0x6C..+0x78), MapPlacement +0x6C
// (PDB +0x7C), PlayerPositionInfo +0x78 (PDB +0x8C), LoadedMaps +0x90 (PDB +0xAC)).
struct C3DVector
{
    float X;
    float Y;
    float Z;
};

struct C2DCoordI
{
    long x;
    long y;
    C2DCoordI() {}
    C2DCoordI(long ax, long ay) : x(ax), y(ay) {}
};

struct C2DBoxI
{
    long TLX;
    long TLY;
    long BRX;
    long BRY;
    bool Overlapping(const C2DBoxI& other) const;                 // 0x004FAB40
};
#pragma auto_inline(off)
// Same-TU stand-in for C2DBoxI::Overlapping 0x004FAB40 (compiled in the CWorldMap.cpp TU): a leaf reading both boxes.
__declspec(noinline) bool C2DBoxI::Overlapping(const C2DBoxI& other) const
{
    return TLX <= other.BRX && other.TLX <= BRX && TLY <= other.BRY && other.TLY <= BRY;
}
#pragma auto_inline(on)

class CMap
{
public:
    bool IsPassableAt(const C2DCoordI& pos) const;                // 0x0081F3D0
    bool IsCameraPassableAt(const C2DCoordI& pos) const;          // 0x0081F460
};

// CMapInfo (PDB: 72 bytes): Area C2DBoxI +0x00, PMap +0x10, LevelName +0x18, LevelScriptName +0x1C,
// MapUID +0x20, Used +0x24, Placed +0x25, Activated +0x26, Loading +0x27, ... IsSea +0x40.
struct CMapInfo
{
    C2DBoxI        Area;                 // +0x00
    void*          PMap_Data;            // +0x10
    void*          PMap_Info;            // +0x14
    void*          LevelName;            // +0x18
    void*          LevelScriptName;      // +0x1C
    unsigned long  MapUID;               // +0x20
    bool           Used;                 // +0x24
    bool           Placed;               // +0x25
    bool           Activated;            // +0x26
    bool           Loading;              // +0x27
    bool           LoadedOnPlayerProximity; // +0x28
    unsigned char  _pad29[3];
    void*          SerialiseScript;      // +0x2C
    unsigned long  LevStamp[2];          // +0x30
    unsigned long  TngStamp[2];          // +0x38
    bool           IsSea;                // +0x40
    unsigned char  _pad41[7];
};

// STLport vector<CMapInfo> (CArray<CMapInfo>), 12 bytes in retail.
struct CMapInfoArray
{
    CMapInfo* _M_start;
    CMapInfo* _M_finish;
    CMapInfo* _M_end_of_storage;
    CMapInfo* begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    const CMapInfo& operator[](unsigned int n) const { return *(begin() + n); }
    CMapInfo& operator[](unsigned int n) { return *(_M_start + n); }
};

class CWorldMap
{
public:
    virtual ~CWorldMap();                                            // slot 0
    virtual void DeleteAllLevels();                                  // slot 1
    virtual void DeleteAllRegions();                                 // slot 2
    virtual void LoadFromFile();                                     // slot 3
    virtual void SaveToFile();                                       // slot 4
    virtual void RemoveLevel(long map);                              // slot 5
    virtual void LoadTopologyFromFile();                             // slot 6
    virtual void PostLoadTopology(long map);                         // slot 7
    virtual void ForceLoad(long map);                                // slot 8
    virtual void ForceUnload(long map);                              // slot 9
    virtual bool IsMapLoaded(long map) const;                        // slot 10 (+0x28) 0x004FC050
    virtual bool IsMapUsed(long map) const;                          // slot 11
    virtual long GetGranularity() const;                             // slot 12
    virtual const C2DBoxI& GetDimensions() const;                    // slot 13
    virtual CMap& GetMapAt(const C2DCoordI& pos);                    // slot 14
    virtual const CMap& PeekMapAt(const C2DCoordI& pos) const;       // slot 15
    // overloaded virtuals are laid out in reverse declaration order (C3DVector form is slot 16)
    virtual long GetMapNumberAt(const C2DCoordI& pos) const;         // slot 17 (+0x44) 0x004FC080
    virtual long GetMapNumberAt(const C3DVector& pos) const;         // slot 16 (+0x40) 0x004FC0C0
    virtual CMap& GetMap(long map);                                  // slot 18 (+0x48) 0x0051CE20
    virtual const CMap& PeekMap(long map) const;                     // slot 19 (+0x4C) 0x0051CE40

    long            f4;                    // +0x04
    void*           World;                 // +0x08
    void*           Component;             // +0x0C
    void*           PThingMapManager;      // +0x10
    unsigned long   LoadedScriptedMapBrushes[3]; // +0x14 STLport map (12 bytes)
    CMapInfoArray   Maps;                  // +0x20 CArray<CMapInfo>
    unsigned long   Regions[3];            // +0x2C vector<CRegion>
    unsigned long   RegionTime[3];         // +0x38 vector<long>
    void*           PRegionGraph_Data;     // +0x44
    void*           PRegionGraph_Info;     // +0x48
    bool            SoundThemesDisabled;   // +0x4C
    long            Granularity;           // +0x50
    long            GranularityShift;      // +0x54
    long            MapPlacementPitch;     // +0x58
    C2DBoxI         Area;                  // +0x5C
    unsigned short* MapPlacement_start;    // +0x6C vector<unsigned short>
    unsigned short* MapPlacement_finish;   // +0x70
    unsigned short* MapPlacement_eos;      // +0x74
    unsigned long   PlayerPositionInfo[3]; // +0x78
    unsigned long   HeightChangedAreas[3]; // +0x84
    long*           LoadedMaps_start;      // +0x90 CArray<long>
    long*           LoadedMaps_finish;     // +0x94
    long*           LoadedMaps_eos;        // +0x98

    struct __true_type { __true_type() {} };
    struct CULongVector
    {
        unsigned long* _M_start;
        unsigned long* _M_finish;
        unsigned long* _M_end_of_storage;
        void _M_insert_overflow(unsigned long* pos, const unsigned long& x, const __true_type& tag,
                                unsigned int n, bool atend);                     // 0x00464ACE
        void push_back(const unsigned long& x)
        {
            if (_M_finish != _M_end_of_storage)
            {
                new (_M_finish) unsigned long(x);
                ++_M_finish;
            }
            else
                _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
        }
    };
    void GetAllLoadedMapsInArea(const C2DBoxI& area, CULongVector& maps) const;
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);

#pragma auto_inline(off)
// Same-TU body of the vector growth member (_M_insert_overflow, instantiated in retail's TU): the new storage receives the value
// read through its const reference; the empty tag is never touched.
__declspec(noinline) void CWorldMap::CULongVector::_M_insert_overflow(unsigned long* pos, const unsigned long& x, const __true_type& tag, unsigned int n, bool atend)
{
    unsigned long* p = (unsigned long*)GrowVectorStorage_s7(this, n);
    if (p)
        new (p) unsigned long(x);
}
#pragma auto_inline(on)

void CWorldMap::GetAllLoadedMapsInArea(const C2DBoxI& area, CULongVector& maps) const
{
    for (long* it = LoadedMaps_start; it != LoadedMaps_finish; ++it)
    {
        long map = *it;
        if (Maps[map].Area.Overlapping(area))
            maps.push_back(map);   // long -> unsigned long conversion temporary (homed in the dead `maps` slot)
    }
}