// Grade: DIFFER(586v582) -- cross product (const-left zero products) and frame-kept normal match; residue: retail computes the cell fractions after the parity test from the spilled ix/iy (faddp/fcompp consume both), ours before it (fadd st(1)/fcomp + fstp)
// CWorldMap::GetThemeFaceNormalAt(const C3DVector& pos) const -- the face normal of the theme (min-camera) height surface at a position: pick the cell's triangle (diagonal chosen by cell parity) from the four corner heights and normalise (1,0,dx) x (0,1,dy)
// Leaf: ?GetThemeFaceNormalAt@CWorldMap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063AB90, 580 bytes, `ret 8` (FableWin.pdb ?GetThemeFaceNormalAt@CWorldMap@@QBE?AVC3DVector@@ABV2@@Z).
// Callees: _ftol 0x00BFEA70 (x2), GetThemeMinCameraSizeZOffsetAt(const C2DCoordI&) 0x0063A7E0 (x4), GetDimensions (slot 13,
//   +0x34) for the clamps; the fast inverse-sqrt mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C.
// Layout: the landed 0x0063A7E0 CWorldMap model (retail offsets).
struct C3DVector
{
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    __forceinline void Normalise();
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
    float GetThemeMinCameraSizeZOffsetAt(const C2DCoordI& pos) const;   // 0x0063A7E0
    C3DVector GetThemeFaceNormalAt(const C3DVector& pos) const;
};

extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
__forceinline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, b.X * a.Z - b.Z * a.X, a.X * b.Y - a.Y * b.X);
}
__forceinline C3DVector operator*(const C3DVector& v, const float& s) { return C3DVector(v.X * s, v.Y * s, v.Z * s); }
__forceinline C3DVector Normalise(const C3DVector& v) { return v * FastInvSqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z); }
__forceinline void C3DVector::Normalise()
{
    float scale = FastInvSqrt(X * X + Y * Y + Z * Z);
    X *= scale;
    Y *= scale;
    Z *= scale;
}
#define WM_MIN(a, b) ((a) < (b) ? (a) : (b))

C3DVector CWorldMap::GetThemeFaceNormalAt(const C3DVector& pos) const
{
    float x = pos.X;
    float y = pos.Y;
    long ix = (long)x;
    long iy = (long)y;
    float h00 = GetThemeMinCameraSizeZOffsetAt(C2DCoordI(ix, iy));
    float h10 = GetThemeMinCameraSizeZOffsetAt(C2DCoordI(WM_MIN(ix + 1, GetDimensions().BRX - 1), iy));
    float h01 = GetThemeMinCameraSizeZOffsetAt(C2DCoordI(ix, WM_MIN(iy + 1, GetDimensions().BRY - 1)));
    float h11 = GetThemeMinCameraSizeZOffsetAt(C2DCoordI(WM_MIN(ix + 1, GetDimensions().BRX - 1), WM_MIN(iy + 1, GetDimensions().BRY - 1)));
    float fy = y - (float)iy;
    float fx = x - (float)ix;
    float dx, dy;
    if ((ix + iy) % 2 == 1) {
        if (fy + fx > 1.0f) {
            dx = h11 - h01;
            dy = h11 - h10;
        } else {
            dx = h10 - h00;
            dy = h01 - h00;
        }
    } else if (fx < fy) {
        dx = h11 - h01;
        dy = h01 - h00;
    } else {
        dx = h10 - h00;
        dy = h11 - h10;
    }
    C3DVector normal = CrossProduct(C3DVector(1.0f, 0.0f, dx), C3DVector(0.0f, 1.0f, dy));
    normal.Normalise();
    return normal;
}
