# acctC lane, hand-back c6a (2026-11-13): 1 row matched, 4,540 bytes -- staged, NOT landed

| address | name | length | grade | test |
|---|---|---:|---|---|
| 0x00BB3E80 | CEngineClothRenderer::RenderClothPrimitive | 4,540 | RELOCATION_MATCH | PASS |

Re-verified by the lane coordinator against `FullDecomp` at 55262db83 (read-only `vland.py`, `LANDVERIFY_STRICT_HEADERS=1`, overlay =
only the changed headers the row includes; `coordverify.log`). No flags. Real length. No oracle file here. Three agents and about
350 grades (C2, C3, C6); the earlier open draft in `2026-11-13_acctC_c2a/open_00bb3e80` is superseded by this folder.

## What closed it: Lionhead's release-build ASSERT (new shared header `fable_assert.h`)
- The residue (operand order of `test`/`or` and load order in the 16 inline SetRenderState expansions 0x00BB46E0..0x00BB4E80) is a
  c2 record-id tie. It depends on the function's record count, including dead code that c2 deletes after numbering (agent C3's
  measurements, NOTES.md).
- `hdrdiff/NEW_fable_assert.h`: `FABLE_RETAIL_ASSERT(c)`. Under the new opt-in `FABLE_RETAIL_ASSERT_DEAD_ARM` it is the debug
  build's assert control flow (FableWin: DoErrorMessage -> IsDebuggerPresent -> int 3) behind `if (0)`; without the opt-in and
  always under `FABLE_PORTABLE` it is `((void)0)`. It emits no bytes. `GFAssertFailed` is a descriptive name (never emitted).
- The row uses the repo's DEFAULT SetRenderState / AddStateToUpdateList text (none of the fitted DESIRED / BOOKMARK / DIRTY
  opt-ins) plus nine `FABLE_RETAIL_ASSERT` statements.

## Decision and review item for the owner
**Accepted as inferred statements** (owner rule 2026-11-12): a compiled-out assert cannot change behaviour on any compiler, is
portable, and is what a Lionhead programmer plausibly wrote. What is evidenced and what is fitted, plainly:
- Evidenced: that the function had asserts (FableWin's debug body is 126 /Od instructions larger than this text), and the debug
  shape of ASSERT (FableWin disassembly, cited in the header).
- FITTED: the **count** (nine; the window that matches is 9..10) and the **conditions** (the pointers and ranges the function
  dereferences; not recovered). FableWin's size gap alone suggests about two debug asserts in this body, so the dead-arm macro body
  may weigh less than the real one did; the exact release form of Lionhead's macro is unknown.
- The same mechanism probably explains the fitted setter opt-ins of earlier landed render rows; nothing landed was touched.

## Header diffs (`hdrdiff/`)
16 diffs + 3 new headers (`fable_assert.h`, `C3DClothPrimitive.h`, `CEngineInternalPrimitiveMorphedStaticMesh.h`); table in
hdrdiff/README.md. New opt-ins only, moved asserts pinned. Whole-tree `equiv_check.py` still to run when applied. Several of these
headers overlap hand-back c2a (same overlay lineage): apply c2a first, then this.

## Update after the agent's final hand-back (same day)
- Header diffs refreshed from the agent's final overlay: it moved the row's local `CEngineClothRenderer` / `C3DClothPrimitive`
  models into shared headers and pinned 341 moved asserts (most from the c2a overlay lineage). Coordinator re-verified against that
  overlay: this row RELOCATION_MATCH PASS, and the c2a row 0x00651B04 also still RELOCATION_MATCH PASS. Apply THIS folder's
  header diffs in place of c2a's for the headers both touch.
- A measured point against a uniform release macro, for the owner's review: dead arms placed inside the inline SetRenderState /
  AddStateToUpdateList change the frame and inlining (4514 / 4704 bytes), although FableWin's SetRenderState has asserts. So either
  those helpers' asserts were compiled differently, or the dead-arm form is only an approximation of the real record effect.
- `open_00bb30a0/`: best source of the sibling 0x00BB30A0 Render2DClothPrimitive, DIFFER(3538v3538), two record-count sites left
  (0x00BB33B7 operand order, 0x00BB3D28 load order); the text has about 20-24 record units too many. Agent C8 continues it.
