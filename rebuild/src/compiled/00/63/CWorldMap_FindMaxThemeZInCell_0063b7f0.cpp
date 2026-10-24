// CWorldMap::FindMaxThemeZInCell(const C3DVector* cell) const -- highest theme height over the four in-map corner blocks of a cell (0 when none)
// Leaf: ?FindMaxThemeZInCell@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0063B7F0, 154 bytes (+ int3 padding), `ret 4`; byte twin of
// FindMaxGroundZInCell 0x0063B750 apart from the height query.
// Identity: Ego_r.pdb `?FindMaxThemeZInCell@CWorldMap@@IBEMPBVC3DVector@@@Z` (the PDB has no "ThemeMinCamera" cell
// query; the catalog label "CWorldMap_FindMaxThemeMinCameraZ" and the landed caller's FindMaxThemeMinCameraZInCell follow
// the propagated callee label); caller the landed FindLastUnblockedThemePositionAndReportCollision 0x0063DB30.
// Callees: virtual slot 13 GetDimensions (+0x34, result unused), IsPosInMap(const C2DCoordI&) 0x004FC5B0,
// the per-block theme height 0x0063A7E0 (landed as GetThemeMinCameraSizeZOffsetAt(const C2DCoordI&), whose own note
// says it forwards to CMap::GetThemeSizeZAt and is probably the theme-height query: Ego_r.pdb's GetThemeSizeZAtBlock), __ftol2.
// Shape: the corner block is two named longs (edi/ebx) and each query gets its own C2DCoordI temporary, so both 8-byte
// temporaries have their address taken inside the loop: that is the `and esp,-8` aligned frame. `this` lives in a frame
// word, the table index walks in esi by 8.
// ---- local CWorldMap model (the landed 0x0063A660 GetGroundSizeZAtBlock model: retail offsets and vtable; the PDB
// header rebuild/include/engine/CWorldMap.h uses 16-byte STL containers where retail STLport vectors are 12 bytes, so it
// does not fit). Only the vtable up to slot 13 GetDimensions (+0x34) and the out-of-line block queries are needed.
struct C3DVector {
    float X;
    float Y;
    float Z;
};

struct C2DCoordI {
    long x;
    long y;
    C2DCoordI() {}
    C2DCoordI(long ax, long ay) : x(ax), y(ay) {}
};

struct C2DBoxI {
    long TLX;
    long TLY;
    long BRX;
    long BRY;
};

class CMap;

// The four block-corner offsets of a cell ({0,1} {1,1} {1,0} {0,0}), a writable file-scope table in .data shared by
// FindMaxGroundZInCell 0x0063B750 and FindMaxThemeZInCell 0x0063B7F0.
struct CCellCornerOffset {
    long X;
    long Y;
};
extern CCellCornerOffset g_CellCornerOffsets[4];                   // 0x0137B730

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
    virtual bool IsMapLoaded(long map) const;                        // slot 10 (+0x28)
    virtual bool IsMapUsed(long map) const;                          // slot 11
    virtual long GetGranularity() const;                             // slot 12
    virtual const C2DBoxI& GetDimensions() const;                    // slot 13 (+0x34)

    bool IsPosInMap(const C2DCoordI& pos) const;                     // 0x004FC5B0
    float GetGroundSizeZAtBlock(const C2DCoordI& pos) const;         // 0x0063A660
    float GetThemeSizeZAtBlock(const C2DCoordI& pos) const;          // 0x0063A7E0
    float FindMaxGroundZInCell(const C3DVector* cell) const;
    float FindMaxThemeZInCell(const C3DVector* cell) const;
};

float CWorldMap::FindMaxThemeZInCell(const C3DVector* cell) const
{
    GetDimensions();
    float maxZ = 0.0f;
    for (unsigned int i = 0; i < 4; ++i) {
        long x = (long)(g_CellCornerOffsets[i].X + cell->X);
        long y = (long)(g_CellCornerOffsets[i].Y + cell->Y);
        if (IsPosInMap(C2DCoordI(x, y))) {
            float z = GetThemeSizeZAtBlock(C2DCoordI(x, y));
            if (z > maxZ)
                maxZ = z;
        }
    }
    return maxZ;
}