// CWorldMap::Validate() const -- walks every loaded map (from 1) and, along its left and top edges, looks up the neighbouring map across the edge; for each different neighbour it converts the two edge cells into each map's local coordinates (the comparison itself is compiled out of retail)
// Leaf: Validate
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063DDF0, 472 bytes (+ int3 padding), plain `ret`; 8-byte aligned frame (`and esp,-8`).
// Callees: CWorldMap virtual slots 10 IsMapLoaded (+0x28), 13 GetDimensions (+0x34), 17 GetMapNumberAt(const C2DCoordI&)
//   (+0x44), 19 PeekMap (+0x4C), 23 GetMapPos (+0x5C, C2DCoordI by hidden pointer); ConvertWorldMapCoordsToLocalCoords
//   0x004FC3C0 (out of line in this TU, C2DCoordI by hidden pointer).
// Layout: the retail CWorldMap model of the landed CWorldMap::IsPosPassable 0x004FCBD0 (Maps CArray<CMapInfo> at +0x20;
//   the PDB header rebuild/include/engine/CWorldMap.h has 16-byte STL containers, retail 12, so Maps there is +0x24);
//   CMapInfo is 72 bytes with Area (C2DBoxI) at +0.
class C2DCoordI
{ public:
    long x;
    long y;
    C2DCoordI() {}
    C2DCoordI(long ax, long ay) : x(ax), y(ay) {}
};

class C2DBoxI
{ public:
    long TLX;
    long TLY;
    long BRX;
    long BRY;
};

class CMap { };

class CMapInfo
{ public:
    C2DBoxI        Area;                 // +0x00
    unsigned char  _pad10[0x38];         // +0x10 .. 0x48
};

struct CMapInfoArray
{
    CMapInfo* _M_start;
    CMapInfo* _M_finish;
    CMapInfo* _M_end_of_storage;
    CMapInfo* begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    const CMapInfo& operator[](unsigned int n) const { return *(begin() + n); }
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
    virtual const C2DBoxI& GetDimensions() const;                    // slot 13 (+0x34) 0x0051CDE0
    virtual CMap& GetMapAt(const C2DCoordI& pos);                    // slot 14
    virtual const CMap& PeekMapAt(const C2DCoordI& pos) const;       // slot 15
    virtual long GetMapNumberAt(const C2DCoordI& pos) const;         // slot 17 (+0x44) 0x004FC080
    virtual long GetMapNumberAt(const class C3DVector& pos) const;  // slot 16 (+0x40) 0x004FC0C0
    virtual CMap& GetMap(long map);                                  // slot 18 (+0x48) 0x0051CE20
    virtual const CMap& PeekMap(long map) const;                     // slot 19 (+0x4C) 0x0051CE40
    virtual void v20(); virtual void v21(); virtual void v22();
    virtual C2DCoordI GetMapPos(long map) const;                     // slot 23 (+0x5C) 0x004FC690

    long            f4;                    // +0x04
    void*           World;                 // +0x08
    void*           Component;             // +0x0C
    void*           PThingMapManager;      // +0x10
    unsigned long   LoadedScriptedMapBrushes[3]; // +0x14 STLport map (12 bytes)
    CMapInfoArray   Maps;                  // +0x20 CArray<CMapInfo>

    C2DCoordI ConvertWorldMapCoordsToLocalCoords(long map, const C2DCoordI& pos) const;   // 0x004FC3C0
    void Validate() const;                                           // protected in the original (?Validate@CWorldMap@@IBEXXZ)
};

void CWorldMap::Validate() const
{
    for (long i = 1; i < (long)Maps.size(); ++i) {
        if (!IsMapLoaded(i))
            continue;
        PeekMap(i);
        C2DCoordI pos = GetMapPos(i);
        if (pos.x > GetDimensions().TLX) {
            for (long y = Maps[i].Area.TLY; y < Maps[i].Area.BRY; ++y) {
                C2DCoordI outside(pos.x - 1, y);
                C2DCoordI inside(pos.x, y);
                long other = GetMapNumberAt(outside);
                if (other != 0 && other != i) {
                    C2DCoordI here = ConvertWorldMapCoordsToLocalCoords(i, inside);
                    C2DCoordI there = ConvertWorldMapCoordsToLocalCoords(other, C2DCoordI(Maps[other].Area.BRX, y));
                }
            }
        }
        if (pos.y > GetDimensions().TLY) {
            for (long x = Maps[i].Area.TLX; x < Maps[i].Area.BRX; ++x) {
                C2DCoordI outside(x, pos.y - 1);
                C2DCoordI inside(x, pos.y);
                long other = GetMapNumberAt(outside);
                if (other != 0 && other != i) {
                    C2DCoordI here = ConvertWorldMapCoordsToLocalCoords(i, inside);
                    C2DCoordI there = ConvertWorldMapCoordsToLocalCoords(other, C2DCoordI(x, Maps[other].Area.BRY));
                }
            }
        }
    }
}