# NF407 hand-back (2026-11-25): the StatGetClassTypeName v binder duplicate regression (landing nf402_b1)

## Summary
- **The regression is confirmed.** With the binder CTCBase::RegisterTCs 0x004EE23F plus the five nf402_b1 rows, a /FORCE link shows:
  - pre-nf402 texts: 0 duplicates against the binder;
  - current HEAD: 5 duplicates, all against the binder.
  - 0x004D3CDE, which NF406 did not measure, is the fifth.
- **Five more rows had the same clash.** 0x004D4249, 0x004D429B, 0x004D430F, 0x004D5FDB and 0x004D4A86 (CTCBerserk) each also give one LNK4006 against the binder at HEAD (5 duplicates).
- **All ten rows are re-formed into the nf406_b2 0x004D31C0 form:**
  - the static is defined in the class body (pick-any, selection 2);
  - an anonymous-namespace helper `?ReferenceStatGetClassTypeName@?A0x...` takes its address and is marked `Fidelity: scaffolding, not retail`.

  This is the nf86 / nf88 form (landing nf387 ruling). Names, literals, includes, leaves and tests are unchanged.
- **Two batches, one unit per row, all 10 ACCEPT** (locks kept as NF407):
  - **batches/b1**, the nf402_b1 regression fix: 0x004D2A74, 0x004D39CF, 0x004D3CDE, 0x004D4232, 0x004D5CE6.
  - **batches/b2**, NF406's unmeasured list: 0x004D4249, 0x004D429B, 0x004D430F, 0x004D5FDB, 0x004D4A86.
  - b1 can land without b2.
- **No table or name changes.**
  - Both TABLE_CHANGES.tsv files have the header only.
  - SELF_SYMBOLS.tsv restates the unchanged HEAD lines. 0x004D4A86 has no self_symbols.tsv line at HEAD, so none is given (T3).
  - No NAME_FIXES. No header edits. No FLAGS. No HOLD rows. No RULINGS_NEEDED.

## Link counts (VC7.1 link /DLL /NOENTRY /FORCE /NODEFAULTLIB, NF398 / NF406 link377.py)

| set | before | after | file |
|---|---|---|---|
| Task step 1: binder + 5 nf402 rows, pre-nf402 texts v HEAD | pre-nf402: **2** total, **0** against the binder | HEAD: **5**, all against the binder | evidence/link_step1_pre_v_head.txt |
| Task step 3: binder + 0x4249 / 0x429B / 0x430F / 0x5FDB / 0x4A86 at HEAD | - | **5**, all against the binder | evidence/link_step3_head.txt |
| b1 gate: binder + 5 rows, HEAD v new | 5 | **0** | batches/b1/evidence/link_b1.txt |
| b2 gate: binder + 5 rows, HEAD v new | 5 | **0** | batches/b2/evidence/link_b2.txt |
| everything: binder + 10 rows + 0x004D31C0 + the 4 nf406_b1 callers, HEAD v new | 10 | **0** | evidence/link_all.txt |
| the 4 callers + their 4 statics, NO binder, HEAD v new | 0 | 0 | evidence/link_callers_nobinder.txt |
| the 10 rows alone, NO binder, HEAD v new | 0 | 0 | evidence/link_rows_nobinder.txt |

- **Step 1 file labels.** link_step1_pre_v_head.txt was produced by link377.py, which labels its first set "HEAD" and its second "new". In that file the "HEAD" set is the pre-nf402 texts, and the "new" set is current HEAD.
- **The two pre-nf402 duplicates are not binder clashes.** 0x004D2A74, 0x004D39CF and 0x004D5CE6 all defined the same free stand-in `?StatGetClassTypeName@@YI?AVCCharString@@XZ`, so they clashed with each other.
- **Pre-nf402 objects.** They come from NF402's orig texts ($S/w22/agentNF402/batches/b1/orig), compiled through vland (work/wd_pre, logs work/vland_pre.log). All five graded RELOCATION_MATCH PASS.
- **Nothing new is unresolved.**
  - Every gate link was run with watch `.`, i.e. every symbol.
  - The HEAD -> new diff has no NEW_UNRESOLVED line, and the unresolved total is unchanged (474 -> 474 with the binder, 1 -> 1 without it).
  - The only RESOLVED_TO_GONE lines are map-header length tokens (`0000cc8eH` and the like), an artefact of watch `.` matching the map's section table. They are not symbols.
- **The 4 nf406_b1 callers still resolve** (0x004DDA94, 0x004D8154, 0x004E07C6, 0x004E3148).
  - With the binder in the set, the statics resolve from either object, so that link alone proves nothing.
  - The link that proves it has no binder: the callers' undefined `?StatGetClassTypeName@<Class>@@SI?AVCCharString@@XZ` all resolve to the row objects in the new form.
  - That shows the helper really forces the row object to emit the pick-any copy. The only unresolved name is `??0CCharString@@QAE@PBDJ@Z`, the same as at HEAD.

