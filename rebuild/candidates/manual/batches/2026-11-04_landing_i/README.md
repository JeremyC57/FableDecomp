# Landing 2026-11-04i: chunk 746 (CV_MurderTwistFinalScript sheriff / guard dialogue, 4 of 5)

## Result
- **Landed 4 rows** (`logs/land.log`); graded read-only on the merged tree first (`logs/new2.log`): RELOCATION_MATCH + PASS.

  | Address | Name | Grade |
  |---|---|---|
  | 0x00EB0830 | `CV_MurderTwistFinalScript::CMurderTwistSheriff::DoRemindingHeroToKillAssassin` | RELOCATION_MATCH |
  | 0x00EB2900 | `CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::DoRemindingHeroToKillAssassin` (catalog said DoAskingHeroToSpeakToBossAgain) | RELOCATION_MATCH |
  | 0x00EB2DC0 | `CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::DoAskingHeroToSpeakToBossAgain` | RELOCATION_MATCH |
  | 0x00EB3280 | `CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::DoCongratulatingHero` (catalog said DoAskingHeroToSpeakToBossAgain) | RELOCATION_MATCH |

  - No asm, helper, pragma, `volatile`, two-view or goto device. The shared MovieBase (implicit dtor), an out-of-line
    `CCountedPointer<MovieBase>` dtor declaration and the RAII pause object give retail's per-exit inline/out-of-line split.
- **Not landed** (`pending/2026-11-04_linux_hand746/`): 0x00EAF810 `CMurderTwistSheriff::DoAskingHeroToKillAssassin`, DIFFER(3021v2945)
  after 13 grades and about 30 probes: one extra spill of the pause object's Game pointer in the talk block shifts every later temporary
  by 4 bytes. The next idea is the PDB function-local records for this function.

## Headers
- New `engine/CV_MurderTwistFinalScript.h` (hdrcheck2 `NEW`): the level script at retail size 0x88 with the sheriff and guard as nested
  classes (0x38 each). No existing header changed, so no landed source is affected.

## Names
- Three same-size guard bodies (1203 bytes) shared one propagated label. They were told apart by donor member order in both PDB name
  sets, the guard Main's call order (0x00EB2269 .. 0x00EB2385) and the text literals.
- **Labels:** `ghidra_out/labels_landing_2026-11-04i.tsv`, 13 labels (the 4 rows, the parked row, guard Main 0x00EB2140 = vtable
  0x012EFEE8 slot 1, and corrected names for landed neighbours); `apply_labels.py`: 7 catalog and 9 manifest renames.
- Line-1 comments of landed 0x00EB15C0 (DoGivingRewardToHero) and 0x00EB19C0 (DoSayingWellDoneToHero) corrected; their leaves were right.
- **Queued PRIORITY** (`land0411i`): landed 0x00EB23E0, 0x00EB3740, 0x00EB4F80 are defined on global `struct` stand-ins and emit
  un-nested symbols (re-model onto the new header); reloc_bindings carries non-NScript spellings for 0x00EB07D0 and 0x00EB0700.
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.
