# 2026-11-17 landing nf73: name-fix round 7, NF73 (3 rows: game-side ports NF66 and NF71 left)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent
to speed up naming queue completion".

NF73 took nine game-side rows NF66 had blocked and NF71 had not reached. It ported 4 of them plus 0x005955AB, which two of them
depend on. Three landed; two are held. Brief, list and records: `naming/2026-11-17_agentNF73/`.

## Rows (STATUS.tsv: 9 + 0x005955AB)

| Status | Rows |
|---|---|
| ported and landed | 3: 0x004091C0, 0x00416953, 0x005955AB |
| ported, held by the coordinator | 2: 0x00596CCE, 0x00597006 (below) |
| blocked (1-3 grades each) | 5: 0x0041B550, 0x00549B20, 0x005778CA, 0x00599E3F, 0x006D53C0 |

- **0x004091C0** is `CUserProfileManager::GetSortedSaveNamesAndTimes` (Ego_r.pdb; was the made-up `LoadFileList`), on the shared
  headers; GetFileModifiedTime binds the CAFile spelling.
- **0x00416953** `CMainGameComponent::PostInit` matched on its first grade: its calls now bind ActivateWorld, ConvertWorldToISMThingyPath,
  LoadGameState and NResourceDirectoryNames::GetLevelDir under their PDB names (were PostLoad, GetMapPath2 /
  ConvertWorldToStaticMapPath2, LoadGame, GetLevelsDirectory).
- **0x005955AB** `NUISystem::CFrontEndManager::GetProfileNames` now binds ToCharString, CAFile::IsPathFile and CCharString::NullChar.

**0x00596CCE / 0x00597006 are held.** Both match (`logs/recheck_all5_before_hold.log`), but they bind
`CCountedPointer<NUISystem::CList>::ReleaseData` at 0x0059A994 and `::ShareData` at 0x0059AA86, while landed rows (0x0041E6D3,
0x005729D8) bind the same two names at 0x00429288 / 0x00429486. Both pairs have identical bodies, and the catalog already names
0x0059A994 and 0x00429288 alike. Retail has no identical-code folding and the linker keeps one copy per decorated name, so
the two addresses are two different functions, probably another `CCountedPointer<T>` instance. That has to be settled before these
rows land, or one name would sit at two addresses. Drafts are in `held/`; the question is queued (`land1117fu` 0x00596CCE).

## Entries (APPLIED.tsv)
NF73 recorded 4 done (nf41:e36, nf41:e34, q10474, nf41:e172) and 11 blocked. With the two rows held, nf44:7 / nf44:54-type entries
served only by them stay blocked.

## Rows and checks
- **Section 1 (as landed): 1 self rename (0x004091C0, 2 file moves).** NF73's other two self renames (the protected `IAEJXZ` leaves of
  the held rows) are not applied. The coordinator also corrected their module to `NUISystem::CFrontEndManager` (NF73 dropped the
  namespace the PDBs and catalog carry); that applies when they land.
- **Headers merged three-way:** NF73's seven overlays were copied before the nf72 landing and the assert pins; each was merged with
  `git merge-file` against its base at a298c10f7e (no conflicts). `lane_merge/assert_names_check.py`: 0 mismatches. Opt-ins:
  stlport/fable_string.h `STLP_CHAR_TRAITS_COMPARE_INLINE`, fable_containers.h `STLP_VECTOR_ERASE_RANGE_NOINLINE`, CFrontEndManager.h
  `GETPROFILENAMES_BODY`, CCountedPointer.h `RELEASEDATA_NOINLINE` (a new `#if` branch; the `_DECL_ONLY` and default paths unchanged),
  CMainGameComponent.h `INITSTRUCTURE`, CWorld.h `STATICMAPEXPORT`, CAFile.h `COPYFILE`. Some serve only the held rows.
- **Funclets:** none affected (1,355 checked, 0 broken).
- **Recheck (`logs/recheck.log`):** the 3 landed rows, RELOCATION_MATCH + PASS.
- **Checks:** `naming/rebind.py` removed 15 binding lines and added 8; bindings_multi_check is at its baseline (`NullCoord`).

## Name fixes queued (`land1117fu`, 8)
NF73's NAME_FIXES.tsv (7: the callers 0x00597B20 / 0x0059A238 of the held rows' IAE spelling, ToCharString 0x0099B2C0,
CAFile::IsPathFile 0x00999A30, CAFile::GetFileModifiedTime 0x009992A0, ActivateQuest's bool return 0x004B4A10, the CDeletion copy
constructor for 0x005907AB), plus the held pair's question above.

## Rule slips (NF73)
- The 5 blocked rows ended at 1-3 grades, not 15 (the terminal-grade rule).
- One `llvm-pdbutil` call on Ego_d.pdb was denied by the permission system; a brief overlay copy of the root fable_string.h was removed.
