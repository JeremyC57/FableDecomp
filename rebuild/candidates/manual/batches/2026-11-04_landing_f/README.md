# Landing 2026-11-04f: chunk 742 (CParticleEmitter::ReadBinary, CEnginePrimitiveManagerParticleGroup::RenderSubPrimitive)

Chunk 742's agent finished its work (src/, differ/, payload.json, a read-only vland run, NOTES.md) but was lost in the second
container restart before it reported. It was landed from its scratch directory.

## Result
- **Landed 2 rows** (`logs/land.log`). Before landing, both were graded read-only on the merged tree with no overlay
  (`logs/new2.log`): RELOCATION_MATCH + PASS.

  | Address | Name | Grade |
  |---|---|---|
  | 0x00AEFAA0 | `NParticleEngine::CParticleEmitter::ReadBinary` | RELOCATION_MATCH |
  | 0x00BBC460 | `CEnginePrimitiveManagerParticleGroup::RenderSubPrimitive` (slot 5) | RELOCATION_MATCH |

  - No row source or test contains `__asm`, `naked`, `_emit` or `volatile`.
- **Not landed** (staged in `pending/2026-11-04_linux_hand742/`):
  - 0x00BBE090 RenderSingleSpriteTrail: DIFFER(2432v2464), 46 lines.
  - 0x00BC3F30 ParticleLineGroup::RenderSubPrimitive: DIFFER(3272v3357), 242 lines.
  - 0x00BE0EB0 2DParticleGroup::RenderSubPrimitive: DIFFER(2889v2889), 14 lines.
  - 0x00BC25F0 ParticleMeshGroup::RenderSubPrimitive: not attempted; NOTES.md has a decoded outline.
  - 0x00BC3F3D is no function: it lies inside 0x00BC3F30's prologue. A NOTE to retire the manifest row is queued.

## Headers (7 changed; every change sits behind its retail branch, and the default path is the same)
Changed: CParticleEmitter.h, CParticleSystem.h, CEngineComponent.h, CEngineLightingManager.h, CEnginePrimitiveRenderUtil.h,
CEngineResourceManager.h and CMatrix3x4.h. The chunk NOTES.md has the table.
- **Merge:** none of the seven changed in the repo since the overlay base 9e3ba62ea. Two of them remove lines, both inside retail
  branches:
  - CEngineLightingManager.h: a forward declaration becomes its include, and the opaque `_pad_0x88` is split into the named members
    +0xCE..+0xEB;
  - CParticleEmitter.h: a forward declaration of CDataInputStream is added.
- **Siblings:** this goes further than the chunk's own recheck, which covered 7 macro definers.
  - `includers.py` (landing d) found 206 landed sources and tests that reach a changed header.
  - `ppcmp.py` compared each between HEAD's headers and the merged mirror (`logs/ppcmp.txt`): 190 are token-identical and 16 differ.
    The 6 "cpp-err" lines are the tool's header line, not files.
  - The 16 that differ include those that reach the FABLE_RETAIL_CMATRIX3X4 TransformPoint through the auto-defining
    CEngineLightingManager.h / CShaderRenderManager.h.
  - All 16 were rechecked on the merged tree (`logs/recheck1.log`, `logs/recheck2.log`, `sibling_grades.tsv`): 16 RELOCATION_MATCH +
    PASS, the same grade every one had at its last recorded grade. No regressions.
- hdrcheck2 after the merge: `logs/hdrcheck2_post_merge.txt`.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04f.tsv`, 12 labels. `apply_labels.py` made 3 catalog renames and 6 manifest
  renames. The descriptive data label g_particleLineRenderEnabled_0139b864 is left out.
- **Bindings:** +4 (`bindings_multi_check.py` prints 0, and no address gained a second spelling).
- **Queued** (`land0411f`):
  - PRIORITY: 0x00AF4660 CParticleSystem::ReadBinary, 0x00BBC130 RenderSubPrimGroups, 0x00BBE090, 0x00BC04F0 RenderSingleSprite and
    0x00AF0EF0 `vector<CCountedPointer<CParticleSystem> >::reserve`;
  - NOTE: 0x00BC3F3D (not a function).
- `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.

## Coverage
46,573 of 49,564 (93.965%); 5,780,192 function bytes (56.514%); 54,125 landed rows (195 `retail_asm`).
