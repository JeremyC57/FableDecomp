# 2026-11-11 landing bt -- chunk H40c (Fable 5.2, third pass on the longest parked rows): 3 NScript Main bodies + one global name

Source: `pending/2026-11-11_linux_handH40c/`. vland --land: 3 RELOCATION_MATCH, tests PASS (g++ and VC7.1 /Od).
- 0x00E64FB0 CV_ChickenKickingScript::CChickenMaster::Main (5 grades in all): both arms of the first-talk walk test end with their own
  PlayCutscene + FadeScreenIn; VC7.1 cross-jumps the tails, the first arm's copy carrying the rotation step retail has.
- 0x00D5AE70 CQ_GuildTrainingSkillScript::CTheRealGuildmaster::Main (6): own middle tick in the first-person round; grade loop index zeroed
  before `score` is read.
- 0x00D647F0 CQ_GuildTrainingWoodsDepartureScript::CFinalMaze::Main (6): the bow-round lightning line with its own conversation local
  (12th candidate re-ranks ebx); one counter for all three rounds.
- Global 0x0143E908: one descriptive name `g_ScriptFormattedText` (no PDB / string / FSE name), applied to its definer 0x01224C83 and user
  0x00E81B40 (was the placeholder g_UnprovenWideString_0143E908; both rechecked RELOCATION_MATCH). The atexit thunk 0x0122AE6A still binds an
  invented AutoTinyThunkTarget object (queued).
Headers (base a9e379083, hdrcheck2 default=same): CScriptGameResourceObjectScriptedThingBase (FORCEINLINE_ASSIGN), NScript_CV_ChickenKickingScript
(CHICKENMASTER); new CQ_GuildTrainingSkillScript.h, CQ_GuildTrainingWoodsDepartureScript.h. Parked 28 (4 at the 10-grade budget).
