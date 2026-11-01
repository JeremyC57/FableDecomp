# Chunk 660 -- 10 of 10 landed (2026-11-01, NA005 of the never-attempted pool: landed-twin rows)

Cut 2026-11-01 from `pending/never_attempted/CHUNKS.tsv` (NA005). Landed via `coordinator/land_chunk.sh 660` (vland --land 10/10
RELOCATION_MATCH + PASS, multi-check 0; 8 rows first probe, 2 a second probe only for a header warning): 0x007E3370 ~CTCCameraCollision,
0x0087B660 NCameraModes::~CProjectileWeapon, 0x007D7950 ~CCardDef, the identical 75-byte pool 0x0072D3E0 / 0x0072D430 / 0x006DF920 /
0x009771A0 (SheatheItemToInventory_ForDivineWrath, _ForUnholyPower, SheatheTrophy, JackScream destructors), 0x0090C780
CTCVillage::SetGuardFollowingPlayer, 0x01219F00 dynamic initializer for GGame, 0x0121AB3F the CTCInventoryWeapons TU's NEWLINE
initializer (both rows above 0x01200000 are .CRT$XCU entries, compiler code: no library grade). No row parked.
16 NAME_FIXES queued (c660). Labels: ghidra_out/labels_chunk660_2026-10-27.tsv (17). Decisions at landing (owner delegation
2026-11-01): 0x013B8BA8 = `anonymous namespace'::NEWLINE, const CWideString, bound under the PDB spelling with our TU hash
(ESTABLISHED_SPELLINGS; the g_NewLineWideString binding and its callers respell in wave 15); the local CParentDefClassBase model is
header debt (engine/CParentDefClassBase.h declares ~CParentDefClassBase out of line, retail inlines it into derived implicit dtors).
Lessons in GOTCHAS (1 line) and LESSONS.md.
