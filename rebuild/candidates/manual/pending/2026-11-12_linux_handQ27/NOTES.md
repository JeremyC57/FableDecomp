# Chunk Q27 notes

All five rows were ported fully onto the shared engine headers (overlay opt-ins in hdr/, see hdr/README.md) and graded: 2 match, 3 differ.

## Matched
- **0x00EB40C0 CMurderTwistAssassin::DoWatchingForTrouble**: RELOCATION_MATCH. Ported onto engine/CV_MurderTwistFinalScript.h,
  the shared CScriptGameResourceObjectMovieBase, the RAII guard `CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game)` and
  `TRetVoid _Fail_Val` / `return _Fail_Val.Return()`. Q26's "block placement of the shared early exit" was a control-flow error in
  the draft: the talked-to arm does NOT end the function; it falls through into `if (!HasBeenThreatened && IsFeelingThreatened())
  ... else if (hit ...)`. No overlay needed. Speak modes come from engine/ETextGroupSelectionMethod.h.
- **0x00D39960 NScript::CQ_FireHeartScript::StartRound**: RELOCATION_MATCH. The fix Q26 was looking for is the script-macro spelling:
  `TRetVoid _Fail_Val;` at the top and `return _Fail_Val.Return();` at every terminate exit (as in the sibling script rows); this
  puts the post-loop terminate block where retail has it and keeps every return on the same cleanup copy. CTimer members are defined
  inline in the TU through `pDSTGame` 0x0143E8F8 slots 87-90 (CTimer.h declares them; the bodies are the real inline bodies, no
  retail out-of-line copy). Overlay: CQ_FireHeartScript.h opt-in for TriggerProphetComment 0x00D38400 (landed).
Both tests pass under VC7.1 and g++; both regrade RELOCATION_MATCH with LANDVERIFY_STRICT_HEADERS=1.

## Differ (best source in differ/<addr>.cpp, residue on line 2)
- **0x006EC850**: the true identity is `CDefendingCombatantInfo::CCombatWheel::Init(CCombatManager&, CWorldMap&, const map<...>&,
  const CThingCreatureBase&)`. Ego_r and FableWin agree (4 args, ret 10h). "PopulateCombatRings" is a made-up manifest label, so it is in
  NAME_FIXES.tsv. Residue: frame 0x3c vs 0x30 only. Retail shares one slot between the PeekTC key temporary and the three map insert_unique pair
  results. Tried without effect: braces, named results, out-of-line insert, the default PeekTC.
- **0x00A8AD40 C3DMesh2::LoadBinary**: length-exact (1621). The residue is register allocation of vertexCount/indexCount (retail edi/edx,
  spilling mainSize; ours spills vertexCount), plus the knock-on primData slot and the prologue `push ebp` position. The size checks
  have to be `__int64` locals (inline casts fold the xor/jl/jg away).
- **0x0090E5D0 CAIStateGroup_BanditGossip::Update**: the full port took the row from about 70 hunks to 10 (1362 v 1367). Remaining:
  - The frame is 8 bytes smaller, so the filter and IntelligentPointer temporaries sit 4-8 bytes lower.
  - The CTCSolidToThing lookup uses a different register order.
  - Ours keeps the creature across the AddTC argument pushes, where retail reloads it.
  - The brief asked whether the draft's CInterfaceMap::LowerBound (0x0040F020) body should stay. It was dropped: the port uses the shared CThing GetTC header path, and the TU defines no stand-in body.
  - CTCGossip and CCreatureAction_SheatheWeapons have no engine header, so they are local minimal models: CTCGossip from the Ego_r layout, SheatheWeapons from RTTI and its ctor 0x008CC370.
  - Do not declare an explicit specialization of the member template FindMostRecentMessage<TGossipRequestFilter>. Doing so ICEs VC7.1 (C1001).

## Names
- 006ec850 respelled to CCombatWheel::Init. The callees AddRing, Reset and AssignAttackersToRing are in LABELS.tsv.
- No made-up names were introduced.
