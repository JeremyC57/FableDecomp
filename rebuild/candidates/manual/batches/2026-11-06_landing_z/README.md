# Landing 2026-11-06z: chunk 825 (3 of 6 rows landed, 3 parked)

One expert agent authored the rows, sharing the lane with the chunk 824 agent. Five rows were retries of earlier parks; one was new,
and the agent proved its name. All three landed rows are plain C++ and grade `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00E44980 | NScript::CV_BordelloScript::WatchForHeroLeavingRegionWithBeer | RELOCATION_MATCH PASS (new, 184 bytes, first attempt) |
| 0x0092FAB0 | CAIStateGroup_MinionReactToPlayerInteraction::TestOrDoInteraction | RELOCATION_MATCH PASS (8 attempts; 311 frame-offset lines before) |
| 0x00F0C140 | NScript::CQ_AmbushTradersScript::PhaseTwo | RELOCATION_MATCH PASS (6 attempts) |

- **WatchForHeroLeavingRegionWithBeer.** The name is proven by two sources:
  - CMagicman::Main (0x00E40E80) builds the thread name "ParentClass." + "WatchForHeroLeavingRegionWithBeer" (0x012E48E0) and stores
    0x00E44980 as the CSpawnedFunc<CV_BordelloScript> member pointer (0x00E411E2, 0x00E42B1D);
  - Ego_r.pdb and FableWin.pdb list `?WatchForHeroLeavingRegionWithBeer@CV_BordelloScript@NScript@@QAEXXZ` just before
    IsHeroWearingBeard (0x00E44A40 in retail).

  The row was in neither the manifest nor the catalog. It was added to `rebuild/manifest/boundary-starts.tsv` and the manifest
  before landing (known functions 57,615); its label is in `ghidra_out/labels_landing_2026-11-06z.tsv`.
- **TestOrDoInteraction.** Two spelling changes fixed the frame:
  - the three SetText button strings are implicit `L"..."` conversions;
  - case 0 keeps `phrase` in its own block, with a flag set by `if (... == SUCCESS) spoke = true;`.
- **PhaseTwo.** Three changes:
  - the region loop checks `IsActiveThreadTerminating()` at its top;
  - `long i = 0;` sits before the `if (count > 0)` guard;
  - the teleport count is written `TeleportInfo.end() - TeleportInfo.begin()`, because `size()` was called out of line there.
- **Headers:** none changed.
- **Names:**
  - The agent's four NAME_FIXES (0x00F0CA30 vector<TeleportData>::swap, 0x006588CF CTCVillage::GetCostToBribe, 0x006CEAE0
    CTCCreatureOpinionOfHero::ResetLastOpinionReactionFrame, 0x00798D30 CTCShopKeeper::GetHint) and the 0x0092F310 GetGossipPhrase
    label were already queued.
  - 0x0048DB13 (manifest placeholder `CWorld::Query_48db13`, PDB `CWorld::IsPerformingRegionLoad`) is queued as `land0611z`.
  - TestOrDoInteraction binds PDB / template spellings at nine targets that still carry older non-PDB spellings from earlier rows
    (CIntelligentPointer conversions, IS_THING_ALIVE, AIAction_SpeakAISpeechToTarget / SpeakToTarget, GetGreetDialogueGroupIndex,
    CSubtitleRenderer::SetText). No symbol is bound at a second address. The older spellings are listed for the naming pass in one
    `land0611z` CHECK line.
- **Landing checks:**
  - `vland --land` ran (`logs/land.log`); RELOC_NAME warnings come from the catalog labels named above.
  - The manifest sync dry run prints 0.
  - bindings_multi_check prints only the known NullCoord case.

## Parked (`pending/2026-11-06_linux_hand825/`)
| Row | Grade | Residue |
|---|---|---|
| 0x007C4430 CTCHealLife::StartHealingEffect | DIFFER(1980v1952), 222 lines, 13 attempts | the loop's float push_back is expanded where retail calls 0x00568850. A probe with a foldable `if (!target) continue;` outlines it and gives retail's 1952 bytes but moves `this` to edi (a lead; retried in chunk 827) |
| 0x00EE57B0 NScript::CV_TourGuideScript::CTourGuideGuide::Main | DIFFER(4356v4246), 235 lines (was 271), 6 attempts | register allocation of the two CTimer indices, so two DeregisterTimer pads do not merge (per-exit ~CTimer blocker). Test newly written |
| 0x00DEA3F0 NScript::CQ_RansomVictimScript::CVictimsBrother::Main | DIFFER(3056v3076), 161 lines, 6 attempts + 3 diagnostics | the CIsThingAlive copy-inline blocker. Tried a `__forceinline ~CIsThingAlive` and a named block-scoped predicate (not in GOTCHAS before) |
