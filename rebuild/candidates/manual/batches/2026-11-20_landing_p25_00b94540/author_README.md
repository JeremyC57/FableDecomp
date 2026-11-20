# 0x00B94540 CEngineSceneMap::ResizeLevels (p25A)

**Grade:** RELOCATION_MATCH (664/664). The strict vland run passes (LANDVERIFY_STRICT_HEADERS=1, overlay `hdr/`; `vland.log`), and so does the real-object behaviour test. Mutants: 24 of 24 killed.

**Oracle note:** the hand2c oracle row is 672 bytes, which includes the 8 int3 padding bytes after `ret 4`. The function is 664 bytes (the next function, CEngineSceneMap::Update, starts at 0x00B947E0). `oracle.tsv` carries the 664-byte row. Against the 672-byte row the same build grades DIFFER(664v672).

## Source

`src/00b94540.cpp` replaces the hand280 draft, which was labelled `CEngineScene::ResizeGrid` and used local models. The new source uses shared headers only: engine/CEngineSceneMap.h (plus the add-only opt-in below), engine/CCountedPointer.h, engine/CArray.h and the shared STLport containers.

**Identity:** `?ResizeLevels@CEngineSceneMap@@AAEXJ@Z`, from three sources:
- Ego_r.pdb;
- FableWin.pdb (S_GPROC32 in Engine_Scene_Grid.obj);
- Ego_d.pdb (debug length 392, parameter new_levels).

The catalog label `ResizeGrid` was descriptive.

**Structure:** follows the Ego_d.pdb block tree and C13 line table (`egod_lines/00b94540.tsv`, VC10 /Od sizes):
- The clamp is one 13-byte line: `if (new_levels < 1) new_levels = 1;`.
- Both loop tests and both level reads go through `GridLevels.size()`.
- The new grid is built inside the counted-pointer constructor (line 1320, 82 B).
- last_grid is read back from new_grid after push_back (11 B: one Get).
- above_grid = GridLevels[level - 1].
- In the shrink loop, last_grid = GridLevels[level] comes before above_grid.
- Locals and scopes follow Ego_d: block 1 has last_grid, new_grid, above_grid, level; block 2 has last_grid, above_grid, level.

The private grid constructor (out of line at 0x00B93EB0) is expanded here together with its GridCells.resize.

**Flags:** `/d2inlT150`.

## Inline budget (line 2) and ablation

There are two devices: the per-row flag and an existing shared STLport opt-in, FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL (stlport/fable_containers.h). That opt-in makes `__destroy_aux` a noinline /Gr function with its body visible. Retail calls `std::__destroy_aux` 0x00B96920 from `_M_clear` at 0x00B946F2. Landed rows 0x004B93E0 and 0x00B3CB30 already use the same opt-in.

| change from the matching source | grade |
|---|---|
| no `/d2inlT` flag (default budget) | DIFFER(491v664): `_M_insert_overflow` is called out of line. Retail expands it inside push_back, 0x00B9465D..0x00B94718. |
| `/d2inlT100` | DIFFER(491v664) |
| `/d2inlT120` .. `/d2inlT500` | RELOCATION_MATCH (the window) |
| FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL removed, T150 | DIFFER(491v664) |
| FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL removed, T200 | DIFFER(735v664): the range destructor is expanded too. |

The FABLE_RETAIL_CENGINESCENEMAP_RESIZELEVELS opt-in is a layout/declaration opt-in, not an inline device. Without it the row does not compile: the default CEngineSceneGrid declaration is incomplete.

## Header overlay (`hdr/`, `hdr.diff`)

The overlay is add-only: 29 `+` lines. The single `-` line is pin_asserts.py rewriting the existing `FABLE_STATIC_ASSERT(sizeof(CEngineSceneMap)==0x28)` as `FABLE_STATIC_ASSERT_AT(68, ...)` (pinned 1).

