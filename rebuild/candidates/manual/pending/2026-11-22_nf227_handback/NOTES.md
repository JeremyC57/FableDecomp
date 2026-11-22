# NF227 notes (2026-11-22, round 10): the nf226 follow-ups (q12573-q12585)

Base: the repo working tree with nf226 applied. Locks NF227 (new mkdir): the 12 vtable-slot rows, 0x0042BE50, 0x0042CE63 0x0042B7D7
0x0042B796 0x0042BF67 0x0042BF85 0x0042D28B, 0x00495610 (rows.txt). No AUTH15 / AUTH16 lock, Codex pass 27 row or 0x00A4EFC0 touched.
hdr/: empty (no header change; one probe opt-in was tried on NUISystem_CObservable.h and removed: REJECTED.tsv). Tools: NF226's
(re-pathed) plus tools/vtsyms.py / vtcensus.sh (vtable COMDAT slot spellings of landed objects) and pkf.sh (grade + pk.py one source).

## Entry 1 (q12582 / q12583): the 12 CManager vtable-slot self rows -- done, 12 rows
Census: callers.py finds no E8/E9 reference to any of the 12; absrefs.py finds only the CObservable vtable 0x01230044 and the CManager
vtable 0x01230134 slots. vtcensus over 199 candidate objects (every landed row with a local NUISystem CObservable / CManager model or a
CObservable / CManager ctor / dtor, the 12 self rows, 0x0041E641 / 0x0041E3F6 / 0x0042BEC0 / 0x0042FB40 / 0x0041E5F2 / 0x0055CB10 ...;
work/vtc_all.txt): the only objects emitting those vtables are 0x0041E641 / 0x0041E3F6 (header ??_7CManager, 45 slots, UAE spellings) and
0x0042BE7B (a 1-slot ??_7CObservable binding slot 0 only); the old QAE / placeholder / descriptive spellings were defined only by the
self rows. After: each address has exactly one spelling (the PDB UAE one; all three PDBs carry each of the 12, grep counts in work/).
- 0x0055CA40 / 0x0055C9C0 / 0x0055CA90 / 0x0055CA00 / 0x0055CAE0: ported onto engine/NUISystem_CObservable.h (STLport input-iterator
  find FABLE_RETAIL_STLP_FIND_INPUT + _FIND_IF_INPUT, FABLE_RETAIL_STLP_LIST). 1 form each.
- 0x0042D592, 0x0042D4F0, 0x0042D5AA: ported onto engine/NUISystem_CManager.h (MATCH). 0x0042D4F0 / 0x0042D5AA were gapscan placeholders
  (catalog vfunc_11 / vfunc_39): catalog + file renames in RENAMES section 1 (MouseDisabled +0xC9, RightStickForce +0x94 per the header).
- 0x0041E3B2 GetLayer: ported (FABLE_RETAIL_STLP_TREE_MFIND_DECL; operator[] declaration-only default path). Return type EEngine2DLayer
  (PDBs + header slot 36). 3 forms: pragmas above the include -> RELOCATION_MATCH but a 21-B frame-pointer pick-any
  `??9_Rb_tree_base_iterator` (owner 0x0041C830, 22 B); two row-tail pragma variants did not help (that copy is a non-template inline,
  compiled in the mode in force where it is DEFINED); pragmas moved below the include (EXTRA_RULES 2026-11-20 nf189 placement) -> the copy
  is EQ. Binders 0x0042A879 / 0x0042D246 move from CLayerTree stand-ins to the header STLport spellings (RENAMES section 2).
