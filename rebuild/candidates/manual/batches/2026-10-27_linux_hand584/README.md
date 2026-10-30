# Chunk 584 (2026-10-30): 8 of 10 released pass 9 rows landed (the last clean quest-script-range rows)

All RELOCATION_MATCH + PASS (g++ and VC7.1), on the retail NScript headers. Rows: CQ_MinionCampScript::CBriarRose::
PlayCutscene, CQ_UndeadRisingScript::ActivateNextLamp, CV_ChickenKickingScript::PlayCutscene, CV_FishermanScript::Main and
CFisherman::Init, CV_HeroDuelScript::DuelChallenge, CV_RandomPopulationSimScript::SetCurrentPopulationRegionFromRegionName,
CV_TalentlessBardScript::IsHeroDressedAsAWoman. All 10 manifest labels were wrong.
Parked (../../pending/2026-10-27_linux_hand584/differ/): 0x00EF5840 CBanditWalker::Main (768v727: inside the inlined dtor of
the outer CPredicate_And<CPredicate_And<A,A>,A> retail calls the member ~CPredicate_And<A,A> 0x00EF5B50 out of line; VC7.1
expands it; inline_depth cannot split a depth-2 base dtor from a depth-2 member dtor) and 0x00F3CF50 CTriangleStripifier2::
CalcSectionPolyMask (679v683: empty-allocator temporary through a dead argument slot; `i*2` strength reduction).
Header debt (NOTES.md): CBaseObject 0x0129A7D8, CScriptGameResourceObjectBase 0x0126008C, ...ScriptedThingBase 0x0127094C,
...MovieBase 0x01260EF4 and CSpawnedFunc1<T,P> are local (generated headers are flat; migration wave 5 is writing the retail
ones); local STLport map/_Rb_tree and minimal vector; opaque CQ_SunnyvaleMasterData.
NAME_FIXES.tsv: 6 rows (3 proven: ActivateNextLamp with NScript, CalcSectionPolyMask AAE, 0x00CD23B9 IsValid not IsScripting
in three bound forms; 3 lower-confidence STLport signatures).
