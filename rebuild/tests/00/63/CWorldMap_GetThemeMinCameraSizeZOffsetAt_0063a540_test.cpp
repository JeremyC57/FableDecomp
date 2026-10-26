#include <stdio.h>
// CWorldMap::GetThemeMinCameraSizeZOffsetAt(const C3DVector&) -- per-block query: forwards to CMap::GetThemeMinCameraSizeZOffsetAt of the loaded map holding the block, else 0
// Retail 0x0063a540, 161 bytes (world-map theme/height TU, 0x0063Axxx). Callees: IsPosInMap(C3DVector) 0x004FC600,
// slot 17 GetMapNumberAt(C2DCoordI) [+0x44], slot 10 IsMapLoaded [+0x28], ConvertWorldMapCoordsToLocalCoords(long,
// C3DVector) 0x004FC2A0 (out of line here), slot 19 PeekMap [+0x4C], CMap::GetThemeMinCameraSizeZOffsetAt 0x0081FA90. Default 0.0f (pool 0x0122DEDC).
// Retail calls PeekMap after the conversion and only then forms &local, so the map reference is a named local.
// ---- local CWorldMap model (retail offsets; the PDB header rebuild/include/engine/CWorldMap.h
// uses 16-byte STL containers, retail uses 12-byte STLport vectors, so every member after +0x14
// sits 4/8/12.. bytes lower in retail: Maps +0x20 (PDB +0x24), Granularity +0x50 (PDB +0x60),
// MapPlacementPitch +0x58 (PDB +0x68), Area +0x5C..+0x68 (PDB +0x6C..+0x78), MapPlacement +0x6C
// (PDB +0x7C), PlayerPositionInfo +0x78 (PDB +0x8C), LoadedMaps +0x90 (PDB +0xAC)).
class C2DCoordI;
class C3DVector
{ public:
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    operator C2DCoordI() const;
};

class C2DCoordI
{ public:
    long x;
    long y;
    C2DCoordI() {}
    C2DCoordI(long ax, long ay) : x(ax), y(ay) {}
};

C3DVector::operator C2DCoordI() const { return C2DCoordI((long)X, (long)Y); }

class C2DBoxI
{ public:
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
    float GetThemeMinCameraSizeZOffsetAt(const C2DCoordI& pos) const;
    float GetThemeSizeZAt(const C2DCoordI& pos) const;            // 0x00820100
    float GetGroundSizeZAt(const C2DCoordI& pos) const;           // 0x00820080
    long GetSoundAt(const C2DCoordI& pos) const;                  // 0x0081F590
    bool IsPassableAtBlock(const C2DCoordI& pos) const;           // 0x0081F630
    C3DVector GetGroundFaceNormalAt(const C3DVector& localPos) const; // 0x0081F6C0
    long GetStrongestEngineThemeAt(const C2DCoordI& pos, long* strength) const; // 0x0081F910
};

// CMapInfo (PDB: 72 bytes): Area C2DBoxI +0x00, PMap +0x10, LevelName +0x18, LevelScriptName +0x1C,
// MapUID +0x20, Used +0x24, Placed +0x25, Activated +0x26, Loading +0x27, ... IsSea +0x40.
class CMapInfo
{ public:
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
    float GetThemeMinCameraSizeZOffsetAt(const C2DCoordI& pos) const;
    float GetThemeMinCameraSizeZOffsetAt(const C3DVector& pos) const;
};

