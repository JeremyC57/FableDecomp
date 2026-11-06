# chunk 810 header overlay

Every change is opt-in or a brand-new header, so no landed includer can change bytes (the default path of every existing header
is untouched: `diff rebuild/include/engine/<h> hdr/engine/<h>` shows additions only, no removed or changed line). Includers
rechecked: none needed (no default-path edit).

## New headers (behind `#ifdef FABLE_RETAIL_NSCRIPT`, no `#error`)
- `engine/CQ_GuildTrainingMeleeScript.h`: `NScript::CQ_GuildTrainingMeleeScript : CLevelScriptBase<CQ_SunnyvaleMasterData>`
  (RegisterMain 0x00D55DD0, Main 0x00D55E90, Init 0x00D55DA0; members GenericTutorialCounter +0x48, TutorialState +0x4c,
  TalkedToWhisper +0x50 .. MeleeOpponentReset +0x56, sizeof 0x58) and its nested `CMeleeOpponent` (Alloc 0x00D566A0,
  GetParentScript 0x00D56740, Init 0x00D56750, Main 0x00D56790; ParentClass* +0x14, PMasterData +0x18, BadHit +0x1c,
  RepeatMelee +0x20, sizeof 0x24). CTheRealGuildmaster / CMeleeThunder forward-declared only.
- `engine/CQ_GuildTrainingWoodsDepartureScript.h`: the level script (RegisterMain 0x00D61C60, Main 0x00D61D20; MissionSucceeded
  +0x48, MissionFailed +0x49, MissionOver +0x4a, DepartureMissionPoint +0x4c, sizeof 0x50) and nested `CArtifactThief` (Alloc
  0x00D62360, GetParentScript 0x00D62400, Init 0x00D62410, OnPredicateFail 0x00D62450, Main 0x00D62480; ParentClass* +0x14,
  PMasterData +0x18, HoldingArtifact +0x1c, AlreadyTalkedTo +0x1d, NotAttacked +0x1e, sizeof 0x20). CFinalMaze / CScorpionHome
  forward-declared only.
- Evidence: struct_layouts_egor.tsv member names / offsets (retail = PDB offset - 0x14 for the entity scripts; the PDB's ParentClass
  is a pointer, kept as a pointer here; the older NScript headers use a reference, same codegen) checked against the retail Main
  bodies' member loads; vtables from the landed Alloc stores (0x012D0328 / 0x012D16DC), slot order read from the vtable contents.
  `FABLE_STATIC_ASSERT` sizes in both.

## Opt-in additions to existing headers
- `engine/CQ_BountyHuntScript.h` (`FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGLEADER`): the nested `CBH_GangLeader` class (sizeof 0x2c,
  vtable 0x012CA5C4, Alloc 0x00D16380) and the two thread members it spawns, WatchForLeaderGruntsKilled 0x00D1C9E0 and
  WatchForLeaderChargeGruntsKilled 0x00D1CAC0 (Ego_r + FableWin PDB name sets; retail functor stores 0x00D1B955 / 0x00D1BEB9).
- `engine/CQ_HerosOldHouseScript.h` (`FABLE_RETAIL_CQ_HEROSOLDHOUSESCRIPT_CFISHERMANSWIFE`): nested `CFishermansWife` (sizeof 0x24,
  vtable 0x012D50DC, Alloc 0x00D8A650; TalkedTo +0x1c, LeavingHappy +0x1d, initial_angle +0x20).
- `engine/CV_BordelloScript.h` (`FABLE_RETAIL_CV_BORDELLOSCRIPT_CBORDELLOLADY`): the private helper
  `CCharString CBordelloLady::GetLHTSTag(const CCharString&)` 0x00E403D0 (landed as CBordelloLady_GetLHTSTag_00e403d0.cpp).
