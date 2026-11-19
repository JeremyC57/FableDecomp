# 2026-11-19 landing nf93b: naming round 8, NF93 part 2 (29 rows, q11261..q11295)

Round 8: see `batches/2026-11-19_landing_nf93a/README.md`. The second NF93 instance continued from q11261 and stopped for context after q11295;
a third continues from q11296. Records: `naming/2026-11-19_agentNF93_part2/` (APPLIED / STATUS hold parts 1 and 2; RENAMES_all.tsv is the
whole hand-back, `RENAMES_applied.tsv` here is part 2 only).

## Entries (part 2)
- **done (5):** q11269, q11270, q11284, q11287, q11288. **done-already (2):** q11264, q11295 (0x0129BA3C is bound only as
  `?DELTA_NOTIONAL_ZERO@@3MB`; the one stale spelling is the label table `ghidra_out/labels_landing_2026-11-11cl.tsv`, queued).
- **blocked (6):** ports q11261, q11262, q11274, q11275, q11293 (11 rows need a port, not drafted); header q11272 (CTexture.h LEVELBYTELENGTH
  spells QBEKK; 0x009FB750 needs a header claim).
- 41 entries (q11296 on) continue in part 3.

## Rows (29): 9 self renames (RENAMES_applied.tsv, 4 file moves) + 20 caller respellings
Recheck from the repo after landing (`logs/recheck.log`): 29 RELOCATION_MATCH, every test PASS. Three action temporaries had the wrong class
on landed rows, settled from the RTTI of the vtable their ctor stores: 0x008DF640 ScorpionRecover (not GoUnderground), 0x008663D0 BeDazed (not
AlertToPlayer), 0x008A9320 ForcePushInto (not SheatheWeapons). 0x0096C880, 0x00799E30 and 0x00914AA0 were respelled inside their existing local
models (no new model).

## Headers (12 opt-ins in 13 headers; token-identical without them)
The CCombatAction_* / CCreatureAction_* `_CLASS` / `_FINISHACTION` / `_CTOR(S)` opt-ins for WillEnflameInto, WillShieldInto, CastWillEnflameSpell,
BriarRoseShotFarInto / FarOutof, GoUnderground, ScorpionRecover, CastStrengthMultiStrike, ScreamerDrainAttack, ScreamerDrainOutOf and BeDazed,
and CThingCreatureBase.h `_GETDAZEDHEALTHTHRESHOLD`. Asserts pinned (BriarRoseShotFarInto, WillShieldInto, ScreamerDrainAttack);
`lane_merge/assert_names_check.py`: 0 mismatches. The existing `_SCREAMERDRAINATTACK_PROTOTYPE` user (0x006F5C10) is in the includer recheck.
Direct-includer recheck: `logs/includer_recheck.txt`.

## Checks
Synced with FullDecomp-B first (B's hand4f_b5 landing, 2 rows; no overlap). `naming/rebind.py`: removed 57 binding lines, added 27;
`bindings_multi_check.py` at baseline (`NullCoord`); funclets 1,355 checked, 0 broken.

## Queued
`land1119nf93` (3): 0x008663D0 (caller 0x008C2810, NF94's row), 0x0072D990 (caller 0x0072CAA0), 0x0129BA3C (the 11-11cl label table).
`land1119Bhand4fb5` (5): Account B's hand4f_b5 NAME_FIXES.

## Rule slips (NOTES "Part 2 rule slips")
One grade without the cpp precheck spun Wine on an `#error` (only its own processes were killed; every later grade used the cpp-first wrapper);
one foreground sleep attempt (blocked by the harness); one check command waited on stdin and was stopped.
