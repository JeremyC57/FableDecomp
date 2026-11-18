# NF85 notes: queue q11056..q11096 (NF82's NAME_FIXES)

All counts below come from the files in this directory: STATUS.tsv, APPLIED.tsv, RENAMES.tsv and NAME_FIXES.tsv.

## Totals
- **Entries:** 41 in all. 36 done, 5 done-already (q11058, q11073, q11077, q11083, q11089), 0 blocked, 0 rejected.
  - The done-already labels come from `overrides.tsv`.
  - REJECTED.tsv lists 6 stale binder claims (5 WORKLIST rows plus the slice row 0x006276F0). None of them changes an entry's symbol.
- **Rows:** 99 slice rows plus 4 claimed extras.
  - Slice: 43 ported, 56 not-a-binder.
  - Extras: 4 ported (0x007564F0, 0x004B9850, 0x004B84F0, 0x004B93E0).
  - 0 blocked, 0 deferred.
  - rows/ holds the 47 ported rows. Each was regraded by vland in `recheck_final.log`: 47 of 47 MATCH or RELOCATION_MATCH with test PASS.
  - STANDIN_LEAF warnings (non-blocking):
    - 7 rows (0x004B84A0, 0x004B93E0, 0x004E36C3, 0x007564F0, 0x0077F190, 0x009B8170, 0x00A41960) are flagged against stale catalog
      names, which RENAMES section 1 corrects.
    - 3 rows (0x00696B70, 0x00756B60, 0x00756C60) keep their own leaves unchanged. standin_leaf.py decodes them as (insert_unique /
      equal_range / erase, _Rb_tree), as the catalog names them; the flag comes from parsing the catalog's nested std:: template
      arguments.
  - RELOC_NAME warnings are catalog labels of outside callees, now in NAME_FIXES (0x005BC4C4, 0x009A9E20, 0x009AA320) or already right in spelling
    (0x00755CB0, 0x0077E820).
- **RENAMES.tsv:** 39 section-1 lines and 64 section-2 caller respellings.
- **NAME_FIXES.tsv:** 27 addresses with proven fixes outside the slice. Every caller or self row listed there was probed as landed
  (MATCH or RELOCATION_MATCH).

## Caller sweep (work/sites.py, work/sites.txt, work/sites_check.txt)
The sweep covered every retail E8 call to the 39 RENAMES section-1 addresses: 349 call sites.
- Each site was resolved to the landed row that contains it (start <= site < start + catalog size).
- 80 sites, in 22 (target, caller) groups, sit in functions that are not landed. Examples are the 0x004B3960.. and 0x004BB0..
  clusters that call LowerBound / UpperBound / `__copy` / `__destroy_aux`.
- The other (target, caller-row) pairs were probed:
  - 87 bind the new spelling.
  - 27 bind a stand-in, across 25 distinct caller rows. All are in NAME_FIXES.
  - 2 could not be checked:
    - 0x008AA480 CGameScriptInterface::GetGossipFactionsSize (site 0x008AA49A calls 0x007AEBE0). Its landed row probes DIFFER: it
      models a Dinkumware `<map>` of CArray<CLipSyncEntry> and grades another function, so it cannot bind 0x007AEBE0's STLport spelling.
      Worth a separate look.
    - 0x009C5890 CFileInstaller::CFileInstaller (site 0x009C59D5 calls 0x009AC700). Its source includes `<process.h>`, which the
      Linux cpp probe cannot preprocess.

## Rows outside the queue's slice (claimed, `$S/w22/nflocks/<addr>/owner` = NF85)
- **0x007564F0 `_M_upper_bound`:** the equal_range port binds the private ABE spelling. The self row moved onto stlport/fable_map.h.
- **0x004B9850, 0x004B84F0, 0x004B93E0:** the callees of the q11087 binder CQuestManager::ClearQuestPersistScript (0x004B39B0).
  - 0x004B9850 is CVectorMap<CCharString,CPersistInfo>::UpperBound. Its CQuestMapValue name was a stand-in.
  - 0x004B84F0 is STLport `__copy`. Its old leaf was the placeholder `Copy_PairCharStringPersistInfo_004b84f0`.
  - 0x004B93E0 is STLport `__destroy_aux`. It was modelled as Dinkumware `_Destroy`.
  - The landed 0x004BB6B0 and 0x004BB400 already bound the exact `__copy` / `__destroy_aux` spellings, so each name has two sources.
  - Their other landed callers, 0x004B41F0 ClearAll and 0x004B4770 ~CQuestManager, still bind stand-ins. They are listed in NAME_FIXES.

## Notable per-entry decisions
- **q11087 (0x004B9750 LowerBound):**
  - The value type is CRuntimeQuestPersistence::CPersistInfo, not CQuestMapValue.
  - Evidence: the Ego_r.pdb PersistScripts map type, and the const twin 0x004B97D0, which is byte-identical apart from the call.
