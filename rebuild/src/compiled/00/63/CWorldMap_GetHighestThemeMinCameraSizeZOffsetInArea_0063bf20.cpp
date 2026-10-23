// CWorldMap::GetHighestThemeMinCameraSizeZOffsetInArea(const C3DVector& tl, const C3DVector& br) const -- highest theme camera offset along the box edges (corners, then each whole-block step) and at every block inside the box
// Leaf: ?GetHighestThemeMinCameraSizeZOffsetInArea@CWorldMap@@QBEMABVC3DVector@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063BF20, `ret 8`, ebp frame.
// Callees: _ftol 0x00BFEA70, CWorldMap::GetThemeMinCameraSizeZOffsetAt(const C3DVector&) 0x0063A540, GetThemeMinCameraSizeZOffsetAtBlock(const C2DCoordI&) 0x0063BB40 inlined (PeekMap called twice, CMap::GetThemeMinCameraSizeZOffsetAt 0x0081FA90).
// Layout: the landed 0x0063A7E0 CWorldMap model (retail offsets).
#include <math.h>
struct C3DVector
{
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};

struct C2DVector
{
    float X;
    float Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
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

class CMap
{
public:
    bool IsPassableAt(const C2DCoordI& pos) const;                // 0x0081F3D0
    bool IsCameraPassableAt(const C2DCoordI& pos) const;          // 0x0081F460
    // theme/height TU callees (0x0063Axxx rows)
    float GetGroundSizeZAt(const C3DVector& localPos) const;      // 0x0081EA50
    float GetInterpolatedSizeZAt(const C3DVector& localPos) const; // 0x008200B0
    float GetThemeMinCameraSizeZOffsetAt(const C2DCoordI& pos) const; // 0x0081FA90
    float GetThemeSizeZAt(const C2DCoordI& pos) const;            // 0x00820100
    float GetGroundSizeZAt(const C2DCoordI& pos) const;           // 0x00820080
    long GetSoundAt(const C2DCoordI& pos) const;                  // 0x0081F590
    bool IsPassableAtBlock(const C2DCoordI& pos) const;           // 0x0081F630
    C3DVector GetGroundFaceNormalAt(const C3DVector& localPos) const; // 0x0081F6C0
    long GetStrongestEngineThemeAt(const C2DCoordI& pos, long* strength) const; // 0x0081F910
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

    // out of line in this TU (CWorldMap.cpp, 0x004FCxxx)
    bool IsPosInMap(const C2DCoordI& pos) const;                     // 0x004FC5B0
    bool IsPosInMap(const C3DVector& pos) const;                     // 0x004FC600
    C2DCoordI ConvertWorldMapCoordsToLocalCoords(long map, const C2DCoordI& pos) const; // 0x004FC3C0
    C3DVector ConvertWorldMapCoordsToLocalCoords(long map, const C3DVector& pos) const; // 0x004FC2A0
    float GetThemeMinCameraSizeZOffsetAt(const C3DVector& pos) const;   // 0x0063A540
    __forceinline float GetThemeMinCameraSizeZOffsetAtBlock(const C2DCoordI& pos) const;   // 0x0063BB40 (inlined here)
    float GetHighestThemeMinCameraSizeZOffsetInArea(const C3DVector& tl, const C3DVector& br) const;
};
__forceinline float CWorldMap::GetThemeMinCameraSizeZOffsetAtBlock(const C2DCoordI& pos) const
{
    if (IsPosInMap(pos))
    {
        long map = GetMapNumberAt(pos);
        if (map != 0 && IsMapLoaded(map))
        {
            C2DCoordI local = ConvertWorldMapCoordsToLocalCoords(map, pos);
            const CMap& theMap = PeekMap(map);
            return PeekMap(map).GetThemeMinCameraSizeZOffsetAt(local);
        }
    }
    return 0.0f;
}

float CWorldMap::GetHighestThemeMinCameraSizeZOffsetInArea(const C3DVector& tl, const C3DVector& br) const
{
    C3DVector corner(0.0f, 0.0f, 0.0f);
    C2DCoordI start, end;
    {
        C2DVector a(tl.X, tl.Y);
        C2DVector b(br.X, br.Y);
        start.x = (long)a.X + 1;
        start.y = (long)a.Y + 1;
        end.x = (long)b.X;
        end.y = (long)b.Y;
    }
    float highest = GetThemeMinCameraSizeZOffsetAt(tl);
    float h = GetThemeMinCameraSizeZOffsetAt(br);
    if (h > highest) highest = h;
    corner.X = tl.X;
    corner.Y = br.Y;
    h = GetThemeMinCameraSizeZOffsetAt(corner);
    if (h > highest) highest = h;
    corner.X = br.X;
    corner.Y = tl.Y;
    h = GetThemeMinCameraSizeZOffsetAt(corner);
    if (h > highest) highest = h;
    float yStart = (float)((long)tl.Y + 1);
    float xEnd = (float)(long)br.X;
    float yEnd = (float)(long)br.Y;
    for (corner.X = (float)((long)tl.X + 1); corner.X <= xEnd; corner.X += 1.0f) {
        corner.Y = tl.Y;
        h = GetThemeMinCameraSizeZOffsetAt(corner);
        if (h > highest) highest = h;
        corner.Y = br.Y;
        h = GetThemeMinCameraSizeZOffsetAt(corner);
        if (h > highest) highest = h;
    }
    for (corner.Y = yStart; corner.Y <= yEnd; corner.Y += 1.0f) {
        corner.X = tl.X;
        h = GetThemeMinCameraSizeZOffsetAt(corner);
        if (h > highest) highest = h;
        corner.X = br.X;
        h = GetThemeMinCameraSizeZOffsetAt(corner);
        if (h > highest) highest = h;
    }
    C2DCoordI pos;
    for (pos.x = start.x; pos.x <= end.x; ++pos.x) {
        for (pos.y = start.y; pos.y <= end.y; ++pos.y) {
            h = GetThemeMinCameraSizeZOffsetAtBlock(pos);
            if (h > highest) highest = h;
        }
    }
    return highest;
}