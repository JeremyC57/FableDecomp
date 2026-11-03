# Naming pass wave 34 (2026-11-03): cx12 respells, small pass-12 second spellings, chunk 718-721 hand-backs queued

Inputs: the PRIORITY rows of `naming/NAME_FIXES_QUEUE.tsv` with no ` done]` marker, worked in this order: cx12 (rows 4318-4382), the small
pass-12 `queued_correct_open` targets (`batches/2026-11-03_codex_pass12/SECOND_SPELLINGS_cx12.tsv`), then c718 rows; chunk 718-721
NAME_FIXES (all rounds) appended to the queue as `c718`..`c721` (103 rows, 2 duplicates skipped). One agent; scratch in `$S/wave34`.
No file under `rebuild/include/` was edited; no row of `2026-11-03_decisions/RETAIL_ASM_ROWS_IN_FLIGHT.tsv` was touched.

## Counts
- Queue rows worked: 63 (cx12 59 + c718 4); closed (` [2026-11-03: wave 34; done]`): 55; partial (annotated): 10; deferred: 10 rows /
  14 entries (`DEFERRED.tsv`); decisions: 1 (`DECISIONS_W34.tsv`, the g_hklCurrent data spelling). 103 c718-c721 rows and 7 `w34` rows added.
- Landed rows respelled: 186 sources; 8 tests rewritten / made self-contained (0x0041A4C0, 0x004067C0, 0x00401780, 0x00494410, 0x009FD100,
  0x00404440, 0x00B239C0, 0x005EEDCF). Recheck (lane_tools/recheck.py + vland --recheck-landed, 4 splits): 186 / 186 PASS = 185 RELOCATION_MATCH + 1 MATCH
  (`logs/grades_all_runs.tsv`); 357 unchanged xref binders rechecked only to read what they emit (357 RELOCATION_MATCH PASS).
- Renames: 9 (`RENAMES.tsv`; definers re-authored or respelled first). Labels: 15 (`LABELS.tsv` = `ghidra_out/labels_naming_wave34_2026-11-03.tsv`).
- Bindings: 132 RETIRE / 37 ADD (`BINDINGS.tsv`), applied with `coordinator/apply_bindings.py`; `bindings_multi_check.py` = 0 after each step.

## What changed (STATUS.tsv T1-T33)
CPersistContext::Transfer / TransferVector member templates; CStringParser family (callback `void (__fastcall*)(const CCharString&)`, PeekNextItemType,
ReadAsString*); NResourceDirectoryNames / NLocalisation / CSystemRegistry / CUserProfileManager front-end chain; CChunkedFileChunk /
CChunkedFileSuperChunk protected ctors (48 rows); NScript class / nested-enum spellings; the NUISystem component ctors (16 rows: bases in
`namespace NUISystem` taking `long`, `_Retail` stand-ins renamed); CFileLargeThemePalette; CReplaceSerialise<CFileFormatLevel,COldFileFormatLevel3>
as the 5-byte CRTP base; std::vector<long> / <CCharString> / <float> / <CScriptThing> instances (explicit specialisations of the local stand-ins,
`scripts/vecspec*.py`); CTCHeroStats::MirroredCriminalStatus; NProfileTimer / NProgressDisplay; CTCInventoryAbilities const + EHeroAbility;
CPlayer::SetPlayerCharacter; NCameraModes::CScriptModuleTransitionBetweenPoints; 0x0055A890 = CDraggable::GetDataFromDef (row renamed).

## Bindings method (`scripts/mkbind.py`)
RETIRE (s, t) only when no wave row emits s after the respell (final recheck pairs) AND every other landed xref binder of t either was rechecked
and does not emit s (`logs/grades_unchanged_binders_357.tsv`) or (not rechecked) has a source that does not name s. ADD the respelled symbols
only. Four symbols stay bound because an unworked binder still emits them (`logs/mkbind2.out` KEEP): ??1CCharStringVector (0x005F05E5,
0x00859A70), the old 3-argument Transfer<bool> spelling (0x00676BE0), IsPerformingScriptTask non-virtual (0x00DD6440 ...), LoadFromFile(CAFile*)
(0x00521AE0, invented class, deferred).

## Not done here
`DEFERRED.tsv` (header / re-author / family needs, exact need per row), `DECISIONS_W34.tsv`, and the new `w34` queue rows: 0x00661EA0 and 0x005CC300
names (T31 kept their established spellings to avoid binding one symbol to two addresses), the generated definers' Leafs (0x00569F20, 0x00569FE0,
0x0042AEDA, 0x00528760, 0x00694EF0, 0x00568850, 0x00487B80), and the NUISystem vtable sub-object base names. The large pass-12 families (6-78
second spellings over hundreds of binders: CCharString / CBasicString / STLport instances) and most c718-c721 rows are not reached.
