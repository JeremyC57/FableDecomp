# 2026-11-11 landing ct -- chunk Q11 (parked near misses, Opus 5.5): 3 rows

vland --land: 3 wins (RELOCATION_MATCH), every test PASS. bindings_multi_check is at its NullCoord baseline.
- **0x00DDC070 NScript::CQ_PrisonRaceScript::CPrisonRaceWinningPostGuard::Main.**
  - Uses the final-loop spelling from Q10.
  - The out-of-line handle-destructor exits wrap their `return` in `#pragma inline_depth(0)`, and `// Flags: /d2inlT200` expands
    the second inline destructor site. Both are owner-ladder inline-budget devices, noted on line 2.
- **0x00D33280 NScript::CQ_EndGameBossBattleScript::CSwordVoid::Main.**
  - Same final loop. Data members use the header's PDB names.
  - The function ends with a pragma-wrapped `return;`.
- **0x00940960 CAIStateGroup_VillageTaskHorn::Update.**
  - Spelled as the landed Torch sibling 0x0093EA90.
  - New opt-in FABLE_RETAIL_CCREATUREACTION_USEOBJECT_IMPLICIT_DTOR: this TU expands the action temporary's destructor (0x00940A84),
    where the Torch TU calls it.
  - vland's RELOC_NAME warning: 0x0129BA3C binds `?DELTA_NOTIONAL_ZERO@@3MB` (the PDB name, from the merged Codex pass 21 headers),
    while older rows still say g_NormaliseEpsilon. That respell is queued as land1011cr.

**Headers**
- Q11's copies predate the Codex pass 21 merge and NF20, so they were 3-way merged with base 347fbb5c2. All merged clean, and none
  loses a repo line. hdrcheck2 reports default=same.
- Opt-ins: CQ_PrisonRaceScript WINNINGPOSTGUARD, CQ_EndGameBossBattleScript CSWORDVOID, CTCVillage HORNTASK, CAIStateGroupBase
  MOVETOPOSPROXIMITY, CCreatureAction_UseObject IMPLICIT_DTOR.
- New header: CAIStateGroup_VillageTaskHorn.h, from the Ego_r layout.

**Names**
- Labels: ghidra_out/labels_landing_2026-11-11ct.tsv. Besides the 3 rows: SetHornTaskAsAcceptedByThing 0x006584F9,
  SetHornTaskAsCompleted 0x0065A00F, and AIAction_MoveToPosProximity 0x0097E890.
- No name fixes.

The chunk record is in pending/2026-11-11_linux_handQ11 (37 near misses). The closest: 0x00843310 (347v347, one register),
0x009392A0 (needs the 0x008A8C60 header set) and 0x00F00C90.
