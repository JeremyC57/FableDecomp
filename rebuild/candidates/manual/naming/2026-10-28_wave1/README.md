# Naming pass wave 1 (2026-10-28)

The 742 NAME_FIXES handed back by chunks 520-541 merge into 643 retail addresses: 592 proven (478 exact mangled matches
in a PDB name list; the rest SETTLED_NAMES, RTTI vtables, STLport 4.6 template spellings, static CRT) and 51 probable
(DECISIONS.tsv; ten conflicts settled in CONFLICTS.md). Proven names are in
`ghidra_out/labels_naming_wave1_2026-10-28.tsv`.

Applied (315 rows, 369 files; recheck.log: 315 of 315 RELOCATION_MATCH, tests PASS):
- 0x008574D0 `?SetCombatAnimation@CCombatActionBase@@QAEXABVCCharString@@@Z` (66 rows; was SetAnimationName /
  SetAnimation / SetPDef).
- 0x00993CA0 `?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z` (private; 245 rows): base-less
  CMemoryDataInputStream models derive from an empty CDataInputStream that declares it; polymorphic models and view
  structs reach it through a friend.
- Pre-existing RELOC_CONFLICTs: 0x0048F881's `_s_Default` is its own file-local array (renamed `s_TextGuiSkipScene`);
  `??1CBase@@UAE@XZ` replaced by `CBaseClassNonCopyable` (0x0099A300 `??1CBaseClassNonCopyable@@MAE@XZ`) in 0x00A5B260 and
  `CBaseClass` in 0x00CF8A70; 0x0099CF10 calls `GetAsCharArray` (0x0099AF10), and the `??BCWideString@@QBEPBGXZ` binding
  is retargeted to its real address 0x0099AF00.
Bindings: five stale spellings removed, one retargeted (BINDINGS.tsv). Overlap with the operator= pass composes (the
operator= transform re-run over these files makes no edits).

Deferred (DEFERRED.tsv): 573 proven decisions over the row cap (wave 2, WAVE2_ROWS.tsv has the exact row bindings),
51 probable, 16 needing a catalogue-wide wchar_t decision, the rest of 0x00993CA0.
