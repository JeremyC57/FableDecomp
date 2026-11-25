# NF439 hand-back (2026-11-25): T1_B caller respells, declaration generator

## Result
| batch | rows | lines (renames) | shapes | table (TABLE_CHANGES) | land order |
|---|---:|---:|---|---|---|
| p1 | 40 | 43 | LOCAL 24 lines, NSFUNC 19 lines | 29 delete, 14 rewitness, 12 add | first |
| q1 | 31 | 34 | LOCAL 27 lines, NSFUNC 7 lines | 9 delete, 3 rewitness, 3 add | after p1 (its table was computed on reloc_bindings with p1 applied) |

- 71 ACCEPT rows, 77 callee respells, 43 distinct B targets. All plain (no header change, HEADERS.tsv empty). 27 B targets have no wrong
  table spelling left after p1 + q1; 15 more are partly fixed (RESIDUE.tsv code PARTIAL); 837 lines in RESIDUE.tsv.
- UNITS.tsv (units423 over p1 + q1): rows sharing a stand-in table line are one unit, and some units span both batches
  (unit ids 007471a0 007fc820 0080c210 00906050 006622d0 007122d0 009061e0 0095b5a0, per units423): land p1 and q1 together, p1 first.
- Locks: NF439 on exactly the 71 ACCEPT rows; every other NF439 lock is released (holds, pilots). Targets were never locked (definers are compiled from HEAD only).
- HOLD: p1_hold (10), q1_hold (4); reasons in their RESULTS.tsv. Five of them wait on RULINGS_NEEDED r1 (an empty local `enum E {};` for a header-defined PDB enum).
- No NAME_FIXES found. Note for the catalog: 0x008E8E50's catalog label is `GetStrafeAnimationName`; Ego_r/FableWin give `NCreatureMode::GetStrafingAnimationName` (row 0x008E9110 held on the RELOC_NAME warn).

## Shapes (tools/gen439.py, one generator, judged by gate423)
- **LOCAL**: the caller's own local model class already has the PDB scope; the callee declaration is re-spelt to the PDB prototype from
  `llvm-undname` (name, parameter types, const, access via an inline `protected: ...; public:` label), calls renamed, arguments shimmed by
  cast only (pointer->reference `*p`, pointer / integer / enum casts), return re-cast only where the call is used as a value.
- **NSFUNC**: a free `__fastcall` stand-in -> the PDB `NFoo::` namespace function: declaration moved into the namespace, calls qualified.
- Unknown parameter classes get a forward declaration (`class X;`); std containers get `#include <vector>` etc. Anything else is residue
  (VIRTUAL, SIG count, value / template type changes, scope moves, ctor / dtor / operator forms, static members, T3 col 2/3 rows, locks).
- Caller census: table witnesses + every landed row whose retail bytes call the target and whose source names the stand-in, confirmed by a
  compiled HEAD census (tools/census439.py: leaf relocations resolved against retail; work/census_0*.tsv, 769 rows). The high fan-out
  families (CCharString ctor/dtor, CBaseIntelligentPointer, CWideString copy, CActiveEntityScriptBase ctor, ...; work/big_targets.txt) were
  set aside: their callers use namespace-local data-bearing models of header classes (header-port work, not callee-only).

## Gates (every ACCEPT row)
- vland HEAD/new at one LANDVERIFY_WORK path (runpair.sh): grade equal and PASS both sides, tests PASS, 0 LITERAL, no new WARN kind (warncmp; RELOC_NAME warns removed 21 + 20).
- objgate (gate423): ALL_EQ: every non-debug section equal under the rename, DEF set equal, UND delta exactly old -> new, .drectve equal.
- name_gate.py HEAD v new with the HEAD test (tools/ngate439.py): new FAIL lines a subset of HEAD's for every row (the callee lines go DEBT -> OK).
- pk (pkgate423): 0 new NEQ, copies unchanged.
- /FORCE link (linkgate423, definers = HEAD objects of the landed target rows): duplicates p1 3 -> 3, q1 0 -> 0; NEW_UNRESOLVED 0; every renamed
  callee with a landed definer resolves (p1 45/45, q1 16/16 on the pre-split superset). The one ACCEPT callee without a definer is
  0x00787CA5 -> ?CreateShot@NThingShot@@... (0x007C0AD0, no landed self row): both Ego_r and FableWin publics name it (CHECKS name_sources, RESULTS reason). Change to NF423's gate: an old stand-in may stay UND in the new set when another (unrespelt)
  row still calls it; objgate proves each respelt row dropped it.
