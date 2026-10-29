# Chunk 573 (2026-10-29): 5 of 6 quest-script-range released pass 9 rows landed

All RELOCATION_MATCH + PASS (two agents: the first was interrupted, the second continued from its drafts):
0x00E04BD0 `NScript::CQ_TraderEscortScript::CDarkwoodTrader::Init`, 0x00E2AD50 `NScript::CV_AmbushScamScript::OnPersist`,
0x00E9C4B0 `NScript::CV_IntroductionToTrophiesScript::Main`, 0x00EE6E90 `NScript::CV_TrophyDealerScript::Main` (both
first-probe matches from the RockTroll 0x00EC3D40 model), 0x00F08590 `NScript::CQ_AmbushTradersScript::WatchForEnemyDeath`.
Names: quest/entity vtable slot + Ego_r (and FableWin). String literals checked against retail by hand (vland skips
string relocations): 0 mismatches. Parked (pending/2026-10-27_linux_hand573/differ): 0x00E29B20
`NScript::CQS_ScytheInfoScript::Main` 856v846, one duplicate CTimer exit block placed at the function end instead of
inline (~30 probes). NAME_FIXES.tsv: 2 rows queued. Review: 0x0143E8F8 g_pGameScriptInterface has ~8 landed spellings;
0x00E05330 "PersistTransfer_BrainState" owner unproven (Ego_r has no CDarkwoodTrader::OnPersist).
