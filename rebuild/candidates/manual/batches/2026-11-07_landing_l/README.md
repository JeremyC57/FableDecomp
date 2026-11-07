# Landing 2026-11-07l: chunk 836 (2 of 6 rows landed, 4 parked)

| Row | Grade | Note |
|---|---|---|
| 0x00D328C0 NScript::CQ_EndGameBossBattleScript::CEndGameSister::Main | **RELOCATION_MATCH + PASS, landed** | new header engine/CQ_EndGameBossBattleScript.h (copied in: new file, opt-in `FABLE_RETAIL_CQ_ENDGAMEBOSSBATTLESCRIPT_CENDGAMESISTER`); final idle loop in the chunk 817 else-wait form |
| 0x00ECD9E0 NScript::CV_SickChildScript::CSickChildsMother::Main | **RELOCATION_MATCH + PASS, landed** | implicit movie ctor reproduces retail's mixed called / inlined ctor sites; chunk 808 `parent` reference release |
| 0x00E799F0 NScript::CV_ExposeMayorScript::CEM_Rhodri::Main | DIFFER(2580v2580), 8 | chunk 822 nested predicate dtor: ~CIsThingConscious called whole where retail inlines it and calls the nested ~CScriptThing |
| 0x00CFBDF0 NScript::CQ_AwakeningTheOracleScript::CNecropolisGhost::Main | DIFFER(2278v2317), 303 | same blocker; diagnostic /d2inlT60 meter 54 lines |
| 0x00CF0D20 NScript::CQ_ArenaHoldingScriptScript::CEntranceGuard2::Main | DIFFER(2655v2755), 503 | function-wide inline budget (all 8 movie dtors called; retail inlines them) |
| 0x00D21040 NScript::CQ_BreakSiegeScript::CImprisonedBanditLeader::Main | DIFFER(2742v2758), 40 | CIsThingAlive ctor's copy called while the by-value DestMarker copy inlines (GOTCHAS "inline revisit 3") |

- **Names:** all bound callees proven; first binding of CV_SickChildScript::PlayCutscene 0x00ECE460. The 0x00CDBF70 / 0x00CDBFB0 std::map
  ctor / dtor label fixes were already queued (`land0711g` and earlier).
- **Headers:** the three nested-entity opt-ins (CNecropolisGhost, CEM_Rhodri, CEntranceGuard2) serve only parked rows; they stay in
  `pending/2026-11-07_linux_hand836/hdr/`.
