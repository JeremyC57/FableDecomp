# Large parked rows claimed 2026-11-26 (Account B)

Regraded on cl 13.10.3077 against retail `Fable.exe`. Original drafts were not edited.

## 0x00500540 CWorldMap::LoadRegion — 2412 bytes

PDB: Ego_r `egor_pdb_names.tsv` 0x004D48D0 and FableWin `fablewin_pdb_names.tsv` 0x01C86780, `?LoadRegion@CWorldMap@@QAEXJW4ELoadType@1@_N@Z`.

Recorded best was DIFFER(2396v2412). The 2026-11-13 draft does not compile on current headers. Missing members: `CEnvironment::SetRegionTheme`, `CEnvironment::SetTheme`, `CQuestManager::OnRegionLoaded`. `CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber` and `CWorldMap::GetThingFilenameFromLevelFilename` need `FABLE_RETAIL_CAFILE_MODIFIEDTIME_WACKY` and `FABLE_RETAIL_CWORLDMAP_LOADLEVEL`, which this copy defines.

`00500540.cpp` here is a copy that spells `level_files.back()` as `*(end() - 1)` held in a pointer. It stops at the same three missing members, so there is no new byte diff. The original draft stays at `pending/2026-11-13_codex_released/differ/00500540.cpp`.

## 0x00590F0B CTCInventory::GetItemsListUIComponentPC — 2907 bytes

PDB: Ego_r 0x005FEF40 and FableWin 0x02142E25, `?GetItemsListUIComponentPC@CTCInventory@@QAEPAVCComponent@NUISystem@@PAV23@@Z`.

Regrade of the unchanged 2026-11-13 draft: DIFFER(2831v2907). 76 bytes short. The recorded residue still stands: the ability-skip `continue` does not share retail's release chain, and the text-bank local takes `ebx` ahead of `this`.

## 0x00D21B40 NScript::CQ_BreakSiegeScript::CBanditSiegeLeader::Main — 2965 bytes

PDB: Ego_r 0x00D2BC30 and FableWin 0x036C7320, `?Main@CBanditSiegeLeader@CQ_BreakSiegeScript@NScript@@EAEXXZ`.

Regrade of the unchanged 2026-11-13 draft: DIFFER(3013v2965). 48 bytes over. The recorded residue still stands: cut-scene frame slots are swapped, and `~CScriptGameResourceObjectMovieBase` is called at the pad where retail expands it.
