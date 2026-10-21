// CGameMap::Resize(long sizeX, long sizeY, long offsetX, long offsetY) -- refuse sizes whose cell count overflows a long; otherwise build a fresh cell vector of the new size (every cell reset), copy across the cells that overlap the old map at the given offset, swap it in and record the new size
// Grade: DIFFER(414v434) -- first draft: the min/max-by-reference bounds, loop counters homed in the dead offset argument slots and the destination index arithmetic (rows stored bottom-up, 7-byte cells) are not yet reproduced; the reset for_each uses an LTCG register convention
// Leaf: Resize
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B75A0, 432 bytes, `ret 0x10`.
// Callees: vector<CGameMapCell>(n) 0x006B7D60, for_each(mem_fun_ref(&CGameMapCell::Reset)) 0x006B7AF0 (LTCG register
//   convention: result in ecx, first in edx), CGameMapCell::Reset 0x006B7490, vector::swap 0x006B7E20, free 0x00BFEA14.
// Layout: local projection -- Cells (STLport vector) +0x04, SizeX +0x1c, SizeY +0x20 (retail packing of the PDB order);
//   a cell is 7 packed bytes.
#include <stdlib.h>
#pragma pack(push, 1)
struct CGameMapCell {
    unsigned long  A;
    unsigned short B;
    unsigned char  C;
    void Reset();                                        // 0x006B7490
};
#pragma pack(pop)
struct CMemFunRef { void (CGameMapCell::*Fn)(); };
CMemFunRef __fastcall ForEachCell(CGameMapCell* first, CGameMapCell* last, CMemFunRef fn);   // 0x006B7AF0
struct CCellVector {
    CGameMapCell* Start;
    CGameMapCell* Finish;
    CGameMapCell* EndOfStorage;
    CCellVector(long count);                             // 0x006B7D60
    ~CCellVector() { if (Start) free(Start); }
    void swap(CCellVector& other);                       // 0x006B7E20
};
template <class T> inline const T& Max(const T& a, const T& b) { return (a > b) ? a : b; }
template <class T> inline const T& Min(const T& a, const T& b) { return (a < b) ? a : b; }
struct CGameMap {
    void*       VTable;
    CCellVector Cells;                                   // +0x04
    void*       Map;                                     // +0x10
    void*       World;                                   // +0x14
    void*       WorldSeed;                               // +0x18
    long        SizeX;                                   // +0x1c
    long        SizeY;                                   // +0x20
    void Resize(long sizeX, long sizeY, long offsetX, long offsetY);
};

void CGameMap::Resize(long sizeX, long sizeY, long offsetX, long offsetY)
{
    __int64 total = (__int64)sizeX * sizeY;
    if (total < 0 || total > 0x7fffffff)
        return;
    CCellVector cells(sizeX * sizeY);
    CMemFunRef reset = { &CGameMapCell::Reset };
    ForEachCell(cells.Start, cells.Finish, reset);
    long srcX = Max(-offsetX, 0L);
    for (long x = Max(offsetX, 0L); x < Min(offsetX + sizeX, SizeX); ++x, ++srcX) {
        long srcY = Max(-offsetY, 0L);
        for (long y = Max(offsetY, 0L); y < Min(offsetY + sizeY, SizeY); ++y, ++srcY)
            cells.Start[srcY * sizeX + srcX] = Cells.Start[(SizeY - y - 1) * SizeX + x];
    }
    Cells.swap(cells);
    SizeX = sizeX;
    SizeY = sizeY;
}
