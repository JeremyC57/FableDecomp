# Naming pass wave 29 (2026-11-01/02): 396 landed rows edited + 2 shared headers; 697/697 rechecked (10 MATCH + 687 RELOCATION_MATCH, all PASS)

## Inputs and index
- Brief naming/briefs/BRIEF_W29.md; QUEUE_W29_input.tsv (80 rows: 39 carried, 26 c698 and 15 c699 appended mid-wave); DEFERRED_W28_INPUT.tsv.
- idx/full.jsonl is HEAD: the wave 28 after.jsonl plus chunk 698 (idx/delta_c698.jsonl, 10 rows) and chunk 699 (idx/delta_c699.jsonl,
  10 rows), found by modification time (no git). Before hand-back nothing under rebuild/src, tests, include or build_candidates.ps1 is newer
  than idx/.c699_marker. idx/after.jsonl is HEAD plus ov_final. No c698 / c699 row is edited by this wave or includes an edited header.

## Commands (from $S/naming/w29, S exported, $S/wine/env.sh sourced)
1. `python3 scripts/compose29.py ov_final` runs scripts/hand29/h29_<fam>.py on fresh repo copies; writes runs/<fam>.rows.txt and ov_<fam>/.
2. `bash scripts/chain29.sh` (SKIP_RK=1 reuses the recheck): recheck of runs/final_plus.rows.txt = the 396 family rows + runs/hdrdep.rows.txt
   (301 unchanged landed includers of engine/CSpawnedFuncBase.h / engine/CWorld.h), 4 parts; mkindex, after.jsonl, qscan29, mkq29
   (+ mkq29_notes), mkdef29, mkbind29 (+ runs/reloc_bindings.after.tsv), bindings_multi_check (0), mkren29 + fixren29, mklabels29 + fixlab29.
New helpers: scripts/ovinc.py is now transitive (a repo header that includes an overlay-edited header is shadow-copied to <ov>.shadow/ with its
includes pointed at the overlay; shadows are never part of ov_final); scripts/includers.py (landed includers of a header, through other headers);
scripts/stubs.py (funclet stubs including a parent: none here); scripts/sfbh_probe.py (compile-and-classify the includers); scripts/cflags.py;
scripts/d7map.py / d7shape.py / d7id.py / d7aes.py (D7 identities: vtable, storer, slot 4, PDB). mkindex needs BS=1 when sources may not
compile (one front-end error drops the whole cl batch).

## Families (compose order; runs/<fam>.rows.txt)
- **sfbh (346 rows + engine/CSpawnedFuncBase.h)** -- scope 1, DECISIONS_W28 D5: the retail branch has the implicit destructor and Ego_r's
  CCharString Name / AssociatedRegionName and CCountedPointer<const NScript::CPredicateBase> PPredicate; contract as CEntityScriptBinding.h
  (includer defines CCharString and CCountedPointer<T> first; runs/sfbh_fixes.tsv: cp 221, cs 121, local 3, move 1). All 512 landed includers
  rechecked (logs/recheck_sfbh.log and again in logs/recheck_final.log). A CSpawnedFunc<T> ??_G built from it calls ~CSpawnedFuncBase out of
  line, as retail.
- **sfbr (14)** -- scope 1, chunk 696 decision 2: the CScriptProcessBaseRuntime / CSpawnedFuncBaseRuntime pair (virtual
  ~CSpawnedFuncBaseRuntime, in-TU RuntimeHookCDD410/420) replaced by the header's retail CSpawnedFuncBase; unused flattened includes dropped.
- **sfg (29)** -- scope 2, D7: the CSpawnedFunc1<T,CScriptThing> (26), CSpawnedFunc1<CHeroBoastsScript,CCharString> (1) and CSpawnedFunc2 (2)
  rows re-authored as the compiler's ??_G (MakeOne idiom, the 0x00ED7DB0 precedent); identities runs/sfg_table.tsv + FableWin.pdb. RENAMES 29.
- **gws (2 + engine/CWorld.h)** -- DECISIONS_W28 D1: CWorld.h declares ?GetWorldSeed@CWorld@@QBEPAKXZ; 0x00914C60 / 0x008A7E00 drop their
  GetPRandomSeed derived-struct stand-ins. 145 CWorld.h includers rechecked. (gen_class_headers.py needs the member: DECISIONS_W29 D5.)
- **zlib (2)** -- c698 rows 63-65: _z_inflateEnd / _z_deflateEnd (Z_PREFIX; both PDBs), CCompressorZlib_Retail dropped, Leaf lines added; the
  0x00A3A930 test stub renamed with its callee (the test links the row).
- **ewm (3)** -- c698 rows 49 / 50: ?CloseStaticMapFile@CEngineWorldMap@@QAEXXZ, private ?DeleteMap@CEngineWorldMap@@AAEXXZ, and the binder
  0x00B428E0 off its invented CEngineWorldMapRetail (private OpenStaticMaps(CEngineMap::STATIC_MAP_OPEN_STATE) / LoadWaterData). RENAMES 2.

## Not done (DEFERRED.tsv / DECISIONS_W29.tsv)
- D7's 121 CActiveEntityScript<T> rows (D1: VC7.1 RTM does not expand ~CActiveEntityScriptBase into the ??_G from the shared headers; the T of
  every row is proven in runs/d7aes.tsv) and 41 entity-script rows (D2) -- runs/deferred29_manual.tsv.
- The ??_G rows 0x0072DA40 / 0x00A399E0 (D3: emission site); 0x00699560 (CQuakeWorld GetRandom); scope 3 otherwise not reached (~CScriptThing
  and ~CTCBase binder families, c697 destructor families, most c698 rows, all c699 rows) and scope 4.

## Outputs
- QUEUE_W29.tsv: 80 rows -- 7 respelled, 5 respelled-partial (carried), 68 deferred. DEFERRED.tsv: 71 queue + 163 wave-29 findings + 438 carried
  (1 wave-28 entry resolved: runs/w28_deferred_resolved.tsv; holds are never resolved by the index test).
- BINDINGS.tsv: 9 RETIRE, 2 ADD, 0 CONFLICT; runs/reloc_bindings.after.tsv; multi-binding check 0. RENAMES.tsv 31. LABELS.tsv 33.
- DECISIONS_W29.tsv: 7 items. runs/hdrdep.rows.txt: the 301 unchanged header includers for the coordinator's recheck.
- Tests: only 00a3a930's test changed (its stub carries the bound name); the sfbr / sfg tests are self-contained and unchanged (bard precedent).
