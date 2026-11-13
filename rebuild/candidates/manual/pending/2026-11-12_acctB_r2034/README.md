# acctB hand-back, near-miss retry of 0x0097BDA0 (1 previously parked row: matched)

Chunk 2034 parked this row at 2025v2009; a near-miss agent (Opus 5.5) continued in the same directory and closed it.
Re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x0097BDA0 `?UpdateCombat@CAIStateGroup_ScorpionKing@@IAEXXZ` | RELOCATION_MATCH 2009v2009 (624/624 instructions), test PASS (g++ and VC7.1) | **supersedes `2026-11-12_acctB_c2034/rows/parked/0097bda0.cpp`**. 134 graded full-row compiles in the retry (ruler sweeps included). No flags |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test
runs, read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE; three `WARN RELOC_NAME`, all generic
catalog labels, below). Purity and padded-struct greps clean. `grades.log` has one line per grade.

## What closed it (all FITTED: found by grading against retail, no second source)

1. **The 16 extra bytes.** The far arm tests the tail-strike case first:
   `if (ChargeCount >= battle_def->NumChargesBeforeTailStrike || GetCreature().GetHealth() <= (HealthAtLastTailStrike - battle_def->DamageToCauseTailStrike)) { tail plunge } else { charge }`.
   This is the "swap the if / else arms" probe applied to the far arm, not to the block that differed: first grade
   2009 bytes, retail's frame, `boss` in ebx in the near arm, the shared near release.
2. **Tail-compare load order** (0x0097C15A, 0x0097C234): `(HealthAtLastTailStrike - DamageToCauseTailStrike)` in
   parentheses at both tail tests.
3. **The `fstp st(0)` pair** (0x0097BE80 / 0x0097BE85): the existing repo opt-in
   `FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN`. The FableWin debug size still differs from every Normalise text tried
   (+7 instructions), so that body stays fitted.
4. **x87 multiply operand order** (0x0097BFFF, 0x0097C009, 0x0097C092): a by-value
   `C3DVector facing = ...GetFacingVector();` and the side test written as
   `to_enemy.Dot(C3DVector(facing.Y, -facing.X, 0.0f))`. Mechanism measured with a record-id trace: with both factors
   in memory VC7.1 loads the one whose field record has the larger id, and naming `facing.X` / `facing.Y` in the
   function body creates those records early. Retail multiplying `to_enemy.Z` by 0.0f at 0x0097C0A0 fits a Z = 0
   vector being dotted. A named `right` vector or a const `facing` also match; `CrossWithUp` does not.

The behaviour test's far arm was reordered to the tail-strike-first shape; behaviour is the same.

## Review items

- No header changed (`hdr/engine/` is the chunk 2034 overlay; the row drops `FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES` and
  adds the existing `NORMALISE_UNPAREN`), so no landed row needs a regrade.
- `NAME_FIXES.tsv` / `LABELS.tsv`: 0x0091F990 is
  `??$?0VCThingCreatureBase@@@?$CIntelligentPointer@$$CBVCThing@@@@QAE@AAV?$CIntelligentPointer@VCThingCreatureBase@@@@@Z`
  (FableWin.pdb 0x02B673C0, exact spelling); two landed rows (0x009378F0, 0x0097A630) bind it under local-model names
  and need respelling. 0x0097C900 is `??1CCreatureAction_ScorpionChargeInto@@UAE@XZ`; 0x007AED50 is
  `?ResetStrikesSinceHitHero@CTCBoss@@QAEXXZ`; 0x0097D980 and 0x008C4E20 as in the chunk 2034 hand-back.
- The only descriptive name is the dead local `charge_start`.

## How to land

`rows/0097bda0.cpp` + `rows/0097bda0.test.cpp` with `hdr/engine/`, then `payload.json` through your usual vland.
