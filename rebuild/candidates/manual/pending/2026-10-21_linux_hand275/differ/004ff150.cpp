// Grade: DIFFER(437v436) -- retail keeps the `maps` argument in ebp for the whole body (clear, both push_backs); ours reloads it from its arg slot, so ebp takes the erase result, the __copy_trivial ternary arms invert and the push_back size step sits in a register
// CWorldMap::GetAllMapsInArea(const C2DBoxI& area, vector<unsigned long>& maps) const -- clamps the box to the world area and refills `maps` with the map numbers it touches: the single map from the placement grid when the box lies in one level, else every used map whose area overlaps it
// Leaf: GetAllMapsInArea
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FF150, 436 bytes (+ int3 padding), `ret 8`; vtable 0x01244AEC slot 20.
// Callees: memmove 0x00BFEAE6 (the inline STLport erase(begin(), end())), CWorldMap::AreaCoversOneLevel 0x004FB160,
//   C2DBoxI::Overlapping 0x004FAB40 (same-TU stand-in), vector<unsigned long>::_M_insert_overflow 0x00464ACE.
// Layout: the retail CWorldMap model of the landed sibling GetAllLoadedMapsInArea 0x004FF310 (slot 21).
#include <string.h>
inline void* __copy_trivial(const void* first, const void* last, void* result)   // STLport 4.6 _algobase.h
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}
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
    static unsigned long* __copy_ptrs(const unsigned long* first, const unsigned long* last, unsigned long* result, const __true_type&)
    {
        return (unsigned long*)__copy_trivial(first, last, result);
    }
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
        unsigned long* erase(unsigned long* first, unsigned long* last)
        {
            unsigned long* i = __copy_ptrs(last, _M_finish, first, __true_type());
            _M_finish = i;
            return first;
        }
        unsigned long* begin() { return _M_start; }
        unsigned long* end() { return _M_finish; }
        void clear() { erase(begin(), end()); }
    };
    bool AreaCoversOneLevel(const C2DBoxI& area) const;              // 0x004FB160
    void GetAllMapsInArea(const C2DBoxI& area, CULongVector& maps) const;
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

#pragma auto_inline(off)
// Same-TU stand-in for CWorldMap::AreaCoversOneLevel 0x004FB160 (CWorldMap.cpp): with its body in view VC keeps the clamped
// box corners in registers across the call.
bool CWorldMap::AreaCoversOneLevel(const C2DBoxI& area) const
{
    return MapPlacement_start[(area.TLY >> GranularityShift) * MapPlacementPitch + (area.TLX >> GranularityShift)]
        == MapPlacement_start[(area.BRY >> GranularityShift) * MapPlacementPitch + (area.BRX >> GranularityShift)];
}
#pragma auto_inline(on)

void CWorldMap::GetAllMapsInArea(const C2DBoxI& area, CULongVector& maps) const
{
    C2DBoxI box = area;
    long maxX = Area.BRX - 1;
    if (box.TLX < 0) box.TLX = 0; else if (box.TLX > maxX) box.TLX = maxX;
    if (box.BRX < 0) box.BRX = 0; else if (box.BRX > maxX) box.BRX = maxX;
    long maxY = Area.BRY - 1;
    if (box.TLY < 0) box.TLY = 0; else if (box.TLY > maxY) box.TLY = maxY;
    if (box.BRY < 0) box.BRY = 0; else if (box.BRY > maxY) box.BRY = maxY;
    maps.clear();
    if (AreaCoversOneLevel(box)) {
        maps.push_back(MapPlacement_start[(box.TLY >> GranularityShift) * MapPlacementPitch + (box.TLX >> GranularityShift)]);
        return;
    }
    for (long i = 1; i < Maps.size(); ++i)
        if (Maps[i].Used && Maps[i].Area.Overlapping(box))
            maps.push_back(i);
}
