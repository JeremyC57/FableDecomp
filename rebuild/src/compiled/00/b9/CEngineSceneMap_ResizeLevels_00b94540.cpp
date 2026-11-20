// CEngineSceneMap::ResizeLevels(long new_levels) -- at least one level: while there are fewer grid levels than asked, append a new CEngineSceneGrid of the next level (2^level cells per side, Size*Size empty cell pointers) held by a counted pointer, link it below the level above (LevelUp / LevelDown) and let the level above move its primitives down; while there are more, let the top level move its primitives up, unlink it from the level above and drop it
// Inline-budget: the per-row flag /d2inlT150 (retail expands vector<CCountedPointer<CEngineSceneGrid> >::_M_insert_overflow inside push_back, 0x00B9465D..0x00B94718, no call; default T: called out of line, DIFFER(491v664); window T120..T500) and the STLport header opt-in FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL (retail calls the /Gr range destructor std::__destroy_aux<CCountedPointer<CEngineSceneGrid>*> 0x00B96920 from _M_clear, `call 0x00B96920` at 0x00B946F2 with the tag in a dead argument slot; without it T150 calls _M_insert_overflow, DIFFER(491v664), and T200 expands the range destructor, DIFFER(735v664)).
// Leaf: ?ResizeLevels@CEngineSceneMap@@AAEXJ@Z
// Flags: /d2inlT150
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B94540, 664 bytes (+ int3 padding), thiscall, `ret 4`; speed TU (frameless, `sub esp,0Ch`).
// Identity: Ego_r.pdb ?ResizeLevels@CEngineSceneMap@@AAEXJ@Z (private; the catalog label CEngineSceneMap::ResizeGrid and the draft's
//   CEngineScene::ResizeGrid were descriptive), called by the landed CEngineSceneMap::Update 0x00B947E0. Ego_d.pdb: debug length 392,
//   parameter new_levels (long); { last_grid, new_grid (CCountedPointer<CEngineSceneGrid>), above_grid, level (long) };
//   { last_grid, above_grid, level }.
// Shape (Ego_d.pdb C13 lines 1314..1338, egod_lines.tsv, VC10 /Od sizes): the clamp is one line (1315, 13 B: cmp / jge / mov);
//   both loop tests and both level reads go through GridLevels.size() (1317 20 B, 1319 14 B, 1329 16 B, 1331 17 B: `add ecx,18h ;
//   call`, not GetCurrentLevels); the new grid is built inside the counted-pointer constructor (1320, 82 B); last_grid is read back
//   from new_grid after push_back (1322, 11 B: one Get call) and above_grid is GridLevels[level - 1] (1323, 28 B); in the shrink loop
//   last_grid = GridLevels[level] (25 B) comes before above_grid (28 B). The grid constructor (private, out of line at 0x00B93EB0) is
//   expanded here with its GridCells.resize.
// Callees: operator new 0x00BFEA1A (0x20 grid, 0xC count block), CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0, memmove
//   0x00BFEAE6 (the shrink arm of the inline resize), vector<CEngineSceneGridCell*>::_M_fill_insert 0x00B97250, malloc 0x00BFEA0E /
//   free 0x00BFEA14 (the inline _M_insert_overflow), std::__destroy_aux 0x00B96920, CEngineSceneGrid::MovePrimitivesDown 0x00B93860 /
//   MovePrimitivesUp 0x00B91F00, operator delete 0x00BFE9BC. Data: vtable 0x012A3E58 (CEngineSceneGrid), the count block's deleter
//   CCountedPointer<CEngineSceneGrid>::DeleteData 0x00B94F90.
// Overlay (add-only opt-in): engine/CEngineSceneMap.h FABLE_RETAIL_CENGINESCENEMAP_RESIZELEVELS (the Ego_r.pdb CEngineSceneGrid layout,
//   its private constructor body as the landed 0x00B93EB0 writes it, MovePrimitivesDown / MovePrimitivesUp).
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL
#define FABLE_RETAIL_CENGINESCENEMAP_RESIZELEVELS
#include "engine/CEngineSceneMap.h"

void CEngineSceneMap::ResizeLevels(long new_levels)
{
    if (new_levels < 1) new_levels = 1;
    while (GridLevels.size() < (unsigned long)new_levels) {
        long level = GridLevels.size();
        CCountedPointer<CEngineSceneGrid> new_grid(new CEngineSceneGrid(level));
        GridLevels.push_back(new_grid);
        CEngineSceneGrid* last_grid = new_grid.Get();
        CEngineSceneGrid* above_grid = GridLevels[level - 1].Get();
        last_grid->LevelUp = above_grid;
        above_grid->LevelDown = last_grid;
        above_grid->MovePrimitivesDown();
    }
    while (GridLevels.size() > (unsigned long)new_levels) {
        long level = GridLevels.size() - 1;
        CEngineSceneGrid* last_grid = GridLevels[level].Get();
        CEngineSceneGrid* above_grid = GridLevels[level - 1].Get();
        last_grid->MovePrimitivesUp();
        above_grid->LevelDown = 0;
        GridLevels.pop_back();
    }
}