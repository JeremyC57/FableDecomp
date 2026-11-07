# Landing 2026-11-07m: expert retry chunk 835 (1 of 10 rows landed, 9 parked)

| Row | Grade | Note |
|---|---|---|
| 0x00EA5230 NScript::CV_LostTraderScript::CLostTraderBanditSpawn::Main | **RELOCATION_MATCH + PASS, landed** | rewritten from the hand648 local model onto the shared headers; header opt-in `FABLE_RETAIL_CV_LOSTTRADERSCRIPT_BANDITNAME` (GetRandomBanditName 0x00EA5690) copied into rebuild/include (additive; 6 landed includers rechecked PASS); same-type cast lever at both attacker waits (owner ruling 2026-11-07); row-local inline CScriptThing::IsAlive / ~CScriptThing (established: 83 / 400 landed rows) |
| 0x00DE2610, 0x00E27E90, 0x00D82640, 0x00DC5F80, 0x00DAF080 | DIFFER, 35 / 31 / 38 / 36 / 147 | CIsThingAlive / CIsThingConscious nested-copy blocker, now characterised as structural (retail CALLS the nested CScriptThing copy 0x004ABE90 at 100% of ~500 predicate construction sites; not LTCG). Only a `#pragma inline_depth(0)` region round the predicate special members reproduces it (residues fall to 9-22 lines; one CPredicateBase() call remains): owner question in agent_NOTES.md |
| 0x0068CB90 CInputProcessControlFreeCamera::ProcessInput | DIFFER, 27 | x87 kill placement, cross-product copy scheduling |
| 0x00D3D150 CAppleGirl::Main | DIFFER, 28 | register colouring (26 spellings over three chunks) |
| 0x00E50910 CV_BeardyBaldyScript::WatchForQuestCardConditions | DIFFER, 57 | ebx zero-register live range across the talk loop |
| 0x00E6BC50 CCOE_AcolyteEvil::Main | DIFFER, 36 | loop register pair |

- **Names:** ReachedTrader +0x4c proven (Ego_r.pdb); no wrong landed spelling found.
