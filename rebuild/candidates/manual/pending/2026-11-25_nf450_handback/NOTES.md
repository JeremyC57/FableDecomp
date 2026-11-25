# NF450 (2026-11-25): T1 special-member / operator caller respells, continuing NF449

## Result: n1 14 rows ACCEPT; m1_ready_r450b 10 rows (every gate passes, held for ruling r450b); n1_hold 1 row (ruling r450a)
| batch | rows | targets | mode | TABLE_CHANGES | rulings |
|---|---:|---|---|---|---|
| n1 | 14 | 005ed070 (9 rows, RESCOPE, implicit base init), 0099a2e0 (00529cc0), 004c2e90 (004c3360), 00441b90 / 004420d0 / 00442190 (r449c) | RESCOPE (r449a); DTORX through engine/CBaseClass.h and engine/C3DAnimationInterpolator.h (r2; r449d: header, not a bodiless class); DTORX on bodiless NPlayerGui classes with no engine header (r2/r3, r449c) | add 3, delete 6 | r449a, r449c, r2, r3 |
| m1_ready_r450b (HOLD) | 10 | 00442280 (closes fully) | 4 DTORX (NF442 b3_hold BaseDestructor rows, regenerated at HEAD); 6 model renames: the row's own base CRecordBase / CBase_00443220 becomes NPlayerGui::CStatBase (renamed, wrapped in its namespace + using-declaration; no layout change; the dtor is called implicitly by the derived dtor) | delete 4 | **r450b requested** (class rename), r449c, r2/r3 |
| n1_hold | 1 | 00af7c10 + 00af8ad0 | through engine/CDecal.h | - | r450a (objgate .drectve +libcp) |

- Landing order: n1 now. If r450b is granted, m1_ready_r450b as its own batch, all ten rows together (its BEST_GUESS line needs every one). No shared (symbol, target) key; CROSSBATCH empty in both. BEST_GUESS.tsv: n1 3 lines, m1 1 line.
- Targets closed fully: 005ed070, 00441b90, 004420d0, 00442190, 00442280. Partly: 0099a2e0 (3 spellings left), 004c2e90 (1 left: 00bd9250 QAE model).
- T3_PORT_QUEUE: lines nf303 0x00441B90, nf334 0x004420D0, nf295 0x00442190, nf301 0x005ED070, nf303 0x00442280 resolved fully; nf315 0x004C2E90, nf303 0x0099A2E0 partly (evidence/T3_RESOLVES.tsv per batch).

## Gates (HEAD v new, one LANDVERIFY_WORK path per batch)
- vland normal: grades equal, LITERAL 0, no new warn kinds (RELOC_NAME warns removed: n1 5, m1 10). Tests PASS (tests unchanged: all are standalone model copies).
- vland strict (LANDVERIFY_STRICT_HEADERS=1): grade lines identical HEAD v new in both batches; 004c3360 / 00af7c80 stay strict PASS (r449d met).
- objgate ALL_EQ (UND delta exactly old -> new) for all 24; 00af7c80 NOT_EQ (.drectve) -> n1_hold.
- name_gate --test NO_WORSE (25/25). pk 0 BAD; reachflip ($S/tools/reachflip.py on batches/*/evidence/pk): n1 15 pairs, m1 10 pairs, 0 NEQ reach flips (evidence/reachflip.txt). /FORCE link: duplicates n1 4 -> 4, m1 1 -> 1, NEW_UNRESOLVED 0, every new name defined by its target's object (n1 8/8, m1 1/1), HEAD unresolved stand-ins n1 8 -> 0, m1 4 -> 0.
- xrcheck: renames checked 14 / 10, bad 0. reserved_check 0. screen: no Account-B claim.
- apply_table --dry: per batch 0 misses; together (evidence/ALL_TABLE.tsv: add 3, delete 10) 0 misses, multi-target 1 -> 1, none new.
- descriptive_check --extra (both BEST_GUESS): at HEAD 4 FAIL, all OTHER_SPELLING of the spellings the batches delete; after applying both TABLE_CHANGES to a copy of the table (work/sim): 0 FAIL (batches/*/evidence/descriptive_check_after.txt).
- headcheck449: head/payload.json = repo text, orig drift 0 (n1, m1, n1_hold).
- Strong definitions (evidence/strongdefs.txt): the self rows define these destructors implicitly (compiler-generated), so a text grep sees none, not even the self rows; census instead: self_symbols.tsv (004420d0 only; 00441b90 / 00442190 have no line, their HEAD objects grade RELOCATION_MATCH and define the names), reloc_bindings.tsv (00442280 only), and the /FORCE link gate (each new name defined by its target object, no new duplicates). Standalone tests define local models of some classes (test-local, not linked).
- n1 objgate / pk / link / vland evidence was computed with 00af7c80 still in the batch (15 rows); finalize split it into n1_hold afterwards. The 005ed070 delete rests on all 10 candidate rows compiling and binding the PDB name (work/census_db.tsv, AGREE).

## Findings
- NF449's "DTORX models CAInputProcess (9) / CBaseClass (3)" were census false positives (token match in comments / other functions); compiled
  census (evidence/null_census.tsv) shows those rows already bind the PDB dtor. The remaining 00a0d3d0 stand-ins are stale table lines
  (evidence/STALE_TABLE_00a0d3d0.tsv, for NF448), except 0048cec0 (real binder, model rename).
- SIGCTOR "declared more than once": 9 of 10 are class templates; 008f7bd0 defines the CBaseClass bodies itself.
- SIGCTOR "no construct site": the ctor is reached by implicit base initialisation; RESCOPE needs no call-site edit (ctor449.py IMPLICIT=1).

## Tools (tools/)
- NF449's tools, repointed. ctor449.py: IMPLICIT=1 (pure RESCOPE reached by implicit base init). dtor449.py: FWONLY=1 + FWONLY_TARGETS (r449c).
- rename450.py: model rename of a row's own base class to the PDB class (namespace wrap + using), libclang prescreen (definitions unchanged,
  declaration old -> new).

## Resume point
- RESIDUE.tsv lists what is left with the reason; next mechanical candidates: 00442610 (port 00443cf0 onto engine/CMiniMapDisplay.h),
  004582d6 / 00459abe (_hdr opt-in declaring the dtor), 00739e20 (_hdr opt-in: nested CModeBase dtor in CTCCreatureModeManager.h).
