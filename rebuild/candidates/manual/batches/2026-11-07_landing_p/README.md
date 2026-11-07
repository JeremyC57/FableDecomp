# Landing 2026-11-07p: chunk 841 (1 of 6 rows landed, 5 parked)

| Row | Grade | Note |
|---|---|---|
| 0x00E71F10 NScript::CV_DemonDoorsScript::CDF_DressAsBandit::Main | **RELOCATION_MATCH + PASS, landed** | repo headers only; `break;` rather than `return;` after SetRegionExitAsActive; inline-ctor pause object, out-of-line movie dtor |
| 0x00EE3140 NScript::CV_TravellingHeroesScript::CTH_Whisper::Main | DIFFER(1350v1366), 115 | CIsThingAlive copy blocker (retail expands the SetIsPushableByHero copy, calls the predicate copy) |
| 0x00DA8BF0 NScript::CQ_MinionClifftopChaseScript::CMinionSpawnAdv::Main | DIFFER(1546v1447), 84 | per-exit ~vector<CScriptThing> split; diagnostic /d2inlT60 gives retail's split (function-wide inline budget) |
| 0x00DDD430 NScript::CQ_PrisonRaceScript::CPrisonRaceStartingGuard::Main | DIFFER(2112v2044), 102 | per-exit ~CTimer split (three expanded exits v one call) |
| 0x00EE4DD0 NScript::CV_TourGuideScript::CTourGuideFollower::Main | DIFFER(1925v1946), 91 | per-exit ~CTimer split |
| 0x00EEAEF0 NScript::CExpression_PickpocketScript::Main | DIFFER(1860v1906), 101 | CIsThingConscious dtor variant of the blocker |

- **Names:** six PDB-proven Main names (relabelled at the chunk cut). 0x00CBE716 FadeOutAndKillEntity and 0x00EF7AB0 ~CIsThingConscious fixes were
  already queued (check for ICF-folded predicate dtors before respelling 0x00EF7AB0).
- **Headers:** all five overlays (TravellingHeroes PLAYCUTSCENE, MinionClifftopChase, PrisonRace STARTINGGUARD, TourGuide FOLLOWER, the new
  CExpression_PickpocketScript.h) serve parked rows; they stay in `pending/2026-11-07_linux_hand841/hdr/`. Note: chunk 837's
  `FABLE_RETAIL_CQ_PRISONRACESCRIPT_MAIN` also forward-declares CPrisonRaceStartingGuard; merge that declaration when either lands.
