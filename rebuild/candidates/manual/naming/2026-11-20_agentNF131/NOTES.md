# NF131 notes (2026-11-20, round 10, PRIORITY slice 9 remainder)

Hand-back on context. Counts come from APPLIED.tsv / STATUS.tsv / recheck_final.log.
- Entries (11): done 4 (q4946, q5141, q6180, q6181); blocked 1 (q6182: two rows left plus the waiting self rename); deferred 1 (q4650: its
  only wrong binder is held by the nf127 landing); open 5 (q4994 / q5140 partial, q5021 partial, q4993 and q5030 not reached). Rejected 0.
- Rows: 28 changed and handed back in rows/ (27 RELOCATION_MATCH + 1 MATCH, every test PASS: recheck_final.log, one vland run). Blocked:
  0x009FF590 (port) and 0x00994930 (self rename waits; draft graded RELOCATION_MATCH, work/00994930_ready.cpp). Deferred: 0x00A62120 (NF129).
- Tests: one rewritten (rows/00bdc180_test.cpp: the two opt-in macros and the two new stub signatures, same PassPattern); every other changed
  row keeps its landed standalone test.

## Descriptive names
- No new made-up names. Every name applied is a PDB spelling (Ego_r.pdb + FableWin.pdb). The landed descriptive `g_bias` (0x01293344) is kept
  in 0x0094EF70 as it was.

## Self renames (RENAMES section 1) and the no-new-split check
- 0x0091A6F0 IsInWorkHours: all 8 REFS rows bind the template IBE spelling after this hand-back (0x0091A390 / 0x00946F30 nf127, 0x00943C20
  already, five here). The self file name changes with the module (CAIStateGroup_WorkBase<CAIStateGroupBase>).
- 0x008FCFD0 GetPWorldSeed: all 99 REFS rows bind ?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ once nf127 lands (its six ported rows) plus this
  hand-back (five binders). This rename depends on the nf127 landing: land it after (or with) nf127.
- Landing order: BOTH self renames (0x0091A6F0 and 0x008FCFD0) must land with or after nf127. At HEAD 0x0091A390 / 0x00946F30 still bind the old
  QAE IsInWorkHours and 0x0091E4B0 0x00938F70 0x0093AD70 0x009431E0 0x00957A20 0x0095B020 still bind GetPWorldSeed stand-ins; nf127's
  RENAMES section 2 moves all eight onto the new spellings (checked line by line in work/NF127_round1/RENAMES.tsv).
- Leaf-only, no landed referrer (exe scan): 0x0091B170, 0x009284B0, 0x0094EF70. Leaf-only, the one referrer already binds the PDB spelling:
  0x00914F00 (0x00915350).
- No EH funclet in rebuild/src/compiled/01/21 includes any changed parent (checked by file name).

## Callee spellings that moved to an existing PDB spelling (the address's spelling count does not grow)
- 0x00662A00 GetAnimFromName, 0x00842570 / 0x008425E0 PlayAnimation ctors (0x00914F00): onto the Ego_r / FableWin spellings other rows bind.
- 0x0099A2F0 ??0CBaseClassNonCopyable@@IAE@XZ (0x009B08C0): was the local model's QAE; IAE is both PDBs and the shared header.
- 0x004978A0 ?GFFloatRandom@@YIMMPAK@Z (0x0094EF70): was ?Method@FollowPlayerProximitySource@@QAEMM@Z.
- 0x00BE04C0 GetBlockData (0x00BDC180, 0x00BF8030, 0x00BF84C0): onto the PDB ?GetBlockData@CEngineStaticMapFileBlockManager@@QBEPBEJ@Z; all
  five landed callers now bind it; the self row 0x00BE04C0 still defines QAEPAXJ (NAME_FIXES; its const body needs the set find 0x00BE0930 const).

