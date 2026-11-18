# NF72 notes (round 7, land1117fu, q10859..q10909)

## Descriptive names (marked as the naming rule requires)
- **STLport spellings are descriptive-by-library.** Both PDBs are Dinkumware builds, so every `std::_Rb_tree<...>`,
  `_Rb_tree_iterator<...>::operator--`, `vector<...>::~vector`, `list<...>::~list`, `pair<...>::pair`, `_M_lower_bound`, `__copy_ptrs`,
  `__uninitialized_copy`, `sort` and `__destroy_aux` instance used here is the STLport 4.6 spelling from stlport/fable_containers.h or
  stlport/fable_map.h. These are the spellings the landed callers already bind.
- **`g_ParticleReadPriority` (0x00AF1360 row) and `gDataBankReadPriority` (0x009C80F0 row).** These are the read-priority globals the rows
  pass to the async reader. No name was found in the PDBs, the strings or FSE, so both are descriptive.
- **Stand-in names kept on purpose.** Each address has no caller and no data reference, so the real owner cannot be proven:
  - CRetailTree_erase00817280 / _008b1d40 / _008b1d80 / _008b1dc0 ::~... (0x00817AF0, 0x008B27D0, 0x008B2860, 0x008B28F0);
  - CAggregate_0090332e;
  - the free-function scalar-deleting-destructor stand-ins (0x006C4430, 0x004E0122; agentNF69 0x004E1023 form).
  Only their callee bindings changed.
- **Local typedef names.** `CThingPointerVector` (0x004E3EE1), `CThingVector` (0x007800B0) and `CThingPointerList` (0x004E1437) are now
  plain typedefs of the shared std::vector / std::list. They are not class models and not renaming macros, and the mangled names are
  the std spelling. The typedef name is only a local alias that kept the edit minimal.

## Header opt-ins
See hdr/README.md. **The CREATEENTRY opt-in (FABLE_RETAIL_CBANKFILEASYNC_CREATEENTRY) is unused by any landed row or test**
in rebuild/src or rebuild/tests. The only landed CBankFileAsync::CreateEntry binder (0x00AF1360) now uses FABLE_RETAIL_CBANKFILEASYNC_GETASYNCENTRY.
It is still defined by pending candidates of row 0x009FDE80, under rebuild/candidates/manual/pending/:
- 2026-11-13_acctC_c37a/open
- 2026-11-14_acctC_wfB5/open
- 2026-11-16_acctC_open/wfB5

Remove it once those move to GetAsyncEntry. 0x00B42530 / 0x00B41FA0 bind 0x009D58D0 through a local CStaticMapBankFile::CreateEntry model,
which is a separate (blocked) port.

## Known deviations for review
- **0x006C4430** (CTCFadeOutAndIn scalar-deleting stand-in): FadingThings is protected in engine/CTCFadeOutAndIn.h. The free-function
  stand-in therefore reaches it at its header offset (`reinterpret_cast<vector*>((unsigned char*)this + 0x18)`). This is not an absolute
  cast address, but it is an offset cast. A friend opt-in or a real ??_G TU would remove it.
- **0x009C80F0:** `const_cast` on PGroup for the Reset in a const member, as retail's const BeginLoading does it.
- **FABLE_RETAIL_STLP_SORT_FASTCALL_BODY:** a raw `__forceinline` under `_MSC_VER` (hdr/README.md).
- **0x004B8940:** new behaviour test `rows/004b8940_test.cpp`. The old test called the stand-in CActiveFile_OnReadFinished (LINK_FAIL
  after the respell).

## 0x0047EEE4 (q10909)
Moved onto fable_string.h, engine/NGameText_CDataBank.h (DATABANK_GROUPTEXT + GETPGROUPENTRY), NUISystem_CManager.h,
NUISystem_CUIMiscThingsDef.h, CDefPointer.h and the fable_containers.h bitset. RELOCATION_MATCH on grade 4.
- **Slot 42.** It is GetMiscThingsDef (Ego_r + FableWin), and the "date/time separator" is CUIMiscThingsDef::SpaceSeparator at +0x3C. The
  landed 0x0054C4C0 calls the same slot that way.
- **0x009C7980.** It binds NGameText::ConstructString (both PDBs). NAME_FIXES lists the two landed binders outside the slice that still use
  the namespace-less spellings.
- **Still local.** These seven models stay, each a SHARED_HEADER_AVAILABLE warning: CAFile, CDateAndTime, CTCHeroStats, CMainGameComponent,
  CUserProfileManager, CThing, CTCBase. The brief named five models to remove (CWideString, CCharString, CDataBank, CManager,
  CGroupEntry); those five, and the CIVCountedPointer / CDefPtr / bitset helpers, are gone.

## q10884 binders (all blocked)
Retail's derived CCreatureAction destructors are compiler-generated, and VC7.1 drops their vtable reset. The 0x0048C950 probe on the
shared engine/CCreatureAction_SheatheItemToInventory.h (`~T(){}`) shows the result: DIFFER 81v75, the extra 6 bytes being the
`??_7..@6B@` store. The fix is an IMPLICIT_DTOR opt-in for each derived header, with the COMDAT graded from a caller's TU (GOTCHAS 163).
That is a header wave, so these rows were not forced here.

## recheck_final.log RELOC_NAME triage (21 warnings)
- **8 are already in the row's original dump** (x_o): 0x004E3EE1, 0x007800B0, 0x00938BF0, 0x009614A0, 0x009C8700, 0x009CD740, 0x00A62120, 0x00B32300.
- **10 are the catalog labels my section-1 renames replace:**
  - the _M_erase calls of the 8 tree rows / tree binders;
  - SetAtFailed 0x00970590 from 0x0097CCD0;
  - __copy 0x009D1C30 from 0x009D3D90.
- **2 pre-date this round.** They are access-only rows whose bindings are already in reloc_bindings.tsv: 0x009613A0 → GetHitsThisPhase
  0x007AED40, and 0x00970B60 → SetUser 0x0087C390. Their catalog labels are still address placeholders (CTCBoss::Getter_007aed40,
  _unattributed::AssignTarget_0087c390), outside this slice.
- **1 is the 0x00A84160 sort row.** __introsort_loop 0x00A83AB0 replaces the IntrosortLoop stand-in (RENAMES section 2); the catalog label
  is already the std::__introsort_loop instance.

None is a new disagreement with a PDB name.

## EH funclets
No changed row has an EH frame: no __CxxFrameHandler / __ehhandler relocation in any graded dump. No landed source includes a changed row's
source, and no funclet label (ghidra_out/labels_funclets_*.tsv) names a changed parent. Nothing to regrade.
