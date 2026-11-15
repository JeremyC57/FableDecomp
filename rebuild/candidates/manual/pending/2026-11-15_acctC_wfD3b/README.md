# acctC hand-back wfD3b (parked rows, 2026-11-15)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00DCFA60 | NScript::CQ_OrchardFarmRaidScript::DoCutsceneIfRequired | 2,412 | RELOCATION_MATCH | PASS |
| 0x00D3D150 | NScript::CQ_GuildTrainingScript::CAppleGirl::Main | 4,252 | RELOCATION_MATCH | PASS |
| 0x00DC4D70 | NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageMan::Main | 4,319 | RELOCATION_MATCH | PASS |

Coordinator-verified (`coordverify.log`) and rule-audited by an independent agent: no violations.
REVIEW ITEMS:
- 0x00DCFA60: in-body scoped `#pragma inline_depth(0)` around one `if (IsActiveThreadTerminating()) return;`, marked
  `// Inline-budget:` (retail 0x00DCFC14 goes to the all-call tail 0x00DD03B8; 2451v2412 without it; no /d2inlT value splits it).
- 0x00DC4D70: scoped `#pragma inline_depth(0)` around the row-local CIsThingConscious ctor / dtor (landed predicate idiom, as
  0x00DAF080). Its comments cite the "matched twin 0x00DC5F80", which is staged (wfD2), not landed.
- 0x00D3D150: clean; no device, nothing fitted.
