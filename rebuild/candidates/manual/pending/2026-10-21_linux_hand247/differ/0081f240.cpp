// Grade: DIFFER(368v392) -- the two EngineBlend copies: retail reloads this->HeightMap three times and spills the source cells base to the frame (register pressure), ours keeps them; the cell copy and the palette loop match up to one `sub eax,[esi+4]` fold
// CMap::CopyPos(const CMap& source, const C2DCoordI& from, const C2DCoordI& to) -- copy one height-map cell from another map: the whole cell, then each of its three theme layers re-registered in this map's palette, then the two blend bytes
// Leaf: CopyPos
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081F240, length 390 bytes (padded to 400), `ret 0Ch`.
// Ego_r.pdb: `?CopyPos@CMap@@IAEXABV1@ABVC2DCoordI@@1@Z`.
// Callees: CMap::SetEngineThemeAtIncludingExtraCells(long x, long y, unsigned long themeDefIndex, long layer) @ 0x0081EF30 (landed).
// Layout: landed CMap model (0x0081EF30): ThemePaletteEntryList (STLport vector<CThemePaletteEntry>, 8-byte entries, ThemeDefIndex
//   at +0) +0x1a6c, HeightMap +0x1a7c; retail CHeightMap Cells +0x10, SizeX +0x204, SizeY +0x208; CHeightMapCell (12 bytes):
//   Height +0, EngineThemePaletteIndex[3] +4, EngineBlend[2] +7, Flags +9. No engine/ header exists for CMap.
// Shape: every access goes through the inline GetCellIncludingExtraCells accessor (re-evaluated each time, nothing cached across
//   the calls); the cell is copied as a 12-byte struct, the palette index is looked up in the source map's palette.
struct C2DCoordI {
    long X, Y;
};

struct CHeightMapCell {                            // PDB, 12 bytes
    float         Height;                          // +0x00
    unsigned char EngineThemePaletteIndex[3];      // +0x04
    unsigned char EngineBlend[2];                  // +0x07
    unsigned char Flags;                           // +0x09
    unsigned char _pad0a[2];
};

template <class T> struct CRetailVector {          // STLport vector: begin / end / end of storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned long n) { return *(_M_start + n); }
    const T& operator[](unsigned long n) const { return *(_M_start + n); }
};

struct CThemePaletteEntry {                        // PDB, 8 bytes
    unsigned long ThemeDefIndex;                   // +0
    bool          ThemePaletteIndexUsed;           // +4
};

struct CHeightMap {
    virtual ~CHeightMap();
    void*                          World;          // +0x04
    void*                          Map;            // +0x08
    unsigned long*                 WorldSeed;      // +0x0c
    CRetailVector<CHeightMapCell>  Cells;          // +0x10
    unsigned char                  _MapUpdateAreas[0x1e8];   // +0x1c
    long                           SizeX;          // +0x204
    long                           SizeY;          // +0x208
    __forceinline CHeightMapCell& GetCellIncludingExtraCells(long x, long y) { return Cells[(SizeY - y) * (SizeX + 1) + x]; }
};

template <class T> struct scoped_ptr {
    T* ptr;
    T* operator->() const { return ptr; }
};

struct CMap {
    unsigned char                      _pad00[0x1a6c];
    CRetailVector<CThemePaletteEntry>  ThemePaletteEntryList;   // +0x1a6c
    void*                              GameMap;                 // +0x1a78
    scoped_ptr<CHeightMap>             HeightMap;               // +0x1a7c
    void SetEngineThemeAtIncludingExtraCells(long x, long y, unsigned long themeDefIndex, long layer);   // 0x0081EF30
    void CopyPos(const CMap& source, const C2DCoordI& from, const C2DCoordI& to);
};

void CMap::CopyPos(const CMap& source, const C2DCoordI& from, const C2DCoordI& to)
{
    HeightMap->GetCellIncludingExtraCells(to.X, to.Y) = source.HeightMap->GetCellIncludingExtraCells(from.X, from.Y);
    for (long layer = 0; layer < 3; ++layer)
        SetEngineThemeAtIncludingExtraCells(to.X, to.Y,
            source.ThemePaletteEntryList[source.HeightMap->GetCellIncludingExtraCells(from.X, from.Y).EngineThemePaletteIndex[layer]].ThemeDefIndex,
            layer);
    HeightMap->GetCellIncludingExtraCells(to.X, to.Y).EngineBlend[0] = source.HeightMap->GetCellIncludingExtraCells(from.X, from.Y).EngineBlend[0];
    HeightMap->GetCellIncludingExtraCells(to.X, to.Y).EngineBlend[1] = source.HeightMap->GetCellIncludingExtraCells(from.X, from.Y).EngineBlend[1];
}
