# NF371 (Account A naming queue, 2026-11-24): the seven CCountedPointer<T>::Reset rows and CScriptDefinitionManager::Set from NF366

**Two run dirs (land_scripted form):**
- `batches/b1/` **ACCEPT, lands now:** Set 0x00CD3F40 (self respell) and its binder CScriptInfoManager::Init 0x00CB5D80, with their two table
  lines (Allocate -> Reset at 0x00CD43A0, GFSetPDefinitionManager -> Set at 0x00CD3F40). Both objects are ALL_EQ under the renames. The Reset
  that Set now binds stays unresolved until b2 lands (the HEAD Allocate was unresolved too; NF302 Q2).
- `batches/b2/` **HOLD (verdict HOLD, land_scripted skips it):** the seven Reset self respells with the seven `?Destroy_<addr>` table lines.
  Every gate is measured and passes except objsec's one non-name difference (ruling R2); 0x00A299C0 and 0x00F39CA0 also need R1. When the
  rulings allow, flip RESULTS column 2 to ACCEPT (or, under R1-b, swap in `variants/<a>_inst.cpp`).
No header change, no test change. TABLE_CHANGES (all nine): 9 `replace` lines (apply_table.py --dry: 0 misses, no new multi-target symbol).
The landed HEAD Reset texts carry a `// Retail bytes:` comment that is an instruction listing (mnemonics, no hex); the new texts drop it, and
both head/payload.json files were built from the repo sources (mkbatch asserts equality), as D-f asks.

## Form (pilot on 0x00A299C0, `evidence/pilot_form.txt`)
HEAD's stand-in Reset is a plain function, COMDAT selection 1 (VC7.1 /O2 implies /Gy). Two header forms were graded (both RELOCATION_MATCH):
- **Adopted: a strong `template <>` specialization** of engine/CCountedPointer.h's Reset with the landed body, under the landed opt-ins
  `FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL` (Reset declaration only; with the in-class body a specialization emits nothing, NF360)
  and `FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY` (the stored deleter stays an external reference to its nf366 owner). T is
  forward-declared, as the landed Reset siblings do. Selection 1 both sides, leaf bytes / relocations / characteristics equal under the renames.
- **Not adopted: the sibling explicit instantiation** (`template void CCountedPointer<T>::Reset(T*);`, `variants/*_inst.cpp`): the leaf
  becomes selection 2 and the object gains a pick-any `?ReleaseData@?$CCountedPointer@<T>@@QAEXXZ` COMDAT (53 B, unreferenced): NF320 Q2.

## Gates
- **vland --recheck-landed** (logs/): 9/9 RELOCATION_MATCH PASS HEAD and new, behaviour PASS. New warnings: only RELOC_NAME on the DeleteData
  targets (catalog still `sub_<addr>`, nf366 RENAMES not applied) and STANDIN_LEAF on 0x00CD3F40 (catalog still CEngineSubPrimitiveBase::
  operator_new; RENAMES). HEAD's STANDIN_LEAF on 0x00A299C0 / 0x00F39CA0 / 0x00CD3F40 (old names) are gone.
- **objsec** (`tools/objsec371.py`, explicit rename map, `evidence/objsec/`): 0x00CD3F40 and 0x00CB5D80 ALL_EQ. Each Reset row: leaf,
  `.drectve`, UND set and DEF set equal under the map; the one remaining difference is the unreferenced pick-any ctor COMDAT that every
  count-block `new` emits: HEAD's stand-in `??0SCountBlock@@QAE@PAX@Z` (1 parameter) becomes the header's `??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z`
  (3 parameters, FableWin.pdb name), both 25 B, so the body differs (ruling R2).
- **Pick-any** (pk_all + pickany_vs_retail, `--leaf` per side): SAME for all 9, GATE PASS; reversed control PASS; planted control (a leaf-called
  NEQ line in N_00f39ca0) FAIL as it must; reachflip 0 of 9 pairs.
