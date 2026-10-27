// CEngineSceneGrid::GetTotalCellCount(unsigned long x, unsigned long y) -- count the scene-grid cells that exist under one cell: 0 when the cell was never created, else 1 plus the totals of its four children on the next level down
// Leaf: ?GetTotalCellCount@CEngineSceneGrid@@AAEKKK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B91E70, 129 bytes, thiscall `ret 8`.
// Identity (proven): Ego_r.pdb `?GetTotalCellCount@CEngineSceneGrid@@AAEKKK@Z` (private, unsigned long (unsigned long,
//   unsigned long)); in Ego_r donor order it follows GetCellTotalPrimitiveCount / GetCellRenderMask and precedes
//   MovePrimitivesUp, exactly as retail 0x00B91D20 / 0x00B91DC0 (landed) / 0x00B91E70 / 0x00B91F00 (landed). It calls
//   itself on LevelDown for the 2x2 children. The catalog row is a gap-scan placeholder.
// Layout (Ego_r.pdb CEngineSceneGrid, no engine header): vptr +0, Level +4, LevelUp +8, LevelDown +0xC, Size +0x10,
//   GridCells CArray<CEngineSceneGridCell*> +0x14 (as the landed GetCellRenderMask 0x00B91DC0).
// Shape: `if (!cell) return 0;` first (the early-out keeps its own epilogue), the count starts at 1 (ebp), the two
//   counted loops run dy outer / dx inner with unsigned counters (`jb`).
class CEngineSceneGridCell;

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                         // +0x0
    _Tp* _M_finish;                                        // +0x4
    _Tp* _M_end_of_storage;                                // +0x8
    _Tp& operator[](unsigned int __n) { return *(_M_start + __n); }
};
}

template <class T> class CArray : public std::vector<T> { };

class CEngineSceneGrid {
public:
    virtual ~CEngineSceneGrid();
    unsigned long                  Level;                  // +0x04
    CEngineSceneGrid*              LevelUp;                // +0x08
    CEngineSceneGrid*              LevelDown;              // +0x0C
    unsigned long                  Size;                   // +0x10
    CArray<CEngineSceneGridCell*>  GridCells;              // +0x14
private:
    unsigned long GetTotalCellCount(unsigned long x, unsigned long y);
};

unsigned long CEngineSceneGrid::GetTotalCellCount(unsigned long x, unsigned long y)
{
    if (GridCells[Size * y + x] == 0)
        return 0;
    unsigned long count = 1;
    if (LevelDown) {
        for (unsigned long dy = 0; dy < 2; ++dy)
            for (unsigned long dx = 0; dx < 2; ++dx)
                count += LevelDown->GetTotalCellCount(x * 2 + dx, y * 2 + dy);
    }
    return count;
}