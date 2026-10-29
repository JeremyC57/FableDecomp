# Chunk 575 (2026-10-29): 10 of 10 quest-script-range released pass 9 rows landed

All RELOCATION_MATCH + PASS (g++ and VC7.1), at most 3 grades per row, no flags or pragmas. Seven quest
`WatchForTermination` threads (0x00D01290 CQ_BanditCampScript, 0x00DC3BD0 CQ_OpeningGraveyardSecretPassageScript,
0x00F01A40 CQ_CinemaTestScript, 0x00D68600 CQ_GuildTrainingWoodsWillScript, 0x00D9B2F0 CQ_HobbeToothContestScript,
0x00D62140 CQ_GuildTrainingWoodsDepartureScript, 0x00D04260 CQ_BanditCampBossBattleScript), 0x00EFC300
`NScript::CHeroTurningDebuggerScript::MoveHeroTest`, and the two TeleportData `std::vector::_M_insert_overflow` rows
0x00D18CA0 / 0x00F0D4E0 (protected, IAE). Every catalog label was invented; proven names in
`ghidra_out/labels_chunk575_2026-10-27.tsv` (including four quest Mains not yet landed). NAME_FIXES.tsv: 4 rows queued
(0x00D18CA0 caller binds QAE; 0x007E72F0 MoveToPosition virtual; the two TeleportData copy ctors emit a global name).
