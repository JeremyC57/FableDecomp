# NF423 hand-back (2026-11-25): T1 caller respells, then the whole-T1 generator (coordinator scope change)

## Result
- **158 rows ACCEPT** in 6 batches. Locks kept as NF423 on exactly these 158. Every other NF423 lock is released: HOLD rows, pilots, and the definer self rows I locked only to compile them.
- **76 rows HOLD** in 5 `*_hold` folders. Their locks are released.
- Everything else is in **RESIDUE.tsv** with a reason code.

| batch | rows | what | table |
|---|---:|---|---|
| b1 | 8 | CCountedPointer member release, free `__fastcall` stand-ins -> `??1` / `ReleaseData` of `CCountedPointer<T>` (DTOR_DECL_ONLY / RELEASEDATA_DECL_ONLY), qualified call | 8 delete + 1 add |
| c1 / c2 / c3 | 50 / 50 / 20 | the whole CThingSwitch family: `?AutoTinySecondMemberDestructor@@YIXPAX@Z` -> `??1?$CCountedPointer@VCEntityScriptBase@@@@QAE@XZ` (0x00CE1000; retail E8 scan: all 120 rows call it). Unit `sw120`, landed in order c1, c2, c3 | c1 rewitness to a c3 row; c3 delete |
| g1 / g2 | 20 / 10 | generator shape DTFREE: free stand-in -> qualified public destructor of a plain class, with a bodiless `class C { public: [virtual] ~C(); };` | 27 delete + 13 add |
| b1_hold, d1_hold | 4 + 1 | pilots, see RESULTS | - |
| g1_hold / g2_hold / g3_hold | 30 / 40 / 1 | DTFREE rows that gain WARN SHARED_HEADER_AVAILABLE (RULINGS_NEEDED r2) | - |

UNITS.tsv: rows sharing one stand-in line are one unit:
- b1: 008ced20 + 00741eb0 + 007190f0;
- g1: the four CAnimFilterOf3Groups rows;
- c1 + c2 + c3: sw120.

## Gates (every ACCEPT row)
- **vland:** HEAD and new compiled at one LANDVERIFY_WORK path (tools/runpair.sh). The grade is the same on both sides and PASS. There are 0 LITERAL warns and no new WARN kind (tools/warncmp.py). Every row's RELOC_NAME warn for the stand-in disappears.
- **objgate** (tools/gate423.py):
  - every non-debug section pairs by COMDAT symbol, and data, characteristics and relocations are equal under the rename;
  - the DEF sets are equal;
  - the UND delta is exactly old -> new;
  - `.drectve` is equal.

  All 158 rows are ALL_EQ.
- **Target binding:** tools/xrcheck423.py resolves each new object's leaf relocations against the retail bytes. Every renamed symbol (160 renames over 158 rows) resolves to exactly its target, and no old symbol remains: 0 BAD (batches/*/evidence/xrcheck.txt). This includes the 14 symbols that had no table line before.
- **pk** (tools/pkgate423.py): pk_all + pickany_vs_retail on HEAD and new, with the leaf named. There are 0 new NEQ and the copy counts are unchanged.
- **/FORCE link** (tools/linkgate423.py): the batch objects plus the target self rows (compiled from HEAD into work/g/<a>_D). This is NOT a census of every pick-any definer in the tree. The argument that it suffices: the new objects are UND-only for every new name (objgate DEF sets equal), so they cannot add a duplicate against any definer anywhere.
  - Duplicates are not up: b1 0->0, g1 1->1, g2 0->0, c1 49->49, c2 49->49, c3 19->19. The c numbers are the family's existing `?CThingSwitch_CompositeScalarDeletingDestructor` self-duplicate debt.
  - NEW_UNRESOLVED is 0, and every new name is DEFINED.
  - Every stand-in that was unresolved at HEAD is gone.
  - The new objects add no DEF symbol, so they cannot become a definer (the nf402 trap).
- **apply_table --dry:** 0 misses and no new multi-target symbol, for each batch and for all of them together (work/ALL_TABLE.tsv: 36 delete + 14 add). bindings_multi_check is unchanged (1).
- **reserved_check** (RESERVED_REFS=""): 0 for every batch, HOLD folders included.
- **Tests:** every HEAD test is self-contained (a stub copy under the stand-in name, NF284 D2) and the leaf bytes are unchanged, so no mutation control (NF419 r1).
- **orig/:** re-diffed against the repo at hand-back, 468 files, 0 drift.
- **Screens** (tools/screen423.py): no row or target whose newest COORDINATION_CLAIMS line is Account B. No ACCEPT row is named in T3_PORT_QUEUE col 2/3. 24 T3 lines name an ACCEPT batch's targets, listed in evidence/T3_RESOLVES.tsv:
  - 19 are resolved fully;
  - 5 are resolved partly, with one other spelling left, e.g. 0x0048C5C0 keeps 0x00715110's `??1CCountedPointer@@QAE@XZ`.
