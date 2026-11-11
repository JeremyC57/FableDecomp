# Chunk Q11 notes (agentQ11, 2026-11-11)

All grades with `FABLE_HDR_OVERLAY=$A/hdr` exported (hdr/README.md lists the opt-ins). Every row was regraded once as it sits
(ATTEMPTS.log, `regrade-as-is`); rows whose draft had no `// Leaf:` line graded the largest function in the object (the row).

## Matched (3, all RELOCATION_MATCH, on shared headers, tests PASS under g++ and VC7.1 /Od)
- 0x00DDC070 NScript::CQ_PrisonRaceScript::CPrisonRaceWinningPostGuard::Main -- identity: registration "PrisonRaceWinningPostGuard"
  0x00DDAEFF, Alloc 0x00DDBF90, vtable 0x012DC9E8 slot 1, Ego_r + FableWin PDB name. Levers: Q10's do/while final loop; per-return
  inline_depth(0) for the out-of-line handle dtor exits; `// Flags: /d2inlT200` for the second inline dtor site (line-2 Inline-budget note).
- 0x00D33280 NScript::CQ_EndGameBossBattleScript::CSwordVoid::Main -- identity: "SwordVoid" 0x00D2F839, Alloc 0x00D2FCE0, vtable
  0x012CC6EC slot 1, both PDBs. Data respelled from the old local model: Ready = FinalDecision +0x49, SwordChosen = FinalDecisionMade
  +0x4a, EndingPlayed = FinalCutscenePlayed +0x4d, master SwordChoice = JackBossBattleResult +0x88 (headers' PDB names). The cast map
  is std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> (fable_map.h), the pause the RAII CWideScreenMagicPauseEntities.
- 0x00940960 CAIStateGroup_VillageTaskHorn::Update -- spelled as the landed Torch sibling 0x0093EA90. Callee names corrected against
  the PDBs: the old draft's `HornTaskCompleted` is SetHornTaskAsCompleted (0x0065A00F), `IsValidForThing` slot 7 is
  CVillageTaskBase::IsCreatureAssignedToTask, the 0x0129BA3C epsilon is DELTA_NOTIONAL_ZERO; retail calls CThing::GetPos 0x004C73D0 on
  the horn and drops the result (0x009409BD), kept as `horn->GetPos();`.

## Parked (37) -- differ/<addr>.cpp line 2 has the measured residue
- Worked: 0x00843310 (closest, 347v347 on the local model: anim id register eax v edx and the SetAnimation push schedule; next: add
  a retail opt-in class to the flattened engine/CCreatureAction_PerformExpressionExtended.h -- PDB ctor
  (CThingCreatureBase&, CThing*, const CDefPointer<const CExpressionDef>&, long, bool, unsigned long, long, long); members FrameCount
  +0xbc, SoundEffectID +0xc4; the old draft named +0x44/+0x54 Untargeted/TargetedAnimName, engine/CExpressionDef.h says AnimationLoop /
  AnimationToOtherLoop; and retail tests ToOtherLoop only when PPerformToThing is set, then falls to AnimationLoop -- and port the
  probe; base ctor / SetAnimation / CCreatureActionBase::GetRandomAnimFromName 0x006924B0 need declarations in CCreatureActionBase.h).
- 0x009392A0: the CAnimFilterAnyGroup base-vtable reset is not reproduced by local-model tweaks; the landed 0x008A8C60 gets the same
  reset on the shared headers (CANIMFILTER / CCREATUREACTION_SITDOWN / CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR/_DTOR opt-ins): migrate onto
  those plus a retail CAIStateGroup_SitRoundCampfire class (header is flattened). Globals: 0x013BB6EC = g_CCharString_013bb6ec
  ("ST_SIT_ON_FLOOR"), 0x013BB6F8 = g_scaled_013bb6f8 (RANGE_TIME_TO_SIT, anonymous namespace), 0x013BB6F4 = MIN_TIME_TO_SIT.
- 0x00F00C90: a shared-header migration (engine/COracleMinigameTestScript.h already has COracleSpectator with NextLine +0x1c)
  graded 1134v1203 with the RAII pause object; the as-is local model (1190v1203) stays in differ/.
- 0x0062BD80: SRC_FAIL as it sits -- the draft relies on an overlay opt-in (CGAMEEVENTDATAPACKAGE_TYPEDREADERS) that never reached
  the repo headers.
- The other 33 were regraded once as they sit and parked unchanged (local-model drafts; most need a shared-header migration first).
  Identity notes seen in passing: 0x00681CD0's draft is CTCCreatureOpinionOfHero::SetOpinionReactionMask while the catalog says the
  CTCCreatureOpinionOfHero ctor; 0x00843310's catalog label is the base class (RTTI CCreatureAction_PerformExpressionExtended,
  vtable 0x012744FC). Not re-proven here.

## Header debt
- engine/CCreatureAction_PerformExpressionExtended.h, engine/CAIStateGroup_SitRoundCampfire.h, engine/CSubtitleRenderer.h (Text as
  CWideString) are flattened layouts; rows 0x00843310, 0x009392A0, 0x00493281 need retail opt-in classes there.

## Names
- No landed spelling proven wrong (NAME_FIXES.tsv empty). LABELS.tsv: the three rows plus SetHornTaskAsAcceptedByThing 0x006584F9,
  SetHornTaskAsCompleted 0x0065A00F (new bindings), AIAction_MoveToPosProximity 0x0097E890.
