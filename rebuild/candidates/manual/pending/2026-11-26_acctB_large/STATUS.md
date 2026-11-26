# Large parked rows claimed 2026-11-26 (Account B)

Regraded on cl 13.10.3077 against retail `Fable.exe`. Original drafts were not edited.

## 0x00500540 CWorldMap::LoadRegion — 2412 bytes

PDB: Ego_r `egor_pdb_names.tsv` 0x004D48D0 and FableWin `fablewin_pdb_names.tsv` 0x01C86780, `?LoadRegion@CWorldMap@@QAEXJW4ELoadType@1@_N@Z`.

Recorded best was DIFFER(2396v2412). The 2026-11-13 draft does not compile on current headers. Missing members: `CEnvironment::SetRegionTheme`, `CEnvironment::SetTheme`, `CQuestManager::OnRegionLoaded`. `CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber` and `CWorldMap::GetThingFilenameFromLevelFilename` need `FABLE_RETAIL_CAFILE_MODIFIEDTIME_WACKY` and `FABLE_RETAIL_CWORLDMAP_LOADLEVEL`, which this copy defines.

`00500540.cpp` here defines the existing wacky-time and LoadLevel opt-ins plus two new declaration-only opt-ins, `FABLE_RETAIL_CENVIRONMENT_SETREGIONTHEME` (`SetRegionTheme(long)`, `SetTheme(long, long, long)`; FableWin 0x01C87BD0 / 0x01C87C10, not in Ego_r) and `FABLE_RETAIL_CQUESTMANAGER_ONREGIONLOADED` (`OnRegionLoaded(const CCharString&)`; Ego_r 0x004B5200 and FableWin 0x01C5E290). Both macros are default-off and add no fields. With those declarations the copy compiles. `level_files.back()` grades DIFFER(2445v2412). Spelling that as `*(end() - 1)` grades DIFFER(2460v2412). Both are longer than the recorded DIFFER(2396v2412). The original draft stays at `pending/2026-11-13_codex_released/differ/00500540.cpp`. `pin_asserts.py` was not re-run; the new macros are off unless a row defines them.

## 0x00590F0B CTCInventory::GetItemsListUIComponentPC — 2907 bytes

PDB: Ego_r 0x005FEF40 and FableWin 0x02142E25, `?GetItemsListUIComponentPC@CTCInventory@@QAEPAVCComponent@NUISystem@@PAV23@@Z`.

Regrade of the unchanged 2026-11-13 draft: DIFFER(2831v2907). 76 bytes short. The recorded residue still stands: the ability-skip `continue` does not share retail's release chain, and the text-bank local takes `ebx` ahead of `this`.

`00590f0b.cpp` here declares `abilityDef` before `GetDef`, tests `GetDef`'s result instead of `IsNull`, and holds the text bank through a pointer. That grades DIFFER(2838v2907), 69 bytes short. The original draft is unchanged.

## 0x00D21B40 NScript::CQ_BreakSiegeScript::CBanditSiegeLeader::Main — 2965 bytes

PDB: Ego_r 0x00D2BC30 and FableWin 0x036C7320, `?Main@CBanditSiegeLeader@CQ_BreakSiegeScript@NScript@@EAEXXZ`.

Regrade of the unchanged 2026-11-13 draft: DIFFER(3013v2965). 48 bytes over. The recorded residue still stands: cut-scene frame slots are swapped, and `~CScriptGameResourceObjectMovieBase` is called at the pad where retail expands it.

Declaring the movie before the cast map, with the out-of-line PImp destructor kept (`00d21b40_reorder.cpp`), is still DIFFER(3013v2965). Leaving the PImp destructor implicit (`00d21b40_implicit.cpp`) is DIFFER(2933v2965), 32 bytes short. Both changes together (`00d21b40.cpp`) are DIFFER(2917v2965), 48 bytes short. The original draft is unchanged.
