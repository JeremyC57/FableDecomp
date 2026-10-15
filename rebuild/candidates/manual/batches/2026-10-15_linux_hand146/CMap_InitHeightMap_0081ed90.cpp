// CMap::InitHeightMap() -- build the map's height map from its height-map size and the world seed and install it in the scoped pointer (the old height map, if any, is destroyed after the new one is stored)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081ED90, length 98 bytes.
// Callees: operator new @ 0x00BFEA1A (0x214 bytes), CHeightMap::CHeightMap(CWorld*, CMap*, const CHeightMapInit&) @ 0x008F63C0, the old
//   height map's virtual slot 0 (scalar deleting destructor, flag 1).
// Layout (retail CMap, PDB offsets less the debug list/vector words): World +0x20, WorldSeed +0x24, HeightMap (scoped_ptr<CHeightMap>) +0x1a7c,
//   HeightMapSizeX +0x1d14, HeightMapSizeY +0x1d18; CHeightMapInit (PDB) SizeX, SizeY, WorldSeed as a 12-byte frame local.
// Shape: the init record is filled before `new` (its three stores interleave with the size push); scoped_ptr::reset stores the new pointer
//   and then deletes the old one (`delete` of a polymorphic object = null test + virtual scalar deleting destructor).
struct CWorld;
struct C2DCoordI {
    long X, Y;
};
struct CHeightMapCell {                            // PDB, 12 bytes
    float         Height;                          // +0x00
    unsigned char EngineThemePaletteIndex[3];      // +0x04
    unsigned char EngineBlend[2];                  // +0x07
    unsigned char Flags;                           // +0x09 Changed / Passable / ShorePoint / CameraPassable / NavPreferred bits
    unsigned char _pad0a[2];
};
template <class T> struct CRetailVector {          // STLport vector: begin / end / end of storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned long n) { return *(_M_start + n); }
};
struct CHeightMapInit {                            // PDB, 12 bytes
    long           SizeX;
    long           SizeY;
    unsigned long* WorldSeed;
};
struct CMap;
struct CHeightMap {                                // retail layout (PDB CHeightMap with the 12-byte release vector)
    virtual ~CHeightMap();                         // slot 0: the scalar deleting destructor
    CWorld*                        World;          // +0x04
    CMap*                          Map;            // +0x08
    unsigned long*                 WorldSeed;      // +0x0c
    CRetailVector<CHeightMapCell>  Cells;          // +0x10
    unsigned char                  _MapUpdateAreas[0x1e8];   // +0x1c C2DBoxI[30] MapUpdateAreas, +0x1fc NoMapUpdateAreas, +0x200 Changed
    long                           SizeX;          // +0x204
    long                           SizeY;          // +0x208
    long                           SizeXIncludingExtraCells;   // +0x20c
    long                           SizeYIncludingExtraCells;   // +0x210
    CHeightMap(CWorld* world, CMap* map, const CHeightMapInit& init);   // 0x008F63C0
    __forceinline CHeightMapCell& GetCellIncludingExtraCells(long x, long y) { return Cells[(SizeY - y) * (SizeX + 1) + x]; }
};
template <class T> struct scoped_ptr {
    T* ptr;
    T* operator->() const { return ptr; }
    void reset(T* p) { T* old = ptr; ptr = p; delete old; }
};
struct CMap {
    unsigned char           _pad00[0x20];
    CWorld*                 World;                 // +0x20
    unsigned long*          WorldSeed;             // +0x24
    unsigned char           _pad28[0x1a7c - 0x28];
    scoped_ptr<CHeightMap>  HeightMap;             // +0x1a7c
    unsigned char           _pad1a80[0x1d14 - 0x1a80];
    long                    HeightMapSizeX;        // +0x1d14
    long                    HeightMapSizeY;        // +0x1d18
    void InitHeightMap();
};
void CMap::InitHeightMap()
{
    CHeightMapInit init;
    init.SizeX = HeightMapSizeX;
    init.SizeY = HeightMapSizeY;
    init.WorldSeed = WorldSeed;
    HeightMap.reset(new CHeightMap(World, this, init));
}
