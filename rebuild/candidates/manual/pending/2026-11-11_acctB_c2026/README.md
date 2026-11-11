# 2026-11-11_acctB_c2026 -- acctB hand-back, chunk 2026 (2 reserved rows, both matched)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed. 0x0092DA20 is the row chunk
2017 did not reach.

| address | result | notes |
|---|---|---|
| 0x0092DA20 CAIStateGroup_MinionSleep::Update (vtable 0x0128B140 slot 12) | RELOCATION_MATCH 1802v1802, test PASS | 2 grades. Defines the same-TU MinionSleep::IsInSleepTime, which retail expands |
| 0x0094E7B0 CAIStateGroup_EarthTroll::Update (vtable 0x0128C044 slot 12, protected) | RELOCATION_MATCH 1980v1980, test PASS | 5 grades. REVIEW: the ground pound is written out as `AIAction_PerformAction(CCreatureAction_TrollGroundPound(GetCreature(), *PEnemy))`; the Ego_r helper CAIStateGroup_TrollBase::GroundPound has no copy in retail and, defined in the TU, is not expanded by VC7.1 (c2 inline score 431 against 74), so whether retail wrote the call out is unproven |

Tests pass under g++ and VC7.1. Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/`
(grades, both test runs, read-only vland: 2 wins, no RELOC_IDENTITY / RELOC_CONFLICT; three label-only RELOC_NAME warnings
on 0x006BB390, 0x0091F990, 0x00733210, in NAME_FIXES.tsv). Purity and padded-struct greps are clean.

BONUS for the coordinator: the IsInSleepTime body in `rows/0092da20.cpp` grades RELOCATION_MATCH against 0x0092C520 with leaf
`?IsInSleepTime@CAIStateGroup_MinionSleep@@QBE_NXZ` (checked by the acctB coordinator with q.py). That row is landed under
the invented name `CAIStateGroup_GoToBed::IsInSleepTimeWithWakeDelay` and can be respelled from this TU.

To land: the rows need `hdr/` (3 new headers; opt-in blocks in 17 copied headers; CAIStateGroupBase.h is c2017's copy;
`hdr/README.md`). Rebuild the oracle from Fable.exe (no retail bytes are in this folder). NAME_FIXES.tsv: 13 entries
(e.g. 0x00733210 landed as `CWADFile::Close` is CTCCombat::ResetComboChainPos; 0x0094E5F0 bound as the invented
`CCombatAction_StabThing` is the TrollGroundPound ctor).