## Decisions for the coordinator
- In-place base-list changes of row-local derived models (rule names namespace / template-ness, not base lists): 0x00944730, 0x00944E80,
  0x00947280, 0x009473E0 re-base their existing local CAIStateGroup_WorkSawingWood / _WorkHammerAnvil / _WanderAroundShops models from the
  dropped local CAIStateGroupBase onto CAIStateGroup_WorkBase<CAIStateGroupBase> from the shared header (RTTI); 0x009D0380 / 0x009D4C30 /
  0x009B08C0 re-base their local CAFile / CDiskFileWin32 models onto the RTTI chain (CDiskFileWin32 : CAFile : CBaseClassNonCopyable), and
  0x009D0380 / 0x009D4C30 turn the local stream model's `void* __vftable` word into a declared virtual destructor. No new class token is
  defined in any of them (selfcheck 0). Precedents: landed 0x00945060 (local HammerAnvil on the shared WorkBase), 0x00943C20.
- q4650: may the self rename of 0x0098B5E0 (removes a stand-in spelling; 3 -> 2) go before the held binder 0x00B5E700 moves? Not done here.
- q6182: the self rename of 0x00994930 (PBEJ -> PBXK, graded RELOCATION_MATCH in work/00994930_ready.cpp) waits for 0x00A62120 (NF129, binds
  PAXK) and 0x009FF590 (MemoryInput stand-in; blocked: its InputStream stand-in is also the parameter type of the GraphicHeader::Load stand-in
  callee, so the move needs that callee and its row together). The old no-Leaf rows 0x00BDC510 / 0x00BE7D70 / 0x00BE8920 / 0x00BF3B60 DIFFER by
  design (nf127 probe) and were left.
- 0x0096C9A0 / 0x0092A0C0 keep their stand-in own leaves (only their callee bindings moved): EatAtTable has only a quarantine header, and the
  PDB GetDummy needs its three callers moved first (NAME_FIXES).
- Follow-up 0x009104D0 (GetSeat GetAllLeafThings macros): not done; nf127's CThingSearchTools.h / CAIStateGroupBase.h opt-ins are not in the
  working tree yet (waits for the nf127 landing).
- 0x00957B80 is catalogued `CAIStateGroup_HandyMan::IsRunnable` but calls 0x009578A0 / 0x00957A20 (another class's pair, not HandyMan's
  0x0091A6F0 / 0x0091B010); its stand-in leaf equals the old 0x0091B170 one. Not investigated further.

## Header opt-ins (hdr/README.md)
- engine/CAIStateGroup_HandyMan.h `FABLE_RETAIL_CAISTATEGROUP_HANDYMAN` (0x0091B170).
- engine/CAIStateGroup_FollowPlayer.h `FABLE_RETAIL_CAISTATEGROUP_FOLLOWPLAYER` (0x0094EF70).
- candidates/landscape_background_lod_compiled.h `FABLE_RETAIL_CENGINESTATICMAPFILEBLOCKMANAGER_GETBLOCKDATA`,
  `FABLE_RETAIL_CMEMORYDATAINPUTSTREAM_LOD_CTOR` (0x00BDC180 + its test).
No shared-header member was renamed (no "needs header claim" item). No overlay touches a header NF127, NF132 or NF133 overlays.

## Flags lines
- None added.

## Locks
- Claimed: 0x00944730, 0x00947280, 0x009C8FB0, 0x00A42EA0, 0x00A4DF10, 0x00A99510, 0x00A9B740, 0x00BDE390, 0x00BF84C0; 0x00AC3E00 read
  `released` and was taken. 0x00A62120 is NF129's (deferred). 0x00BE04C0 was claimed for a look and released again (not changed).

## Rule slips
- One Bash call exited 1 during orientation (a grep pipeline whose pattern ended in `|`); read-only.
- One vland check printed whole test sources into the transcript (a grep on the payload JSON); no file was affected.
- Order: the specifics order was followed for q4946 and q5141; q6182 / q6180 / q6181 (cheap closures) were taken before q4994 on advice.
