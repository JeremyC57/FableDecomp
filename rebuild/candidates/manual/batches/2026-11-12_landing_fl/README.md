# 2026-11-12 landing fl -- 0x00959BF0 CAIStateGroup_HoverWander::Update (wf18, after repair)

Source: `pending/2026-11-12_linux_handWF18/`. The row matched in workflow wf18 with three verifier findings. An Opus repair
(`lane_tools/workflows/repair_and_verify_row.js`, 3 grades) fixed them without moving a byte:
- the source uses the PDB enumerators (ACTION_REPEAT_UNTIL_FINISHED, ACTION_REPEAT_CALLBACK, AI_ACTION_FINISHED,
  AI_ACTION_IN_PROGRESS, AI_MOVEMENT_HOVER_SLOW) instead of raw casts; the look-at flags 0x11 stay a literal (a plain long);
- engine/CAIStateGroup_HoverWander.h (new opt-in) declares the FableWin-proven public override IsCombatStateGroup() const;
- engine/CAIStateGroup_EnemyCreatureBase.h (existing VSLOTS branch): slot 20 IsCombatStateGroup is public and marked as an
  INFERRED placement (no PDB has it on this base, only on the 15 derived classes; it is declared so slots 21 and 22 land at the
  retail offsets), and the slot comment states the real source of each name. The repair corrected the verifier here: on disk Ego_r
  proves only GetCombatMinDistance; the other six names come from FableWin.pdb and the retail signature transfer.
Mechanical verifier: ok. Evidence verifier: one comment finding (the world seed is fetched again inside the loop, 0x00959E54 /
0x00959E5B), fixed by the coordinator and regraded. RELOCATION_MATCH, test PASS, LANDVERIFY_STRICT_HEADERS=1.
The one landed row that uses the VSLOTS branch, 0x00959F80, was rechecked after the header change: RELOCATION_MATCH, test PASS
(`logs/recheck_vslots_user.log`). Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
