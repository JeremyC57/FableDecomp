# NF252 notes (2026-11-22, round 19): the E3 0x01296F14 unit (with L101-L103); handed back after it

Handed back at an entry boundary (context), as the brief allows ("Hand back after it if it is large"). 6 rows changed, all MATCH /
RELOCATION_MATCH + test PASS in one `vland --recheck-landed` run with the two unchanged TU-mates 0x0096DF50 / 0x0096DBC0
(recheck_final.log, 8 rows). One header changed (engine/CCreatureAction_ScreamerIntoAggressive.h: one add-only opt-in; hdr/ + orig/,
equiv_check in hdr/README.md). No new split: every address touched ends on the same or fewer spellings (census below). Link of the
six new objects + the 0x0096DF50 HEAD object: 0 LNK4006 / LNK2005 (control link of a duplicated object: 1 LNK4006, so the check is live).
Repo root listed before / after: identical. No git, no foreground sleep, every grade under timeout, every grade after a clean clang precheck.

## E3 0x01296F14 (done): the vtable and every binder as one unit
What the repo already had: engine/CCreatureAction_ScreamerIntoAggressive.h ALREADY wraps the class in `namespace { }` (Codex pass 27, for
0x0096DF50 Update), so the namespace opt-in the brief / nf250 plan described was not needed. The only header edit is the ctor opt-in.

| row | before | after | grade |
|---|---|---|---|
| 0x0096DB60 ctor | `??0...@@QAE@PAVCThingCreatureBase@@PAVCThing@@@Z`, row-local CSoundPair / CCombatActionBase / class | `??0CCreatureAction_ScreamerIntoAggressive@?A0x<tu>@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z` on the header (new `_CTOR_OUTOFLINE` opt-in, declaration only) | RELOCATION_MATCH |
| 0x0096E510 Clone | `?Clone@...@@QAEPAVCCombatActionBase@@XZ` (non-virtual, local model) | `?Clone@...@?A0x<tu>@@UBEPAVCCreatureActionBase@@XZ` (+ existing FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_COPYCTOR) | RELOCATION_MATCH |
| 0x0096E3F0 ??_G | free `__fastcall` stand-in | `??_G...@?A0x<tu>@@UAEPAXI@Z`, anonymous-namespace emission harness | RELOCATION_MATCH |
| 0x0096DBA0 (L101) | `?Get@T@@QAE_NXZ` | `?IsToFaceTargetAtStart@...@?A0x<tu>@@UBE_NXZ` (same harness) | MATCH |
| 0x0096DBB0 (L102) | `?Get@T@@QAE_NXZ` | `?IsToFaceTarget@...@?A0x<tu>@@UBE_NXZ` (same harness) | MATCH |
| 0x0096DDB0 OnReplace | 13 row-local models, global-namespace stand-in vtable (v00.. slot order) | the 0x0096DF50 header set + engine/campfire_types.h (CBRAINOUTPUT_ONLY) + CAIBrain SETOUTPUT | RELOCATION_MATCH |

- Prelude: every changed row uses the #define / #include block of the landed 0x0096DF50 (its HEAD object emits this vtable), so each of the
  six vtable COMDATs is slot-for-slot that copy (vtcheck.py, hash-normalised diff 0 of 81 slots, all six incl. OnReplace). No object
  emits any other `??_7` (listed with objdump).
- Slot identity: retail 0x01296F14 slot 0 = 0x0096E3F0, 11 = 0x0096E510, 12 = 0x0096DBC0, 73 = 0x0096DBB0, 74 = 0x0096DBA0 (vslot.py);
  the 0x0096DF50 HEAD vtable COMDAT puts IsToFaceTarget at 73 and IsToFaceTargetAtStart at 74 (CCombatActionBase.h declaration order).
  Names: FableWin.pdb S_PUB32 for IsToFaceTarget / IsToFaceTargetAtStart / the ctor (??0 ... AAV, AAV) / ??1 / ??_E; Ego_r.pdb + FableWin for
  Clone (UBE, CCreatureActionBase*) and Initialise.