- **Identity spot check:** 0x004DB461 (CTCPlot's ??_G, RTTI vftable 0x012409B4 slot 0) calls `??1CTCDiceGame`. This fits the landed model `class CTCPlot : public CTCDiceGame`, and Ego_r gives both classes the same size (24).
- **g1 / g2 split:** g1 and g2 were split after gating. Their head/payload.json is filtered to the ACCEPT rows (tools/payfilter423.py). The gate logs and linkgate.txt cover the 50-row pre-split superset (same objects; noted in CHECKS).

## Names
Each new name has two sources:
- the T1 tracker PDB spelling: 80 of the 101 generated rows' names are in both PDB name sets, the other 21 and all b1/c names in FableWin;
- the landed self row's Leaf, whose object DEFINES it: the 72 + 8 definer objects in work/g.

## The generator (coordinator's scope change): tools/gen423.py
- **Input:** tracker/OPEN.tsv (T1, 1,850 targets) and reloc_bindings.tsv.
- **Shapes it writes:**
  - CPFREE: free stand-in -> CCountedPointer `??1` / ReleaseData / DeleteData, with T a plain or N-namespace class.
  - DTFREE: free stand-in -> public plain destructor.
- **Callers:** every landed source naming the stand-in, not only the table witness. Each must reference the target in its retail bytes.
- **Screens:** locks, T3 col 2/3, an own CCountedPointer model, a CCountedPointer opt-in already set, and the class already named in the row.
- **Pipeline per batch** (all scripted in tools/; driver tools/pipeline423.sh):
  1. gen423 -> plan (gen423 takes no locks)
  2. mkbatch423.py: batch folders, lock claims, reserved_check input
  3. prefam.sh (clang prechecks of HEAD row, test and new row)
  4. runpair.sh (vland HEAD/new)
  5. gates_bg.sh (gate423 + pkgate423 + linkgate423; definers by predef.sh / defobj.py)
  6. finalize423.py (verdicts, TABLE_CHANGES with an identifier census, RESULTS / CHECKS / UNITS / head/payload.json)
  7. split423.py (ACCEPT v HOLD folders)
  8. finalize again with the accepted set
  9. units423.py, payfilter423.py, xrcheck423.py, screen423.py, residue423.py, release423.sh
- **Reproducibility of CPFREE:** b1 and c1–c3 were made by hand and by tools/famsw.py before gen423 existed. A clean run (`SKIP=none`, work/gen0_*) regenerates:
  - all 120 family rows, code-identical except one redundant second `#include "engine/CCountedPointer.h"`;
  - the b1 single-respell rows, the same code (0x0065E935 spells the cast `reinterpret_cast` instead of `static_cast`).
  - The two-respell rows 0x00A437B0 / 0x00AF1BD0 are residue for the generator (one generated edit per row).
- **Run on the whole T1 set:** 101 DTFREE rows planned, 30 ACCEPT. The other 71 gained SHARED_HEADER_AVAILABLE.
- **Header variant** (tools/hdrvar.py, evidence/hdrvariant): include engine/C.h instead of the local declaration.
  - 15 of the 30 tried fail precheck: the class is behind a FABLE_RETAIL_ opt-in.
  - The 15 that compile DIFFER (e.g. 34v28).
  - So it is not a mechanical fix, and these rows are RULINGS_NEEDED r2.

## Residue (RESIDUE.tsv, one line per wrong table spelling or caller row, with the reason)
Top codes (vs the PDB spelling):

| code | lines | meaning |
|---|---:|---|
| SPECIAL | 1266 | ctor / dtor / operator forms, `@x@N`, stand-in classes |
| OTHER_NAME | 623 | identity research |
| SCOPE_AND_TAIL | 428 | |
| TAIL_ONLY | 207 | signature: needs call-site type edits |
| SCOPE_ONLY | 121 | stand-in class with the same member and tail: needs the real class model |
| C_NAME | 108 | e.g. zlib `_deflate` v `_z_deflate`: vendor prefix |
| BATCH_HOLD | 76 | |
| A0X_HASH | 3 | anonymous-namespace path hash only: a tracker normalisation issue, not a fix |
| screens | rest | |

SCOPE_ONLY is the next mechanical candidate, but the d1 pilot (0x0088EA20, CThing_Retail -> CThing via engine/CThing.h) DIFFERs, so a caller-only respell through the shared header is not byte-safe there.

## Findings
- **Explicit destructor calls need the qualified form.** VC7.1 emits a pick-any `??_G<T>` COMDAT for an unqualified explicit destructor call `p->~T()` on a declared destructor. The qualified `p->T::~T()` emits none. HEAD rows that used the unqualified form carry such junk `??_G` stand-ins (0x0065FE69's `??_GCValue_0065d5a0`).
- **RELEASEDATA_DECL_ONLY on a member emits a NEQ COMDAT.** A CCountedPointer member under that opt-in makes VC7.1 emit `??1` as a 5-byte `jmp ReleaseData` pick-any copy (0x005CC091). That is NEQ to retail where `??1` is the full body.
- **Including the class's shared header changes the leaf.** Including engine/CThing.h (or a class header) into a row that called through a local model changed the leaf bytes in 16 of 16 measured rows. Caller respells through headers need per-row ports.
