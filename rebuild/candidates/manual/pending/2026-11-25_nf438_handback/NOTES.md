# NF438 hand-back (2026-11-25): T1_A caller rebinds through libclang (rulings r2/r3 form)

## Result
- **161 rows ACCEPT** in 5 plain batches (no header changes; HEADERS.tsv empty), 167 callee rebinds onto 83 T1_A targets.
- 16 rows HOLD in `*_hold` folders. Every hold row's lock is released.
- Table: 93 delete, 27 add, 11 rewitness. `work/ALL_TABLE.tsv` holds all five batches.
  - Each batch's census counts as moved only the rows that rename that exact (old, target) pair, in that batch and the earlier ones.
  - Lines are deduplicated across batches.
  - apply_table --dry: 0 misses per batch and for all five together.
- Tracker effect: 58 T1_A targets are fully closed (no wrong spelling left in the table) and 25 are partly closed. RESIDUE.tsv lists the other 801 untouched targets plus the 25 partial ones: 1,795 lines, one per (target, wrong spelling, caller row), each with a reason.

| batch | rows | rebinds | targets | table |
|---|---:|---:|---:|---|
| q1 | 48 | 49 | 25 | 23 delete, 9 add, 3 rewitness |
| q2 | 44 | 46 | 24 | 28 delete, 10 add, 3 rewitness |
| q3 | 21 | 21 | 10 | 16 delete, 1 rewitness |
| r1 | 47 | 50 | 28 | 25 delete, 7 add, 4 rewitness |
| r2 | 1 | 1 | 1 | 1 delete, 1 add |

**Landing order.** Land q1, q2, q3, r1, r2 in that order (UNITS.tsv: `nf438_seq`).
- Each batch was finalized with only itself and the earlier batches counted as moved, so any landed prefix leaves a correct table.
- An earlier batch rewitnesses a line to a later row, and the batch that last renames it deletes it (the NF423 c1-to-c3 pattern).
- Before any delete, the census was re-checked in three ways:
  - the libclang locator was run on the 146 unfiltered census rows (0 bind an old symbol);
  - the rows whose locator parse failed (0096ff10, 008575c0, 006cc800) were re-resolved and bind the PDB names;
  - no accepted new object has `UND` for a deleted symbol.

**RESULTS.** The `old` and `new` columns both hold the row's unchanged self symbol (from rebuild/oracles/self_symbols.tsv, else the row's `// Leaf:`). The callee renames are in the reason column and in renames.tsv. SELF_SYMBOLS.tsv lists each row's self symbol.
- All 161 were checked with the lander's own `verify_and_land.obj_text` on the new object.
- 4 rows differ from self_symbols.tsv: 00787c62, 008985a0, 0089df10 and 00904540. Their object defines a different self than the source Leaf, which is pre-existing self debt and identical at HEAD.
- For those 4 the columns carry the graded symbol. 00787c62 uses the `?A0x` prefix form, as the lander's anon rule expects. Each RESULTS reason says so.

## Method (new generator: tools/rebind438.py; the gen423 shapes covered only 2 of the A rows)
1. **census438.py.** For every A target t and every table symbol s bound to t (s is not the PDB spelling P), list the landed rows whose source names s's identifier and whose retail bytes reference t. cfilt438.py then keeps only rows whose text names every identifier of demangled s (5,027 rows).
2. **ast438.py.** libclang 18 parses each row with the precheck flags (pip `clang==18.1.8` bindings in `pylib/`, `/usr/lib/llvm-18` libclang). It records every main-file reference whose MSVC mangled name is s: call sites, callee and base extents, arguments, and the stand-in declaration. 1,462 rows bind a wrong spelling.
3. **rebind438.py** rebinds every A callee of a row in one pass, without touching the row's own model:
   - **MEMBER** (127 rebinds). Inserts a bodiless `class Q { public: [virtual] RET M(PARAMS) [const]; };`, wrapped in its namespaces, before the top-level declaration that holds the first call. Each callee `B->M` / `B.M` / `M` becomes `reinterpret_cast<[const ]Q*>(B / &(B) / this)->M`, written `Q::M` when P is virtual. This is the NF423 r2/r3 form.
   - **MEMBER/model-edit** (32 rebinds). When the row already models Q locally, the PDB method is added at the end of the model (`public:` before the closing brace, so no other member changes access and the layout is unchanged), or the stand-in declaration is respelt in place (const). No new class is added.
   - **FREE** (7 rebinds). The stand-in prototype is replaced by the PDB prototype and the callee is renamed.
   - **STATIC** (1 rebind). Uses `Q::M`.
   - **Adaptation.** A `T*` parameter that becomes `T&` gets `*(arg)` (or `&x` becomes `x`). A differing pointer parameter gets a `reinterpret_cast`. A return type that changes between pointer and reference is wrapped. Class types the row does not declare are forward-declared.
   - Stand-in member declarations that nothing uses any more are dropped.
   - Each row gets a `// Naming (NF438 gen ...)` comment listing target, PDB name and old spelling.
   - clang locates the call sites; the VC7.1 objects decide whether a rebind is accepted.