## Gates per row (all 10 pass)
- **vland, HEAD v new, each at one LANDVERIFY_WORK path** (b1: work/wd_a; b2: work/wd_b; logs in batches/<b>/logs/vland_{a,b}{h,n}.log):
  - RELOCATION_MATCH PASS -> RELOCATION_MATCH PASS for all 10, so no grade drop.
  - The WARN sets are identical on both sides: SHARED_HEADER_AVAILABLE on 2a74 / 3cde / 5ce6 / 4249 / 430f / 5fdb / 4a86, the known false positive on layout-rename rows (queued T3 `nf402 tools`).
  - There is no WARN LITERAL_DIFF or LITERAL_UNCHECKED on either side.
- **objsec**, HEAD v new (evidence/objsec.txt in each batch, objsec356 by COMDAT symbol). Each row shows exactly:
  - the leaf's data, relocations and characteristics equal, with COMDAT selection 1 -> 2;
  - one ADDED 6-byte COMDAT, the helper;
  - one SYM+ (the helper), and nothing else.

  That is the nf387 shape. The `??_C@` string section does not move.
- **pk_all + pickany_vs_retail**, HEAD v new (evidence/pk, evidence/pk_summary.txt, `--leaf=` given, repo table; the batches change no table):
  - 0 NEQ on both sides.
  - The new side's only extra copy is the helper: strong, NOADDR, reach n.
  - Reach flips: 0 (evidence/reachflip.txt).
- **Binder copy v row copy** (evidence/binder_comdat_cmp.txt, cmpcomdat by symbol, bytes + relocations):
  - Nine rows are EQUAL to the binder's same-named pick-any COMDAT (19 bytes, 2 relocations).
  - **0x004D4A86 differs.** It keeps HEAD's cast `(const char*)0x0123C274`, an immediate with no relocation, where the binder pushes a `$SG` literal relocation. Both copies are 19 bytes and both grade EQ to retail.
  - Converting the cast would add a relocation and a string section, which breaks the objsec gate, and would force a test change. So it stays as is. Under R10 a second body under one pick-any name is debt, not a hold (T3).
- **Tests:** unchanged, all PASS in vland. Mutation controls (evidence/test_mutation.txt) link the vland test object with a literal mutant of the new row:
  - **b1 rows and 0x004D4A86:** the real object PASSes and the mutant FAILs. These tests link the row object, so they test the new form.
  - **0x004D4249 / 0x004D429B / 0x004D430F / 0x004D5FDB:** the tests are self-contained. They redefine the static themselves and never link the row object, so their PASS says nothing about the row. They are left unchanged (HEAD debt, T3).
- **reserved_check** (logs/reserved_check.txt): reserved 0, mentions 0.

## Texts
- **Generated by tools/gen.py from the HEAD sources.**
  - The out-of-line definition becomes the in-class body.
  - The helper is added after the class.
  - A `// Form (NF407, ...)` comment paragraph is added.
- **One stale comment is fixed in 0x004D2A74:** "no table binder" now says the table binds it (witness 0x004DDA94, nf406_b1).
- **orig/rows and orig/tests are the HEAD byte copies.** tests/ holds the same files as orig/tests.
- **Leaves are kept.** The b2 CTCD* rows keep their prefix-form leaf (`StatGetClassTypeName@<Class>`), as in self_symbols column 4.

## Found on the way: the whole family clashes (T3)
- **evidence/family_census.tsv:** a text census of every `*StatGetClassTypeName_*.cpp` row against the binder's 320 in-class statics. The binder class list comes from a grep that drops namespaced names.
- **At least 250 more rows clash.**
  - The census finds 251 out-of-line rows that name a class the binder registers and have no self_symbols.tsv line. Without this batch's 0x004D4A86, 250 remain.
  - All 251 have 0x004D4A86's shape, checked by grep across all 251: a local `class CCharString`, a cast `(const char*)0x...` literal, and an out-of-line strong definition. Each mangles to the binder's exact name, so each should be a further NoDuplicates v Any duplicate at HEAD.
  - Measured: 3 of 3. A spot link of 0x004D51A2 CTCTavernTankard, 0x004D3989 CTCWound and 0x004D2E61 CTCQuestCard with the binder gives 3 duplicates (evidence/link_family_sample.txt).
- **52 rows are unclassified, a possible further clash:**
  - 20 out-of-line rows whose filename class is namespaced (NGraphicAppearance::CTCGraphicAppearance*, NThingShot::CTCShotDefinition*). The census did not match them against the binder.
  - 32 rows whose source shape the census did not parse (form "other": the CTCActionUse* / CTCCarriedActionUse* block at 0x004D2B04..0x004D2D13, CTCD* drivers, CTCTargeted, CTCRotate, CTCTargetingPlayer, CTCAssassinRush).
- **This debt predates nf402**, so it is not part of the nf402 regression. The binder links clean only for the 11 rows done so far: 0x004D31C0 (nf406) plus these 10.
  - The fix is the same mechanical re-form; tools/gen.py handles the shape.
  - Those rows also carry cast literals (the NF388 queue), so their copies would differ from the binder's the way 0x004D4A86 does.

## Budget / hygiene
- **vland runs:** one HEAD context run (16 rows); one pre-nf402 run (5 rows); two HEAD/new pairs; one 3-row family sample.
- **Other compiles:** 6 mutant compiles.
- **No repo writes, no git, no wineserver -k.**