- Ctor self signature PAV, PAV -> AAV, AAV: FableWin.pdb S_PUB32 + the body (two pointer-sized arguments, `ret 8`, handed to the base ctor's
  reference parameters) + the header's existing declaration. No E8 / E9 site calls 0x0096DB60 (xsites.py; control: 0x00857F30 lists its
  callers), retail keeps the out-of-line copy; the callers expand it.
- Pick-any copies: every function COMDAT the six objects define is byte- and relocation-identical to 0x0096DF50's HEAD copy (cmpobj.py,
  hash-normalised), except ones 0x0096DF50 does not define: OnReplace's `??1?$CCountedPointer<CCreatureActionBase>` EQ 0x0048C5C0,
  `??1CBrainOutput` EQ 0x006D60C0, `?SetAsPerformAction@CBrainOutput` EQ 0x00860130 (pk.py, owners.tsv); the harness functions
  (anonymous namespace, never called); Clone's implicit copy ctor `??0...@?A0x<tu>@@QAE@ABV01@@Z` (25 B, unreferenced: Clone expands it;
  no retail owner; HEAD's Clone object defined the same implicit member under the global name `??0...@@QAE@ABU0@@Z`).
- Relocation moves (RENAMES section 2): 0x01296F14 global -> anonymous (3 rows); 0x0073A8A0 `?HasMode@CTCCreatureModeManager@@QAE_NJ@Z`
  (stand-in) -> `?IsModeActive@CTCCreatureModeManager@@QBE_NK@Z` (both PDBs; 0x0096DF50 binds it), 0x00857F30 PAV -> AAV (both PDBs) in
  OnReplace and the ctor. Both old spellings keep other landed binders (table witnesses 0x009621F0 / 0x0066A690): count-neutral there.

### Census (normalised: `?A0x[0-9a-f]{8}` stripped, EXTRA_RULES 2026-11-20)
| address | HEAD spellings | after |
|---|---|---|
| 0x01296F14 vtable | global (0x0096DB60, 0x0096DDB0, 0x0096E510) + anonymous (0x0096DF50) = 2 | anonymous only = 1 |
| 0x0096DB60 ctor | global PAV (self, OnReplace COMDAT) + anonymous AAV (0x0096DF50) = 2 | 1 |
| 0x0096E510 Clone | self QAE / CCombatActionBase* + global UBE (OnReplace slot 11, undefined) + anonymous UBE (0x0096DF50) = 3 | 1 |
| 0x0096E3F0 ??_G | free stand-in + global (OnReplace) + anonymous (0x0096DF50) = 3 | 1 |
| 0x0096DBA0 / 0x0096DBB0 | `?Get@T@@QAE_NXZ` + anonymous (0x0096DF50) = 2 each | 1 each |

## Evidence caveats / slips
- PDB names were read from the ghidra_out/{egor,fablewin}_pdb_names.tsv dumps of Ego_r.pdb / FableWin.pdb, not from the PDB files; the
  rows' "S_PUB32" wording means those public-symbol dumps.
- The vtcheck / pk.py / cmpobj.py evidence is from the objects graded before two comment-only edits (the RTTI-hash wording, the line-1
  titles); recheck_final.log is over the final text (8 rows, all PASS).
- "Every grade after a clean clang precheck" holds for my new sources (all six prechecked clean; the line-1 title sed came after the last
  precheck, comment only). The HEAD compiles of landed sources had no precheck.
- 0x0096DBC0 was compiled (HEAD census) and rechecked before I claimed its lock (it had none); claimed NF252 afterwards. 0x0096DF50 is
  locked NF252 but unchanged (the reference vtable).

## Decisions needed
1. **The anonymous-namespace hash.** Retail's own RTTI type descriptor is `.?AVCCreatureAction_ScreamerIntoAggressive@?A0x6cb5dcbb@@`
   (Fable.exe; rtti-vtables.tsv module); Ego_r.pdb / FableWin.pdb say `?A0x4c815337` (their builds). So the catalog's `?A0x6cb5dcbb` is NOT
   a rebuild hash (the nf250 plan and OPEN_LINES said it was): I kept it, and RENAMES / TABLE_CHANGES / Names lines write the retail
   hash. EXTRA_RULES 2026-11-20 says "the PDB spelling with the PDB's hash": for a retail RTTI class the retail hash looks the better
   reading. Confirm, or respell to 4c815337.