4. **Pipeline per batch.** mkb438.py claims locks at plan time and writes orig copies by address. Then prefam (clang prechecks), runpair (vland HEAD and new at one LANDVERIFY_WORK path), gate438 (objgate), pkgate438, linkgate438 (/FORCE link with the target self rows compiled from HEAD as definers), namegate438.py (name_gate.py HEAD v new with `--test`), finalize438, split438, relink, finalize again, payfilter, xrcheck438 + xr2_438, screen438, reserved_check, apply_table --dry. The driver scripts are `work/run_q.sh` and `work/run_r.sh`.
5. **Generator iterations** (r1 to r4, precheck pass counts 123, then 16, then 47, then 55):
   - support for editing the row's own model;
   - forward declarations;
   - argument and return casts;
   - a const-away cast form (CONSTAWAY list);
   - classes defined in an included header, used only when the header already declares P.

## Gates (every ACCEPT row)
- **vland:** HEAD and new grades are equal and PASS (RELOCATION_MATCH / MATCH). Tests are unchanged and PASS. There are 0 LITERAL warnings. The only new WARN kind is SHARED_HEADER_AVAILABLE, accepted under rulings r2/r3 (`ruling.txt` = r3) and booked as one T3 header-debt line (T3.txt: 112 rows, 23 header classes, plus 4 classes with no header yet).
- **objgate:** 161/161 rows are ALL_EQ. Every non-debug section is equal in data, characteristics and relocations under the rename. DEF sets are equal. The UND delta is exactly old -> new. `.drectve` is equal.
- **name_gate (HEAD v new, `--test`):** 161/161 rows are NO_WORSE by FAIL count (evidence/namegate_summary.tsv).
  - The only FAILs the new rows add are 97 SHARED_HEADER_AVAILABLE (row) and 15 LOCAL_CLASS, one per bodiless declaration (open-pattern tally). Each replaces the removed stand-in FAIL.
  - **Vacuous for 6 rows:** 00787c62, 00846100, 008985a0, 0089df10, 00904540, 0041e6d3. name_gate cannot grade these rows at HEAD or new: a LEAF_MISMATCH from a path-hashed anonymous namespace or a self-leaf mismatch, or a DIFFER under its own compile. The comparison measured nothing; their RESULTS reasons say so, and objgate ALL_EQ is the evidence.
- **pk:** 0 new NEQ, and copy counts are unchanged.
- **/FORCE link with the definers:**
  - duplicates do not go up: q1 13->13, q2 8->8, q3 1->1, r1 4->4, r2 0->0;
  - NEW_UNRESOLVED is 0 and every new name is DEFINED;
  - the control fires (HEAD unresolved stand-ins 29/31/17/32/1, nonzero duplicates);
  - q1 keeps two old names because other q1 rows still bind them for other reasons: Draw2DText@CGameUI in 0064499c / 0064560d, and GetFrame@CWorld in 006e2dc0.
- **xrcheck:** every renamed callee resolves to its target in the retail bytes.
  - 3 rows (005dd440, 0074c5f0, 00820e20) were re-proven by xr2_438.py, because nf.py's leaf-COMDAT pick was wrong for them.
  - 0064499c was re-proven by hand, which is noted in its xrcheck line.
