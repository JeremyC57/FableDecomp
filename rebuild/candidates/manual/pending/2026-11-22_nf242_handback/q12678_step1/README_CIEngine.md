## engine/CIEngine.h (q12678 step 1: the four PDB slot declarations become the default; landing nf240 ruling 5)
- Base: the repo working tree (`orig/engine/CIEngine.h`).
- Change (a default-path change by the entry's instruction, not an opt-in): the `#ifdef FABLE_RETAIL_CIENGINE_RENDER / _SCREENPOS /
  _CONVERTSCREENPOS / _CONVERTWORLDPOS ... #else VirtualNN_<addr>() = 0; #endif` blocks lose their `#ifdef` / `#else` / placeholder
  lines: slot 8 `Render(const CEngineInterpolationInfo&, unsigned long)`, slot 11 `ConvertScreenPosToWorldPos(const C2DVector&, C3DVector&)
  const`, slot 12 `ConvertScreenPosToWorldPosAtViewDistance(float, const C2DVector&, C3DVector&) const`, slot 13
  `ConvertWorldPosToScreenPos(const C3DVector&, C2DVector&) const` (all pure, as before; CEngine overrides Ego_r.pdb 0x009B39A0 / 0x009AE850 /
  0x009AE730 / 0x009AE5F0, FableWin.pdb 0x02C9D1D0 / 0x02C9FA10 / 0x02C9F7B0 / 0x02C9F4A0). The `class CEngineInterpolationInfo;`
  forward declaration loses its `#ifdef FABLE_RETAIL_CIENGINE_RENDER` guard. The four macros are now no-ops (17 rows / tests still define
  them; harmless; engine/CEngine.h lines 63 / 67 still gate CEngine's two overrides on two of them, untouched: step 2's business).
  The top comment's "descriptive VirtualNN_" sentence is replaced. Only inside `#ifdef FABLE_RETAIL_CIENGINE` (the default flattened branch
  is untouched). No layout change, no slot change.
- pin_asserts (repo tree v hdr/): pinned 4 (the four asserts below the removed lines keep their old typedef names via
  FABLE_STATIC_ASSERT_AT(168 / 188 / 189 / 190, ...)). assert_names_check (repo tree v a copy with this header): 0 mismatches.
- Includers: CIEngine.h is included by engine/CEngine.h (closure: 2 headers); 149 landed rows have a source or test in that closure
  (128 sources and 21 tests include CIEngine.h directly; 116 files define FABLE_RETAIL_CIENGINE).
- Equivalence (equiv_check, repo tree v a copy with only this header): 122 differing files over 108 rows (107 sources, 15 tests;
  work/equiv2.tsv). Every one of the 108 rows is rechecked with VC7.1 (vland --recheck-landed, work/c0[0-5].log; the ten placeholder
  tests replaced by rows/<addr>_test.cpp). No landed row SOURCE spells a placeholder (grep), so a source's object can change only where
  it emits a vtable with one of the four slots overridden by a placeholder name: none does.
- The ten tests that overrode the placeholders by name (0x004A1840 0x008764E0 0x004BC1A0 0x00AEEBA0 0x0064AE80 0x00646F67 0x0064A639
  0x00650E98 0x0064E978 0x0054EF00) are respelled to the PDB signatures (`{}` bodies of the bool slots become `{ return false; }`).
