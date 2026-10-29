# Chunk 569 (2026-10-29): 3 of 5 large released pass 9 rows landed

Landed (RELOCATION_MATCH + PASS): 0x00CBD700 `std::__introsort_loop<CActiveScriptPair*, CActiveScriptPair, int,
CActiveScriptLess>` (969; STLport 4.6 verbatim; caller sort 0xCBDEF0 from CScriptBase::OnScriptedEntityActivated),
0x00E12B30 `NScript::CQ_WaspBossScript::GuildmasterHelp` (993; PDB + spawn string + DoMission store at 0xE12A37),
0x00E39140 `NScript::CV_AssassinAttacksScript::WatchForAssassinDead(CScriptThing)` (967; PDB + CSpawnedFunc1 spawn
string). Parked (pending/2026-10-27_linux_hand569/differ): 0x00BEB6C0 CEngineInternalPrimitiveMeshBase::Calc2DObjectMatrix
(970v978, x87 add order / store scheduling in two inlined translation multiplies), 0x00C8F200 libvorbis `_vds_shared_init`
(1006v1007, LTCG-private static keeping v in EBX: lane gap). Every manifest label in the chunk was wrong. NAME_FIXES.tsv: 8
rows queued. LESSONS.md: GOTCHAS lines.
