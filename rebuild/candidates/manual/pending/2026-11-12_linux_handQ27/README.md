# Chunk Q27 (five large parked rows, full port required, Opus 5.5)

All 5 rows reached and fully ported: 34 grades, 2 probes, about 182 tool calls. 2 matched and landed in landing dw:
- 0x00EB40C0 CMurderTwistAssassin::DoWatchingForTrouble: Q26's "block placement" residue was a control-flow error in the draft
  (the talked-to arm falls through into the threatened / hit test).
- 0x00D39960 NScript::CQ_FireHeartScript::StartRound: the script-macro spelling `TRetVoid _Fail_Val;` + `return _Fail_Val.Return();`
  at every terminate exit puts the post-loop block where retail has it and shares one cleanup copy.

3 still differ (differ/, residue on line 2), all on the shared headers now:
- 0x006EC850: really `CDefendingCombatantInfo::CCombatWheel::Init` (both PDBs; "PopulateCombatRings" was made up). Frame 0x3c vs
  retail 0x30: retail shares one slot between the interface-type key and the three map insert results.
- 0x00A8AD40 C3DMesh2::LoadBinary (1621v1621): which of vertexCount / indexCount / mainSize is spilled.
- 0x0090E5D0 CAIStateGroup_BanditGossip::Update (1362v1367): down from about 70 hunks to 10; frame 8 bytes smaller, one register
  order, one reload.

**hdr/ holds the 13 overlay headers these three rows need; they are NOT landed.** Only CQ_FireHeartScript.h was needed by the
matches. Before reuse, re-base each on the repo copy: stlport/fable_map.h conflicts with landing dv's STLP_TREE_ERASE_BODY block, and
the C3DMesh2.h copy rewrites one default-path line.
