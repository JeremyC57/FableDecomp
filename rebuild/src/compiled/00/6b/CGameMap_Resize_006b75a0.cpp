// CGameMap::Resize(long width, long height, long x_offset, long y_offset) -- refuse a size whose cell count overflows a long; otherwise build a fresh cell vector of the new size with every cell cleared, copy across the cells the old map shares with it (the old map shifted by the offsets; rows are stored bottom-up), assign it to Cells and record the new size
// Leaf: ?Resize@CGameMap@@QAEXJJJJ@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B75A0, 434 bytes, thiscall, no frame pointer, `ret 10h`.
// Identity: FableWin.pdb ?Resize@CGameMap@@QAEXJJJJ@Z 0x024A0160; Ego_d.pdb parameters width, height, x_offset, y_offset and locals
//   overflow_check (__int64), new_cells, to_x, from_x, to_y, from_y, to_index.
// Shape: FableWin 0x024A0160 statement for statement (its PREfix ASSERT on width*height and the to_index < size() ASSERT compiled
//   out): the __int64 overflow check, vector<CGameMapCell> new_cells(width * height), for_each(.., mem_fun_ref(&CGameMapCell::Clear)),
//   the two loops over std::max / std::min bounds (to_* from max(0, -offset), from_* from max(0, offset)), to_index =
//   (height - 1 - to_y) * width + to_x, new_cells[to_index] = GetCell(C2DCoordI(from_x, from_y)); then Cells = new_cells and the size.
// Callees: vector<CGameMapCell>(size_type) 0x006B7D60, for_each<CGameMapCell*, mem_fun_ref_t<void, CGameMapCell> > 0x006B7AF0
//   (__fastcall COMDAT), CGameMapCell::Clear 0x006B7490 (address only; FableWin pushes ?Clear@CGameMapCell@@QAEXXZ), vector<CGameMapCell>
//   ::operator= 0x006B7E20, free 0x00BFEA14.
// TU: the game-map TU is /Gr (its free template COMDATs are __fastcall: for_each 0x006B7AF0, __copy 0x006B79A0, __uninitialized_copy
//   0x006B7B70, uninitialized_fill_n 0x006B7CC0, the landed rows). FABLE_RETAIL_STLP_VECTOR_C: STLport's _vector.c is in the TU, so
//   operator='s body is seen but called out of line (without it the implicit member-wise assignment is emitted: 436v434;
//   declaration-only operator=: 432v434). Same-TU COMDATs graded from this source with /Gr: operator= 0x006B7E20, vector(size_type)
//   0x006B7D60 (needs FABLE_RETAIL_CGAMEMAP_RESIZE's private cell members: the PDB's access makes `_Tp()` write nothing),
//   uninitialized_fill_n 0x006B7CC0, __uninitialized_fill_n 0x006B7C10, __uninitialized_copy 0x006B7B70, __copy 0x006B79A0: all
//   MATCH / RELOCATION_MATCH. Without /Gr the row still matches but operator= does not (331v323).
#define FABLE_RETAIL_CGAMEMAP
#define FABLE_RETAIL_CGAMEMAP_RESIZE
#define FABLE_RETAIL_STLP_VECTOR_C
#include "engine/CGameMap.h"
#include "stlport/fable_algorithm.h"

void CGameMap::Resize(long width, long height, long x_offset, long y_offset)
{
    __int64 overflow_check = (__int64)width * (__int64)height;
    if (overflow_check < 0 || overflow_check > 0x7fffffff)
        return;
    std::vector<CGameMapCell> new_cells(width * height);
    std::for_each(new_cells.begin(), new_cells.end(), std::mem_fun_ref(&CGameMapCell::Clear));
    for (long to_x = std::max(0L, -x_offset), from_x = std::max(0L, x_offset); from_x < std::min(SizeX, width + x_offset); ++from_x, ++to_x) {
        for (long to_y = std::max(0L, -y_offset), from_y = std::max(0L, y_offset); from_y < std::min(SizeY, height + y_offset); ++from_y, ++to_y) {
            long to_index = (height - 1 - to_y) * width + to_x;
            new_cells[to_index] = GetCell(C2DCoordI(from_x, from_y));
        }
    }
    Cells = new_cells;
    SizeX = width;
    SizeY = height;
}