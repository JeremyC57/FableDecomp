# Landing 2026-11-04c: chunk 738 (CPSCRenderSprite InterpolateTo / ReadBinary / DatabaseSerialise)

## Result
- **Landed 3 rows** (`logs/land.log`). Before landing, each was graded read-only on the landing tree (HEAD after landing b, plus
  the new `engine/GFPackFloat.h`) with no overlay (`logs/new3.log`): RELOCATION_MATCH + PASS.

  | Address | Name | Grade |
  |---|---|---|
  | 0x00AF9840 | `NParticleEngine::CPSCRenderSprite::InterpolateTo` (slot 14) | RELOCATION_MATCH, `retail_asm` (0x00AF99B4 `fistp dword [esp+14h]`) |
  | 0x00AFA2B0 | `NParticleEngine::CPSCRenderSprite::ReadBinary` (slot 15) | RELOCATION_MATCH |
  | 0x00AFBCE0 | `NParticleEngine::CPSCRenderSprite::DatabaseSerialise` (slot 17) | RELOCATION_MATCH, `retail_asm` (0x00AFC5B0 `fistp dword [esp+1Ch]`) |

  - No row source or test contains `__asm`, `naked`, `_emit` or `volatile`. Every bare `fistp dword` comes from the shared
    `engine/GFFloatToLongNear.h`, which has the FABLE_PORTABLE C++ twin.
- **Not landed** (staged in `pending/2026-11-04_linux_hand738/differ/`):
  - 0x00AFAE50 AddToBoundingSphere (slot 19), DIFFER(2406v2405):
    - registers are rotated by one around the aspect movzx pair, IsActive and the pos copy;
    - the pos TransformPoint has a different X-row term order.
  - 0x00AFCC30 PreparePrimitives (slot 13), DIFFER(5338v5274):
    - the three `CArray<long>()` resize temporaries keep their destructors, which retail does not emit;
    - the frame is 0x334 against retail's 0x328.

## Headers: overlay reconciled with landing b
Chunk 738 ran on the 40c4d6d63 mirror at the same time as chunk 740. Both independently added a FABLE_RETAIL_CSYMBOLMAP branch and
counted-pointer assignments.
- **Kept HEAD's (chunk 740's) `CSymbolMap.h`.** It has the nested, class-keyed `CSymbolMap::CSymbolInfo`, FableWin's exact spelling
  `?FindFirstSymbolWithVal@CSymbolMap@@QBEPBVCSymbolInfo@1@ABVCCharString@@J@Z`. Chunk 738 had a global `struct CSymbolInfo`, which is
  the wrong spelling.
  - The one use in 0x00AFBCE0 was respelled `CSymbolMap::CSymbolInfo` before grading.
  - It still matches, and it now binds the correct symbol.
- **Not taken** (only the parked drafts reached them): chunk 738's `CIVCountedPointer.h` / `CEnginePrimitiveHandle.h` operator= and
  `C3DBoundingSphere.h` AddToBounds. HEAD already has chunk 740's FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN branch for the same retail
  `??4`. The parked drafts must be ported to it when they are retried.
- **New: `engine/GFPackFloat.h`** with `__forceinline` GFRoundFloatNear / GFPackFloat / GFUnpackFloat (FableWin `YI` names). Compiled
  out of line, GFPackFloat grades RELOCATION_MATCH against the landed 0x00AF8D50, and GFUnpackFloat is instruction-identical to
  0x00AF8DC0. It has no asm of its own. No landed source includes it, so there are no siblings to recheck.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04c.tsv`, 9 labels: the five CPSCRenderSprite slots, DatabaseCopy 0x00AF9410
  (catalog vfunc_18), GFUnpackFloat 0x00AF8DC0, GetOneOverTickRate 0x00AEAA00 and C3DBoundingSphere::AddToBounds 0x00ADDD80.
- **0x00F36540: the agents disagreed** (chunk 740: GetGaussian2DNoise; chunk 738: Get2DNoise). Landing b's GetGaussian2DNoise is kept
  for two reasons:
  - retail order (0x00F36010, Get3DNoise 0x00F361D0, 0x00F36540) follows the declaration order Get2DNoise, Get3DNoise,
    GetGaussian2DNoise;
  - 0x00F36540 reads one scalar per lattice corner.

  A NOTE is queued (`land0411c`).
- **Queued PRIORITY** (`land0411c`): 0x00AF9410 DatabaseCopy (landed as `vfunc18`). Chunk 738's other fixes were already queued with
  the same spellings.
- `bindings_multi_check.py` prints 0. `sync_manifest_from_catalog.py --dry-run` prints 0 / 0 / 0.

## Coverage
46,558 of 49,564 (93.935%); 5,757,980 function bytes (56.296%); 54,110 landed rows (194 `retail_asm`).
