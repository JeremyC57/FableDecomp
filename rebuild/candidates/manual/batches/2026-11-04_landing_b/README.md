# Landing 2026-11-04b: chunk 740 (CPSCRenderMesh InterpolateTo / ReadBinary / DatabaseSerialise)

## Result
- **Landed 3 rows** (`logs/land.log`). Before landing, each was graded read-only on the merged tree with no overlay
  (`logs/new3.log`): RELOCATION_MATCH + PASS. That tree already carries landing a's (chunk 739) header branches.

  | Address | Name | Grade |
  |---|---|---|
  | 0x00AFF1B0 | `NParticleEngine::CPSCRenderMesh::InterpolateTo` (slot 14) | RELOCATION_MATCH, grade `retail_asm` |
  | 0x00AFFFE0 | `NParticleEngine::CPSCRenderMesh::ReadBinary` (slot 15, catalog "vfunc_15") | RELOCATION_MATCH |
  | 0x00B027D0 | `NParticleEngine::CPSCRenderMesh::DatabaseSerialise` (slot 17) | RELOCATION_MATCH, grade `retail_asm` |

  - Neither `retail_asm` row has any asm of its own. Each reaches its bare `fistp dword` roundings through the shared
    `engine/GFFloatToLongNear.h` helper, which also has a FABLE_PORTABLE C++ path. The cited instructions are in Fable.exe:
    0x00AFF22D `db 5c 24 14` and 0x00B032BD `db 5c 24 20`.
  - No row source or test contains `__asm`, `naked`, `_emit` or `volatile`.
- **Not landed** (staged in `pending/2026-11-04_linux_hand740/differ/`):
  - 0x00B01900 AddToBoundingSphere (slot 19), DIFFER(3755v3777):
    - the stack frame is 0x298 against retail's 0x2B0, and the loop-head alignment pad is missing;
    - the Z rows of the trail-end TransformPoint and the trail magnitude are loaded in a different order;
    - the !CentredOnPos matrix block differs.
  - 0x00B03CA0 PreparePrimitives (slot 13), DIFFER(6601v6468):
    - the stack frame is 0x44C against retail's 0x3C0;
    - the `CArray<long>()` temporary passed to resize keeps its destructor and EH frame, which retail has neither of;
    - Release compiles to `mov/dec/mov` where retail has `dec [ecx+4]`;
    - the colour arrays sit 4 bytes off.

## Headers (3 changed, add-only, unchanged since the overlay base 40c4d6d63)
- `CSymbolMap.h`, behind the new FABLE_RETAIL_CSYMBOLMAP: the nested `CSymbolInfo`, FindFirstSymbolWithVal, SymbolExists and
  GetSymbolSLONG. The landed 0x00B027D0 uses it.
- `CIVCountedPointer.h` and `CEnginePrimitiveHandle.h`, behind the new FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN: FableWin's counted `??4`.
  Only the parked 0x00B03CA0 draft uses it.
- hdrcheck2 prints `default=same` for all three, and every existing retail macro is `same` (`logs/hdrcheck2_overlay_pre_merge.txt`).
- No landed source or header names either new macro, so there are no siblings to recheck.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04b.tsv`, 14 labels: CPSCRenderMesh slots 13-20 and 29 (vtable 0x0129F96C),
  GetOneOverTickRate, CParticle::GetInterpolatedLifetimerInSeconds, GetParticleWorldTransform, the CEnginePrimitiveParticleMesh ctor
  and NFractal::CPerlinNoise::GetGaussian2DNoise. `apply_labels.py` made 4 catalog renames and 7 manifest renames.
- **`reloc_bindings.tsv`:** +1 row. The landed 0x00B027D0 binds 0x009E50C0 with FableWin's exact
  `?FindFirstSymbolWithVal@CSymbolMap@@QBEPBVCSymbolInfo@1@ABVCCharString@@J@Z`.
  - The older global-struct spelling bound from 0x006F9BE0 is the PRIORITY fix already queued in c724.
  - Its sibling 0x009E5090 is now queued too (`land0411b`): three binders carry three wrong spellings, and they are respelled as one
    wave with the 0x009E50C0 definition.
  - `bindings_multi_check.py` prints 0.
- **Queued PRIORITY** (`land0411b`):
  - 0x00AFEAD0, the CEnginePrimitiveParticleMeshGroup ctor (landed as `??0SMeshCreateInfo`);
  - 0x00AFE910 `vector<CArray<long> >::resize`;
  - 0x009E5090.
  - Chunk 740's other fixes (0x00AEAA00, 0x004014A0, 0x004428F0, 0x00443F90) were already queued with the same spellings.
- `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.

## Coverage
46,557 of 49,564 (93.933%); 5,748,561 function bytes (56.204%); 54,107 landed rows (193 `retail_asm`).