2. **Leaf lines stop at `?A0x`** (as the sibling 0x0096DBC0 and most landed anonymous-namespace rows do): the grader matches Leaf by
   substring, and a full hash (retail's or the PDB's) matches no symbol in a rebuild object (it would grade "the largest function"). The full
   spelling is on each row's Names line. OK?
3. **Catalog renames** (RENAMES section 1): 0x0096DBA0 vfunc_74 -> IsToFaceTargetAtStart, 0x0096DBB0 vfunc_73 -> IsToFaceTarget (module
   `_gapscan` -> `?A0x6cb5dcbb::CCreatureAction_ScreamerIntoAggressive`; the files are `gapscan_sub...`), and the ctor / Clone (and Initialise,
   NAME_FIXES) catalog names gaining the `?A0x6cb5dcbb::` namespace their siblings carry. 0x0096E3F0 keeps its catalog name.
4. **Table line**: replace `??_7CCreatureAction_ScreamerIntoAggressive@@6B@` (witness 0x0096DB60) with the anonymous spelling (TABLE_CHANGES;
   the hash in the line is decision 1).
5. The ??_G / slot-row harness (`EmitScreamerIntoAggressiveVirtuals`, anonymous namespace, `// Fidelity: scaffolding, not retail`) is the
   nf250-accepted pattern; three rows share it (each its own leaf). vland's STANDIN_LEAF warning on 0x0096E3F0 is the catalog text (non-blocking).

## Header (hdr/README.md)
engine/CCreatureAction_ScreamerIntoAggressive.h: `FABLE_RETAIL_CCREATUREACTION_SCREAMERINTOAGGRESSIVE_CTOR_OUTOFLINE` wraps the existing
in-class ctor in `#ifdef ... #else <existing lines> #endif` (declaration only on the opt-in path). The static assert below moved 6 lines:
pinned (`FABLE_STATIC_ASSERT_AT(17, ...)`, pin_asserts.py). Sole landed includer: 0x0096DF50 (it does not define the macro).

## Flags lines / funclets / tests / descriptive names
- Flags: none needed (no row in the unit has CompilerFlags; none of my rows has a `// Flags:` line). No new CompilerFlags for the coordinator.
- Funclets: none of the changed sources is included by an EH funclet row (grep of `../src/compiled` includes: none).
- Tests: all six keep their HEAD tests (host models / trivial PASS stubs; PASS in recheck_final.log). Rewriting them on the shared header is owed.
- Descriptive names: the harness function `EmitScreamerIntoAggressiveVirtuals` (scaffolding, anonymous namespace). The class names are PDB / RTTI.

## Open (not started, context): L4 / L5, L28, L57, L111, L114, L120, L37, L129 (APPLIED.tsv keeps each line's census)
- L4 / L5 kill checks done and passed: both PDBs spell GetRewardItems over `vector<pair<long,CCharString> >` (J), so the pair<int> -> pair<long>
  self signature has its two sources; 0x0074FCE0 builds /Gr (CompilerFlags). Census: 0x0074F4F0 (2 sites) and 0x0074F530 (1 site) are
  called only from 0x0074FCE0; 0x0074FCE0 is called by 0x0074FE50 / 0x008A9AE0 / 0x008ED1F0 / 0x008ED440 under three table spellings
  (STLport vector<pair<long,CCharString> >, CBoastVector, CContainerItemVector stand-ins). So 0x0074FCE0's own leaf move is a family of its
  four binders on top of the 3-row unit; the sibling 0x0074FE50 (push_back) already spells the STLport overflow (its model is the J054 one).
- L114 sketch: N3DPrimitive::CAnimatedBlock::CDeviceIndependent is declared nested in engine/C3DPrimitive2.h (FABLE_RETAIL_CANIMATEDBLOCK_DATA:
  CIVCountedPointeeBase + CArray<unsigned long> +0x08 + CArray<C3DIndexedBoneWeight> +0x14), so the implicit virtual ~ (0x00A978C0) and the
  ??_G (0x00A978A0, which calls it) could come from one emission harness on that header; vtcheck its vtable first (who emits it at HEAD:
  0x00A90AE0 / 0x00A92900 include the opt-in).
