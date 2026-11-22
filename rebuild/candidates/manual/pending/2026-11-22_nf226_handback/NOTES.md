# NF226 notes (2026-11-22, round 10): the nf225 follow-ups (q12566-q12570)

Base: the repo working tree with nf225 applied. Locks NF226: 0041e641 0041e3f6 0041e5f2 00494840 0054ed90 0042fb40 0042db20 (were
`released`), 0041e1cd (new mkdir). No AUTH12 lock (00cf4ec0), Codex pass 27 row or 0x00A4EFC0 touched. Tools: NF225's (tools/, re-pathed)
plus tools/ownwalk.py (propagates retail owners from a leaf through every COMDAT's relocations), vtcheck.py (vtable COMDAT v retail, slot
by slot), mkobj.sh (objects for the link check), qd.sh, tst.py (behaviour_of's test path, with per-step timeouts and EXTRA_OBJ).

## Entry 1 (q12566 + q12567): the access specifiers -- done, as ONE 4-row unit
Why one unit: HEAD 0x0041E3F6 (local model with a virtual ~CManager) defines the pick-any ??_GCManager@NUISystem@@UAEPAXI@Z and a 1-slot
??_7CManager, which call ??1CManager@NUISystem@@UAE@XZ -- the symbol HEAD 0x0041E641 defines and its port removes. So 0x0041E641 cannot move
alone; and the ctor on the header calls the virtual SetMetaLayer directly, which needs 0x0041E1CD's self leaf to move too (no new split).
- **0x0041E641** (1 form): onto engine/NUISystem_CManager.h; the header layout is retail's (sizeof 0xD0, every member offset as the
  retail bodies use; work/t/probe_layout.cpp) -- the "does not reproduce the retail offsets" comments in 0x0042A9B7 etc. are stale.
  New opt-in FABLE_RETAIL_CMANAGER_SETGRAPHICSBANK (declaration). Leaf IAE; RELOCATION_MATCH.
