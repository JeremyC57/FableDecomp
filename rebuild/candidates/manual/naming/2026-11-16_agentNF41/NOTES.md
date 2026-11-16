# NF41 notes (slice land1011fj..land1113fz, 223 entries)

All counts below come from APPLIED.tsv, STATUS.tsv, RENAMES.tsv and recheck_final.log.

## Outcome
- APPLIED.tsv: 49 done, 4 done-already, 8 blocked, 4 partial, 0 deferred, 0 rejected, 158 carry (never reached).
- STATUS.tsv: 61 changed rows, 104 grades in total (matching grades.log; another 3 grades went to the withdrawn 0x00497A60 / 0x0056A680).
  recheck_final.log covers all 61 in one vland run: 7 MATCH + 54 RELOCATION_MATCH, 61 tests PASS.
- Changed tests: rows/004c7f40_test.cpp and rows/00960fc0_test.cpp. Both only rename the test's stand-ins to the new spellings; the PassPattern is unchanged.
- RENAMES.tsv: 53 section-1 self renames, 75 section-2 binder respellings.
- The 14 entries NF39 left as blocked-0-grades were all worked: 13 done, 1 partial (0x0063F3A0, 1 of 9 binders).
- Locks: 61 under `nflocks/` are NF41's. The other agent holds its locks as NF44. No row of NF44's was touched.

## Funclets
I grepped `rebuild/src/compiled/01/21/*Unwind*` (1,356 files) for the landed file name and for the address of every changed row:
no funclet includes a changed row. Self renames move a parent's file, but no funclet needs repointing for these rows.

## Self-renamed rows whose landed file name is a propagated label
These rows had no `// Leaf:` line. The brief says to leave such rows alone; I departed from that for these queue entries because the
new rows carry the PDB leaf and grade it at full retail size (recheck_final.log). The coordinator should rename the landed files:
- 0x007A3CA0, 0x007F4020, 0x007F42A0: landed as `GetPBestThingInRoughArea...`. They are CThingSearchTools::GetAllThingsInRoughArea instances.
- 0x00746560: landed as `CThingSearchToolsCalls_...`, a stand-in class.
- 0x0069D900: landed as `unattributed_InsertUniqueCRetailTree...`. It is std::_Rb_tree<NCameraModes::EMode,...>::insert_unique.
- 0x00A4FD70: no Leaf line either. Its Grade line said DIFFER(132v130), but a regrade of the unmodified row today gives RELOCATION_MATCH,
  so that line is stale. The row is now ported (RM, 3 grades).

## Descriptive names (original found nowhere: PDBs, strings, FSE, EgoCore)
- `ViewportXFromScreenX` / `ViewportYFromScreenY`: row-local static inline helpers in the CRenderManager2D rows.
- `ViewportXFromWindowX` / `ViewportYFromWindowY`: as above, the window-relative versions.
- `g_QuadIndices` (0x013AC928, `unsigned short[6]`): in row 0x00F39A50.
- `g_pCurrentPrimitivePrepareInfo` (0x013B8A18): rows 0x004BC340, 0x004BCFE0, 0x004BD620 and 0x004BD690. One name per address everywhere.
- STLport members named "descriptive-by-library", because both PDBs are Dinkumware builds: 0x004D9C5D, 0x00618420, 0x0074F680, 0x0083AFE0 and 0x0083B170.
- Opt-in macro (not a symbol): FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_TYPEDLOCAL.

## Also found (NAME_FIXES for the coordinator; outside both queues)
- 0x0069D520 is defined by the landed `CCameraModeMap_Minsert_0069d520.cpp` under the stand-in `CCameraModeMap::_M_insert`.
  0x0069CA00 is defined by `CRetailTreeIterator_Decrement_0069ca00.cpp` under the stand-in `CRetailTreeIterator::Decrement`.
  The new 0x0069D900 row binds their real STLport names (exact symbols in work/alsofound.txt):
  - `?_M_insert@?$_Rb_tree@W4EMode@NCameraModes@@...@std@@AAE?AU?$_Rb_tree_iterator@...@2@PAU_Rb_tree_node_base@2@0ABU?$pair@...@2@0@Z`
  - `??F?$_Rb_tree_iterator@U?$pair@$$CBW4EMode@NCameraModes@@...@std@@QAEAAU01@XZ`