- **Drop test** (`evidence/droptest.txt`): dropped names are the stand-in selves (self_symbols only, no table line, no other user), the
  `??0SCountBlock` pick-any copy (no table line; 833 landed sources use that stand-in name, none references it), and the table stand-ins
  `?Destroy_<addr>`, `?Allocate@CFixedAllocator`, `?GFSetPDefinitionManager` (each witnessed only by the row that drops it; replaced).
- **Link** (`evidence/link/`, /FORCE, HEAD v new; controls HEAD-compiled): every Reset with its DeleteData owner and its retail callers' rows.
  - The DeleteData reference now resolves for all seven (HEAD left `?Destroy_<addr>@@YIXPAX@Z` unresolved).
  - 0x00CD43A0 + 0x00CD42E0 + 0x00CD3F40 + 0x00CB5D80: Reset, Set and DeleteData all resolve (HEAD: Allocate, GFSetPDefinitionManager, Destroy
    unresolved); 0 duplicates.
  - New duplicates: 2 LNK2005, the strong Reset<C3DMesh2> v 0x00A24520's pick-any copy and the strong Reset<CRenderTarget> v 0x00F38990's
    copy; both copies EQUAL (cmpcomdat: 103 B, bytes and relocation symbols, `evidence/cmpcomdat_reset_*.txt`) (ruling R1). The two DeleteData
    LNK2005 in those links are nf366's N2 pair, unchanged.
  - Binders that stay unresolved H and N (stand-in classes, queued): `?Reset@CEntryCountedPointer@...` (0x00A26D40), `?Reset@CAllocatorPointer@...`
    (0x00B3B240). Nothing HEAD resolved is left unresolved.
- **Census** (`evidence/symscan_controls.txt`, 36 objects: the rows H and N, 16 HEAD controls; closure below): definers of a new Reset name besides the self:
  only 0x00A24520 (C3DMesh2) and 0x00F38990 (CRenderTarget), both pick-any and EQUAL. Controls: the seven DeleteData owners, the retail
  callers 0x00A26D40 / 0x00A24520 / 0x00B3B240 / 0x00F38990 / 0x00CB5D80, and every landed source that names CCountedPointer<T> and calls
  Reset: 0x00A27030, 0x00BA6240, 0x00B81F30, 0x00B82100, 0x00B9A4A0 (none of the last five defines a new name).
  - Closure: any Reset<T> copy names DeleteData<T> (defined or UND). NF366's committed 41-object symscan (its controls include 0x00A254A0 and
    0x00AA4210) searched the seven DeleteData names and found them only in the owners, the Reset rows' own references and the pick-any copies
    in 0x00A24520 / 0x00F38990. So no other control defines a Reset copy of these Ts.
  - Not compiled here: 0x00A254A0 (held row; covered by that closure) and 0x00AA4210 (T3 column 2, reserved). NF366's `link_00aa4310.txt`
    shows that object linking beside 0x00AA4460 with 0 duplicates and naming only the C3DOptimisedMeshBase spellings (it leaves
    `?DeleteData@?$CCountedPointer@VC3DOptimisedMeshBase@@@@SIXPAX@Z` UND), none of which this batch adds.
- **PDB spelling** (`evidence/pdb_exact.txt`): all 7 Reset names, Set and ??0CCPPointerInfo are whole entries in the FableWin.pdb name set
  (Set also in Ego_r).

