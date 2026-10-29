# Chunk 571 (2026-10-29): 5 of 5 ~900-byte released pass 9 rows landed

All RELOCATION_MATCH + PASS, no row past 4 grades: 0x00BDBB70 `CEngineVSConstantLayoutBasic::CEngineVSConstantLayoutBasic()`
(RTTI vtable 0x012A7D40, derived ctors call it), 0x00D99A30 `NScript::CQ_HobbeCaveScript::CheckIfGotTreasure()` (spawn
string), 0x00ECE460 `NScript::CV_SickChildScript::PlayCutscene(CCharString, CCharString)`, 0x00EE3B20
`NScript::CV_TravellingHeroesScript::CTH_Trigger_Guild_Maze::Main()` and 0x00F08E90
`NScript::CQ_AmbushTradersScript::CAT_Sentry::Init()` (vtable slots stored by the landed Allocs). All three named manifest
labels were wrong. NAME_FIXES.tsv: 29 rows queued (shared-helper rows: rule out folded aliases before respelling).
Review: landed 0xF045E0 CAT_Sentry::Main misnames members (+0x1C SentrySeenSpy, def +0x9F4
AmbushTradersSpySentryProximity); landed 0xF083A0 CAT_Phase1Guard::Init uses invented types.
