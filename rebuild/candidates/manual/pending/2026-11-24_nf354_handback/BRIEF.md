# Agent NF354 (Account A naming queue, 2026-11-24): move the CCutsceneCastMap stand-in binders of 0x00CD3D2E onto the real map symbol

Standing brief: /home/user/FableDecomp/rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md. Read it in full; NF302 Q5, NF318 Q1 /
Q5, NF320 Q2, NF326 Q3 and the NF340 scope line matter most. Lock name: NF354. Folder: $S/w22/agentNF354/.

Read first:
- batches/2026-11-24_landing_nf350_b1/README.md. The owner 0x00CD3D2E now defines the 4-argument STLport `??A` (the exact symbol is in
  that README and in NAME_FIXES_QUEUE line 534).
- NAME_FIXES_QUEUE.tsv lines 534 and 571 (tags 534 / 571): binders that call 0x00CD3D2E through made-up spellings
  `??ACCutsceneCastMap@@QAEAAVCScriptGameResourceObjectScriptedThingBase@@ABVCCharString@@@Z` and
  `??ACCutsceneCastMap@Row00e80d60@@...`.
- T3 line nf311 0x00CD3D2E: binders 0x00E2A900 / 0x00E80D60 bind through MADEUP.
- NAME_FIXES land1120nf178 (0x00CD3D2E / 0x00CBFB7D stand-in callers).

## Scope
1. Census every landed row whose object references 0x00CD3D2E through a made-up / stand-in spelling (any `CCutsceneCastMap`-style
   class, row-local scopes, FREE / C stubs). Sources: reloc_bindings.tsv lines targeting 00cd3d2e, a source grep, and a scan of the
   compiled objects.
2. Move each binder onto the real 4-argument `std::map<CCharString,CScriptGameResourceObjectScriptedThingBase,...>::operator[]`:
   - via stlport/fable_map.h under the landed switch FABLE_RETAIL_STLP_MAP;
   - with `FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY` where the map's operator[] body would otherwise be emitted, so the row does not gain a
     NEQ copy.
   Per binder, prove:
   - grade unchanged;
   - objsec ALL_EQ under the rename (a drop of a made-up stand-in COMDAT in a MOVE is held by NF318 Q5; report it rather than
     dropping);
   - both pick-any checkers HEAD v new, with nothing turning NEQ;
   - the reference resolves to the landed owner in a link check;
   - a libcp `.drectve` token only under NF318 Q1's own conditions.
   Group all binders of one made-up spelling as one unit (NF302 Q5); TABLE_CHANGES with apply_table.py --dry.
3. A binder that cannot move without changing bytes is STATUS skipped, with the measurement.

## Paths (this lane)
- S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad. In every Bash call:
  `export S; source $S/wine/env.sh; export S; unset TMPDIR TMP`. Run `mkdir -p /run/user/0` if missing.
- Retail exe: "$S/dl/game/Fable The Lost Chapters/Fable.exe".
- Checkers:
  - rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py (python3 -I).
  - tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py. Set env FABLE_EXE and FABLE_ROOT=/home/user/FableDecomp BEFORE any
    run. Pass --row-length AND --leaf=<Name@Class> from the row's `// Leaf:` line; without --leaf, pk_all takes the largest function
    (GOTCHAS 2026-11-24).
- Tools: tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/agent_tools/nf350/ (objsec_rn.py for renames), .../nf349/leafrel.py.

## Hand-back
land_scripted run dirs (batches/bN/): rows/, RESULTS.tsv, head/payload.json, TABLE_CHANGES.tsv, UNITS.tsv, orig/rows + orig/tests,
tests/ wherever a test names a changed symbol, head/HEAD_FROM_REPO.txt. Also NOTES.md, STATUS.tsv, RENAMES.tsv and evidence/.

Read COORDINATION.md, ASM_RULES, EXTRA_RULES and GOTCHAS before the first compile. Stay off:
- Account B's claimed or staged rows;
- Codex reservations;
- the locks of agents NF352 (CComponent size copies, 0x00626750) and NF353 (the map operator[] family port: the ~67 emitters of the
  operator[] COPY). If a binder of yours is also one of NF353's emitters, leave it to NF353 and list it in NOTES.
Never write into the repo, never run git. Run long jobs in the background.
