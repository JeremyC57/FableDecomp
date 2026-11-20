# Re-port amendment: 0x004FC4D0 CWorldMap::ClipAndConvertWorldMapAreaToLocalArea (p25N, 2026-11-20)

Landing order 1 of 2 (then `batches/004feb70`). It replaces the landed file's local models with the shared-header text that 0x004FEB70
copies verbatim.

- **Grade:** RELOCATION_MATCH (142/142). Landed file: rebuild/src/compiled/00/4f/CWorldMap_ClipAndConvertWorldMapAreaToLocalArea_004fc4d0.cpp
  (`src_004fc4d0.diff`).
- **Strict vland:** passes (`vland.log`, `--recheck-landed`): LANDVERIFY_STRICT_HEADERS=1, FABLE_HDR_OVERLAY=`hdr/`, fresh LANDVERIFY_WORK,
  behaviour PASS.
- **No device left:** the old file's `#pragma auto_inline(off)` round a local ClipTo copy is gone, with no pragma or flag in its place.
  ClipTo now comes from C2DBoxI.h's new add-only opt-in FABLE_RETAIL_C2DBOXI_CLIPTO_BODY, an inline header body.
  - Ego_d.pdb puts ClipTo's line table in a header (file id 2736, lines 97..102), not in CWorldMap.cpp (file id 4704), although its debug
    copy sits in CWorldMap.obj's module stream. So every TU that calls it compiles the body. c2 keeps it out of line, and the linker keeps
    one COMDAT at retail 0x0049C810 (landed; the same four statements).
  - The ClipTo COMDAT in this object compiles to 0x0049C810's bytes (cmpcomdat SAME, 92 B).
- **Text:** Ego_d.pdb CWorldMap.cpp lines 1387..1402.
  - Locals: map_number, area, map (`const CMapInfo&`), clipped_area, clip_area. The landed names were info / box / mapArea.
  - Line sizes at /Od for 1389..1401 are 12 / 12 / 12 / 11 / 11 / 12 / 12 / 15, and this text gives exactly the same
    (`evidence/od_lines_row.txt`). Line 1388 is 15 in Ego_d and 18 here: Dinkumware's out-of-line operator[] against the CArray's.
  - ClipTo: Ego_d 98..101 = 48 / 54 / 52 / 54, ours the same. Parameter name `box` (Ego_d).
- **Defines:** FABLE_RETAIL_C2DBOXI_COPY, the existing memberwise user copy constructor opt-in that names this row. Without it the copy is
  148 B against 142.
  - The define block is CWorldMap.cpp's common block, shared with 0x004FEB70.
  - Per-define ablation on 0x004FEB70 (`batches/004feb70/evidence/`): every define is needed except C2DBOXI_COPY, which is kept for this
    row. CCHARSTRING and SCOPED_PTR were byte-neutral and are dropped.
- **Tests:**
  - The landed test (`rebuild/tests/00/4f/..._004fc4d0_test.cpp`, a self-contained model) still passes.
  - The new `test/004fc4d0.cpp` links the REAL object: 4 maps × 6 boxes, covering inside, outside, straddling, degenerate, and inverted
    input. It checks the clamp-then-shift result, the returned pointer, and that nothing else is written.
- **Mutants:** 9/9 KILLED, including two mutants in the new header ClipTo body.

## Header change (`hdr/`, `hdr.diff`; add-only, no `-` lines)
| header | opt-in | content |
|---|---|---|
| C2DBoxI.h | **new** FABLE_RETAIL_C2DBOXI_CLIPTO_BODY (inside the FABLE_RETAIL_C2DBOXI retail branch) | `inline void C2DBoxI::ClipTo(const C2DBoxI& box)`, the landed 0x0049C810 body with Ego_d's parameter name |

- newly_flagged_rows: none (`newflags.py hdr/`).

## For the coordinator
The landed ClipTo row 0x0049C810 still defines ClipTo non-inline over its own local C2DBoxI. Once this opt-in exists, that row could include
C2DBoxI.h with FABLE_RETAIL_C2DBOXI_CLIPTO_BODY. Its body is already the same four statements, and only the parameter name differs (other
-> box). That would be one more re-port; it is not done here.
