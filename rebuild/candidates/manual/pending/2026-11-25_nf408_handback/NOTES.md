# NF408 hand-back (2026-11-25): the whole StatGetClassTypeName v binder family (NF407 T3 `family debt`)

## Summary
- **Measured worklist: 270 rows.** The HEAD census link has 463 objects: the binder CTCBase::RegisterTCs 0x004EE23F plus every landed row whose source mentions `StatGetClassTypeName`. That is 463 sources, no includers, no Option S thin rows.
  - It shows **272 duplicates: 270 against the binder, 2 others** (evidence/link1_census_head.txt).
  - The 270 split into 250 plain out-of-line rows (NF407's census, minus 0x004D4A86 already landed) and 20 namespaced rows (NGraphicAppearance:: x7, NThingShot:: x13).
  - The text census and the link agree exactly: every row the generator's shape parses duplicates, and no other row does.
- **NF407's 52 unclassified rows** (evidence/unclassified_52.tsv, from the link):
  - The 20 namespaced rows duplicate against the binder, so they form batch b6.
  - The 32 "other" rows do not duplicate against the binder.
    - 30 have no duplicate at all: the CTCActionUse* / CTCCarriedActionUse* free stand-ins, CTCRotate, CTCTargeted, CTCTargetingPlayer, CTCAssassinRush, the CTCD* driver stand-ins and others.
    - 2 (0x004D3EF9, 0x004D41DF) duplicate `??0CThingComponentDefNameTag@@QAE@XZ` against 0x004D3DFC, a stand-in clash of three CTCD* rows, not this family (T3).
  - The 53rd blank-status census line is 0x004D31C0, already in the in-class form.
- **All 270 rows re-formed, all ACCEPT, 0 HOLD.** The re-form is mechanical:
  - the static is defined in the class body (pick-any, selection 2);
  - an anonymous-namespace helper `?ReferenceStatGetClassTypeName@?A0x...` (`Fidelity: scaffolding, not retail`) takes its address. This is the nf86 / nf88 form (nf387 ruling), the NF407 / nf406_b2 0x004D31C0 form.
  - Names, literals, includes, leaves and tests are unchanged.
  - Locks are kept as NF408 on all 270.

| batch | rows | range | shape |
|---|---|---|---|
| b1 | 50 | 0x004D2D26..0x004D38CD | plain |
| b2 | 50 | 0x004D38E0..0x004D4716 | plain |
| b3 | 50 | 0x004D4746..0x004D50D3 | plain |
| b4 | 50 | 0x004D5103..0x004D5CD3 | plain |
| b5 | 50 | 0x004D5CFD..0x004D674D | plain |
| b6 | 20 | 0x004D4415..0x004D5E61 | namespaced |

- **Batch contents.** One unit per row; any batch lands alone. Each batch holds RESULTS.tsv, UNITS.tsv, rows.txt, rows/, tests/ (HEAD copies), orig/rows + orig/tests (HEAD copies), head/payload.json + oracle.tsv, logs/ (vland HEAD/new, reserved_check) and evidence/.
- **Table files.**
  - TABLE_CHANGES.tsv and SELF_SYMBOLS.tsv are header-only.
  - None of the 270 has a self_symbols.tsv line at HEAD, so the family has no self_symbols lines to restate (T3).
  - No header edits, no FLAGS, no RULINGS_NEEDED.
  - One NAME_FIXES line (0x004D5853, below).

## Link counts (VC7.1 link /DLL /NOENTRY /FORCE /NODEFAULTLIB /OPT:NOREF /OPT:NOICF, response file)

| set | before (HEAD) | after (new) | file |
|---|---|---|---|
| **whole census: binder + 462 objects (the 270 swapped)** | **272 total, 270 against the binder** | **2 total, 0 against the binder** | evidence/linkgate_all270.txt, link1_all270_all_{head,new}.txt |
| binder + the 270 rows (link377) | 270 | 0, NEW_UNRESOLVED 0 | evidence/link_all270.txt |
| per batch: binder + batch rows | 50 / 50 / 50 / 50 / 50 / 20 | 0 each, NEW_UNRESOLVED 0 | batches/bN/evidence/link_bN.txt |
| per batch: binder + 462 with that batch swapped | 270 against the binder | 220 / 220 / 220 / 220 / 220 / 250; no new duplicate | batches/bN/evidence/linkgate.txt |
| no binder: 462 census objects, HEAD v new (whole set and per batch) | 2 dup, 179 unresolved | 2 dup, 179 unresolved; NEW_UNRESOLVED 0 | linkgate_*.txt (`nb`) |
| emission probe: one TU calling all 270 statics + the 270 row objects, NO binder | unresolved: CCharString ctor / dtor only | same: every static resolves from its row object | evidence/link1_probe_{h,n}.txt, link_probe.cpp |

- **Unresolved sets.** They are identical HEAD v new in every link: 651 with the binder, 179 without.
- **The 2 remaining duplicates** are the CThingComponentDefNameTag stand-in ctor (T3), unchanged.
- **Why the probe exists.** Only 4 census objects reference a family static by its real name: the NF406/NF407 callers, none of them in the 270. So the no-binder census link alone does not prove emission. The probe does: it is a caller TU that UND-references all 270 symbols, linked without the binder against the new objects, and nothing is unresolved but CCharString.
- **Tool fix.** NF407/NF398 link377.py put the objects on the command line. With 271 objects the wine command line overflows, and link.exe silently prints nothing, which reads as "0 -> 0". tools/link377.py now uses a response file (T3 tools note).

## Gates per row (all 270 pass; tools/gates408.py checks each pattern strictly, and anything else would HOLD)
- **vland**, HEAD v new at one LANDVERIFY_WORK path per batch (work/wd_bN, moved to wd_bNh / wd_bNn by tools/runpair.sh; logs in batches/bN/logs):
  - all 270 go RELOCATION_MATCH PASS -> RELOCATION_MATCH PASS;
  - the WARN set is identical on both sides: SHARED_HEADER_AVAILABLE on 167 rows, none on 103;
  - there is no LITERAL_DIFF or LITERAL_UNCHECKED, with FABLE_EXE set (batches/bN/evidence/vland_cmp.txt).
- **objsec**, HEAD v new (objsec356, by COMDAT symbol; batches/bN/evidence/objsec.txt). Every row shows exactly:
  - the leaf's data, relocations and characteristics equal, 19 B, COMDAT selection 1 -> 2;
  - one ADDED 6-byte COMDAT `?ReferenceStatGetClassTypeName@?A0x<hash>@@YAP6I?AVCCharString@@XZXZ`;
  - one SYM+ (the helper), no SYM-, and no .drectve delta.
- **pk_all + pickany_vs_retail** (official pass26 copy), HEAD v new, `--leaf=` the full mangled leaf:
  - HEAD shows no copies;
  - new shows only the helper (strong, NOADDR, reach n);
  - pvr has no NEQ on either side, pkcmp346 gives GATE PASS, and reach flips are 0 (batches/bN/evidence/pk*, reachflip.txt).
- **Binder copy v row copy (R1 tier 1).**
  - Result: for all 270 the only difference is the literal form (tools/r10cmp.py, batches/bN/evidence/binder_comdat_cmp.txt): both copies are 19 B; every byte outside the literal field and every other relocation (the CCharString ctor) is equal.
  - How the copies differ: the binder's `return #TCClass;` relocates a `??_C@` literal of the class name. The row pushes HEAD's cast address.
  - The cast address is checked for all 270 (evidence/literal_check.tsv): it equals retail's own `push imm32` at the row's address, and its NUL-terminated text in Fable.exe is the class name.
  - So these are R10 debt, booked like NF407's 0x004D4A86: a second body under one pick-any name, the same text, no hold.
- **Tests are unchanged.**
  - All 270 tests declare the static and link the row object; none is self-contained (evidence/test_kind.tsv).
  - All PASS in vland on the new objects.
  - Mutation control on 12 rows, the first and last of each batch: a cast-address+1 mutant FAILs (BAD_TEXT) and the real object PASSes (evidence/test_mutation.txt).
- **reserved_check:** 0 reserved and 0 mentions in every batch. It was run with `RESERVED_REFS=""` because agent lanes run no git, so the B branch was not consulted (T3).

## Texts
- **tools/gen408.py** is NF407's tools/gen.py with two extensions:
  - it finds the row anywhere under rebuild/src/compiled;
  - it handles the namespaced shape (`namespace N { class C {...}; }` plus `N::C::StatGetClassTypeName()` out of line), keeping the class inside its namespace and taking `&N::C::StatGetClassTypeName` in the helper.
- **The transform is otherwise gen.py's:**
  - the out-of-line return statement and its trailing comment move into the class body unchanged;
  - the helper goes after the class;
  - a `// Form (NF408, ...; NF407's mechanical re-form)` paragraph is added after the leading comment block.
- **The diff is the same shape in all 270 rows.** After normalising class names, addresses and text, exactly 2 removed lines plus the out-of-line header, and the same added lines.
- **Pilot.** Four rows (plain, `: public <Class>Layout`, two namespaced) passed every gate before the full run; they are in b1..b6 like the rest.

## Found on the way (T3)
- **0x004D5853 binding conflict.** reloc_bindings.tsv binds `??0CTargetingResult_Retail@@QAE@XZ` (witness 0x004DEC09, a stand-in) at the address of CTCTargetingSpirit::StatGetClassTypeName. That is one address with two names (NAME_FIXES.tsv). It needs a rewrite of 0x004DEC09's model, not done here.
- **Stand-in callers.** 113 of the 270 addresses are called from landed GetClassTypeName rows through stand-in spellings such as `?Init_<addr>@T@@QAEXXZ` (evidence/standin_callers.tsv).
- **CThingComponentDefNameTag clash.** The ctor stand-in clash of 0x004D3DFC / 0x004D3EF9 / 0x004D41DF.

## HEAD moved during the run (other landings)
- **None of the 270 row or test files changed during the run.** All 270 still equal the orig/ copies, and the binder source is unchanged.
- **Three shared headers changed:**
  - engine/CTCCreatureGroup.h at 02:35, before the batch pairs (02:47..02:50), so both sides of each pair saw the same text;
  - engine/CCodaObjectBase.h and engine/CSalOutputKS.h at 03:00, after the pairs.
- **The 19 rows whose include tree reaches them were rerun as one pair on the 03:00 headers.** A plain #include walk that ignores #if over-approximates which rows those are. The rows: 0x004D2E61 2EA7 2F96 2FC9 30DB 32C7 33F9 34ED 3533 35B9 376D 37B3 39BC 4617 4716 47B9 4CD8 55BD 5FC8.
- **Result:** all 19 pass every per-row gate again (evidence/recheck_hdr/).
- **Repo files newer than the start of this run are other agents' landings,** not this run's: rebuild/src/compiled/00/c5/*, README.md, landed_metrics, the three headers, and tools/__pycache__/landed_metrics.*.pyc. One file may be this run's:
  - tools/decomp_pipeline/crawl/__pycache__/purity.cpython-313.pyc was rewritten at 02:53:37, 27 ms after another agent edited purity.py.
  - This run's link tools import verify_and_land, which imports crawl.purity, without `-B`. So the bytecode cache may have come from this run's import.
  - It is not deleted (deleting is also a repo write). Run the lane tools with `python3 -B` in future.

## Budget / hygiene
- **vland runs:** one 464-entry HEAD census (the binder entry is counted twice; 463 objects), one 4-row pilot pair, and six batch pairs.
- **Other compiles:** 12 mutant compiles and one probe TU.
- **No git, no wineserver -k, no repo edits** (the one possible bytecode-cache write is above). NF407's tools were copied into tools/ and repointed, never run in place.
