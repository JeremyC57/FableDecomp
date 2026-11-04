# Landing 2026-11-04zc: chunk 766 (26 groups-ledger bodies, mostly libpk), 7 landed

## Result
- **Landed 7 of 26** (`logs/land.log`), each graded read-only on the merged tree first (`logs/new2.log`): RELOCATION_MATCH + PASS.
  - NUISystem::CUIMiscThingsDef ctor (0x004304E0)
  - CSubtitleRenderer::GetActionNameFromEmbeddedTag (0x0049124E)
  - PKCDefaultFileManager::AsyncReadThread (0x00C1A9C0)
  - ReadBankData (0x00C30F60)
  - RemoveAt (0x00C42B80)
  - Issue (0x00C47F00)
  - CCodaVoice::UpdatePitchEnvelopeStages (0x00C5A700)
- **Asm rules (owner, 2026-11-04):** no asm, helper, pragma or `volatile` in a landed row. The parked 0x00C5CA60 draft uses only the
  shared `libpk/FtoL.h`.
- **Parked 7** (`pending/2026-11-04_linux_hand766/differ/`):
  - **0x004199C0:** a catalog bounds error. It starts inside 0x00419880 (MIDSTART).
  - **Six DIFFER rows with 10-12 grades each:** CEffectFilter::SetShelves, Stage_00c77800, CSoftwareVoice::ApplyParameters,
    CTCQuickAccessMenu::GetExpressionsListUI, PKCDefaultFileManager::OpenFile and ReadDeferredBank. The residues are an x87 CSE
    spilled at a join, uniform scratch-register rotations, and callee-saved push placement.
- **Not attempted: 12 rows**, because the agent ran out of time. They go to chunk 769.
  - 0x00C54190 is blocked on naming: its 15 setters are landed under four class names (PROPOSAL queued).

## Headers (3 changed; new sub-macros, `default=same`)
- FABLE_RETAIL_DEF_CLASS_BASE_CTORS (CDefClassBase.h, CParentDefClassBase.h) and FABLE_RETAIL_CVECTORMAP_CTOR (CVectorMap.h).
- `ppcmp.py`: of 1,016 includers, 2 differ. Both were rechecked on the merged tree (`logs/recheck.log`): RELOCATION_MATCH + PASS.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04zc.tsv` (5).
  - Three are two-source names: 0x004304E0, 0x0049124E and 0x00627BE0.
  - The other two respell the garbled `_Cons_val<...>` catalog names of 0x00C42B80 / 0x00C47F00 to their landed leaves
    (`CUnprovenPtrList_00c42b50::RemoveAt`, `CAIStateGroup_BirdFlyToFood_00c48300::Issue`). Those are the placeholder classes the
    landed callers already use.
  - `finish_landing.sh` now does this from the payload's module/name when the catalog name is garbled.
- **Queued as PROPOSAL** (`land0411zc`), among others:
  - OpenFile 0x00C1A510, replacing the landed `Resolve@CSBAgentsFactory_00c1abb0`;
  - the four CSegmentFile members 0x00C2DA00..0x00C2DBE0, which are landed under four spellings;
  - one class each for the +0x290 parameter setters and for RemoveAt / Remove;
  - HasBankSegments / HasWaveBankSegments return int;
  - the 0x0129BA3C label is still EPSILON (really DELTA_NOTIONAL_ZERO).
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.

## Coverage
46,803 genuine of 49,568; 5,838,659 function bytes (57.13%).