engine/CEngineSceneMap.h, opt-in `FABLE_RETAIL_CENGINESCENEMAP_RESIZELEVELS`:
- **Layout:** the Ego_r.pdb CEngineSceneGrid layout at retail offsets, 0x20 bytes. The PDB says 36 because its CArray is 16 bytes; the retail STLport CArray is 12.
- **Fields:** base CBaseClassNonCopyable (vtable 0x012A3E58), Level +4, LevelUp +8, LevelDown +0xC, Size +0x10, GridCells `CArray<CEngineSceneGridCell*>` +0x14.
- **Constructor:** the private `CEngineSceneGrid(unsigned long)`. Its body is the one the landed 0x00B93EB0 row writes (initialiser list, then `Size = 1 << level; GridCells.resize(Size * Size, 0);`).
- **Declarations:** the private MovePrimitivesDown (0x00B93860) and MovePrimitivesUp (0x00B91F00).
- **Friendship:** the map stays the grid's friend.
- **Includes:** CBaseClassNonCopyable.h and a forward declaration of CEngineSceneGridCell. The full CEngineSceneGridCell.h pulls in CEnginePrimitiveHandle and does not compile in this TU.

`newly_flagged_rows.tsv` lists six landed rows of this TU that model these classes locally (0x00B93EB0, 0x00B949A0, 0x00B91F00, 0x00B93860, 0x00B95A30, 0x00B94F90). They are re-port candidates; none of their bytes change.

## Test (`test/00b94540.cpp`, links the real row object)

The test links the row together with the TU's own COMDATs: the expanded grid constructor, DeleteData, push_back / _M_insert_overflow / pop_back, the noinline `__destroy_aux`, and the CEngineSceneGrid vtable with its own scalar deleting destructor.

- **Fixtures** use the native layout: the map's GridLevels vector at +0x18, 0x20-byte grids, and 12-byte count blocks {RefCount, DeleteFunc, Data}.
- **Stubs:**
  - operator new / delete: malloc / free with a live counter.
  - The CBaseClassNonCopyable ctor marks the object; its dtor records the deletion. The dtor is reached through vtable slot 0, i.e. the TU's real `??_G` called by DeleteData.
  - `_M_fill_insert` appends n copies.
  - MovePrimitivesDown / Up record the call order and the grid.
- **Runs** (8):
  - grow 1→3, with two reallocations;
  - grow 3→4 in spare capacity;
  - shrink 4→2;
  - clamp 0→1;
  - clamp -5 (no-op);
  - equal (no-op);
  - grow 1→2;
  - shrink 2→1.
- **Checks per run:**
  - every level's Level, Size, cell count and zeroed cells;
  - the LevelUp / LevelDown chain;
  - count-block refs, deleter and data;
  - the exact call order: grow = Down on the level above each new grid; shrink = Up on the top grid, then its deletion, top first;
  - the cleared slots past the new end.
- **End check:** every grid and count block the row made was freed through operator delete.
- **Mutants:** `mutants/00b94540.tsv` holds 24 one-decision mutations: 21 of the row source and 3 of the overlay's constructor. All are killed (`mutants/00b94540.out`).

## vland warnings (labels DB, not source names)

- `RELOC_NAME`: DeleteData 0x00B94F90 is labelled `_global::DeleteViaVtable0_00b94f90`.
- `RELOC_NAME`: `__destroy_aux` 0x00B96920 is labelled `_global::DestroyRange_CountedPointer_00b96920`.

Both are covered in NAME_FIXES.

## NAME_FIXES (`NAME_FIXES.tsv`)

- **0x00B94F90:** `?DeleteData@?$CCountedPointer@VCEngineSceneGrid@@@@SIXPAX@Z`. Evidence: this row's relocation, plus FableWin's Engine_Scene_Grid.obj listing. Caveat: DeleteData<CEngineSceneMap> in the same TU has the same bytes and may be ICF-folded to this address.
- **0x00B95A30:** `?Reset@?$CCountedPointer@VCEngineSceneGrid@@@@QAEXPAVCEngineSceneGrid@@@Z`. Evidence: FableWin TU listing, and the Initialise row calls it on GridLevels[0].
- **0x00B96920:** the STLport `__destroy_aux<CCountedPointer<CEngineSceneGrid>*>` name. Candidate, one source: the compiled COMDAT name. FableWin is Dinkumware.
- **0x00B94540:** the catalog label ResizeGrid becomes `?ResizeLevels@CEngineSceneMap@@AAEXJ@Z`.