float CWorldMap::GetThemeMinCameraSizeZOffsetAt(const C3DVector& pos) const
{
    if (IsPosInMap(pos))
    {
        long map = GetMapNumberAt(C2DCoordI((long)pos.X, (long)pos.Y));
        if (map != 0 && IsMapLoaded(map))
        {
            C3DVector local = ConvertWorldMapCoordsToLocalCoords(map, pos);
            const CMap& theMap = PeekMap(map);
            return theMap.GetThemeMinCameraSizeZOffsetAt(local);
        }
    }
    return 0.0f;
}
static bool g_inside; static long g_number; static bool g_loaded;
static int g_mapNumberCalls, g_loadedCalls, g_convertCalls, g_peekCalls, g_queryCalls;
static long g_blockX, g_blockY, g_localX, g_localY;
static float g_result; static bool g_bad;
static CMap g_map;
bool CMap::IsPassableAt(const C2DCoordI&) const { return false; }
bool CMap::IsCameraPassableAt(const C2DCoordI&) const { return false; }
float CMap::GetGroundSizeZAt(const C3DVector&) const { return 0; }
float CMap::GetInterpolatedSizeZAt(const C3DVector&) const { return 0; }
float CMap::GetThemeMinCameraSizeZOffsetAt(const C2DCoordI& p) const { if(g_peekCalls!=1)g_bad=true; ++g_queryCalls; g_localX=p.x; g_localY=p.y; return g_result; }
float CMap::GetThemeSizeZAt(const C2DCoordI&) const { return 0; }
float CMap::GetGroundSizeZAt(const C2DCoordI&) const { return 0; }
long CMap::GetSoundAt(const C2DCoordI&) const { return 0; }
bool CMap::IsPassableAtBlock(const C2DCoordI&) const { return false; }
C3DVector CMap::GetGroundFaceNormalAt(const C3DVector&) const { return C3DVector(); }
long CMap::GetStrongestEngineThemeAt(const C2DCoordI&, long*) const { return 0; }
CWorldMap::~CWorldMap() {}
void CWorldMap::DeleteAllLevels() {}
void CWorldMap::DeleteAllRegions() {}
void CWorldMap::LoadFromFile() {}
void CWorldMap::SaveToFile() {}
void CWorldMap::RemoveLevel(long) {}
void CWorldMap::LoadTopologyFromFile() {}
void CWorldMap::PostLoadTopology(long) {}
void CWorldMap::ForceLoad(long) {}
void CWorldMap::ForceUnload(long) {}
bool CWorldMap::IsMapLoaded(long map) const { if(map!=g_number)g_bad=true; ++g_loadedCalls; return g_loaded; }
bool CWorldMap::IsMapUsed(long) const { return false; }
long CWorldMap::GetGranularity() const { return 0; }
const C2DBoxI& CWorldMap::GetDimensions() const { static C2DBoxI b={0,0,0,0}; return b; }
CMap& CWorldMap::GetMapAt(const C2DCoordI&) { return g_map; }
const CMap& CWorldMap::PeekMapAt(const C2DCoordI&) const { return g_map; }
long CWorldMap::GetMapNumberAt(const C2DCoordI& p) const { ++g_mapNumberCalls; g_blockX=p.x; g_blockY=p.y; return g_number; }
long CWorldMap::GetMapNumberAt(const C3DVector&) const { return 0; }
CMap& CWorldMap::GetMap(long) { return g_map; }
const CMap& CWorldMap::PeekMap(long map) const { if(map!=g_number || g_convertCalls!=1 || g_queryCalls)g_bad=true; ++g_peekCalls; return g_map; }
bool CWorldMap::IsPosInMap(const C2DCoordI&) const { return false; }
bool CWorldMap::IsPosInMap(const C3DVector&) const { return g_inside; }
C2DCoordI CWorldMap::ConvertWorldMapCoordsToLocalCoords(long, const C2DCoordI& p) const { return p; }
C3DVector CWorldMap::ConvertWorldMapCoordsToLocalCoords(long map, const C3DVector& p) const { if(map!=g_number || g_loadedCalls!=1 || g_peekCalls)g_bad=true; ++g_convertCalls; return C3DVector(p.X + 0.9f, p.Y - 0.9f, p.Z); }
float CWorldMap::GetThemeMinCameraSizeZOffsetAt(const C2DCoordI&) const { return 0; }
int main() {
    CWorldMap w; C3DVector p(3.7f, -4.2f, 9.0f);
    g_inside=false; g_number=2; g_loaded=true; g_mapNumberCalls=g_loadedCalls=g_convertCalls=g_peekCalls=g_queryCalls=0;
    if (w.GetThemeMinCameraSizeZOffsetAt(p)!=0.0f || g_mapNumberCalls || g_loadedCalls) return 1;
    g_inside=true; g_number=0;
    if (w.GetThemeMinCameraSizeZOffsetAt(p)!=0.0f || g_mapNumberCalls!=1 || g_loadedCalls) return 2;
    g_number=4; g_loaded=false; g_mapNumberCalls=g_loadedCalls=0;
    if (w.GetThemeMinCameraSizeZOffsetAt(p)!=0.0f || g_mapNumberCalls!=1 || g_loadedCalls!=1 || g_convertCalls) return 3;
    g_loaded=true; g_result=12.5f; g_mapNumberCalls=g_loadedCalls=g_convertCalls=g_peekCalls=g_queryCalls=0;
    if (w.GetThemeMinCameraSizeZOffsetAt(p)!=12.5f || g_mapNumberCalls!=1 || g_loadedCalls!=1 || g_convertCalls!=1 || g_peekCalls!=1 || g_queryCalls!=1) return 4;
    if (g_blockX!=3 || g_blockY!=-4 || g_localX!=4 || g_localY!=-5) return 5;
    if(g_bad) return 6;
    p=C3DVector(-0.9f,0.9f,-2.0f);g_result=-1.25f;g_mapNumberCalls=g_loadedCalls=g_convertCalls=g_peekCalls=g_queryCalls=0;
    if(w.GetThemeMinCameraSizeZOffsetAt(p)!=-1.25f || g_blockX!=0 || g_blockY!=0 || g_localX!=0 || g_localY!=0 || g_bad) return 7;
    puts("0063a540_TEST PASS"); return 0;
}