- **q11087 binder 0x004B39B0:** now written as
  `RuntimeQuestPersistence.ClearPersistScript(quest_name); World->GetThingManager().ClearPersistenceDataForSection(quest_name);`
  - The inline ClearPersistScript and EraseFromKey both come from two PDBs.
  - The Ego_d locals agree: key, lower, and upper in an inner block.
  - With UpperBound's body in the TU, VC7.1 reuses the end() read as `__copy`'s `last`, as retail does. The old row wrote that out by hand.
- **q11089:** the binder 0x004BBBC0 already binds the right symbol, so the entry is done-already.
- **q11096:**
  - 0x00A333A0 and 0x00A707D0 are `__push_heap` over pair<unsigned long, C3DMeshVolume> and pair<long, C3DMeshVolume>.
  - Their only retail callers, `__adjust_heap` 0x00A33A70 and 0x00A71220, are landed as those instances and already bind exactly these spellings.
- **q11059 (0x005A731F):**
  - The first port kept the landed twin's empty local `class CThingPlayerCreature : public CThing { };`. Including the header had
    failed with C2039 under the default branch.
  - Before hand-back the row was moved to engine/CThingPlayerCreature.h's retail branch (FABLE_RETAIL_CTHINGPLAYERCREATURE over the
    CTHINGPHYSICAL / CTHINGGAMEOBJECT / CTHINGCREATUREBASE chain). It still grades RELOCATION_MATCH.
  - The repo twin 0x005A45F6 keeps its local model. It is outside this slice and was not touched.

## Descriptive names (not from a PDB)
- **0x009EAFF0 CConsole::SetGuessMode:**
  - The FORCEINLINE helper `SetGuessLine` is kept from the landed row. It is descriptive; no PDB member exists for it.
  - The local CConsole, CConsoleInputBase and CConsoleCommandLine have no shared header.
  - Their slot and member names follow Ego_r and the retail vtables 0x0122E5B0 / 0x0122E638.
- **STLport template instances** (`__pop_heap`, `__push_heap`, `__copy`, `__destroy_aux`, `sort_heap`, and others) carry library
  names: the mangled name is what VC7.1 emits for the STLport 4.6 template over the PDB element types.
- **Labels in row comments:** the manifest labels quoted in 0x006BD3A0 and 0x009ABB10 (`RBTreeSet_InsertOrFindString`,
  `map_operator_square_bracket_CCharString_2`) are marked descriptive there. They are not used as names.

## File moves the coordinator should apply (old row file -> suggested name)
- 0x008364B0 `gapscan_sub008364b0_008364b0.cpp` -> `CCountedPointer_DeleteData_008364b0.cpp`
- 0x004E36C3 `global_operatordelete_004e36c3.cpp` -> `std_makeheap_004e36c3.cpp`
- 0x0077E170 `global_CopyBackwardPairCharStringValue0C0077e170_0077e170.cpp` -> `std_copybackward_0077e170.cpp`
- 0x0077EA60 `SGI_AdjustHeapstring160077ea60_0077ea60.cpp` -> `std_adjustheap_0077ea60.cpp`
- 0x004B9750 / 0x004B9850 `CVectorMapCCharStringCQuestMapValueCKeyPairCompareLessCCharStringCQuestMapValue_{LowerBound,UpperBound}_*.cpp`
  -> `CVectorMapCCharStringCPersistInfo_{LowerBound,UpperBound}_*.cpp`
- 0x004B84F0 `global_CopyPairCharStringPersistInfo004b84f0_004b84f0.cpp` -> `std_copy_004b84f0.cpp`
- 0x004B93E0 `vectorstdpairCCharString_Destroy_004b93e0.cpp` -> `std_destroyaux_004b93e0.cpp`
- 0x00A333A0 / 0x00A707D0 `SGI_PushHeapnamed16{u,s}*_*.cpp` -> `std_pushheap_*.cpp`

The same list, with modules, is in `work/filemoves.txt`.

## Header overlays
See `hdr/README.md`: 10 opt-ins on 4 headers. Token identity was checked on 60 includers. pin_asserts pinned 2 in
fable_retail_banks.h on its first edit and 0 everywhere else.

## Rule slips
All are in `work/slips.txt`:
- Three Bash calls exited non-zero:
  - a trailing `cat` on a missing lock file;
  - a wrong pin_asserts.py path (rerun at once with the right path);
  - a trailing `ls` of a file not yet written.
- None of the three changed any state.
- One stray no-op command ran (exit 0).
- One rule slip in a row was fixed before hand-back: 0x005A731F's local CThingPlayerCreature model (see q11059 above).
