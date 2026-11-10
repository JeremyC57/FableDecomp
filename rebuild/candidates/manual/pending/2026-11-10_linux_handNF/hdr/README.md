# agentNF header overlay (2026-11-10)

All three files are full copies of the current repo headers (as of 2026-11-10 05:55, coordinator opt-ins included) with `#ifdef`
opt-in additions only; the default path is unchanged (every default branch is the original text; in CCountedPointer.h the one
existing `#ifdef FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY` became `#elif defined(FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY)` after
the new branch, same condition). Grade with `FABLE_HDR_OVERLAY=$S/w22/agentNF/hdr`.

## engine/CCountedPointer.h
New opt-ins `FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA` and `FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY`:
- `~CCountedPointer() { ReleaseData(); }` (takes precedence over `_DTOR_DECL_ONLY`);
- `ReleaseData()` = release, then `Data = 0; Info = 0;` (Data first, the 0x004291DE byte order); `_DECL_ONLY` declares it without a body
  (needed only in a speed TU, row 0x00429E16, where VC7.1 would otherwise expand the 44-byte release).
- Evidence (two sources): Ego_r.pdb `??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ` is a 5-byte function (0x00438DDC, next symbol
  0x00438DE1) = retail's 5-byte `jmp 0x004291DE` at 0x00429C53, which retail calls at the scope ends of locals (0x0054522E, 0x0056BBB9);
  the 44-byte 0x004291DE is called on LIVE pointers (ClearBastardChild's loop 0x0041DEED/0x0041DEF4, Reset 0x00429C72, ShareData
  0x00429217) and FableWin.pdb has `?ReleaseData@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXXZ` (420 ReleaseData instantiations for
  419 destructors). Same body+jmp pairs: CAction 0x00429160/0x00429B72, CMeshDataBank 0x0042918C/0x00429BF9, CList 0x00429288/0x00429D8E.
- With `_RELEASEDATA` the size-optimised NUISystem TUs reproduce retail by themselves: the 0x00429C53 destructor compiles to the jmp,
  Reset / ShareData call ReleaseData from the DEFAULT header bodies (so `FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CALLS_DTOR` and
  `_DTOR_DECL_ONLY` are no longer needed by any row in this hand-back), and every inline destructor at a scope end expands to
  `call ReleaseData`. 51 landed binders re-graded RELOCATION_MATCH (rows/), 8 of which needed the visible body rather than `_DECL_ONLY`.
- Coordinator decision needed: this supersedes fix [1] (0x004291DE named `??1`). The landed rows 00544d20 / 0056ad40 already bind `??1`
  at 0x00429C53, i.e. the catalog currently has `??1<CComponent>` (and `??1<CAction>`, `??1<CList>`) on two addresses each.
- Open: the "Info-first ReleaseData expansions" the header comment cites (0x00A45330 / 0x00A5F250 / 0x006633A0) were not re-examined; the
  default ReleaseData body (Info first) is left untouched.

## stlport/fable_containers.h
New opt-in `FABLE_RETAIL_STLP_LIST_REMOVE_DECL`: STLport 4.6 `void list::remove(const _Tp&);` declared only (body in _list.c). Row
0x00846100 defines the list<long> instance (0x0084ACE0, `?remove@?$list@JV?$allocator@J@std@@@std@@QAEXABJ@Z`, FableWin) as a same-TU
explicit specialisation with the STLport body.

## engine/CTCInventoryClothing.h
New opt-in `FABLE_RETAIL_CTCINVENTORYCLOTHING_TOTALARMOUR`: declares `long GetCurrentTotalArmorValue() const; // 0x005B4A95` (public,
after the `_ARMOUR` block). Ego_r.pdb `?GetCurrentTotalArmorValue@CTCInventoryClothing@@QBEJXZ`; the row 0x005B4A95 defines it (before,
the row used a local class model). Used only by rows/005b4a95.cpp.

## Rows that need the overlay
- engine/CCountedPointer.h `_RELEASEDATA`: the 37 visible-body ReleaseData rows, 005729d8, 00573792, 005f519f, 0059e6bd, 0056f8a4, and the
  self rows 004291de / 00429c53 / 0042920a / 00429c6f, and 00485524; `_RELEASEDATA_DECL_ONLY`: 00429e16 (STATUS.tsv grade column says
  `FABLE_HDR_OVERLAY (...)` for each).
- stlport/fable_containers.h `_LIST_REMOVE_DECL`: 00846100.
- engine/CTCInventoryClothing.h `_TOTALARMOUR`: 005b4a95.
