// CHeightMap::Clear() -- reset every cell (including the extra row/column) to height 0, palette indices 1, blend {0xff, 0}, and flags Changed/Passable/ShorePoint/NavPreferred off, CameraPassable on
// Grade: DIFFER(123v109) -- constant-register choice: retail hoists 0 into ecx (`cmp eax,ecx` loop guards, `mov [eax],ecx`, `mov [eax+8],cl`) and stores 0xff as an immediate; the lane hoists 0xff into cl, tests with `test`, and forms the memset address in ebp (push ebp)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F6050, length 109 bytes. No callees.
// Shape: the three palette bytes are an inline `memset(.., 1, 3)` (`mov ebx,01010101h` word + byte); the
// Changed bit is cleared on its own before the other stores, the four later bit assignments merge into one
// `and bl,0E9h ; or bl,8`.
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

void CHeightMap::Clear()
{
    for (long y = 0; y < SizeYIncludingExtraCells; ++y)
        for (long x = 0; x < SizeXIncludingExtraCells; ++x) {
            CHeightMapCell& cell = Cells[SizeXIncludingExtraCells * y + x];
            cell.Changed = 0;
            cell.Height = 0;
            cell.EngineBlend[0] = 0xff;
            cell.EngineBlend[1] = 0;
            memset(cell.EngineThemePaletteIndex, 1, 3);
            cell.Passable = 0;
            cell.ShorePoint = 0;
            cell.CameraPassable = 1;
            cell.NavPreferred = 0;
        }
}
