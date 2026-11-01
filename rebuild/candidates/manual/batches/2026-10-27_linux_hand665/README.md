# Chunk 665 -- 10 of 10 landed (2026-11-01, NA010 of the never-attempted pool, authorable band)

Cut 2026-11-01 from `pending/never_attempted/CHUNKS.tsv` (NA010). Landed via `coordinator/land_chunk.sh 665` (vland --land 10/10
RELOCATION_MATCH + PASS, multi-check 0; 1-3 probes each): 0x006ED200 CCombatManager::Update, 0x00C051C0 zlib 1.2.1 `_tr_init` (C),
0x00D2F640 ~CQ_EndGameFocalSitesScript, 0x007AFF70 ~CTCBoss, 0x00DD1F40 ~CQ_OrchardFarmRaidScript, 0x007C5660 ~CTCGhostSword,
0x009A1090 ~CStreamingFontBank, 0x00D83350 ~CQ_HeroSoulsBriarScript, 0x0098E690 ~CThreadedFileManager, 0x00D27290
~CQ_DragonBossFightScript. No row parked.
19 NAME_FIXES queued (c665). Labels: ghidra_out/labels_chunk665_2026-10-27.tsv (18). Decision (owner delegation, chunk 664):
engine/CLevelScriptBase.h drops its explicit dtor declaration in migration wave 10 -- the quest rows carry local copies until then.
Descriptive: g_StreamingFontBanks_013bda50 / g_StreamingGlyphCache_013bda70 (no PDB data symbols); CTCBoss::PhaseStartPos modelled at
+0x24 (Ego_r packed +0x22; bytes identical). Header debt: CTCBase.h, CStreamingFontBank.h, CFontBank.h, CTexture.h flattened.
Lessons in GOTCHAS (1 line) and LESSONS.md.