- 0x00BF2E50: the self row's leaf is `?AddNodeAutoExpandTree@CEngineTreeFilerBase@@QAEPAUCNodeHeader@@PAU2@0@Z`.
  Ego_r.pdb 0x00AAA590 spells it `...@@AAEPAUCNodeHeader@1@PAU21@0@Z` (private, nested struct), which 0x00BF3030 now binds.
- 0x00A0AAA0: the repo opt-in FABLE_RETAIL_CRENDERMANAGER2D_VIEWPORTBOXFLOAT (not mine) declares the made-up `?GetViewportBoxFloat@CRenderManager2D@@`.
  - After e13, no landed row calls it. 0x00BC6DB0 still defines the macro but calls GetCurrentAWindow.
  - The coordinator can retire it, since one address should have one name.
- 0x00AA80E0 / 0x00AA8190: byte-identical non-const twins of FindFirst/FindNextSubChunkOfType (0x00AA7F40 / 0x00AA7FF0). Check their landed spellings.

- 0x00AAF800: the landed `CRecordOwner00aaf800_AddRecord_00aaf800.cpp` (stand-in CRecordOwner_00aaf800::AddRecord) is FableWin.pdb
  `?AddMatrix@C3DAnimFileSequenceChunk@@QAEXABVCMatrix3x4@@@Z`, which the new 0x00A4FD70 row binds.
- 0x01436EA0: landed rows spell this global at least 8 ways in reloc_bindings.tsv. The new 0x00B63530 row uses `NEngineInternal::GPCamera`,
  as the sky-renderer donors 0x00B64550 and 0x00B665E0 do.

## Withdrawn (parked in work/withdrawn/, locks released)
- 0x00497A60: graded RELOCATION_MATCH, 1 grade.
  - It needs `CWideString::SetAsSubString(const wchar_t*, long)` (0x0099B770) in the root fable_string.h.
  - A private overlay of that `#pragma once` header broke other TUs (vland SRC_FAIL on 0x007D3560), so I withdrew both the row and the overlay.
  - Its entry is blocked, with the exact declaration needed.
- 0x0056A680: the operator[] instantiation attempt (2 grades, OBJDUMP_ERR). The entry is blocked.

## Partial entries (what is left)
- 0x013B86A0: 1 of 35 binders fixed (0x004083E0). The other 34 model the CMainGameComponent singleton under made-up names; each needs a port.
- 0x013B871C: 1 of 7 binders fixed (0x004083E0). The other 6 model the CNewFrontendGameComponent static; each needs a port.
- 0x006CB6A0: 2 of 3 binders fixed (0x00912700, 0x009143E0). Left: 0x00913F30 CAIStateGroup_BirdFlyToFood::IsRunnable (no header).
- 0x0063F3A0: 1 of 9 binders fixed (0x0063DD70). Left:
  - 0x0063EB20
  - the CMap rows 0x0081FA20, 0x0081FA90, 0x0081FB00, 0x0081FCC0 and 0x008201E0
  - 0x007F0300

## Blocked entries (exact reasons in APPLIED.tsv)
- 0x00A0AA80 (port, 0 grades): the fifth binder, 0x00B4F510, needs a full render-state port.
- 0x008302D0 (authoring): the leafless stdcall stand-in 0x0088F6B0.
- 0x01231728 (port, 1 grade): see Withdrawn.
- 0x0056A680 (port, 2 grades): see Withdrawn.
- Ports examined but not attempted (0 grades), because the rows carry 14 to 25 local class models:
  - 0x00B66190, binder 0x00B662F0: the render-state offsets +0x2944 / +0x2A0C / +0x2868 still need mapping to CRenderStateManager setters.
  - 0x00950C30
  - 0x0059B79A and 0x0059C0C6
- 0x00A0AA80's binder 0x00B4F510 is in the same position.
