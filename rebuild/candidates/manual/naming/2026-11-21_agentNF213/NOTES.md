# NF213 notes (round 10, 0x00485DBC stand-in binders + UnpackNames family; single agent)

Counts: see STATUS.tsv / APPLIED.tsv. Final recheck: recheck_final.log (one vland run over every changed row).

## Unit 1: q6333 / q6342 (0x00485DBC vector<NUISystem::CComponent*>::_M_insert_overflow, __true_type, IAE)
- Self row: Leaf line completed (same symbol the HEAD object defines; no rename). Catalog-name fix in RENAMES s1 (module/name only):
  `_global::Std_Vector_Insert_Range_Fill` -> std::vector<NUISystem::CComponent *, std::allocator<NUISystem::CComponent *>>::_M_insert_overflow.
- Ported (each now binds the IAE self leaf; each move narrows the address's spelling set, none creates a spelling):
  - 0x00541390 CDynamicList::GetDataFromDef: ScrollingArray stand-in -> std::vector (Ego_r.pdb CDynamicList::ScrollingChildren +0x174,
    struct_layouts_egor.tsv), overflow declaration-only as at HEAD. Test rewritten on the shared headers (HEAD test was a stale copy).
  - 0x005414A0 CDynamicList::InsertChild: ChildVector stand-in -> std::vector; its 0x005418C0 call moves from ?Insert@ChildVector@...
    to 0x005418C0's own self leaf (?insert@?$vector@...; Fable.exe scan: 0x005414A0 is its only landed referencer, so 2 -> 1). The
    same-TU noinline Grow stand-in body is gone. Leaf line added (the symbol the HEAD object defines). Needs per-row flag `/d2inlS12`
    (Flags + Inline-budget lines; the catalog CompilerFlags must carry it).
  - 0x00563130 CTCHeroOnlineScoreboard::ShowScoreboard: whole-row port onto the shared headers (no local class model left). The
    global stand-in `struct CComponent` is NUISystem::CComponent (slot 43 Initialise, slot 48 ChangeState(unsigned long) = the stand-in
    SetVisible(bool)); the TC at interface 0x11 is the inventory (TCI_INVENTORY = 17, NInventory::CTCInventoryBase::GetComponents slot 82,
    as the sibling 0x0055FEC0 reads it). CreateComponent 0x0041DB1D moves from the row-only stand-in ...PAUCComponent@@...PAU3@@Z to the
    shared header's ..._N@Z (already bound by 0x00595A06); 0x0041DB1D keeps 2 spellings (the other stand-in ...PAUCComponent@@..._N@Z is
    0x005E4430's debt). HEAD test kept (a standalone behaviour model that compiles no row and models no shared-header class).
- Blocked (coordinator decision needed): 0x006116E0 CTCInventoryMap::OpenInventory and 0x007E4B60 CTCCreditsUI::Activate (CRetailVector).
  Both match ONLY with the plain /O2 visible noinline STLport overflow (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE: work/*_diag_a.cpp,
  RELOCATION_MATCH), whose pick-any IAE _M_insert_overflow copy is 167 bytes v the owner's 137 (a NEW differing copy: held under the
  EXTRA_RULES pick-any rule). The owner-shaped /Os size-body specialization (which works for 0x005414A0) changes these callers' register
  allocation (0x006116E0 DIFFER 546v535; 0x007E4B60 DIFFER 158v165), and declaration-only differs too (546v535; 164v165). HEAD's comment
  on 0x007E4B60 already records that retail's caller knows the callee does not let `x` escape. Options: (a) accept the /O2 copy (note: the
  landed header rows 0x0056AD40 etc. already define a differing 216-byte copy of the same IAE symbol and a 50-byte push_back copy, HEAD
  debt); (b) an nf96a-style same-TU strong definition; (c) leave the CRetailVector stand-ins. Drafts: work/006116e0_*.cpp, work/007e4b60_*.cpp.
- Option (d), the next form to try (measured on 0x007E4B60 only, diagnostics with scratch overlays under work/hdr_d/, not hdr/):
  the caller difference comes from fill_n being declaration-only inside the size-body block, not from /Os. With fill_n<CComponent**>
  given a visible noinline body the caller grades RELOCATION_MATCH and the overflow copy stays the owner's (137 = 137, LINK_ONCE_DISCARD):
  d1 (fill_n inside the y-off / s-on block): its copy is 26 bytes with a frame v retail 0x0048514C's 23; d2 (fill_n at /O2 before the
  block): 30 bytes (x hoisted out of the loop) v 23. The fill_n owner row 0x0048514C (a folded body, Leaf `fill_n`, the unsigned long
  instance) sets only `#pragma optimize("s", on)`, so the next form is fill_n in its own `optimize("s", on)` block (frame pointer kept
  off) ahead of the overflow block, then cmpbody / exebytes against 0x0048514C, then the same on 0x006116E0. 0x005414A0's test defines
  that fill_n specialization itself and must drop it if the overlay gains the body. Objects: work/007e4b60_diag_d.obj, _diag_d2.obj.

## Pick-any checks (every newly defined symbol with a table line or a landed Leaf; work/owner/ objects; rerun on the final objects)
- Section flags (objdump -h): every owner symbol below is LINK_ONCE_DISCARD in its owner object, and every copy is LINK_ONCE_DISCARD in
  the row objects, so no LNK2005. (The first 0x005414A0 form's overflow specialization was a plain strong definition: the size-body
  block now declares it `inline` + noinline, regraded RELOCATION_MATCH, 137 = 137.)
- IAE _M_insert_overflow: 0x005414A0 copy = owner 0x00485DBC (137 = 137, bytes + relocs True; in-object call target, nf212
  refinement: the owner names the same symbol, pick-any, equal). 0x00541390 / 0x00563130 keep it declaration-only.
- push_back (0x00486024): 0x00541390 / 0x005414A0 / 0x00563130 copies 49 = 49, True.
- 0x00563130 only: CVectorMap<ETCInterfaceType,...>::LowerBound (0x0040F020, 60 = 60 True; now an in-object call target under the
  owner's own Leaf spelling, nf212 refinement), bitset<274>::test (0x00410DE0, 63 = 63 True), CThing::HasTC (table line 0x004118C0 made by
  0x00488620, no landed row there: the copy's bytes equal Fable.exe 0x004118C0 with relocations masked, and it is cmpbody-equal
  (bytes + relocs) to the copy the landed 0x0056AD40 already emits; its relocations reach an in-object "bitset" literal COMDAT, puts
  and abort; not called by the row). The first form also defined the implicit
  ~LHHttp2 (129 v owner 0x008410D0's 250) and ~LHHttpHeaders (27 v 32): FABLE_RETAIL_LHHTTP2_DTOR (existing opt-in) makes the destructor
  the declared user one, so the row calls 0x008410D0 again and neither copy is emitted.
- All other new symbols are shared-header inline COMDATs with no table line and no landed Leaf (tools/owners.sh over HEAD v new objects).

## Unit 2: q12343 (UnpackNames family) -- open, not started (context)
- `?UnpackNames@C3DMeshStats@@ABEXAAV?$vector@VCCharString@@...@std@@_NABVCCharString@@1@Z` (Ego_r 0x00B8E770, FableWin 0x031E4CD0):
  ABE = private, CONST `this` (the brief's "const-less" is wrong); 0x00A2F170's declaration is the model (it already binds it).
- engine/C3DMeshStats.h exists, so the header rule applies. 0x00A2F0F0's own leaf is a non-const QAE over a `CCharStringVector&`
  stand-in (0x00A2F150 QBE stand-in; 0x00A2F120 unprobed): moving the callers onto the header may change their own leaves and pull in
  their referencers (the C3DMeshObject wrappers, e.g. 0x00A35B10). Census those first. The family must move whole (self + 3 binders).

## Descriptive names
- None new. STL instance spellings are the STLport header spellings (descriptive-by-library).

## Flags lines
- 0x005414A0: new `// Flags: /d2inlS12` (catalog has no CompilerFlags; add `/d2inlS12`). Smallest N: S11 and below leave
  allocator::allocate out of line in the overflow copy (135 v 137); S12..S40 all equal.

## Header opt-ins (hdr/README.md)
- stlport/fable_containers.h: FABLE_RETAIL_STLP_VECTOR_OVERFLOW_NUISYSTEM_CCOMPONENT_SIZE_BODY, FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NUISYSTEM_CCOMPONENT_SIZE_BODY
  (size-pragma scaffolding, EXTRA_RULES 2026-11-20 conditions met: owner rows set the same pragmas; the row body grades the same without
  the macro and only the COMDAT changes).
- engine/CTCHeroOnlineScoreboard.h: FABLE_RETAIL_CTCHEROONLINESCOREBOARD_SHOWSCOREBOARD.

## Debt noticed (not changed)
- Landed 0x0056AD40 (and likely the other header-based referencers 0x0055FEC0 / 0x005E5F30 / 0x00622A00) define a 216-byte /O2 copy of
  the IAE overflow and a 50-byte push_back copy, both differing from owners 0x00485DBC / 0x00486024 (cmpbody on 0x0056AD40).
- 0x005414A0's catalog name `NUISystem::CDynamicList::vfunc_147` and its stand-in self leaf (CountedPointer, QAE) are queued (q8305,
  Ego_r 0x005BAF00 / FableWin 0x02260400 ?InsertChild@CDynamicList@NUISystem@@UAEXKAAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z).
- 0x00562E70 Close uses FABLE_RETAIL_LHHTTP2 without _DTOR, so it probably emits the implicit ~LHHttp2 copy (not checked).

## Funclets
- None: no EH funclet row includes the changed rows' files (grep of rebuild/src/compiled for the four file names finds only the rows).

## Locks
- Only rows in rows.txt touched. No new claims.

## Casts
- No stand-in type was cast to a real one in any row. The 0x00541390 test casts byte-array addresses to opaque NUISystem::CComponent*
  (test only, instead of modelling the class).

## Rule slips
- 0x005414A0: one SRC_FAIL compile (FABLE_FASTCALL undefined before rebuild_abi.h was included); counted in grades_used.
- The first 0x005414A0 overflow size body was a plain strong definition (not LINK_ONCE_DISCARD), which would have given an LNK2005
  against the owner; caught by the owner-side section-flag check before hand-back, fixed (`inline` + noinline) and regraded.
- 0x00563130's first grades ran with the overlay fable_containers.h bypassed (include order, see LESSONS); caught by cmpbody.
- 0x007E4B60: three diagnostic compiles beyond the brief's three forms (option d / d2 for the coordinator), listed in work/grades.log.
