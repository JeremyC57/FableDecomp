# NF265 header overlay (2026-11-22)

Three headers, each copied from the repo working tree at the start (orig/engine/<name> = that copy, byte-equal to the repo at hand-back).
Every change is an added `#ifdef` block under a NEW member-specific macro; no existing line is edited apart from the assert lines
pin_asserts.py rewrote (see CLandscapeBackgroundPatch.h). With none of the new macros defined, every header is token-identical to orig/.

## engine/CEngineLandscapeMemoryManager.h (retail branch, FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER)
- 17 added lines just before `void* AllocVirtual(...)` (inside `public:`), three declaration-only opt-ins:
  - `FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEINDEXBUFFER`: `void DeleteIndexBuffer(CIndexBuffer* buffer);` 0x00BDA090,
    `?DeleteIndexBuffer@CEngineLandscapeMemoryManager@@QAEXPAVCIndexBuffer@@@Z` (Ego_r 009c25e0, FableWin 02cb7350);
  - `FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEVERTEXBUFFER`: `void DeleteVertexBuffer(CVertexBufferWin32* buffer);` 0x00BDA0D0
    (Ego_r 009c2640, FableWin 02cb7450);
  - `FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETETEXTURE`: `void DeleteTexture(CTexture* texture);` 0x00BDA110 (Ego_r 009c26a0,
    FableWin 02cb7490).
- pin_asserts.py: 0 pinned (the header's asserts are FABLE_STATIC_ASSERT_AT with fixed numbers).
- Users here: 0x00BDA110 (DELETETEXTURE), 0x00BE5C30 (DELETETEXTURE), 0x00BE6090 (all three).

## engine/CEngineLandscapeRenderer.h (retail branch, FABLE_RETAIL_CENGINELANDSCAPERENDERER)
- 5 added lines right after the `FABLE_RETAIL_CENGINELANDSCAPERENDERER_ZSORT` block: `FABLE_RETAIL_CENGINELANDSCAPERENDERER_UNREGISTERSUBPATCHFORZSORTING`
  declares `void UnregisterSubPatchForZSorting(long& index);` 0x00B677A0, `?UnregisterSubPatchForZSorting@CEngineLandscapeRenderer@@QAEXAAJ@Z`
  (Ego_r 009a8aa0, FableWin 02c87c80). Declaration only, no layout change. Only in the retail class (the flattened generated branch is untouched).
- pin_asserts.py: 0 pinned (FABLE_STATIC_ASSERT_AT). User here: 0x00BE6090.

## engine/CLandscapeBackgroundPatch.h (retail branch, FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH)
- 5 added lines right after the `GetEdgeStrip` declaration: `FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH_CLEARTEXTURES` declares `void ClearTextures();`
  0x00BE5C30, `?ClearTextures@CLandscapeBackgroundPatch@@QAEXXZ` (Ego_r 009c5c00, FableWin 02cdccf0). Declaration only.
- pin_asserts.py: **36 pinned**: the 6 retail-branch and 30 flattened-branch `FABLE_STATIC_ASSERT(...)` lines below the block became
  `FABLE_STATIC_ASSERT_AT(<old line>, ...)`, so their typedef names (`fable_static_assert_<line>`) stay what they were at HEAD.
  User here: 0x00BE5C30.

## Proof
- equiv_check (lane_merge/equiv_check.py, exact text): a hard-link copy of rebuild/include with these three files replaced v the repo
  rebuild/include, over 1,470 landed rows (every row whose source or test includes one of the three headers or a header that includes
  them, transitively, plus every row that includes `../src/compiled/`; tools/eqrows.py, work/eq_rows.txt): **0 differ** (work/equiv.tsv
  empty). Control: the same tool with a planted default-path token in CLandscapeBackgroundPatch.h flags 0x00BE5C60 and 0x00BE7BE0 (DIFF),
  so the check is live. The 36 pinned asserts are covered by this (the retail-branch users 0x00BE5C60 / 0x00BE7BE0 / 0x00BF6930 / 0x00BF7BC0 /
  0x00B68DA0 preprocess identically); 0x00BE5C60 was also regraded with the overlay on (recheck_final.log, RELOCATION_MATCH PASS).
- Landed includers (grep of rebuild/src/compiled): CEngineLandscapeMemoryManager.h 25, CLandscapeBackgroundPatch.h 23, CEngineLandscapeRenderer.h 81
  (plus tests and headers that include them). None defines any of the five new macros (grep of rebuild/: only mentions are in this
  hand-back's rows and NF263's NOTES suggestion), so none changes. NF264's overlay (agentNF264/hdr: CCombatActionBase.h,
  CCombatAction_StrikeResponseScorpionKing.h) has none of these headers.
- Every row that uses an overlaid header includes it itself, first (NF213 lesson).
