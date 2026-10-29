# Chunk 579 (2026-10-29): 10 of 10 quest-script-range released pass 9 rows landed

All RELOCATION_MATCH + PASS (g++ and VC7.1). Rows: CArenaCreatureDef::Transfer, CQ_ArenaScript::CArenaEnemy::OnPredicateFail,
CQ_HeroSoulsArenaScript::Init, CQ_AmbushTradersScript::WatchForSpyDeath, CQ_PrisonRaceScript::CPrisonRaceGuard::Init,
CQS_GuardianSisterInfoScript::Main, CPersonalScript_EmptyScript::CCrate1/CCrate2::Main, std::vector<CHeroSoulsCreatureDef>::
operator=, std::vector<CConversation>::_M_fill_insert. All 10 catalog labels were wrong. Uses the retail opt-ins
FABLE_RETAIL_DEF_CLASS_BASE and engine/CBaseClass.h; no local CBaseClass / CSpawnedFuncBase / CDefClassBase.
NAME_FIXES.tsv: 10 rows queued (invented CRetailVector_elem92 / CElement92 / CElement52 bindings, _M_clear, IAE access,
FadeOutAndKillEntity spelling, IsActiveThreadTerminating).
Header debt (NOTES.md; same as every landed NScript row): CScriptThing and CGameScriptInterfaceBase are local because their
generated headers are flattened; CScriptBase, CEntityScriptBase, CLevelScriptBase<T>, CTimer, the predicate classes and
CEntityScriptBinding<T> have no header; the arena / hero-souls def classes are local over the shared retail CDefClassBase.
