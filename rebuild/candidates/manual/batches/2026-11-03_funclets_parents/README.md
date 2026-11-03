# EH unwind funclets landed as slices of their landed parents (2026-11-03)

Pool: the 609 manifest rows named `Unwind@…` / `Catch@…` that were not in the catalog (PLAN §2 / §5 step 2).
Result: **391 landed**, 218 parked. `STATUS.tsv` has one line per row (status, reason, real parent, FuncInfo,
push site, block offset, length, grade, the `__ehhandler$` symbol of the object block that matched, evidence).

| Status | Rows | Reason |
|---|---:|---|
| LANDED | 391 | slice of the landed parent's object; RELOCATION_MATCH + behaviour PASS |
| PARKED parent-unlanded | 183 | the real parent (from the FuncInfo, below) is not landed |
| PARKED parent-unlanded-catch | 4 | try-map catch handlers of unlanded parents (0x004A1C41, 0x008F92EF, 0x00C29A30, 0x00C29A3C) |
| PARKED not-an-EH-funclet | 19 | no FuncInfo references the address: ordinary functions with a Ghidra `Unwind@`/`Catch@` label |
| PARKED real-parent-not-in-manifest | 6 | the FuncInfo is pushed by an unmapped EH function (not the preceding row) |
| PARKED parent-TU-emits-different-funclet | 6 | the landed parent's object emits different unwind code for that state |

## The parent is NOT the preceding manifest row
For the 586 rows at 0x0121xxxx the nearest preceding non-funclet manifest row is `_strrev` 0x00F3FE1C; the
`@xxxxxxxx` name suffix is not the parent either. The parent comes from the retail EH tables (`tools/ehmap.py`):
every FuncInfo (magic 0x19930520, 834 in Fable.exe) lists its unwind actions (and, through the try map, its catch
handlers); the `__ehhandler$` thunk `mov eax, FuncInfo ; jmp __CxxFrameHandler` names the FuncInfo; the function
whose EH prologue does `push __ehhandler$` is the parent. 834 FuncInfos, 834 thunks, 834 push sites, one each.
The push site must lie inside the parent row's catalog extent: six rows failed that check because the push is in an
unmapped function after the preceding landed row (real parents 0x00C319B0, 0x00C4B160, 0x00CA97A0, 0x00CAA860,
0x00CAA8D0, 0x00CAA9C0). One of them (0x01215D9E) even graded RELOCATION_MATCH against the preceding row's TU, whose
own one-funclet block is byte-identical: it was not landed.

## Mechanism (same as the 145-row precedent, `headers/2026-11-02_funclet_includes/`)
Each landed source has no code: a comment block (parent, FuncInfo, thunk, push site, block offset) and
`#include "../src/compiled/<parent source>"`; flags are the parent's (`/EHsc`, or `/GX` for the CBasePin/CDynamicOutputPin
rows); the test is the parent's test with its `<parent>_TEST PASS` line renamed to `<funclet>_TEST PASS`. The grader
(`verify_and_land.py`, `funclet_offset`) compares `length` bytes at the block offset of a `$L` block of the parent's
object; `recheck.py` reads the offset back from the `funclet block offset` comment (20-row sample rechecked: 20/20).
The offset is the funclet address minus the lowest unwind action of its FuncInfo; the length runs to the next action
or to the `__ehhandler$` thunk (all 586 equal the Ghidra `coverage.tsv` sizes).

Extra gate (`tools/offcheck2.py`): the grader's extractor falls back to "any offset of any `$L` block", which can
match an identical `mov ecx,[ebp-10h] ; jmp` slice of another function's block in the same TU. Each landed row was
also required to match at the **expected** offset of exactly one block whose length equals the retail block length,
and that block's `__ehhandler$` symbol names the parent (`leaf-ok` where the parent has a Leaf line). 11 rows of the
0x00C22520 parent (CEngineVSConstantBlockSet dtor) were first reported as "moved" for that reason and pass the strict check.

## Parked: parent TU emits different funclets (parent model defects; the parents' own grades do not cover `$L` blocks)
- 0x01213B0A (parent 0x00C2D3B0): retail `add ecx,320h`, parent object `add ecx,324h`.
- 0x012134AB (parent 0x00C287E0): retail `add ecx,43Ch`, parent object `add ecx,440h`.
- 0x012134E3 (parent 0x00C28AE0): retail block is one funclet (21 bytes); the parent object's block is 49 bytes.
- 0x01216DE0 (0x00CAA790 ~CVolume) and 0x01216E60 (0x00CAAAC0 ~CJoystickDX): retail unwinds a subobject at +0
  (`mov ecx,[ebp-10h] ; jmp 0x00CA7780`, a base); the parents model a member at +4 (`add ecx,4`).
- 0x012173E0 (parent 0x00CB1890): retail `mov ecx,[ebp-10h] ; add ecx,0Ch`, parent `mov ecx,[ebp-18h] ; add ecx,14h`.

## Coordinator to-do
- Labels: `ghidra_out/labels_funclets_2026-11-03.tsv` (391 rows, `_global::Unwind@<addr>`, the catalog spelling of
  the landed 0x0121xxxx rows), then the usual `apply_labels.py` / `sync_catalog_names.py` / `sync_manifest_from_catalog.py`.
- Parent Leaf lines that name no symbol (their grades fell back to the largest function): 0x00C2A9B0
  (`??0CTCDPhysicalObstruction@` on a destructor; the object's symbol is `??1CTCDPhysicalObstruction_00c2a9b0@@QAE@XZ`)
  and 0x00C62920 (Leaf has `PBD`, the object has `PBG`).
- Six unmapped EH functions to add to the manifest (real parents above).
- The 19 not-an-EH-funclet rows are ordinary functions to rename and author (e.g. 0x00442D50 is a deleting destructor).
