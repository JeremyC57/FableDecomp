# acctC lane, hand-back c8a (2026-11-13): 2 rows matched, 8,078 bytes -- staged, NOT landed

| address | name | length | grade | test |
|---|---|---:|---|---|
| 0x00BB3E80 | CEngineClothRenderer::RenderClothPrimitive | 4,540 | RELOCATION_MATCH | PASS |
| 0x00BB30A0 | CEngineClothRenderer::Render2DClothPrimitive | 3,538 | RELOCATION_MATCH | PASS |

Re-verified together by the lane coordinator against `FullDecomp` at 55262db83 (read-only `vland.py`, `LANDVERIFY_STRICT_HEADERS=1`,
overlay = only the changed headers the rows include; `coordverify.log`: 2 wins, no RELOC_IDENTITY / RELOC_CONFLICT). No flags, real
lengths, no oracle here. **This folder supersedes the earlier hand-back c6a** (0x00BB3E80 alone, nine asserts), which is removed:
the two rows are one retail TU and share helper texts, so they must land together. Four agents (C2, C3, C6, C8).

## Mechanism: c2 record-count ties
The residue of both rows (operand order of `test`/`or` and load order inside inline SetRenderState expansions) is decided by how
many records the compiler holds: the whole-function count for the operand order (it moves in steps of 32), the count created before
a site for its load order. Code that emits no bytes still counts. NOTES.md / LESSONS.md have the measurements and the two meters.

## What matched them
Shared helper texts (both rows), evidence in brackets:
- FlushRender clears stream 0 with `PD3DDevice->SetStreamSource(0, 0, 0, 0)` instead of inline `SelectVertexBuffer(0, 0)` [neutral:
  same bytes; the inline call's folded branch costs about 34 records];
- SetTextureFactor builds `C4DVector(colour)` [FableWin has that constructor];
- new opt-ins `FABLE_RETAIL_C3DCLOTHPRIMITIVE_GETPARTICLEPROGRAM_GET`, `FABLE_RETAIL_C3DMESHINFO_PEEKTEXTUREBANK_MEMBER`, and
  GetCurrent2DAlphaMaskInfo with a named viewport-manager pointer [each: /Od size equals the FableWin debug build];
- the repo's DEFAULT SetRenderState / AddStateToUpdateList text (no DESIRED / BOOKMARK / DIRTY opt-ins);
- `fable_assert.h` (new shared header): `FABLE_RETAIL_ASSERT(c)`, Lionhead's ASSERT as a dead arm under the opt-in
  `FABLE_RETAIL_ASSERT_DEAD_ARM`, `((void)0)` otherwise and always under `FABLE_PORTABLE`. Emits no bytes.
Row texts: 0x00BB3E80 has 11 `FABLE_RETAIL_ASSERT`s; 0x00BB30A0 has 1 plus five named values (tint, tint_colour, factors, mesh,
self_illumination).

## Decisions taken by the lane coordinator, and review items for the owner
- **Accepted as inferred statements** (owner rule 2026-11-12): the dead asserts and the named values emit nothing that differs from
  retail, cannot change behaviour, and are portable. **What is FITTED, plainly:** the assert counts and sites (11 and 1; FableWin's
  size gap in 0x00BB3E80 alone suggests about two debug asserts), the five names in 0x00BB30A0, and the FlushRender spelling.
  Both rows sit on window edges (0x00BB3E80 holds at 0 extra dead locals and fails at 2; 0x00BB30A0 tolerates 6 more records, none
  fewer), so any later change to a shared helper text they expand needs both regraded.
- Against a uniform release macro: dead arms inside the inline SetRenderState / AddStateToUpdateList break the match although
  FableWin's setters have asserts. The dead-arm macro may be an approximation of the real record effect.
- FlushRender spelling: the landed row 0x00BA1B00 (another TU) keeps `SelectVertexBuffer(0, 0)`; this TU needs the raw call.
- An alternative text set also matches both rows (NOTES.md, "candD"); not chosen because FableWin's size is closer to this one.
- Hand-back c2a's row 0x00651B04 was re-verified against THIS overlay: still RELOCATION_MATCH PASS. Apply this folder's header
  diffs in place of c2a's where both touch a header. NAME_FIXES.tsv: 0x00BF0B20 outline helper name, the four spellings of global
  0x01436E84.

## Header diffs (`hdrdiff/`)
New opt-ins only, moved asserts pinned; table in hdrdiff/README.md. Whole-tree `equiv_check.py` to run when applied.

## Open
`open_00b783f0/`: 0x00B783F0 CEngineWaterRenderer::Render, DIFFER(9343v9343), same class of residue; agent C13 continues.
