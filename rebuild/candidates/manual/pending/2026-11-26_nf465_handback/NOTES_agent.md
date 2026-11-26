# NF465 (2026-11-26): bulk T1 respell pool (pool465, 392 targets) -- first batch handed back

## State / resume point
- Base: repo HEAD 8bcce86d54 (FullDecomp, .git/refs; started at eec399a1ff, the 9 table lines changed between touch none of my targets).
  work/base/ = reloc_bindings / self_symbols / descriptive_names at 8bcce86d54 (HEAD.txt). No git run. Nothing written under the repo.
- Tools (tools/): NF451 pipeline copies, NF449 ctor449/dtor449/census449, NF443 rebind443/tablefix443, c458/ (NF453/458 census, evid,
  classify, track458.sh, mkshadow.sh), n463/ (objproof_map, lk2, runpair, sandbox). Paths and lock owner strings repointed to NF465.
  New: prep465.py (per-target table lines + retail xref), rtti465.py (COL at t-4), classify465.py (triage), gen_data465.py (DATARENAME),
  evpair465.py (HEAD+new compile + reloc_identity.check), fullcensus465.py (accuracy gate), checks465.py.
- Census: work/cache_0[012].tsv = all 9,074 landed rows that reference a pool target (retail xref) + witnesses, compiled fresh at eec399a1ff
  (census453); cache_9.tsv = 61 rows drifted since NF458's 57k cache (used for the holder screen). Screen only; gate evidence is evpair/evid.
- work/triage.tsv (per target: binders at exactly t, non-P table lines, unchecked refs), work/binders.tsv, POOL_HOLDS.tsv (every target's verdict).
- -I disclosure: census453 / evid453 / evpair465 import repo modules (verify_and_land, reloc_identity) and call clean.py without -I (as NF453/458);
  repo tools I did not write (apply_table, descriptive_check, naming_status, bindings_multi_check) ran with -I.
- Resume: (1) 0x01436E24 lands as soon as its ghidra label is respelt (d1 held/); (2) data type-code shape (29 targets) needs a typed rename
  with objproof per row; (3) the 6 rebind443 targets need ruling R465b; everything else is port work (see yield).

## Yield per shape (392 targets)
| shape | targets | result |
|---|---:|---|
| table-only (all non-P lines stale) | 0 | none: every pool target has a live non-PDB binder in a compiled object (the pool is post-NF453/458) |
| data, plain global, same type code (DATARENAME) | 8 | 2 ACCEPT (d1, 9 rows); 3 HOLD T3-queue screen (cos tables, GPSubPrimitiveRenderer: 28 rows respelt and gated, held/); 1 HOLD label (9 rows); 2 HOLD owner gate (VIRTUAL_SCREEN_*) |
| data, type code differs / not a file-scope extern | 29 | HOLD (needs a typed respell, bytes may move) |
| data, PDB = class static member | 38 | HOLD port (class declaration needs the shared header) |
| ??_7 with live non-PDB binder | 197 | HOLD port (local class models / wrong base qualifier; r459a) |
| ??_7 no RTTI COL | 14 | HOLD (PDB spelling unconfirmed) |
| HOLD_HOLDER (stale line still UND/DEF elsewhere) | 8 | HOLD |
| func: rebind443 | 74 | 6 targets / 10 rows generated (bodiless-class r2/r3 form) -> HOLD ruling R465b; 68 residue (316 PDB-not-parsed, 154 special member/operator, 111 T3 queue, ...) |
| ctor: ctor449 | 21 | 0 generated (class differs / not modelled / T3 queue) |
| dtor: dtor449 | 3 | 0 generated (T3 queue, row models the class) |
RTTI check: 201 of 215 ??_7 targets have a COL whose type name agrees with the PDB spelling (incl. the 6B base qualifier at non-zero offsets); 14 have none.

## Batches
- batches/d1_dataglobals: 9 ACCEPT / 31 HOLD (README.md). Tracker dry run: 2 split_fixable -> agree_pdb (013750b0, 013b8a88), nothing worse.
- Advisor review found the T3 screen gap (applied: 3 targets held), the Ego_d-only name source (fixed: comment says Ego_d), and the cos-table window (held).

## Rulings needed
- R465a: 0x01436E24 relabel (ghidra labels_chunks718_725 `g_pEngineEnvironmentDef` -> `?GPEnvironmentDef@NEngineInternal@@3PBVCEnvironmentDef@@B`) landing with
  the 9 held rows (r463c-style), or keep held.
- R465b: rebind443's bodiless local class (r2/r3) vs this brief's "never a local model of a class that has a header": allowed for callee-only
  rebinds or not? (10 rows in work/rb465_rows; 5 also cast a namespace-modelled CActionBase to CTCScriptedControl::CActionBase.)
- R465d: D1 (research note): may a callee-only data rename whose objproof is ALL_EQ go through the T3_PORT_QUEUE col 2/3 row hold? Would release
  0x013CB530 / 0x013CD550 / 0x01436E74 (28 rows ready in d1 held/).
- R465c: VIRTUAL_SCREEN_WIDTH / HEIGHT (16 + 15 stand-in binders, pure same-type renames ready via gen_data465.py): lift the 2026-11-01 gate?

## Locks
NF465 holds (work/locks_held.txt) the 40 d1 rows (9 ACCEPT + 31 held) and the 6 targets 013750b0 013b8a88 013cb530 013cd550 01436e24 01436e74.
No other lock taken.
