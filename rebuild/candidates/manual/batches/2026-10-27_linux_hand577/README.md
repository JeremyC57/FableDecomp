# Chunk 577 (2026-10-29): 10 of 10 quest-script-range released pass 9 rows landed

All RELOCATION_MATCH + PASS (g++ and VC7.1), string literals paired with retail. Rows:
CV_BodyGuardScript::GetHenchmanTypeFromString / CBodyGuard::Init, CQ_PrisonWardenGameScript::CSecretBook::Init,
CV_FishingCompetitionScript::OnPersist, CQ_AmbushTradersScript::WatchForAllGuardsDead,
CQ_OrchardFarmRaidScript::CCrateTeamMember::GetNearestCrateToMe, CV_ExposeMayorScript::DoMission,
CQ_SecretPassageScript::Init, std::vector<CFireHeartPatternDef>::operator= and std::vector<CConversation>::operator=.
All 10 manifest labels were wrong (0x00D34D40 was labelled with the wrong element type). NAME_FIXES.tsv: 10 rows queued
(0x00D34E80 / 0x00E54C10 are `_M_clear`, 0x00CDB7C0 / 0x00CDB790 `_Destroy<T*>`, 0x00E54E40 protected, CCrateTeamMember
members 0x00DCEC70 / 0x00DCED10). Header debt: CDefClassBase is modelled locally (see NOTES.md).