- **apply_table --dry:** 0 misses per batch and for all batches together. No new multi-target symbol (1 before, 1 after).
- **reserved_check** (RESERVED_REFS=""): 0, for 187 + 87 rows and targets.
- **Screens:**
  - Generator screens: rows in T3_PORT_QUEUE col 2/3 are skipped entirely (listed in RESIDUE as `row: named in T3_PORT_QUEUE col 2/3`; r1 was not applied), plus locks, newest claim B, and pending RESERVED.
  - screen438 flags target 0x00666D40 because its newest claim line has Account column B. That line (2026-11-25) is the release of B's claim by Account A on owner order, so it does not block. It is a target, not an edited row.
- **orig/:** re-diffed against the repo at hand-back: 161 rows and their tests, 0 drift.
- **Mutation control:** callee-only, with leaf bytes equal to HEAD, so NF419 r1 applies.

## Names
- **Two sources.** Each of the new callee names is in both the Ego_r and the FableWin PDB name sets (checked for all 167). Each is also defined by the target's own HEAD self-row object: `DEF <new>` in work/g/<t>_D/<t>.obj, 0 misses.
- **New owner rule (best-guess names).** It did not apply here: no BEST_GUESS.tsv was needed.
- **NAME_FIXES.tsv: 0x009BEF80.** The self row defines a made-up name (SetIntegerViewportEndpoint). The tracker and both PDB name sets give `?SetViewport@CDisplayManager@@QAEXABVC2DBoxI@@@Z`. This held 4 caller rows.

## Holds (16; reasons in each `*_hold/RESULTS.tsv`)
- **Test link failure (9 rows):** 0047db73, 0066fb50, 00891d50, 0089a070, 0089a0e0, 008c5fd0, 004022b0, 007ea2d0, 00ab5390. The HEAD test links the row object and stubs the callee under the stand-in name. Ruling q2.
- **Target self row misnamed (4 rows):** 009bf160, 009bf220, 00b869a0, 00b89140, all targeting 0x009BEF80. See NAME_FIXES.
- **objgate NOT_EQ (3 rows):**
  - 007c04a0: ADDED EH sections, pk copies 9->13;
  - 008d03a0: CIntelligentPointer<CThing_Retail> COMDAT change;
  - 00449cb0: the GetPlayer ptr->ref rebind changed the code.

## Residue (RESIDUE.tsv) top reasons
| reason | lines |
|---|---:|
| special member / operator (ctor, dtor, operator=, `??_G`, `??A`): needs a model edit | 654 |
| no caller located by libclang (implicit destructor calls, failed parse, stale line) | 255 |
| row screens: T3 col 2/3, parse errors, locks | 253 |
| PDB target protected / private (ruling q1) | 205 |
| template scope or member template (CCountedPointer<T>::Reset, GetSubDef<T>): shared-header route | 102 |
| call-site shapes (callee text, member call through a non-member expression) | 81 |
| row's own model of the class is ambiguous / has overloads / virtual | about 90 |
| static v non-static, parameter count differs (identity research) | 55 |

**Next shapes to automate:**
1. GetSubDef<T> (member template on a bodiless CDefClassBase plus an argument cast; about 110 lines).
2. The protected/private rows (if q1 = yes).
3. Test-stub respells (if q2 = yes).
4. CCountedPointer<T> members through the shared header's DECL_ONLY opt-ins.

## Caveats
- **r3 reliance:** 112 of the 161 rows gain a bodiless local declaration of a class (97 with a shared header, which is SHARED_HEADER_AVAILABLE; 15 without one, which is LOCAL_CLASS). They are booked as one T3 header-debt line in T3.txt.
- **Comment wording:** the inserted `// Naming (NF438 gen ...)` comment says "through a bodiless declaration" on every row, including the FREE (prototype) and model-edit rows. The wording is loose there; the code itself is exact.
- **T1_B overlap:** none. No accepted row or target is in T1_B.
- **reserved_check:** 0 on the final 161 rows plus their 83 targets.
- **T3:** evidence/T3_RESOLVES.tsv (at the agent root) has only target-role lines, no row-role lines.
