# NF449 hand-back (2026-11-25): T1 special-member / operator caller respells

## Result: 65 rows ACCEPT in 3 batches (s1, s2, d1), 65 callee rebinds onto 16 T1 targets (3 close fully, 13 partly)
Plus r1_ready_r449a (11 rows, every gate passed) held for ruling r449a; with it 76 rows, 19 targets, 4 closed fully.

| batch | rows | targets | mode | TABLE_CHANGES | CROSSBATCH | rulings | test edits |
|---|---:|---:|---|---|---:|---|---|
| s1 | 43 | 4 | SIGCTOR: the row's own bodiless model ctor goes from `T*` to the PDB `U&`, construct sites pass `*x` | 3 delete | 1 rewitness (CTCBase ctor, new witness 005bff07) | none (own-model edit, NF443 c1 form) | none |
| s2 | 8 | 3 | SIGCTOR (also CThingPhysical(long) -> (unsigned long), CFileDataInputStream(CDiskFileWin32*) -> (CAFile&)) | 3 delete | 0 | none | none |
| d1 | 14 | 9 | DTORX: a destructor stand-in (free `@X@4` / `X(T*)` fastcall, or member `X::M()`) becomes `reinterpret_cast<Q*>(x)->Q::~Q()` on a bodiless class | 13 delete, 1 add | 0 | r2, r3, q2 (tests 0048b4e0, 00a5e650) | 2 tests respelt (stub renamed with the row) |
| r1_ready_r449a (HOLD) | 11 | 3 | RESCOPE: the row's own global model of X wrapped in `namespace N {}` + `using N::X;` (+ SIGCTOR) | 1 delete, 1 add | 2 rewitness | **r449a requested** | none |

