# 2026-11-11 landing cp -- family FAM06 (`_global::CIVCountedPointer_*` placeholders): 17 rows renamed, 671 left unprovable

Source: `naming/2026-11-11_agentFAM06/` (STATUS, RENAMES, NAME_FIXES, LEFT, NOTES, LESSONS, hdr README).

**Verification**
- vland --recheck-landed: all 17 grade MATCH (7) or RELOCATION_MATCH (10).
- Running every test exe found its catalog PassPattern in 17 of 17. vland's FAIL column on 0x005930F3 and the four component
  destructors comes from custom PassPatterns.
- Bindings were re-derived (-11 / +11). bindings_multi_check is at its NullCoord baseline.

**Renamed**
- STLport helpers:
  - 0x00A6AFD0 and 0x0071EA10 are __uninitialized_copy.
  - 0x00A43890 is fill<CLipSyncDataBankEntry*>. The landed element type CDataBankTextEntry was wrong: the caller is
    vector<CLipSyncDataBankEntry>::_M_fill_insert.
- Compiler-generated pair destructors: 0x005930F3 and 0x005F8736.
- Compiler-generated component destructors: 0x004DB47D, 0x004DC600, 0x004DC916, 0x004DCDA4. Retail stores no vtable in them, which
  only the implicit destructor reproduces.
- 0x00A3C2D0 C3DAnimationBlendState::SetBaseAnimation.
- Catalog/leaf only: 0x004BFD10, 0x005814E5, 0x005DEC70, 0x00AF1C70.
- Spelling fixes with the bytes unchanged: 0x005BD1B3, 0x005E4430, 0x005BE3B1.

**Left (671)**
- 664 bodies have no callers anywhere in the image: COMDATs kept by /OPT:NOREF.
  - CDefPointer<T> (249 T) and CIVCountedPointer<T> (30 T) compile to identical bytes, so with no caller neither the template, the
    member nor T can be proven.
  - The 47-byte "Assign" bodies are SetDynamicCast-shaped, not operator=.
- The rest are referenced rows whose names are unproven, or rows that need a rewrite first (0x00A42980 BeginLoading: queued for NF20).

**Headers**

Opt-ins only, default=same, each header a line-superset of the repo copy:
- C3DAnimationBlendState.h: _SHAREDDATA
- CTCInterestingToVillagers.h: _IMPLICIT_DTOR
- CTCInventoryItem.h: _PDEF

**Debt**
- Rows 0x004DB47D, 0x004DCDA4 and 0x00A43890 declare their classes in the row file, with real PDB members, because no shared header
  exists.
- Six rows use an `Emit...Destructor` helper, marked "not a retail function", after the landed precedent 0x00695F40.
- 0x00A43890's test still models the old element.

**Name fixes**

The 14 NAME_FIXES (land1011cp) are in NF20's queue, among them ~CTCActionUseBase with 34 wrong spellings and ~CTCBase with 8.

Labels: ghidra_out/labels_landing_2026-11-11cp.tsv.
