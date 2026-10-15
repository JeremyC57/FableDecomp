// CMap::SetEngineThemeAtIncludingExtraCells(long x, long y, unsigned long themeDefIndex, long layer) -- set one theme layer of a height-map cell (extra border cells included): a non-zero theme definition is first added to the map's theme palette and its palette index stored, zero clears the layer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081EF30, length 129 bytes.
// Callees: CMap::AddThemeDefIndexToPalette @ 0x008F70F0 (returns the palette index as an unsigned char).
// Layout: CMap HeightMap +0x1a7c; retail CHeightMap Cells (STLport vector<CHeightMapCell>) +0x10, SizeX +0x204, SizeY +0x208;
//   CHeightMapCell (PDB, 12 bytes) EngineThemePaletteIndex[3] at +4. Cell (x, y) = Cells[(SizeY - y) * (SizeX + 1) + x].
// Shape: the theme test is `> 0` on the unsigned index (`jbe`); the palette index is taken into a LONG local first (`movzx edx,al`
//   before the cell address), which also makes VC load the coordinates into registers; each arm inlines the cell accessor.
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
    unsigned char           _pad00[0x1a7c];
    scoped_ptr<CHeightMap>  HeightMap;             // +0x1a7c
    unsigned char AddThemeDefIndexToPalette(unsigned long themeDefIndex);   // 0x008F70F0
    void SetEngineThemeAtIncludingExtraCells(long x, long y, unsigned long themeDefIndex, long layer);
};
void CMap::SetEngineThemeAtIncludingExtraCells(long x, long y, unsigned long themeDefIndex, long layer)
{
    if (themeDefIndex > 0) {
        long paletteIndex = AddThemeDefIndexToPalette(themeDefIndex);
        HeightMap->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[layer] = (unsigned char)paletteIndex;
    } else
        HeightMap->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[layer] = 0;
}
