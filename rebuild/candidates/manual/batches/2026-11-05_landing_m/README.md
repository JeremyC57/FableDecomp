# Landing 2026-11-05m: chunk 786 (4 of 6 quest-script Mains landed, 2 parked)

One expert agent authored the rows. All four landed rows are plain C++, grade `genuine` and need no header change.

| Address | Name | Grade |
|---|---|---|
| 0x00EACF00 | NScript::CV_MurderTwistScript::CMurderTwistBrotherOlderFriend::Main | RELOCATION_MATCH PASS (first grade) |
| 0x00EADBB0 | NScript::CV_MurderTwistScript::CMurderTwistBrotherYounger::Main | RELOCATION_MATCH PASS (first grade) |
| 0x00DD2CE0 | NScript::CQ_PrisonEscapeRescueMotherScript::Main | RELOCATION_MATCH PASS |
| 0x00EA0090 | NScript::CV_KnotholeGladeGatesScript::CKGG_VillagerOutside::Main | RELOCATION_MATCH PASS (first grade) |

- **Method.** Each row started from a matched sibling in the same TU:
  - OlderFriend is the landed 0x00EAC5B0 Friend source with its own four lines;
  - Younger joins the Friend top, the 0x00EABB30 tail and the 0x00EADB60 erase loop;
  - PrisonEscape Main follows the CQ_TraderEscortScript binding model;
  - VillagerOutside's male test needs both text-equality opt-ins: FORCEINLINE_TEXT_EQ and CBASICSTRING_TEXT_EQ_INLINE.
- **Names.** Each entity Main has a single .rdata reference, at slot 1 of its vtable, and the PDB names agree. Labels:
  `ghidra_out/labels_landing_2026-11-05m.tsv`, including 0x00DD9520 SplittingUpCutscene. 4 queue lines (tag `land0511m`):
  - three CCrateTeamMember members: SetState 0x00DCEC50, GoOnPatrol 0x00DCEC70, IsThingCarryingCrate 0x00DCED10;
  - 0x00DD9900, which is CQ_PrisonEscapeRescueMotherScript::ProcessRaces but is landed as CV_ChapelOfEvilScript::EndMission.
- **Landing:** `vland --land` (`logs/land.log`).

## Parked (`pending/2026-11-05_linux_hand786/`)
- **0x00DD5A60 CQ_PrisonEscapeRescueMotherScript::CPrisoner::Main:** DIFFER(2246v2250), 20 lines. In our build the final
  GUARDS-loop exit block pushes ebp before the pDSTGame loads. That merges the cell-door cold exit and splits off the COURTYARD
  terminate exit.
- **0x00DCE230 CQ_OrchardFarmRaidScript::CCrateTeamMember::Main:** DIFFER(2605v2589), 82 lines.
  - The prologue `CIsThingAlive(Me)` expands the CScriptThing copy inline where retail calls 0x004ABE90. This is the unexplained
    case in INLINE_DEPTH_REVISIT.md.
  - Probes a6 and a7 bracket retail: a6 still inlines the prologue copy, a7 fixes the prologue but takes every copy out of line.
