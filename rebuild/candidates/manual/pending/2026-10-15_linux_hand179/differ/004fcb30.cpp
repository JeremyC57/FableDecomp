// CWorldMap::GetMinPlayerDistanceFromMap(long) -- smallest block distance from any set player position to a map's area
// Grade: DIFFER(128v127) -- retail loads only Maps.begin into edi before the two __ftol2 calls and forms lea [begin+map*72] after them; ours (named CMapInfo& local) forms the whole element address before the calls, and the Set test is `mov esi ; add esi,eax` vs retail `lea esi,[ecx+eax]` (non-const member, explicit C2DCoordI(const C3DVector&) ctor) | landing session, 1 try: `const CMapInfo* maps = Maps.begin();` before the coordinate still forms the element address before the __ftol2 calls; parked
// Retail 0x004FCB30, 0x7F bytes (CWorldMap.cpp TU). Walks the four CPlayerPositionInfo records (13 bytes, packed:
// bool Set +0, C3DVector Pos +1) of PlayerPositionInfo (vector, begin at retail +0x78, PDB +0x8C); for each set one the
// position is truncated with __ftol2 (0x00BFEA70) to a C2DCoordI and measured against Maps[map].Area with the
// C2DBoxI distance helper 0x00A3A180 (manifest label `_Dest_val<...CMusicManager...>` is a byte-twin propagation;
// the body clamps the point into the box, so it is a C2DBoxI member). Starts from 0x7FFFFFFF.
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
    explicit C2DCoordI(const C3DVector& v) : x((long)v.X), y((long)v.Y) {}
};

struct C2DBoxI
{
    long TLX;
    long TLY;
    long BRX;
    long BRY;
    bool Overlapping(const C2DBoxI& other) const;                 // 0x004FAB40
    long GetDistanceTo(const C2DCoordI& pos) const;               // 0x00A3A180
};

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
#pragma pack(push, 1)
    struct CPlayerPositionInfo
    {
        bool      Set;                     // +0x00
        C3DVector Pos;                     // +0x01
    };
#pragma pack(pop)
    CPlayerPositionInfo* PlayerPositionInfo_start;  // +0x78
    CPlayerPositionInfo* PlayerPositionInfo_finish; // +0x7C
    CPlayerPositionInfo* PlayerPositionInfo_eos;    // +0x80

    long GetMinPlayerDistanceFromMap(long map);
};

long CWorldMap::GetMinPlayerDistanceFromMap(long map)
{
    long best = 0x7FFFFFFF;
    for (long i = 0; i < 4; ++i)
    {
        const CPlayerPositionInfo& info = PlayerPositionInfo_start[i];
        if (info.Set)
        {
            const CMapInfo& mapInfo = Maps[map];
            long distance = mapInfo.Area.GetDistanceTo(C2DCoordI(info.Pos));
            if (distance < best)
                best = distance;
        }
    }
    return best;
}
