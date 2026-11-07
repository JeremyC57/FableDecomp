# Chunk 837 header overlay

All edits are opt-in branches or new headers; no existing default path changed (diffed against rebuild/include at hand-back).

## engine/CQ_BowerstoneTownLifeIntroScript.h (NEW)
- NScript::CQ_BowerstoneTownLifeIntroScript : CLevelScriptBase<CQ_SunnyvaleMasterData>; virtuals RegisterMain 0x00D23D20, Main
  0x00D23DC0, Init 0x00D23D10, OnPersist 0x00D24660; members ArenaFinished +0x48, DoneGuardSpeech +0x49, PoshGuardComplete +0x4a;
  sizeof 0x4c (static assert); nested CBowerstoneIntroGuard / CBowerstoneIntroGuardToPosh forward-declared.
- Evidence: Ego_r.pdb layout (offsets = PDB - 0x14 for retail CLevelScriptBase 0x48) checked against the retail Main's byte reads
  (+0x48..+0x4a), the vtable slots, and the Alloc size. Includers: only src/00d23dc0.cpp (chunk 834's parked 0x00D24A40 has a local
  model it can switch to).

## engine/CV_StatueMasterScript.h (NEW)
- NScript::CV_StatueMasterScript : CLevelScriptBase<CQ_SunnyvaleMasterData>; RegisterMain 0x00ED3A90, Main 0x00ED3B30, Init
  0x00ED3A80, GetStatuePointingPosition 0x00ED4420 (Ego_r QAEJXZ); float AnglesToFaceList[4] +0x48; sizeof 0x58; vtable 0x012F2E6C
  (slot 4 OnPersist is CScriptBase's own); nested CStatueMasterStatue / CStatueMasterCellarDoors / CStatueMasterChest forward-declared.
- Evidence: Ego_r layout (108 B PDB) at retail offsets; the Alloc's `push 58h`; the Main's +0x48..+0x54 stores. Includer: src/00ed3b30.cpp.

## engine/Cbandit_level_cutsceneScript.h (NEW)
- NScript::Cbandit_level_cutsceneScript : CLevelScriptBase<Cjoss_test_scriptsData> (data class forward-declared in NScript);
  RegisterMain 0x00EFA450 and Main 0x00EFA4F0 only (Init / OnPersist not overridden in vtable 0x012F69E8); nested Cwelcome_guy
  forward-declared; sizeof 0x48. Includer: differ/00efa4f0.cpp.

## engine/CQ_FireHeartScript.h (opt-in FABLE_RETAIL_CQ_FIREHEARTSCRIPT_PRISONER)
- Adds `void UpdateHistory(bool); // 0x00D3A920` and `void CheckCompletion(); // 0x00D3AD60` (Ego_r + FableWin QAEX_N@Z / QAEXXZ;
  landed rows; CSpawnedFunc member pointer of the "ParentClass." + "CheckCompletion" thread). Non-virtual, no layout change.
- Landed includers rechecked with the overlay (default path textually unchanged): 0x00D35090, 0x00D368D0, 0x00D39330 all
  RELOCATION_MATCH. Used by differ/00d3a4a0.cpp.

## engine/CQ_PrisonRaceScript.h (opt-in FABLE_RETAIL_CQ_PRISONRACESCRIPT_MAIN)
- Adds `void WatchForHeroWinOrLose(); // 0x00DDB2F0` (Ego_r + FableWin QAEXXZ; thread-name literal) and forward declarations of the
  nested CPrisonRaceWinningPostGuard / CPrisonRaceWarden / CPrisonRacer / CPrisonRaceStartingGuard / CWaypointSpeedCheck (Alloc rows
  0x00DDBF90 / 0x00DDC2D0 / 0x00DDC3E0 / 0x00DDC500 / 0x00DDC5E0). No landed includer. Used by differ/00ddae40.cpp.
- Header debt (not changed): `void MakeRacerComment()` should be `bool MakeRacerComment(const CScriptThing&, const CCharString&)`
  (Ego_r + FableWin ?MakeRacerComment@CQ_PrisonRaceScript@NScript@@QAE_NABVCScriptThing@@ABVCCharString@@@Z).
