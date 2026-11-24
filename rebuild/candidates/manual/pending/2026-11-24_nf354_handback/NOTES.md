# NF354 hand-back: the CCutsceneCastMap stand-in binders of map::operator[] 0x00CD3D2E (Account A, 2026-11-24)

Folder: `$S/w22/agentNF354/` (S = /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad).
Files: `NOTES.md`, `STATUS.tsv` (one line per binder), `RENAMES.tsv`, `NAME_FIXES.tsv` (empty), `batches/b1` (land_scripted run dir),
`alt/rows` (strict-ALL_EQ alternate, also in batches/b1/alt), `evidence/`, `tools/`. `work/` is scratch (oracle files with retail bytes,
objects, probes). No header changed, no repo write, no git. Every NF354 lock is released (`work/locks.log`).
STB = CScriptGameResourceObjectScriptedThingBase. REAL4 = the 4-argument STLport `??A?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@U?$less@...@std@@V?$allocator@...@4@@std@@QAEAAV...@ABVCCharString@@@Z` the owner defines since nf350 b1.

## Census (evidence/CENSUS_00cd3d2e.tsv)
- Retail side: 140 landed rows whose retail body calls 0x00CD3D2E (tools/callers.py over the catalog bytes).
- 106 + 1 take std::map from stlport/fable_map.h, 18 use a local 4-parameter std::map model (both bind REAL4), 15 use a row-local stand-in
  struct `CCutsceneCastMap`. What each leg proves: reloc_bindings has only lines 16686 / 21869 as stand-in ??A lines for 0x00CD3D2E; the
  object legs below show the spelling each compiled row binds; for the 36 text-only rows, none declares any `operator[]` in non-comment text,
  so their only operator[] is std::map's from fable_map.h (4 template parameters: REAL4).
- Object leg: the 15 stand-in rows compiled at HEAD by NF354 (census_head_syms.tsv: leaf relocation to 0x00CD3D2E -> symbol, plus every
  external naming a CCutscene*Map / RunCutsceneMacro); 89 more from NF350's HEAD objects (idxscan_nf350_hobj.tsv: 70 DEF REAL4, 19 UND
  REAL4, no stand-in). Text only (36): 31 fable_map.h rows (header text closure: 4 template parameters), 4 Account B claims (0x00D1A740,
  0x00E25F70, 0x00E9E420, 0x00EC4A00), 0x00E8A2B0 (fable_map.h without the switch: declaration-only REAL4; locked by NF353, not compiled).
- Spellings: unit u1 `??ACCutsceneCastMap@@QAEAAVCScriptGameResourceObjectScriptedThingBase@@ABVCCharString@@@Z` (14 rows);
  unit u2 `??ACCutsceneCastMap@Row00e80d60@@QAEAAUCScriptGameResourceObjectScriptedThingBase@1@ABVCCharString@@@Z` (0x00E80D60).
- None of the 15 is an NF353 emitter (none includes fable_map.h at HEAD); NF353 holds 0x00CD3D2E, 0x00CBFB7D and 0x00E8A2B0, which I did not compile.

## Result per binder (STATUS.tsv)
| group | rows | verdict |
|---|---|---|
| only the ??A names the stand-in | 0x00E2A320 | ACCEPT (b1) |
| UND-only, type also in clear + RunScriptedCutscene | 0x00E2A900 | ACCEPT (b1), 4 renames |
| ctor / dtor called out of line | 0x00DC21F0 (probed) | skipped: no declaration-only map() switch; probe DIFFER 786 v 750 + ~20 COMDATs |
| explicit Construct() / Destroy() calls (0x00CDBF70 / 0x00CDBFB0) | 0x00D246C0, 0x00DDD030 | skipped, not measured: the same missing ctor switch |
| DEF ??0 / ??1CCutsceneCastMap | 0x00D823A0 + 0x00E3E720 (probed), 0x00E12F20, 0x00E292C0, 0x00F01480, 0x00F02210, 0x00F0BB10, 0x00E1CD40, 0x00EE2DB0 | skipped: NF318 Q5 + NF320 Q2 (probe grade stays RELOCATION_MATCH, but drops the made-up ctor / dtor COMDATs and adds 18) |
| row-scoped value type | 0x00E80D60 | skipped-port (T3 nf311 already) |

