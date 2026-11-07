# Landing 2026-11-07n: chunk 837 (2 of 6 rows landed, 4 parked)

| Row | Grade | Note |
|---|---|---|
| 0x00D23DC0 NScript::CQ_BowerstoneTownLifeIntroScript::Main | **RELOCATION_MATCH + PASS, landed** | new header engine/CQ_BowerstoneTownLifeIntroScript.h (copied in: new file); function-scope guard local reassigned through the established `__forceinline CScriptThing::operator=` (18 landed rows) |
| 0x00ED3B30 NScript::CV_StatueMasterScript::Main | **RELOCATION_MATCH + PASS, landed** | new header engine/CV_StatueMasterScript.h (copied in: new file); the landed SickChild GFArcTan / GET_ROUNDED_ANGLE spelling with `#pragma intrinsic(atan2,fmod)`; RELOC_ALIAS on the `_CIfmod` import thunk only |
| 0x00DDAE40 NScript::CQ_PrisonRaceScript::Main | DIFFER(1180v1180), 2 | one `fstp` after the expanded GetAngleXY call; retail places it after the next GetPos null test |
| 0x00D3A4A0 NScript::CQ_FireHeartScript::CFH_Prisoner::Main | DIFFER(1140v1140), 13 | register colouring (`seh_Prophet[cage]` lea v add, eax/ecx for ParentClass) |
| 0x00EF4B40 NScript::CPersonalScript_EmptyScript::CTestPerson::Main | DIFFER(1252v1268), 13 | per-site handle-ctor split (retail expands for seh_me, calls 0x007E72A0 for seh_hero) |
| 0x00EFA4F0 NScript::Cbandit_level_cutsceneScript::Main | DIFFER(1365v1362), 33 | per-site movie-dtor split (one exit ours expands, retail calls 0x006E7B80) |

- **Names:** WatchForHeroWinOrLose 0x00DDB2F0 proven (labels file; not yet in the catalog). Queue (`land0711n`): CFH_Prisoner::MakeRoundStartComment
  0x00D3A9B0 (under-qualified) and CFH_Prisoner::MakeComment 0x00D3ACE0 (catalog CFishCompOrganiser::SpeakConversationLine; elimination, verify).
  The 0x0099F100 operator+= and 0x01375748 GDefaultToUsingPhysicsInCutsceneAnimations binding fixes were already queued.
- **Headers:** the CQ_FireHeartScript / CQ_PrisonRaceScript opt-ins and the new Cbandit_level_cutsceneScript.h serve only parked rows; they stay
  in `pending/2026-11-07_linux_hand837/hdr/`.
- **Also at this landing:** six script Mains with descriptive placeholder labels were relabelled from their vtables + Ego_r.pdb (CTH_Whisper
  0x00EE3140, CMinionSpawnAdv 0x00DA8BF0, CDF_DressAsBandit 0x00E71F10, CExpression_PickpocketScript 0x00EEAEF0, CTourGuideFollower 0x00EE4DD0,
  CPrisonRaceStartingGuard 0x00DDD430) and cut as chunk 841.
