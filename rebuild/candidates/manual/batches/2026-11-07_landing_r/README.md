# Landing 2026-11-07r: expert authoring chunk 839 (Fable model; 3 of 8 never-attempted rows landed, 5 parked)

| Row | Grade | Note |
|---|---|---|
| 0x00E7AB50 NScript::CV_ExposeMayorScript::PhaseFour | **RELOCATION_MATCH + PASS, landed** | first attempt |
| 0x00E78100 NScript::CV_ExposeMayorScript::PlayCutscene(CCharString, bool, CCharString) | **RELOCATION_MATCH + PASS, landed** | the "NULL" test is `!arg.EqualsCaseInsensitive("NULL")`; established `__forceinline` STB operator= |
| 0x00DE6730 NScript::CQ_RansomVictimScript::PlayCutscene(CCharString, CCharString) | **RELOCATION_MATCH + PASS, landed** | catalog label was CV_ExposeMayorScript (wrong); resynced |
| 0x00E76E90 CV_ExposeMayorScript::HeroFlashesLamp | DIFFER(1613v1612), 10 | `#pragma inline_recursion(on)` (9 landed rows use it) gives retail's one-level self-expansion; one exit block placed differently |
| 0x00E482F0 CV_BanditTollScript::CheckForLeaderDead | DIFFER(1753v1627), 176 | CScriptThing::operator= expanded at one site only in retail |
| 0x00DE4710 CQ_RansomVictimScript::DisappearFollowers | DIFFER(1913v1428), 473 | per-exit ~vector expansion split |
| 0x00D0EE70 CQ_BanditCampBossBattleScript::CheckForFirstAreaMassacre | DIFFER(2536v1989), 640 | per-exit dtor split |
| 0x00775350 CTCShop::UpdateStock | DIFFER(1971v1988), 791 | function-wide inline budget (TU-local MoveTowards expanded at 2 of 5 sites in retail) |

- **Names:** five new queue lines (`land0711r`: 0x00DE6730, 0x00CD319B / 0x0045019A / 0x00450C72 / 0x0046496F vector members); the rest were
  already queued. **Headers:** the STLport erase-by-key / binary_search / random_shuffle opt-ins serve only 0x00775350; they stay in
  `pending/2026-11-07_linux_hand839/hdr/`.
