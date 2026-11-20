# 2026-11-20 landing nf131: PRIORITY slice 9 remainder (25 rows)

Records: `naming/2026-11-20_agentNF131/` (selfcheck 0 problems). 11 entries: 2 done (q4946 IsInWorkHours, q5141 GetPWorldSeed), q6180 / q6181
back to open (their 3 last binders are held, below), q6182 blocked, q4650 deferred, q4993 / q4994 / q5021 / q5030 / q5140 open. The open
entries went to NF134.

## Rows (25): 1 MATCH, 24 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- Self renames: 0x0091A6F0 -> `?IsInWorkHours@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IBE_NXZ`, 0x008FCFD0 -> `?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ`
  (off its free stand-in onto the shared header), 0x0091B170, 0x00914F00, 0x009284B0, 0x0094EF70 (RENAMES section 1).
- IsInWorkHours / GetPWorldSeed binders ported; 0x00944730, 0x00944E80, 0x00947280, 0x009473E0 drop their local CAIStateGroupBase model and
  sit on the shared `CAIStateGroup_WorkBase<CAIStateGroupBase>`. With the virtual base, 0x009473E0's own leaf becomes
  `?InitCreaturePosition@CAIStateGroup_WanderAroundShops@@UAE_NXZ` (slot 3 of the shared base; the PDB-derived data spells it UAE; no row
  binds either spelling).
- All five landed callers of GetBlockData 0x00BE04C0 bind `?GetBlockData@CEngineStaticMapFileBlockManager@@QBEPBEJ@Z` (self row queued).

## Held (3): 0x009B08C0, 0x009D0380, 0x009D4C30 (advisor decision, owner delegated)
Their local CAFile / CDiskFileWin32 models were re-based (CAFile : CBaseClassNonCopyable), so each object newly defines
`??0CAFile@@QAE@XZ`, `??_7CAFile@@6B@`, `??1CAFile@@UAE@XZ` and `??_GCAFile@@UAEPAXI@Z`. The table binds `??1CAFile` (0x0099A300) and
`??_7CAFile` (0x0122ECCC) to retail addresses, and engine/CAFile.h exists, so the rows must be ported onto the shared header instead.
Sources, tests and table lines restored to HEAD (checked equal). Lock for 0x009D0380 / 0x009D4C30 goes to NF134; 0x009B08C0 lands with nf133.

## Headers (add-only opt-ins; assert_names_check 0 mismatches)
engine/CAIStateGroup_HandyMan.h FABLE_RETAIL_CAISTATEGROUP_HANDYMAN, engine/CAIStateGroup_FollowPlayer.h FABLE_RETAIL_CAISTATEGROUP_FOLLOWPLAYER,
candidates/landscape_background_lod_compiled.h FABLE_RETAIL_CENGINESTATICMAPFILEBLOCKMANAGER_GETBLOCKDATA and
FABLE_RETAIL_CMEMORYDATAINPUTSTREAM_LOD_CTOR (the 9 lines it drops are FABLE_STATIC_ASSERT lines pinned by pin_asserts). No landed row
defines any of them.

## Checks
- 28-row run: 642 rows rechecked, 638 good. The 4 bad are pre-existing landscape DIFFERs, the same with HEAD headers
  (`logs/differ_head_check.txt`): 0x00BDC510, 0x00BE7D70, 0x00BE8920, 0x00BF3B60.
- 25-row rerun after the hold: rows, rebind, before pairs, gate eval; gate 0 created splits; bindings_multi_check 1.
- One-definition: full defined-symbol diff (landing vs HEAD objects) for all 25 rows; every new symbol is the row's own leaf or an inline
  from a shared header (CMainGameComponent.h, CSoundPair.h, GFRandom.h, CAIBrain.h, CAIStateGroupBase.h, CThing.h, CTCVillageMember.h,
  STLport bitset). Link test over the 25 objects (`LINK=` cleared): no LNK2005 (`logs/link_landing.log`).
- Table: grew only by witness (p_before / p_land SAME): 0x009FBF90 / 0x009FBFB0 1 -> 2 (row 0x009C8FB0, CResourceMethods stand-ins already in
  the row at HEAD). Shrank: 0x0091A6F0 4 -> 1, 0x008FCFD0 6 -> 1, 0x00BE04C0 3 -> 1, 0x00994930 7 -> 3, 0x0097DB70 11 -> 9, 0x008FD000 7 -> 6,
  0x004978A0, 0x008425E0, 0x00662A00.
