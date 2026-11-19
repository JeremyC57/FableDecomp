# Assert pins restored (2026-11-19)

The 11-19 header merges moved 56 `FABLE_STATIC_ASSERT` lines in 18 headers without the pin step. The typedef names come from `__LINE__`, and VC7.1's
stack-slot order depends on them in some rows (GOTCHAS, landing fz).

**Fix:** `lane_merge/pin_asserts.py <tree at c1d39cbd18> rebuild/include <headers>` respelled each moved assert as `FABLE_STATIC_ASSERT_AT(<11-18 line>, ...)`.
- Pre-11-19 rows therefore see the names they were graded with.
- Three asserts were left unpinned, and none of them is a moved assert:
  - CCreatureAction_SummonerLightningOrbInto 0x11c and CActiveDialogueInfo 0x18 are new, inside their opt-in blocks.
  - CCreatureAction_UseWallMount 0xb0 is inside its new opt-in branch.

**Recheck** of the rows graded under the unpinned names (`recheck_list.txt`, 492 rows: every 11-19 landing and respell, plus the 387 CompilerFlags rows
and the 26 pass-pattern rows): 492 / 492 MATCH or RELOCATION_MATCH + PASS after the pins (`recheck_good.txt`).