- **Landing order:** s1, s2 and d1 in any order (no (symbol, target) key is shared); then s1/CROSSBATCH.tsv. If r449a is granted, land r1_ready_r449a as its own batch, then its CROSSBATCH.tsv.
- `evidence/ALL_TABLE.tsv` (s1+s2+d1 TABLE_CHANGES + CROSSBATCH): delete 19, add 1, rewitness 1; apply_table --dry 0 misses, multi-target 1 -> 1, no new. `evidence/ALL_TABLE_with_r1.tsv`: delete 20, add 2, rewitness 3; 0 misses, 1 -> 1.
- Closed fully: `evidence/targets_closed_full.txt` (006df730, 008d71d0, 00994700); partly: `evidence/targets_partly.txt` (other stand-in spellings remain, mostly in T3 col 2/3 rows, Account-B rows, other agents' locks or other stand-in class names).
- Every RESULTS reason carries `; target 0x<addr>` (binder rows; old = new = the unchanged self).
- Hand proof: the 3-row pilot (work/x1_pilot: 0047df8a, 006dd8f0, 007efe70) passed every gate; its wd_* were removed before xrcheck, and its xrcheck evidence is s1's (same three rows, same text).

## Gates (every ACCEPT row; HEAD v new at one LANDVERIFY_WORK path)
- vland normal: grades equal (RELOCATION_MATCH both), tests PASS, LITERAL 0; the only new warn kind is SHARED_HEADER_AVAILABLE on d1 rows (r3 header debt, T3.txt).
- vland strict (LANDVERIFY_STRICT_HEADERS=1, work/vstrict_<b>{h,n}.log): s1, s2 and r1 grade lines identical HEAD v new. d1: rows already HEADER_POLICY at HEAD stay so (new SHARED_HEADER_AVAILABLE warn lines only); 004c3360 and 00af7c80 went strict PASS -> HEADER_POLICY and are in d1_hold (ruling r449d).
- objgate ALL_EQ (UND delta exactly old -> new, DEF sets equal, .drectve equal). r1: 4 rows NOT_EQ -> r1_hold.
- name_gate --test: NO_WORSE on all rows (d1's q2 tests included).
- pk: 0 BAD, copy counts unchanged; reachflip ($S/tools/reachflip.py on batches/*/evidence/pk): 0 NEQ reach flips (some rows have HEAD NEQ copies, none flips).
- /FORCE link with definers (target self rows compiled from HEAD): duplicates s1 0->0, s2 0->0, d1 2->2, r1 0->0; NEW_UNRESOLVED 0; every new name DEFINED by its target's object; control: HEAD unresolved stand-ins > 0 -> 0.
- xrcheck: every renamed callee resolves to its target (batches/*/evidence/xrcheck.txt).
- Includers: no landed row `#include`s any of the edited sources (no Option S / funclet includers).
- tablefix: deletes / rewitnesses decided from compiled HEAD objects (tools439/census439.py over every candidate row naming the identifier and referencing the target; work/census_db.tsv).
- reserved_check (RESERVED_REFS=""): 0 for every batch (s2 009b08c0 MENTIONS 0x009AFB90, already in the HEAD text).
- screen: no newest Account-B claim; T3 col 2/3 rows skipped by the generators; NF447 / NF448 locked rows skipped.
- orig/ and head/payload.json re-diffed against the repo at hand-back (tools/headcheck449.py): 0 drift, payload = repo text.
- Names: each new callee = the tracker PDB spelling = the landed self Leaf of its target; d1 PDB destructors are in BOTH PDB name sets and bound at no other target. No target in descriptive_names.tsv / the nf441 hold list; REGISTRY_CORRECTIONS.tsv and NAME_FIXES.tsv empty.
- Mutation control: none (leaf bytes equal HEAD; NF419 r1).

## Holds (locks released)
- r1_ready_r449a (11): ready, held for r449a.
- r1_hold (4): 004e7cc3, 0057832d, 005cd2a6, 00606d20: objgate NOT_EQ, because RESCOPE renames the derived vftable `??_7D@@6BCTCInventoryBase@@@` to `...@NInventory@@@`.
- s2_hold (2): 00636e90, 00873a10: target 0x0084E420 is not landed (no definer). Ruling r449b.
- d1_hold (2): 004c3360, 00af7c80: strict HEADER_POLICY. Ruling r449d.

## Method / tools (tools/)
- census449.py: census438 restricted to given targets (fresh HEAD). ctor449.py: SIGCTOR + RESCOPE (env RESCOPE=1); per-symbol llvm-undname; libclang mangled pre-screen (definitions + references, HEAD v new, must equal exactly s -> P). dtor449.py: DTORX (signature from the libclang declaration, so `@X@4` works; libclang names a dtor `??_D`, normalised to `??1`). fin449.sh: finalize442 -> split -> tablefix443 -> payfilter/units/xrcheck/screen/reserved/apply_table. headcheck449.py.
- Pipeline per batch: `python3 tools/mkb438.py <plan> <rows> <prefix> <list>`, `bash tools/runb442.sh <b>`, `bash tools/gsum.sh <b>`, `bash tools/fin449.sh <b>`.
- Lessons: (1) a model ctor signature change needs no test edit when the test is a standalone model copy (most are); (2) a target that is not landed has no definer: screen `landed=y` before batching; (3) re-scoping a model class renames derived vftables under multiple inheritance -> objgate catches it.

## Residue (RESIDUE.tsv: 598 lines = the 663 worklist lines minus the 65 landed, incl. the 13 r1/d1 ready/held lines)
- 250 not attempted: class-template targets (std::vector/map/pair, CCountedPointer<T>, CIntelligentPointer<T>: rows model non-template stand-in classes; model rewrite), member templates ??$ (q442a pending, template args are class templates), operator= / operator[] on stand-in classes.
- ~300 generator residue: ctor of a differently named stand-in class (CScriptThingStored, CBase_<addr>, CElement*_<addr>, CIntelligentPointer*), nested PDB scopes (CQuestManager::CBoast, CTCCreatureModeManager::CModeBase), ctor not modelled in the row, declared twice, FableWin-only dtor names (r449c), rows that model the dtor's class, T3 / locks / Account B / reserved.
- CCountedPointer<CActiveEntityScriptBase> (35 lines): the stand-in class is in the row's own self Leaf (return type), so a callee-only respell is impossible.

## Resume point
- Next mechanical step: DTORX for rows that model the class (9 CAInputProcess, 3 CBaseClass): add the public dtor to the row's own model instead of a bodiless class (prescreen + objgate will tell).
- Then the SIGCTOR "declared more than once" (10) and "no construct site" (9) groups by hand.
- Scratch: $S/w22/agentNF449 (work/sm_list.tsv worklist, work/f*_PLAN/RESIDUE, work/x1_pilot = the 3-row hand proof).
