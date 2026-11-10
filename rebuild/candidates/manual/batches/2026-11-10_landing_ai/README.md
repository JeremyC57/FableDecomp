# 2026-11-10 landing ai -- chunk 1012 (4 rows)

Source: `pending/2026-11-10_linux_hand1012/`.

- 0x006B1A20 CAtmosProcess::UpdateDayNightAtmosSounds and 0x006B2260 CAtmosProcess::Update (RELOCATION_MATCH): the catalog
  placeholders "CGameScriptInterface_SetSoundTheme" / "CGameScriptInterface_UpdateAtmosphere" were wrong; Ego_r / FableWin
  name both, and the landed CWorld::UpdateFrame already calls 0x006B2260 as CAtmosProcess::Update. Manifest respelled.
  Levers: the same-TU GetDef<T>(type, index, out) body (CDefinitionManager.h GETDEF_INDEX_BODY); the rain theme index read into
  a named local before SetAtmosDefIndex. CAtmosProcess / CTCAtmosPlayer / CGameTimeManager have no shared header yet: both rows
  declare only the members they touch, as the earlier landed CAtmosProcess callers do (debt: a shared engine/CAtmosProcess.h).
- 0x0049FFD0 CWorld::SetKickPos (RELOCATION_MATCH): same-TU GetPBestThing body + STLport compose2 / logical_or.
- 0x004A0780 CWorld::SaveGameStateManual (RELOCATION_MATCH): the source path, PathExists and CopyFileInternal written in both
  arms of the save-type test. Its test used std::string::clear, which VC7.1's library lacks (TCC_FAIL); respelled `= ""` and
  re-run (logs/vland_land.log, second part).
- Headers: new CGameEventDataPackage.h, CTCSpecialAbilities.h, CTCGhostSword.h; opt-ins in CDefinitionManager, CEnvironment,
  CThingMarker, CThingSearchTools, CWorld, CTCHeroStats, fable_containers (BINARY_COMPOSE, VECTOR_COPY_OUTOFLINE),
  fable_user_profile, CGameEvent, CThingPlayerCreature, CPlayer, CTCHeroRangedAttacks, CGameCameraManager, CTCSummonSpell
  (hdr/README.md); hdrcheck2 default=same for every one.

Parked (1 grade each, differ/): AutoLoadRespawn 0x004A5E90 (1 hoisted lea), AutoLoadRegionLeaving 0x004A34F0 (frame-slot tie;
catalog extent overruns into AutoLoadInternal 0x004A3730, SPLITS.tsv), EAUseProjectileWeapon 0x0062BEF0 and
EAControlledCreatureStrafe 0x0062BD80 (frame-slot ties), GetHeroSummonedCreaturesList 0x008A8260 (duplicated epilogue).
Not attempted: 0x0089C910, 0x0089CE70, 0x008A7A70, 0x0088EDC0, 0x004A1840, 0x004A1CB0, 0x0062B070.