## Set 0x00CD3F40 and binder 0x00CB5D80
- Set: the row now defines `?Set@CScriptDefinitionManager@@SIXPAV1@@Z` from engine/CScriptDefinitionManager.h (0x00CD42E0's define set) and
  calls Reset<CScriptDefinitionManager> declared only. PClass stays at its address (`CScriptDefinitionManager_PClass` macro, retyped to the
  counted pointer): no data-symbol spelling is on the lane, and the landed row had no relocation there either. Its test (a CFixedAllocator stub
  under the old names) is unchanged (NF284 D2); the PassPattern stays.
- 0x00CB5D80: `static void __fastcall Set(CScriptDefinitionManager*)` added to the landed local model, the free GFSetPDefinitionManager
  declaration removed; ALL_EQ. The local model of a class with a shared header (SHARED_HEADER_AVAILABLE) is pre-existing debt.

## Skipped binders (QUEUE_PROPOSALS.tsv, NAME_FIXES.tsv)
- 0x00A26D40 (Reset 0x00A29D20 via the stand-in CEntryCountedPointer, also the element type of its resize stand-in) and 0x00B3B240 (Reset
  0x00B3E510 via the stand-in CAllocatorPointer with the wrong pointee CPoolAllocator): ports, not name changes. Their table lines stay
  (their witnesses still bind them).
- 0x00AA4210: T3 nf366 (reserved); its Reset<C3DOptimisedMeshBase> line stays.
- 0x00447D90 has no binder row (caller 0x00447216 is not a catalog row).

## Rulings needed
- **R1 (0x00A299C0, 0x00F39CA0: strong owner beside includers' pick-any copies).** The binders 0x00A24520 / 0x00F38990 emit pick-any
  Reset copies (and ReleaseData / DeleteData copies). Two measured texts, both RELOCATION_MATCH:
  - **R1-a, adopted in rows/:** the strong specialization. objsec is as for the other five rows. The link adds one LNK2005 per row against
    the EQUAL copy (cmpcomdat: 103 B, bytes and relocation symbols). This is the nf366 N2 / R3 shape, but GOTCHAS nf80 (line 427) and
    nf135 (line 469) say a strong owner beside includers' pick-any instances is a link error to avoid.
  - **R1-b, `variants/<a>_inst.cpp`:** the sibling explicit instantiation (the form GOTCHAS nf80 prescribes for a template member). The link
    has no new duplicate (`variants/link_<a>_inst.txt`), and pk gate SAME with 0 reach flips (`evidence/pk_gate_inst/`). The costs:
    - the leaf's COMDAT selection changes 1 -> 2;
    - the object gains an unreferenced pick-any `?ReleaseData@?$CCountedPointer@<T>@@QAEXXZ` (53 B), EQUAL to the binder's own copy
      (`evidence/cmpcomdat_releasedata_inst.txt`), which is NF320 Q2;
    - objsec is in `variants/<a>_inst.txt`.
  Which form should land for these two?
- **R2 (pick-any ctor swap):** each Reset object drops the unreferenced stand-in `??0SCountBlock@@QAE@PAX@Z` and emits the header's
  `??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z` (PDB name, no table line, no landed strong definition, equal to the copy every header-form includer
  emits: cmpcomdat over 0x00A24520 / 0x00F38990 / three of these rows). Read as NF304 Q1 (a stand-in pick-any copy of a local inline
  member now in the PDB spelling, no landed strong definition). Any header-form move of a count-block stand-in carries it.

## Coordinator follow-ups
- RENAMES.tsv: catalog Module / name / file proposals for the 8 selves (not applied); self_symbols at the next pass.
- vland RELOC_NAME warnings on the DeleteData targets clear when nf366's catalog renames are applied.

## Process
- Reading list: BRIEF.md, BRIEF_NF_BATCH.md, EXTRA_RULES.md, COORDINATION.md, ASM_RULES.md read in full; GOTCHAS.md grepped (specialization,
  instantiation, CCountedPointer, DeleteData, COMDAT / pick-any / strong).
- Locks: 9 rows + 16 controls claimed; tests prechecked before claiming, sources (HEAD and new) after, all clang PASS (work/pre.log); all
  released at the end (evidence/locks_final.txt).
- Grades: pass.py one-path H x10, N x13 (0x00A299C0: pilot inst + spec + final + final inst; 0x00F39CA0: final + final inst), controls 16
  HEAD; vland chain H and N once (9 rows). The five other rows' instantiation texts were never graded and are not handed back.
- Tools (tools/): NF366's set retargeted, plus objsec371.py, drop371.py, mkrows371.py, mkbatch371.py, run_b1.sh, run_ctl.sh, run_links.sh, run_links_inst.sh,
  objsec_b1.sh. No retail bytes in the hand-back (the vland oracles stay in work/).
