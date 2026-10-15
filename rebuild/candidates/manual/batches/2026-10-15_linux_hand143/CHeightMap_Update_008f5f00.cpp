// CHeightMap::Update() -- clear the per-cell Changed bit over every queued map update area (rows MinY..MaxY, columns MinX..MaxX, the cell at (SizeY - y) * (SizeX + 1) + x) and empty the queue
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F5F00, length 147 bytes. No callees.
// Shape: the area index lives in the `push ecx` frame word; the area walks in edi (+0x24 = &area.MaxX); the
// row start is computed once per row and the bound re-read after every byte store (aliasing).
// Layout: Ego_r.pdb CHeightMap with the retail 12-byte vector (every member after Cells sits 4 bytes low):
// Cells.first +0x10, MapUpdateAreas[30] +0x1c, NoMapUpdateAreas +0x1fc, Changed +0x200, SizeX +0x204,
// SizeY +0x208, SizeXIncludingExtraCells +0x20c, SizeYIncludingExtraCells +0x210 (as the landed SetSizeZAt
// 0x00825440). CHeightMapCell (PDB, 12 bytes): Height, EngineThemePaletteIndex[3], EngineBlend[2] and five
// one-bit flags in the byte at +9.
#include <string.h>
struct CHeightMapCell {
    float         Height;                          // +0x00
    unsigned char EngineThemePaletteIndex[3];      // +0x04
    unsigned char EngineBlend[2];                  // +0x07
    unsigned char Changed : 1;                     // +0x09 bit 0
    unsigned char Passable : 1;                    //        bit 1
    unsigned char ShorePoint : 1;                  //        bit 2
    unsigned char CameraPassable : 1;              //        bit 3
    unsigned char NavPreferred : 1;                //        bit 4
};
struct C2DBoxI { long MinX, MinY, MaxX, MaxY; };
struct CHeightMapCellVector {                      // retail std::vector<CHeightMapCell>: first / last / end
    CHeightMapCell* first;
    CHeightMapCell* last;
    CHeightMapCell* end;
    CHeightMapCell& operator[](long index) { return *(first + index); }
};
struct CHeightMap {
    void*                World;                    // +0x00 (no vtable: the PDB's leading int is padding)
    void*                _04;
    void*                Map;                      // +0x08
    unsigned long*       WorldSeed;                // +0x0c
    CHeightMapCellVector Cells;                    // +0x10
    C2DBoxI              MapUpdateAreas[30];       // +0x1c
    long                 NoMapUpdateAreas;         // +0x1fc
    bool                 Changed;                  // +0x200
    long                 SizeX;                    // +0x204
    long                 SizeY;                    // +0x208
    long                 SizeXIncludingExtraCells; // +0x20c
    long                 SizeYIncludingExtraCells; // +0x210
    void Update();
    void Clear();
};

void CHeightMap::Update()
{
    for (long i = 0; i < NoMapUpdateAreas; ++i) {
        C2DBoxI& area = MapUpdateAreas[i];
        for (long y = area.MinY; y < area.MaxY; ++y) {
            CHeightMapCell* cell = &Cells[(SizeY - y) * (SizeX + 1) + area.MinX];
            for (long x = area.MinX; x < area.MaxX; ++x, ++cell)
                cell->Changed = 0;
        }
    }
    NoMapUpdateAreas = 0;
}
