# Landing 2026-11-06v: chunk 821 (2 of 6 rows landed, 4 parked)

One expert agent authored the rows, sharing the lane with the chunk 820 agent. All six rows were retries of earlier parks, started
from their parked sources. Their names were already proven. Both landed rows are plain C++ and grade `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x007D8E20 | CTCTavernGameSpotTheAddition::FrameUpdate | RELOCATION_MATCH PASS (1 attempt; 18 parked grades before) |
| 0x00D2ED20 | NScript::CQ_EndGameFocalSitesScript::CEndGameScythe::Main | RELOCATION_MATCH PASS (15 attempts) |

- **FrameUpdate.** The residue was a stack slot: retail gives the board-bounds def pointer the slot of the earlier float `halton`.
  Putting halton, its position loop and the PositionMesh loop in a sibling block before the def's block reproduces it.
- **CEndGameScythe, line-2 note.** The source has one `do { } while (0)` round the whole speech SAY block in the talked-to branch
  (`if (GetHealth > 0) { Speak; wait; T }`).
  - This turns both speech-wait returns into inner-loop exits, which VC7.1 expands and tail-merges as retail does.
  - Line 2 says `// Inline-split: ... retail's construct is unproven`. That is the accepted form for this lever, as in the landed
    INLINE_DEPTH_REVISIT lever 3 rows.
- **Diagnostic finding.** `/d2inlS15`, used only as a diagnostic, matched both 0x00D2ED20 and the parked 0x00D2D930 exactly; S14 is
  the default.
  - So those MovieBase sites lose to c2's caller-size penalty, not to the dtor spelling.
  - GOTCHAS chunk 805 said no /d2inlS value reproduces; it had only tried S14 and S16.
  - No source uses the flag.
- **Headers:** none; the agent's hdr/ is empty.
- **vland and bindings:**
  - New bindings: GetSubDef<CTavernGameSpotTheAdditionDef>, two random_shuffle instances, FindTarget, CTCTavernGame::FrameUpdate,
    GetDef, GetNextHaltonSequenceNumber, KillThing, PositionMesh, UpdateHighlightedObject, UpdatePointerScale, and the
    vector<KnickKnack> push_back / resize.
  - No symbol is bound at a second address.
  - Five targets carry more than one symbol, because they are bodies shared by identical code folding:
    - 0x007D9A80 GetSubDef: the new spelling is the PDB template form; the old non-template forms are in the wave 3 queue.
    - 0x005EA000 GetDef: already queued.
    - 0x005EABC0 PositionMesh: queued `land0611v` PRIORITY. Both PDBs carry only `CTCTavernGame::PositionMesh`; the landed row binds
      the derived-class spelling.
    - 0x004BFFC0: the row binds the manifest placeholder `ClearMeshEffect_004bffc0`. Queued to resolve with the open wave 1 decision,
      ClearHighlighted vs ClearMeshEffect.
    - 0x005E9A60 KillThing: queued as unproven. Neither PDB has a KillThing member, and the catalog label is a fold neighbour.
  - RELOC_NAME warnings come from catalog labels that were already there (0x005E9A60, 0x008AB1E0, 0x00CDBFB0).
- **Names:** LABELS.tsv and NAME_FIXES.tsv are empty, so there is no labels file.
- **Landing checks:**
  - `vland --land` ran (`logs/land.log`).
  - The manifest sync dry run prints 0.
  - bindings_multi_check prints only the known NullCoord case.

## Parked (`pending/2026-11-06_linux_hand821/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00CFE030 NScript::CQ_AwakeningTheOracleScript::COracleTablet::Main | DIFFER(3575v3575), 8 masked lines, 14 attempts (183 -> 8) | operand order of two spawn_points[i] addresses, `[edi+ecx]` v `[ecx+edi]` |
| 0x00D0A830 NScript::CQ_BanditCampBossBattleScript::CBanditKing::Main | DIFFER(2700v2720), 20 masked lines, 16 attempts | the merged terminate exit of the "over" speech calls the MovieBase COMDAT where retail expands it |
| 0x00D2D930 NScript::CQ_EndGameFocalSitesScript::CEndGameJackOfBlades::Main | DIFFER(5005v5094), 15 masked lines, 6 attempts + diagnostics | four cut-scene scope-end MovieBase dtors called, not expanded; `/d2inlS15` (diagnostic) matches: caller-size penalty |
| 0x00DE8370 NScript::CQ_RansomVictimScript::CRansommer::Main | DIFFER(4894v4894), 12 masked lines, 10 attempts | register colouring in two rotated IsPerformingScriptTask tests (eax/ecx) |
