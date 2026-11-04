# Landing 2026-11-04zj: chunk 773 (12 groups-ledger Fable bodies, 1.3-1.6 KB), 2 landed. Last chunk before the naming/header plan

## Result
- **Landed 2 of 12** (`logs/land.log`). Both are RELOCATION_MATCH + PASS, graded read-only on the merged tree first, with no warnings:
  - 0x00F38A10 `CSurfaceCompositor::SetBlendMode` (5 grades). The cases are in retail block order. It uses the shared
    CRenderStateCaptureBlock header.
  - 0x00EAF1E0 `NScript::CV_MurderTwistFinalScript::PlayBeingRumbledCutscene` (3 grades).
  - Neither uses a helper, asm, pragma or `volatile`.
- **Parked 3** (`pending/2026-11-04_linux_hand773/differ/`):
  - 0x00671600 CThingCreatureDef::Transfer: 4 lines off after 10 grades (an al/cl swap in a bit merge).
  - 0x00EBE620 CVO_Woman::Main: 4 lines off after 10 grades. It splits the movie-handle ctor per call site, which is plan step 4.
  - 0x005C2118 CPersistTraits<NInventory::CItem>::TransferIn: 6 grades (stack-slot order).
- **Not attempted 7** (surveyed, 0 grades): 0x00524290, 0x0069A2A0, 0x006B97E0, 0x008185B0, 0x00A58130, 0x00A8F1B0, 0x00F3C990.
  - 0x00A8F1B0 is VC7.1's unrolled x87 loop, not hand SIMD.
  - 0x008185B0 is a fastcall fsin/fcos angle sweep, not the catalog's polygon clipper.

## Headers (2 changed, new sub-macros, `default=same`)
- `FABLE_RETAIL_CRENDERSTATEMANAGER_COLOURWRITE4` (CRenderStateManager.h), used by 0x00F38A10.
- `FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE` (stlport/fable_containers.h), used only by the parked 0x00671600.
- `ppcmp.py`: all 3,920 includers read the same.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04zj.tsv` (5). The ungraded 0x00524290 proposal was filtered out.
- **NAME_FIXES (4) queued under `land0411zj`:**
  - 0x004045C0 is the member template `CPersistContext::Transfer<bool>`;
  - 0x00671BE0 `__unique_copy` belongs in `std`;
  - 0x009E2590 is `CPersistTraitsSLONG::TransferIn`, landed under four spellings;
  - 0x006724B0 is a PROPOSAL.
- **Checks:** `bindings_multi_check.py` prints 0. `sync_manifest_from_catalog.py --dry-run` prints 0 / 0 / 0.

## Coverage
46,866 genuine of 49,568; 5,857,597 function bytes (57.31%).