- xrcheck: each renamed symbol resolves to exactly its target (bad 0 on ACCEPT rows; the 2 bad were the q1_hold NOT_EQ rows).
- apply_table --dry: p1 on the repo table, q1 on the table with p1 applied: 0 misses, multi-target 1 -> 1, no new multi-target symbol.
- reserved_check (RESERVED_REFS=""): 0. screen423: no row or target whose newest COORDINATION_CLAIMS line is Account B (the three rows on
  0x0070E8B0, claimed by B, were held). No ACCEPT row is in T3_PORT_QUEUE col 2/3; T3 lines naming the targets: evidence/T3_RESOLVES.tsv.
- TABLE_CHANGES rewitness / delete decided from compiled HEAD objects (tools/tablefix439.py, evidence/tablefix.txt), not from identifier
  tokens: the token census alone proposed 8 wrong rewitnesses in p1.
- Mutation control: leaf bytes equal to HEAD (objgate), HEAD tests unchanged -> none (NF419 r1).
- Gate evidence for vland / objgate / pk / link / name_gate covers the pre-split superset (p1 50 rows, q1 35 rows; same objects).
- orig/ re-diffed against the repo at hand-back: 71 rows, 0 drift.

## Names
Every new callee name was looked up in ghidra_out/egor_pdb_names.tsv and fablewin_pdb_names.tsv: 75 of 77 renames are in both sets
and the target's landed self Leaf defines them; 0x006E1620's ?SetParticleCreationDisabled@CParticleEmitter@NParticleEngine@@QAEX_N@Z is in
FableWin only plus the landed self Leaf of 0x006DFDA0; 0x00787CA5's CreateShot is in both sets with no landed self. BEST_GUESS: none
(owner rule of 2026-11-25 not needed: every name is a PDB name).

## Residue
RESIDUE.tsv: one line per B target not closed (837), code + wrong spellings left (after p1 + q1) + reasons (held rows cite their batch).
Top codes: SHAPE 281 (scope move to another class / static member / calling convention), NOT_ATTEMPTED_SPECIAL 195 (ctor / dtor / operator
forms), T3_PORT_QUEUE col 2/3 140, VIRTUAL 24, SIG 21, PARTIAL 15, FAMILY_HEADER_PORT 12, HOLD 9. Next generator steps: VIRTUAL with a qualified call where the local model already
has a vptr; free stand-in -> static member through a bodiless class (r2/r3 debt, separate batches); scope moves with discovered renames.

## Tools (tools/)
gen439.py, und439.py, cand439.py / cand2_439.py, cls439.py, where439.py, census439.py, tablefix439.py, finalize439.py, ngate439.py,
forcehold439.py, run_batch.sh, plus NF423's copies (gate423, pkgate423, linkgate423 patched, mkbatch423 patched to batch by row and not
lock targets, split423, payfilter423, xrcheck423, screen423, release423).

## Resume note
- Inputs: work/pairs2_all.tsv (witness + compiled-census pairs), work/census_0*.tsv + work/census_db.tsv (compiled HEAD census:
  row, target, symbol, AGREE/WRONG), work/big_targets.txt (families set aside), work/skip_rows.txt (p1 / p1_hold rows).
- Rows already in p1/q1 that call another B target with a wrong spelling need a second pass on top of the batch text (not done).
- Run: `python3 tools/gen439.py work/<w> work/pairs2_all.tsv` -> clang precheck (nf.py pre) -> mkbatch423.py -> run_batch.sh -> predef.sh
  -> linkgate423 -> ngate439 -> finalize439 (TABLE= the table with earlier batches applied) -> tablefix439 -> split423 / forcehold439 ->
  payfilter423, units423, screen423, xrcheck423, apply_table --dry, release423.sh.
- Next shapes: VIRTUAL (qualified call where the local model already has a vptr), free stand-in -> static member (bodiless class, r2/r3
  debt, separate batches), scope moves with discovered renames verified per target, SPECIAL ctor / dtor forms.
