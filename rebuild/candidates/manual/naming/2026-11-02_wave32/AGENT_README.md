# Naming pass wave 32 (2026-11-02): 103 landed rows edited + 1 shared header (opt-in branch); recheck of the family rows and every landed includer

## Inputs and index
- Brief naming/briefs/BRIEF_W32.md; QUEUE_W32_input.tsv (157 rows: wave 31 deferred / partial + c703 rows, + 33 c704 rows appended mid-wave);
  DEFERRED_W31_INPUT.tsv.
- idx/full.jsonl is HEAD: built after idx/.start_marker (after the chunk 704 landing), then chunk 705 (landed 02:50, 14 rows: idx/delta_c705.jsonl,
  runs/delta_c705.rows.txt; 4 replaced, 10 appended) merged in. No chunk 705 row is a family row or an includer of engine/CAIStateGroupBase.h
  (includers.py still 65). ov_final was recomposed and the whole chain (recheck included) rerun after that landing (logs/chain32_final3.log),
  so the recheck is against the current tree and reloc_bindings.tsv. idx/after.jsonl is HEAD plus ov_final.

## Commands (from $S/naming/w32, S exported, $S/wine/env.sh sourced)
1. `python3 scripts/compose32.py ov_final` runs scripts/hand32/h32_<step>.py on fresh repo copies (header from hdr/); writes runs/<fam>.rows.txt.
2. `RK=1 bash scripts/chain32.sh`: recheck (3 parts, runs/final_plus.rows.txt = 103 family rows + 65 includers of engine/CAIStateGroupBase.h
   (runs/hdrdep.rows.txt) + the 7 0x00CA6370 funclets), index of ov_final, after.jsonl, qscan32, mkq32 (+ mkq32_notes), mkdef32
   (+ runs/deferred32_manual.tsv), mkbind32 (+ runs/reloc_bindings.after.tsv), bindings_multi_check, mkren32 + fixren32, mklabels32 + fixlab32.

## Families (compose order; runs/<fam>.rows.txt)
- **aisg (header + 73)** -- hdr/CAIStateGroupBase.h: retail opt-in FABLE_RETAIL_CAISTATEGROUPBASE (CAIStateGroupBase : CBaseClass, 0x14, ctor
  0x008FD255; DECISIONS_W32 D1). h32_aisg: 69 generated CAIStateGroup_* ctors re-authored on it with the chunk 703 CBaseIntelligentPointer /
  CIntelligentPointer<T> model, PDB base ctors and Ego_r members (D2); h32_aisx: own row 0x008FD255, 0x008FF1F8 (protected ??0...UsesSeat@@IAE),
  0x00900241 (WorkBase<T>); h32_aisz: 0x009031BA WhisperPotionCombat / 0x0090310E HoverHornetQueen on CAIStateGroup_EnemyCreatureBase 0x008FF720 +
  STLport vectors (retires SZeroed / CBase_008ff720).
- **aisd (25)** -- derived-only CAIStateGroup ctors: RTTI base chain, the called base declared with its PDB ctor, FableWin-only intermediates implicit.
- **dshow (5)** -- DX9 strmbase (UNICODE): 0x00CA7360 CBaseVideoRenderer ctor (5 vtables 0x012BF5D8 / 598 / 584 / 560 / 54C,
  ResetStreamingTimes 0x00CA52A0 slot 47), 0x00A3B510 CTextureRenderer on it, 0x00CA61D0 CBaseRenderer ctor (vtables 0x012BF498 / 458 / 444),
  0x00CB0190 CAMEvent ctor, 0x00CA6370 ~CBaseRenderer (StopStreaming 0x00CA4E30, ClearPendingSample 0x00CA4BF0, ~CBaseFilter 0x00CAAAC0,
  ~CAMEvent 0x00CB01B0). Replaces the CVolume* / CVolumeDerived* / CEvent_* models.

## Results
- Recheck (logs/recheck_final.log): 175 rows, 2 MATCH + 173 RELOCATION_MATCH, all PASS. The four HEAD failures (0x00692230 / 0x006922C0 /
  0x004D9861 / 0x009E1440) are not in any rows file. Family warnings: SHARED_HEADER_AVAILABLE CBaseObject on the 4 strmbase rows (D7).
- BINDINGS.tsv: RETIRE 31 / ADD 25 / KEEP 2; bindings_multi_check 0. RENAMES.tsv 4 (CVolume* / CEvent* files). LABELS.tsv 29.
- QUEUE_W32.tsv: 10 respelled, 13 respelled-partial, 134 deferred (carried). DEFERRED.tsv: 722 entries.
- DECISIONS_W32.tsv: D1-D7, D12 hold, N1 not reached, N2 disclosure.
