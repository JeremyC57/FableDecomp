# 2026-11-19 landing nf93c: naming round 8, NF93 part 3 (41 rows; NF93's slice finished)

Round 8: see `batches/2026-11-19_landing_nf93a/README.md`. The third NF93 instance worked q11296 to the end of the slice plus the follow-ups
q11450 / q11451. Records: `naming/2026-11-19_agentNF93_part3/` (APPLIED / STATUS cover the whole slice; `selfcheck.log`: 0 problems).

## Entries (part 3): 33 done, 5 done-already, 5 blocked (q11297 identity of 0x00516D90; q11333, q11397, q11413, q11420 ports), 0 open
Whole NF93 slice: 42 done, 7 done-already, 15 blocked, 1 deferred (q11415, Account B's row), 0 rejected, 0 open.
- q11440: the `Cons_val` stand-in 0x00810F80 is now `NEntityEvents::CIsHitEventHitByMelee::operator()` on the shared EventFilters /
  CEventHitBase headers (FableWin.pdb + its place in retail's run of hit filters); its 6 landed binders already bound that spelling.
- **q11450 / q11451 (coordinator decision):** 0x008C2810 and 0x0072CAA0 respell a declaration inside the local model the landed row already
  had (`FinishAction` made virtual; `~CCreatureAction_CastAssassinRush` virtual, vptr taking 4 pad bytes, size 0x138 kept). No class was
  renamed or newly modelled, so this is the in-place respelling that nf93a (CTCLook) and nf95a (five rows) already landed, not a new local
  model; moving them onto the shared headers collides with their dozen-class local models (a later port).

## Rows (41): 16 self renames (RENAMES_applied.tsv, 26 file moves) + 25 respellings
Recheck from the repo after landing (`logs/recheck.log`): 40 RELOCATION_MATCH + 1 MATCH, every test PASS.

## Headers
engine/CAFile.h `FABLE_RETAIL_CAFILE_CREATEDIRECTORY` (nested in the FABLE_RETAIL_CAFILE block: CreateDirectoryW 0x009994A0,
CreateDirectoryForFile 0x00999620, MakePath 0x00997780). Default path token-identical; 0 asserts moved. Includer recheck (CAFile.h users +
the catalog-only renames whose files moved): `logs/includer_recheck.txt`.

## Checks
Synced with FullDecomp-B first (B's 0x00989230 probe snapshot; no headers). `naming/rebind.py`: removed 120 binding lines, added 89;
`bindings_multi_check.py` at baseline; funclets 1,355 checked, 0 broken; selfcheck 0 problems.

## Queued: `land1119nf93` (7 new), incl. the candidate 0x00810F70 `CIsHitEventHitByShot::operator()` (one source).

## Rule slips (NOTES)
11 rows changed without a baseline grade (old symbols read from source); one baseline graded three times; one recheck started without env.sh
(rerun); one scratch file written outside the workspace and removed.
