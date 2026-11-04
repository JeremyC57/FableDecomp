# Landing 2026-11-04zh: chunk 771 (14 groups-ledger Fable bodies), 6 landed

## Result
- **Landed 6 of 14** (`logs/land.log`). All are RELOCATION_MATCH + PASS, graded read-only on the merged tree first (`logs/new2.log`).
  - 0x00AC44D0 `CVertexStreamConverter::ConvertFrom`
  - 0x00AC4910 `CVertexStreamConverter::ConvertTo`, grade `retail_asm`
  - 0x009E8C10 `CPersistTraits<CRGBColour>::TransferIn` (14 grades)
  - 0x006C9490 `CPersistTraits<COpinionDeed*>::TransferIn` (15 grades)
  - 0x006159A0 `CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>::TransferIn` (12 grades)
  - 0x005C19A3 `CPersistTraits<NInventory::CItem>::TransferBinaryIn` (5 grades)
- **Asm rules (owner, 2026-11-04):**
  - 0x00AC4910 has no asm of its own. It calls the shared `engine/GFFloatToLongNear.h` helper (an `__asm` path plus a
    `FABLE_PORTABLE` C++ twin). Its `// Retail-asm:` line cites 0x00AC4999 / 0x00AC499D (bare `fistp dword`).
  - The chunk NOTES.md has the per-site "C++ attempts before the helper" table. It covers 10 spellings (`(long)x`, `(int)x`,
    `(long)(x+0.5f)`, floor with `#pragma intrinsic`, `/QIfist`, the `_mm_cvtss_si32` intrinsic, the macro form, ...). None emits
    the bare dword `fistp`.
  - No other row uses a helper, pragma or `volatile`.
- **Parked 1:** 0x006A5D90 `CThingPhysical::ConstructFromLevelScript`, DIFFER(1268v1268), 2 lines after 26 grades. Retail stores
  the angle temporary's AngleYZ before the `fstp` of position.Y.
- **Not attempted 7:** the agent ran out of budget. Their identities are in the chunk NOTES.md:
  - 0x00645853, 0x00762620, 0x00763060, 0x00774D20, 0x008EEC70, 0x009BCA20, 0x00E10E60.

## Headers (5 changed, new sub-macros used only by the parked row, `default=same`)
- New sub-macros:
  - FABLE_RETAIL_CTHING_LEVELSCRIPT
  - FABLE_RETAIL_CTHINGPHYSICAL_LEVELSCRIPT
  - FABLE_RETAIL_CTHINGBASEDEF (the Ego_r members at retail offsets, PDB minus 4)
  - FABLE_RETAIL_CRIGHTHANDEDSET_ANGLES
  - FABLE_RETAIL_C3DANGLES_CTOR
- `ppcmp.py`: all 425 includers read the same.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04zh.tsv` (16). Five template names and one global name came out garbled from
  the mangled-to-catalog converter. They were fixed by hand: `CPersistTraits<T>::...`, `CCountedPointer<NInventory::CPerItemData>::~CCountedPointer`
  and `DisplayCenteredText`.
  - `finish_landing.sh` now leaves template mangled names unconverted (so that they raise the LABEL-FORM warning) and strips a
    leading `::`.
- **NAME_FIXES (11) queued under `land0411zh`.** They include:
  - 0x009E2590: the static fastcall `CPersistTraitsSLONG::TransferIn`;
  - reference forms (`AAV`/`AAM`) for several CPersistTraits rows;
  - 0x00838930 is CThingGameObject::ConstructFromLevelScript;
  - 0x005C04BF is the CCountedPointer<CPerItemData> destructor;
  - 0x005D8167 is a PROPOSAL.
- **Checks:** `bindings_multi_check.py` prints 0. `sync_manifest_from_catalog.py --dry-run` prints 0 / 0 / 0.

## Coverage
46,861 genuine of 49,568; 5,851,164 function bytes (57.25%).