b1 proofs (both rows): vland --recheck-landed RELOCATION_MATCH PASS HEAD / new / alt (evidence/vland_b1_*.log); objsec_rn with `--expect`
= exactly the intended pairs (ALL_EQ except `.drectve` = HEAD tokens + `/DEFAULTLIB:"libcp"`, every other section equal; alt strict
ALL_EQ); both pick-any checkers identical HEAD v new, run through pk.py (API with the row's Leaf and length) AND the pk_all CLI with
`--leaf=<Leaf line> --row-length=<catalog length>` (evidence/pkcli_*), FABLE_EXE / FABLE_ROOT exported; reach column compared per the
coordinator's 2026-11-24 ruling: no reach or leafref flip on any copy (tools/pkdiff.py prints REACH_FLIP lines; none). No COMDAT added or
dropped (DEF sets equal under the renames). Link (evidence/link_u1_*.txt, /FORCE, owner object = NF350's N_00cd3d2e.obj, whose source is
byte-identical to the landed 0x00CD3D2E): HEAD leaves the stand-in ??A unresolved, new resolves it, 0 duplicates.
Second sources of the respelled callees (0x00E2A900): RunCutsceneMacro = Ego_r.pdb name set (0x00CA0C13) + the owner 0x00CBFB7D's Leaf
text (equal byte for byte) + landed J035 binders (line 17905 witness 0x00D7AA30). The two map clears are NOT in either PDB name set: their
sources are the STLport member on the PDB map types plus one landed RELOCATION_MATCH binder each (0x00CFF9A0 / 0x00D7C470, reloc_bindings
lines) - see decision 5. The clear owners 0x00CD3699 / 0x009AACE0 still
define OnKill stand-ins, so those two stay unresolved before and after (NF302 Q2; NAME_FIXES c602 already queues them).
Tests unchanged: 0x00E2A320's test names no changed symbol; 0x00E2A900's test is a standalone model with its own static `RunScriptedCutscene`
stub (NF284 D2). Prechecks: rows clang PASS; 0x00E2A900's test fails clang only in VC7.1 headers, `cl /Zs` PASS (work/pre_zs.log).

## Table (batches/b1/TABLE_CHANGES.tsv; apply_table --dry: 2 delete, 2 rewitness, 0 misses, no new multi-target)
- delete `?RunScriptedCutscene@@...` -> 0x00CBFB7D and `?clear@CCutsceneCastMap@@QAEXXZ` -> 0x00CD3699: 0x00E2A900 was the only binder.
- rewitness `??ACCutsceneCastMap@@...` -> 0x00D246C0 and `?clear@CCutsceneArgMap@@QAEXXZ` -> 0x00D823A0 (both still UND in those HEAD objects).
- **Note for the landing:** land_scripted.py keeps only TABLE_CHANGES lines whose witness column is a landed row, so it will DROP the two
  rewitness lines (new witnesses 0x00D246C0 / 0x00D823A0 are not in this batch). Apply those two by hand (or the lines stay witnessed by
  0x00E2A900, whose object no longer references them).

## Decisions needed
1. **libcp:** b1 takes the brief's header route, so `.drectve` gains `/DEFAULTLIB:"libcp"` (fable_containers.h includes <new>). I read NF318 Q1
   / the nf350 b1 ruling on 0x00CD3CAE as covering this (map stand-in moved onto stlport/fable_map.h under FABLE_RETAIL_STLP_MAP; all four
   conditions hold). If not, `alt/rows` (declaration-only local std::map, strict ALL_EQ, RELOCATION_MATCH PASS; no SHARED_HEADER warning
   for std::map in vland) is the swap-in; it is not marked ACCEPT on its own.
2. **Unit with movable members only:** u1's spelling keeps 12 binders after b1 (owner already respelled, so nothing HEAD resolved goes
   unresolved). May b1 land with only 0x00E2A320 / 0x00E2A900 (NF302 Q5 read as "the movable binders of the unit")?
3. **Ctor declaration-only opt-in** (0x00DC21F0 / 0x00D246C0 / 0x00DDD030): would an add-only `FABLE_RETAIL_STLP_MAP_CTOR_DECL_ONLY` in
   stlport/fable_map.h (declaration of `map()` only, like DTOR_DECL_ONLY) be ruled? 0x00DC21F0 additionally needs `rand` re-declared
   `extern "C"` on the header route (stdlib.h clash), a callee respell needing its own second source.
4. **The nine ??0/??1CCutsceneCastMap rows** as one T3 type-port unit (family move with a drop test, like the NF353 emitter port)? Probes
   keep the grade but fail COMDAT sets SAME.

5. **0x00E2A900's two clear respells (NF318 Q4):** `?clear@?$map<CCharString,STB>` (0x00CD3699) and `?clear@?$map<CCharString,CCharString>`
   (0x009AACE0) appear in neither PDB name set; each rests on the STLport header member + one landed binder (0x00CFF9A0 / 0x00D7C470) and the
   landed `FABLE_RETAIL_STLP_MAP_CLEAR_DECL_ONLY` opt-in (NF178). So 0x00E2A900 is **ACCEPT pending this ruling**. 0x00E2A320 does not depend
   on it, but UNITS.tsv ties both rows to u1: if the ruling goes against, drop 0x00E2A900 from RESULTS / UNITS / rows.txt and the four
   TABLE_CHANGES lines (they all concern 0x00E2A900), and 0x00E2A320 lands alone.
6. **No `// Fidelity: scaffolding, not retail` block** on the two rows: the pilots (0x00D58490 / 0x00EA59E0 / 0x00E8EF80 / 0x00626750...)
   carry it because MAP_INDEX_DECL_ONLY removed a copy their TU emitted; here HEAD was already declaration-only (the stand-in declared
   operator[] without a body), no copy is removed, and ~50 other landed MAP_*_DECL_ONLY users carry no such block. Add it if you read the
   convention otherwise (a comment-only change).

## Rule notes
- Lock 0x00CD3D2E / 0x00CBFB7D / 0x00E8A2B0 held by NF353: not compiled; the owner object used for the link is NF350's existing object.
- pk.py's pk_all import path pointed at NF350's tools copy; my copy imports the repo's lane_tools/pk_all/pk_all.py (identical file).
- Self-reported slips: the first HEAD chain failed on that import before compiling anything (no object written), then reran; one
  `lock.sh ... release` loop ran without `S` exported (it refused with "S not set"; rerun with S); one stray no-op command
  (`python3 .../usr/bin/true`) in a shell line.
- Coordinator ruling 2026-11-24 (reach flips) applied: tools/pkdiff.py prints REACH_FLIP for any copy whose leaf / reach column changes;
  none for either row, and the pk_all CLI outputs HEAD v new (and HEAD v alt) are byte-identical.
- Staleness: both rows' landed source and test were re-compared (cmp) with orig/ at hand-back time: unchanged.
- All evidence the files cite is in evidence/ (probes/, expect_*.tsv, grades.log, pre.log, pre_zs.log, locks.log, census inputs).
