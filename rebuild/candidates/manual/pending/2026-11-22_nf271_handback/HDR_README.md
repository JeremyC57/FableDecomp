# NF271 overlay (2026-11-22)

## engine/CEngineLandscapeMemoryManager.h (base: orig/engine/CEngineLandscapeMemoryManager.h = the repo copy at the start of this session)
- New add-only declaration opt-in `FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_FREEVIRTUALALIGNED` inside the retail branch
  (`FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER`), after the `_DELETETEXTURE` block: declares
  `void FreeVirtualAligned(void* block);` = `?FreeVirtualAligned@CEngineLandscapeMemoryManager@@QAEXPAX@Z` (retail 0x00BD9CA0; Ego_r 009c2430 /
  FableWin 02cb7a40). Used by row 0x00BD9CA0 and its test (they define the macro and include the header first).
- No existing line changed; the default (flattened) branch and the retail branch without the macro are token-identical.
- pin_asserts.py: pinned 0 (every assert in the header was already `FABLE_STATIC_ASSERT_AT`).
- Proof: equiv_check over the header's closure (tools/eqrows.py: 1,399 rows, every row whose source or test includes it transitively plus every
  `../src/compiled/` includer), repo tree v a copy with this overlay: 0 rows differ (evidence/equiv.tsv empty). Control: the same copy with a
  token planted in the retail branch's FreeVirtual line: 28 lines differ (evidence/equiv_ctl.tsv: every retail-branch includer, src and test).
- Landed includers: the opt-in is named by no repo file (grep), so it changes none of them.
- No other naming agent overlays this header (NF270's hdr/ holds only engine/CCreatureActionBase.h).
- The opt-in comment says "rows 0x00BD9CA0 / 0x00BFAAE0": only 0x00BD9CA0 (and its test) defines the macro. 0x00BFAAE0 binds the same
  spelling through its own row-local manager model (in-place declaration, it cannot take the retail branch while it models CMovableResource
  locally); it is named there as the binder the declaration serves. Left as proven by equiv_check.