- 0x0041DED2 ClearBastardChild: ported (FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE; RELEASEDATA), pragma below the includes (same finding:
  above, the ??9 copy is the 18-B size body that HEAD 0x0041E6D3 also emits). Binders 0x004292C0 (onto the existing STLport
  _Rb_global<bool>::_M_increment line, witness 0x00595222) and 0x0042C02F (onto the STLport tree clear 0x0041E641's copy binds).
  Pick-any: ReleaseData EQ 0x004291DE, _M_increment EQ 0x004292C0, ??9 EQ 0x0041C830.
- 0x0055C950 ClearObservers (was descriptive RemoveAllObservers): ported; **decision needed**: the plain form adds a pick-any
  `_List_base<CObserver*>::clear` copy (/O2, 47 v 47 NEQ owner 0x0042A1E3, a size body; the EQ copies 0x0041E641 / 0x0041E3F6 define
  it too). A row-tail `#pragma optimize("s", on)` (after the function; marked Fidelity: scaffolding) makes that template copy EQ (39 B)
  and the leaf grades the same; it also changes the owner-less `deallocate` copy 18 -> 20 B. This is NOT the nf226 "restores the TU
  default" ruling: it sets the owner row's mode (0x0042A1E3 sets `optimize("s",on)`; size-pragma condition 2). rows/ has the tail form;
  fallback = drop the last two lines (final pk: work/pk_fin.tsv; the plain form's line is in work/pk_new.tsv, tag `new`, first 0055c950 block).
- 0x0041D98A GetIconsDef: **in-place respell, not a header port**: `virtual` on the existing local CManager model (pad 0x64 -> 0x60), as
  its slot-42 sibling 0x0041D8E7 (agentNF16) and 0x0041E1CD (landing nf226). objdump diff = the leaf only. A real port needs, first: the
  CDefPointer / CIVCountedPointeeBase / CCharString donor models off the row onto engine/CDefPointer.h / CIVCountedPointeeBase.h /
  fable_string.h, the row's strong GetDef definitions (0x0042B312 / 0x0042B374, TU-mates; one LNK4006 with 0x0042B312 exists at HEAD too)
  onto CDefinitionManager.h's member template, and the same for 0x0041D8E7.
- Callee census (before -> after spellings, landed binders + self rows): 0x0042A879 2 -> 2 (self non-template local model; ?Find@CLayerTree
  -> header ??$_M_find@J@), 0x0042D246 3 -> 3 (self 2-argument local model; 0x0041E1CD ??ACLayerTree W4; ??ACLayerTree J -> header 4-arg
  map), 0x0042C02F 4 -> 3 (self ?OnKill@T@@; ?clear@CBastardChildMap and ?clear@list_T_0042ce63 -> the STLport clear 0x0041E641 binds),
  0x004292C0 one fewer (?Rb_tree_increment@NUISystem gone). Self rows of those callees queued (NAME_FIXES).
- Link (work/link_check_e1.txt, 26 objects: the 12 + 0x0041E641 0x0041E3F6 + callee / owner rows): 1 LNK4006, GetDef<CUIMiscThingsDef>
  0x0042B312 v 0x0041D8E7, identical in the HEAD control (work/link_check_e1_head.txt).

## Entry 2 (q12578 / q12584): 0x0042AB01 -- done
0x0042BE50 CAction::CAction ported onto engine/NUISystem_CAction.h (FABLE_RETAIL_STLP_LIST + _LIST_BASE_OUTOFLINE, pragmas below the
include): the UIActionParams constructor call binds the STLport ??0?$_List_base<CActionParam*> spelling 0x0041E3F6 binds; ThingActionParams
unchanged (0x0042AB58). 0x0042AB01 spellings 4 -> 3 (self placeholder, _List_base, the folded list ctor 0x005953E2). The nf226 held
table line can now be added (TABLE_CHANGES). Pick-any: the object also emits an unreferenced 21-B `list<CActionParam*>(const allocator&)`
copy (not leaf-bound); 0x005953E2 binds that spelling at 0x0042AB01 (27 B, folded) and HEAD 0x0041E3F6 already emits an 18-B copy:
the q12585-style folded alias, debt at HEAD. **Coordinator: confirm** (as nf226's ~list<CObserver*>).

## Entry 3 (NF226's NAME_FIXES, in order)
- 0x0042BEC0 ~CObservable: **blocked** (NAME_FIXES / REJECTED): the leaf is a compiler-generated destructor; a user-declared one stores the
  vptr (DIFFER 26 v 20). Needs a coordinator device for an implicit-destructor self row (0x0041E641 already emits the EQ pick-any copy).
- 0x0042BE7B ??0CObservable: done-already (self leaf already ??0CObservable@NUISystem@@QAE@XZ; 0x0041E3F6 respelled at nf226). Its local
  model still binds slot 0 (SetEvent 0x0055CB10) as ?v00@ and 0x0042AC0A as a stand-in: a header port (work/e3/0042be7b_f1.cpp,
  RELOCATION_MATCH, vtable = retail's 11 slots) is ready but not handed back (outside the brief).
- 0x0055C950 ClearObservers: covered by entry 1.
- Tree / map placeholders (q12576 / q12577): 0x0042CE63 0x0042B7D7 0x0042B796 (~_Rb_tree) and 0x0042BF67 0x0042BF85 0x0042D28B (map()) ported:
  explicit instantiation of the header member (the 0x008353B0 / 0x008353F0 pattern) with 0x0041E641 / 0x0041E3F6's defines and
  `optimize("s", on)`. All RELOCATION_MATCH, every owned pick-any copy EQ (clear / _M_erase / _Rb_tree ctor / _Rb_tree_base ctor /
  ~_Rb_tree / ReleaseData / ~CCountedPointer / ~pair). Each address 2 -> 1 spelling (binders 0x0041E3F6 / 0x0041E641 already STLport).
  Their callee binders move from stand-ins to the STLport spellings (0x0042C02F, 0x0042ACC3, 0x0042AC76, 0x0042B76C, 0x0042B7AD,
  0x0042B7EE: only these rows bind them by code; NF226's in-copy bindings already use the STLport spellings) -- each address loses a spelling.
- 0x00495610 (q12581): ported onto engine/CFrontendGameComponent.h (FABLE_RETAIL_CFRONTENDGAMECOMPONENT_INITGRAPHICS): self leaf
  ?ChangeTextureColourDepth@CFrontendGameComponent@@UAEXJ@Z (all three PDBs, header virtual; only reference = the word at 0x01238774, slot 3
  of the vtable at 0x01238768 (COL 0x01238764; slots 0x00497720 0x00415920 0x00496CF0 0x00495610 0x00494370). vtcensus of 19 candidates
  (work/vtc_fe.txt): the only landed emitters, 0x00495620 / 0x00496070, define 1-slot stand-in vtables binding slot 0 only
  (?v00@CGameComponentInterface@@UAEXXZ, ??_ECFrontendGameComponent@@UAEPAXI@Z), so no landed object binds 0x00495610 but the self row), callee InitGraphics 2 -> 1 spelling (AAE). Catalog name already right.
  New test rows/00495610_test.cpp on the header (body copied, InitGraphics stubbed, two depth cases, qualified calls on raw storage of
  the header's size): the HEAD test #included the HEAD source by relative path and modelled the stand-in class (TCC_FAIL / cl TIMEOUT
  under vland, work/recheck_try1.log). Every other row keeps its HEAD test (all PASS); several are host models (debt, unchanged).
- 0x004299A8 / 0x013B871C (q12579 / q12580): open (context): recipes in NAME_FIXES (an add-only SetIsUsingRelativeCoords declaration in
  NUISystem_CUIScreenUnitConverter.h; a port of 0x0041E3F6's local CNewFrontendGameComponent model onto its header).

## Decisions for the coordinator
1. 0x0055C950 row-tail `optimize("s", on)` (sets the owner's mode, not the TU default): accept, or land the plain form (clear copy NEQ).
2. 0x0041D98A: in-place `virtual` accepted as for 0x0041D8E7 / 0x0041E1CD, or wait for the full port.
3. 0x0042BE50's unreferenced 21-B list-ctor copy at the folded address 0x0042AB01 (as q12585).
4. 0x0042BEC0: which device (if any) emits an implicit destructor as a self row.
5. Whether to add the 12 optional vtable-slot table lines (TABLE_CHANGES "add (optional)").

## Self-check
- Pick-any and symbol diffs of the final rows/: work/pk_fin.tsv (HEAD and NEW objects, pk.py with NF226's owners.tsv) and
  work/sc_fin.log (relocation / defined-symbol diffs HEAD v new, per row). work/pk_new.tsv / pk_t_*.tsv / pk_below.tsv are the
  intermediate forms.
- Link (entries 2-3, work/link_check_e23.txt): the 8 rows + 22 owner / binder rows (0x0041E641 0x0041E3F6 0x0042AB01 0x005953E2 0x00494840
  0x00496070, the tree clear / _M_erase / ctor / base-ctor owners, ReleaseData / ~CCountedPointer / ~pair owners, 0x0042AB58): 3 LNK4006,
  all between owner rows' own placeholder leaves (?OnKill@T@@ x2, ?BuildTreeArray@Tree@@), identical in the HEAD control
  (work/link_check_e23_head.txt); no LNK2005: the explicit-instantiation leaves are pick-any COMDATs next to 0x0041E641 / 0x0041E3F6's copies.
- recheck_final.log: one vland --recheck-landed run over all 20 changed rows (sources + HEAD tests).
- Header proof: no header changed (hdr/ empty), so pin_asserts / includers: n/a.
- Flags lines: none. EH funclets: none. Descriptive names added: none (ClearObservers / GetMouseDisabled / GetRightStickForce are PDB names).