- **0x0041E3F6** (3 forms, cap 3): f1 inlined CObservable's implicit ctor and the map ctors (DIFFER 532 v 508); f2 adds
  FABLE_RETAIL_COBSERVABLE_CTOR_DECL_ONLY (new, NUISystem_CObservable.h, included first) and the existing FABLE_RETAIL_STLP_MAP:
  RELOCATION_MATCH, but the end-of-TU template COMDATs were built under `optimize("y", off)` and ~CCountedPointer had the default body
  (7 NEQ copies); f3 adds FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA and moves `optimize("y", off)` to just before the ctor with
  `optimize("y", on)` after it (marked Fidelity: scaffolding). **Coordinator: confirm the trailing `optimize("y", on)`** (a row-tail
  pragma, not a header opt-in; it only restores the TU's own "s"-only mode for the instantiations VC7.1 emits at the end of the TU).
- **0x0041E5F2** (1 form): onto the header (GPUNIQUEINSTANCE); the ctor binder moves QAE -> IAE. objdump diff = that UND only.
- **0x0041E1CD** (1 form): in-place `virtual` on the existing local model's SetMetaLayer (pad 0x70 -> 0x6C for the vptr): leaf QAEXJ ->
  UAEXJ (both PDBs; vtable slot 34; absrefs: its only absolute reference is that slot, .rdata 0x012301BC). objdump diff = the leaf only
  (no vtable emitted: no ctor / dtor in the TU). Same class token. **Coordinator: confirm** that adding `virtual` to a member of an existing
  local model of a class that has a header (NUISystem::CManager) counts as an in-place respell (my reading of the 2026-11-20 rule; a port
  onto the header needs the map operator[] stand-in replaced first). Its HEAD test is kept unchanged and is a host model of CManager.
- Pick-any (pk.py with owners propagated by ownwalk.py; owners.tsv "NF226" blocks): 0x0041E641 23 EQ (HEAD 6 EQ), 0x0041E3F6 34 EQ (HEAD 2 NEQ, 0 EQ); work/pk_fin.tsv, sc_fin.log; 0x0041E1CD keeps its HEAD NEQ stand-in copy ??ACLayerTree (17 v 69, unchanged debt) (incl. map / tree /
  _List_base / _Rb_tree_base ctors, ~CObservable, all clear / _M_erase / ReleaseData / ~CCountedPointer copies). **One NEQ in both:**
  `??1?$list@PAVCObserver@NUISystem@@...@QAE@XZ` 5-B `jmp ~_List_base` v 0x0042AC25's 23 B. Facts: no code relocation names it in
  either object (only `.debug$F`); ~CObservable calls ~_List_base<CObserver*>, which retail folds into 0x0042AC25 (that copy is EQ);
  the self row 0x0042AC25 emits only its placeholder leaf ??1list_T_0042ac25@std@@; the table witness 0x0055CB10 defines its own
  ~list<CObserver*> copy of **45 bytes** (calls it 3 times), so that copy already differs from 0x0042AC25 (23 B) at HEAD -- the symbol has
  disagreeing copies before this unit. Link with both: no LNK4006 / LNK2005 on it (pick-any). Reading: not leaf-bound, and the table line
  is a folded alias (EXTRA_RULES 2026-11-21 p25 / GOTCHAS nf104) -> lands with the split queued. **Coordinator: confirm or hold the unit.** No form was spent on it.
- Vtable: both new objects define the header's ??_7CManager@NUISystem@@6B@ (45 slots); vtcheck: slot count and every slot TARGET equal
  retail 0x01230134. HEAD's 1-slot stand-in vtables (both rows) differed from retail (nf176: a further change lands). Slot SPELLINGS v
  the self rows (HEAD objects compiled for the 14 slots whose rows have no full Leaf line; 31 others carry the slot's spelling as Leaf):
  **12 self rows emit another spelling than the slot binds** -- 0x0055CA40 AddObserver, 0x0055C9C0 RemoveObserver, 0x0055CAE0
  IsExclusivelyObserved, 0x0055CA90 AddConcurrentExclusiveObserver, 0x0055CA00 RemoveConcurrentExclusiveObserver, 0x0041DED2
  ClearBastardChild, 0x0042D592 SetRightStickForce, 0x0041D98A GetIconsDef (all `QAE` non-virtual local models v the header's `UAE`),
  0x0041E3B2 (?GetLayer@CManager@NUISystem@@QAEJJ@Z v UAE?AW4EEngine2DLayer@@J@Z), 0x0055C950 (descriptive ?RemoveAllObservers@... v
  ?ClearObservers@CObservable@NUISystem@@UAEXXZ, PDB), 0x0042D4F0 / 0x0042D5AA (placeholders ?Get@T@@QAEDXZ / ?GetAddr@T@@QAEPAUSub@@XZ
  v GetMouseDisabled / GetRightStickForce). Not respelled. **Coordinator: do vtable-slot relocations of a header vtable COMDAT count as
  binders for the split gate?** If yes, those 12 self rows are queued respells (all the header / PDB spellings, slot = address) or the
  unit waits; if no, nothing changes. (0x0041E1CD was respelled because the ctor's CODE calls it directly, not for its slot.)
- Census (no growth anywhere): 0x0041E641 2 -> 1 (UAE gone); 0x0041E3F6 1 -> 1 (QAE -> IAE, self + 0x0041E5F2); 0x0041E1CD 1 -> 1
  (QAEXJ -> UAEXJ, self + 0x0041E3F6 + the vtable COMDATs); 0x0042AB1C one stand-in fewer (SMember60); 0x0042BEC0 / 0x0042BE7B / 0x0042BF67
  / 0x0042BF85 / 0x0042D28B / 0x0042CE63 / 0x0042B7D7 / 0x0042B796 / 0x0042AB01: one stand-in replaced by the PDB / STLport spelling each,
  only bound by this unit (callers.py: 0x0041E3F6 / 0x0041E641 are their only landed callers, except 0x0042AB01: also 0x0042BE50 and
  0x005953E2 on other stand-ins, unchanged). Data 0x013B8710 unchanged.
- Link (work/link_check_e1.txt): the 6 CManager rows + 36 owner rows: 5 LNK4006, all between owner rows' own placeholder leaves
  (?Clear@List / ?BuildTreeArray@Tree / ?OnKill@T), none involving this unit's objects.

## Entry 1 (q12568): 0x00494840 -- done (1 form)
Census: callers.py: the ctor 0x00496070 (already binds ?InitGraphics@CFrontendGameComponent@@AAE_NXZ) and ChangeTextureColourDepth
0x00495610 (binds the stand-in ?InitGraphics@CFrontendGameComponent_00495610@@QAE_NXZ); absrefs: none. Spellings 3 (self QAE, AAE,
stand-in) -> 2 (AAE, stand-in): no growth; 0x00495610's respell queued (NAME_FIXES).
engine/CFrontendGameComponent.h new FABLE_RETAIL_CFRONTENDGAMECOMPONENT_INITGRAPHICS (private InitGraphics, MainFont +0x1E04, a third layout
branch); the local CCharString model -> fable_string.h. Leaf AAE (both PDBs); objdump diff = the leaf; the three size-body copies stay EQ.

## Entry 2 (q12569): tests -- done, 7 rows
All C++03, /Od /W3, no /EHsc, include/ only, a case per condition, print `<addr>_TEST PASS`; no class with a shared header is modelled.
- On the header, self-contained (body copied, out-of-line members stubbed): 0x0041E641, 0x0041E5F2, 0x0042FB40, 0x0042DB20, 0x00494840.
  CManager tests share a generated stub block for the 45 vtable slots (work/t/stubs_virtuals.inc) and reach protected members through a
  derived TestManager; 0x00494840 uses the explicit-instantiation access idiom of the landed ClearString test 0x004944C0.
- Linked with the row object (verify_and_land's second link; the standalone link fails by design): 0x0041E3F6 (its body keeps HEAD's local
  CDisplayManager / CSystemManager / CNewFrontendGameComponent models, so copying it would copy those) and 0x0054ED90 (the row is all
  HEAD local models). 0x0054ED90's test defines no class at all: GetFont / ~CCharString on their headers, and the symbols no shared header
  declares are bound to __fastcall / C stubs with `#pragma comment(linker, "/alternatename:...")`. **Coordinator: confirm that device in a
  test** (else 0x0054ED90's test waits for the row's port onto NUISystem_CText.h / CDefStringTable.h / CSystemManager.h).
- 0x0042DB20 prints both `0042db20_TEST PASS` and the catalog PassPattern `PASS_0042db20` (TABLE_CHANGES: PassPattern change).
- 0x0041E1CD keeps its HEAD test (body unchanged).

## Entry 3 (q12570): ~CCountedPointer<CFontBank> -- report only, no row change
- The two 53-B copies (0x00494840, 0x0054ED90) are hash-identical; 0x0042DDB3's is the 5-B `jmp ReleaseData` (its RELEASEDATA_DECL_ONLY).
- Per PDB: FableWin.pdb ??1?$CCountedPointer@VCFontBank@@@@QAE@XZ (0x018AA572) and ?ReleaseData@...@QAEXXZ (0x018AC94D); Ego_d.pdb: both
  names present (raw grep, 1 hit each; not in the egod_pdb_names.tsv summary); Ego_r.pdb: no ~CCountedPointer<CFontBank> record,
  ReleaseData<CFontBank> 55 B (S_GPROC32).
- FableWin.pdb: CCountedPointer<CFontBank>::~CCountedPointer, 17 bytes, frame pointer, "marked inline" -- the size of a debug
  `{ ReleaseData(); }` body (the CComponent pair's shape, the header's FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA destructor). Ego_r.pdb: no
  ~CCountedPointer<CFontBank> copy at all; ReleaseData<CFontBank> out of line (55 B; retail 0x004057A0 53 B).
- So both copies come from the same source shape, differing only by whether the TU expands ReleaseData; retail kept neither (no copy, no
  referencer). Nothing settles a body; nothing to change.

## Self-check
- recheck_final.log (rerun after every header and row edit): 11 rows (the 5 changed sources, the 3 test-only rows, 3 callers), all RELOCATION_MATCH + PASS, no RELOC_CONFLICT.
  Warnings: SHARED_HEADER_AVAILABLE on 0x0041E3F6 (3, HEAD's), 0x0041E1CD / 0x0054ED90 / callers (HEAD's); RELOC_NAME on the placeholder
  catalog labels (NAME_FIXES); STANDIN_LEAF 0x0054ED90 / 0x0042DB20 (HEAD's).
- Header proof: hdr/README.md (pin_asserts 0 / 1 / 0; assert_names_check 0 mismatches; includers none defining the new macros).
- Flags lines: none. EH funclets: none. Descriptive names added: none (TestManager etc. are test-local).
