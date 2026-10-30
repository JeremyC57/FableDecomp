# Chunk 582 (2026-10-30): 9 of 10 quest-script-range released pass 9 rows landed

All RELOCATION_MATCH + PASS (g++ and VC7.1), on the retail NScript headers. Rows: three PlayCutscene members (HeroSouls
Mother / Guildmaster / Thunder), CQ_WizardBattleScript::CutsceneMazeDeath, CV_BeardyBaldyScript::OnPersist,
CExpression_FollowScript::Main, three spawned threads (WatchForCompletedQuests, WatchForHeroNearExit, StartBarrelTimer).
Parked (../../pending/2026-10-27_linux_hand582/differ/): std::vector<CScriptThing>::_M_fill_insert 0x00D34B30 (474v480):
VC7.1 inlines the CScriptThing copy ctor in both raw-end loops where retail calls 0x004ABE90; inline_depth 0-2, plain-inline
__uninitialized_copy, non-inline copy body in the TU and QFE-4035 all tried -- the same gap as chunk 581's two parked rows
(the implicit-copy inlining question wave 3 of the migration is settling).
Header debt (NOTES.md): CBaseObject 0x0129A7D8, CScriptGameResourceObjectBase 0x0126008C,
CScriptGameResourceObjectScriptedThingBase 0x0127094C, CScriptGameResourceObjectMovieBase 0x01260EF4, NScript::
CIsThingConscious 0x012C2FE8 and CSpawnedFunc<T> are local (retail vtables, no shared header yet); CRGBColour, packed
C3DVector, CCharString, CCountedPointer, minimal std::vector/map, opaque CQ_SunnyvaleMasterData local models.
NAME_FIXES.tsv: 11 rows queued (PlayCutScene / missing NScript / invented FinishMazeStage in landed callers, 0x00411570
Compare and 0x0099E4B0 CCharString ctor junk bindings, two probable). Errors noted in landed sources: 0x00ED85A0 models
SongThingAvailable as a 0x14 bit vector (retail: 0xC byte vector, class 0xE4); 0x00DABAC0 still has a local CScriptBase.
