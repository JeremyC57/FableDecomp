# Landing 2026-11-03b: chunk 726, 0x00AA5C80, 0x008771A0 and the three Codex pass 13 render rows

## Result
- **12 rows landed**, all RELOCATION_MATCH with `vland --land` on the tree (`STATUS.tsv`, `logs/land_*.log`):
  - **Chunk 726, 7 rows:**
    - 0x0053E950 ~CScrollingViewport
    - the two ??_E adjustor thunks 0x0053FC10 / 0x0053FC20
    - 0x00558660 ~CKeyRedefiner
    - 0x005385AE CList::ScrollUp
    - 0x005392FC CList::ScrollDown
    - 0x0053A0A1 CList::ChangeState
  - **Chunk 725 round 2, 1 row:** 0x00AA5C80 C3DMeshFileXLoader::C3DMeshFileXLoader.
  - **Chunk 724 round 3, 1 row:** 0x008771A0 NCameraModes::CTools::GetThingPointIntersectionFromTo.
  - **Codex pass 13, 3 rows:**
    - 0x00989CB0 CShaderRenderManager::UpdateObjectSpaceLighting
    - 0x0098ACF0 CShaderRenderManager::Initialise
    - 0x009DA9F0 CRenderManager2D::Render2DDrawList
- **Held:** 0x00501450 CWorldMap::GenerateRegionConnectivityGraph (optional item, not attempted). The reason is in
  `HEADER_NEEDS.md`.
- **Checks:**
  - `bindings_multi_check.py rebuild/oracles/reloc_bindings.tsv` prints 0.
  - Coverage: 46,531 genuine, 93.88% of the catalog; 5,688,358 bytes, 55.61% of retail function bytes. Before this
    landing it was 46,521 (93.86%) and 55.41%. Landed rows: 54,065 → 54,077.

## Headers (one retail model per class)
Every changed header keeps its default path byte-for-byte: hdrcheck2 gives default=same (`logs/hdrcheck2_final.txt`).
The one exception is `CShaderResource.h`, which has no class branch; it gains one declaration,
`Initialise(CShaderRenderManager&)` (0x00A5D4D0), and its includers were rechecked.

**Render classes.** The Codex pass 13 render models and chunk 723's round 2 and round 3 models were merged into one
class branch each:
- `CRenderManager2D.h` (`FABLE_RETAIL_CRENDERMANAGER2D`): typed members and the statics FlipTable and
  LibraryStateBlockFunctions 0x01396510.
- `CRenderManagerCore.h` (`FABLE_RETAIL_CRENDERMANAGERCORE`): 723's typed members, plus Codex's declarations and
  `GetDisplayManager`.
- `CRenderStateManager.h` (`FABLE_RETAIL_CRENDERSTATEMANAGER`): a new branch. It has nested CStateInfo,
  CStateStackElement and CDeviceState, the PDB D3D*Table statics, `__forceinline` setters and private data. The default
  generated model is unchanged under `#else`.
- `CShaderRenderManager.h`: new, one model, no class macro. Its singleton is 0x013BC470 and sizeof is 0x3E8.
- `CVertexShaderConstantLayout.h`: 723's branch with private typed ranges and a CShaderRenderManager friend.
- `CTexture.h`: `const CTexture& operator=` (PDB spelling) and the pre-allocated-texture ctor.
- Smaller additions in their existing branches: `CDisplayManager.h` (`GetD3DDevice`), `CFontBank.h` (default arguments),
  `C4DVector.h` (CRGBFloatColour ctor, Initialise, `operator*`), `C2DBoxF.h`.

**Other classes:**
- Chunk 726 headers: NUISystem_CList, NUISystem_CManager, and new NUISystem_CKeyRedefiner, NUISystem_CScrollingViewport,
  NUISystem_CViewport.
- Chunk 725 headers: the C3DMeshFileX*Chunk, CChunkedFileChunk and CChunkedFileSuperChunk branches, and new
  C3DMeshFileLoader, C3DMeshFileXLoader, CChunkedFileLoader, C3DIndexedBoneWeight, CVertexGroupBlock and
  C3DMeshFileXAnimatedPrimitiveBlockChunk.
- Chunk 724 round 3: CThingPhysical (its branch), and new CTCCameraCollision, CTCHitLocations and NCameraModes_CTools.
- `stlport/fable_map.h`: chunk 726's in-class `map::operator[]` body was merged into the pass 13 FABLE_RETAIL_STLP_MAP
  branch, which already has the `insert_unique` body.
- `stlport/fable_containers.h`: 726's FABLE_RETAIL_STLP_LIST branch (size / front / push_front).

**Changed: 26.**
- C2DBoxF, C3DMeshFileTrianglesChunk
- C3DMeshFileX{AnimatedPrimitive,BoundingBox,BoundingSphere,MappingInfo,TextureIndex,TriangleStrip,UVTangent,VertexData}Chunk
- C4DVector, CChunkedFileChunk, CChunkedFileSuperChunk, CDisplayManager, CFontBank
- CRenderManager2D, CRenderManagerCore, CRenderStateManager, CShaderResource, CTexture, CThingPhysical
- CVertexShaderConstantLayout, NUISystem_CList, NUISystem_CManager
- stlport/fable_containers.h, stlport/fable_map.h

**New: 31.**
- From chunk 725: C3DIndexedBoneWeight, C3DMeshFileLoader, C3DMeshFileXAnimatedPrimitiveBlockChunk,
  C3DMeshFileXLoader, CChunkedFileLoader, CVertexGroupBlock.
- From chunk 724: CTCCameraCollision, CTCHitLocations, NCameraModes_CTools.
- From chunk 726: NUISystem_CKeyRedefiner, NUISystem_CScrollingViewport, NUISystem_CViewport.
- Render: CIndexedArray, CMatrix4x4, CPreTransposedBoneMatrix, CQuickDrawTriInfo, CRGBFloatColour,
  CRenderStateCaptureBlock, CScopedShadersEnable, CScopedVertexShaderConstantLayout, CShaderConstantRange,
  CShaderGenericLight, CShaderRenderManager, CStateBlockFunctionBase, CTVertexRHWColSpecTex1, CVertexShaderConstant,
  CVertexShaderConstantLayoutEmpty, EPrimitiveType, EVertexType, IDirect3DDevice9, RenderStateEnums.

**Macros folded vs kept separate.**
- **Folded into class branches:**
  - Codex's per-feature `FABLE_RETAIL_RENDER2D_TYPES`, `FABLE_RETAIL_RENDERSTATE_EXPAND_SET` and
    `FABLE_RETAIL_SHADER_LIGHTING` now live in CRENDERMANAGER2D / CRENDERMANAGERCORE / CRENDERSTATEMANAGER /
    CVERTEXSHADERCONSTANTLAYOUT, and CShaderRenderManager and CMatrix4x4 have one model each. Those three macro names
    survive only in comments.
- **Kept separate (one per-TU opt-in):**
  - `FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS` gives the `__forceinline` body of AttachTextureToStage.
  - Retail 0x009DA9F0 calls 0x009A0CF0 out of line, and with the body visible VC7.1 expanded it (DIFFER 3462 v 3344).
    The water passes expand the same body.
  - So both forms exist in retail, and the default is the declaration.
- **Row-local:** 0x00989CB0 defines `CShaderRenderManager::SetVSConstants` `inline` in its own TU. Retail expands it
  there only.

**Not installed:** see `HEADER_NEEDS.md`. This covers 723's remaining class changes, NCameraModes_CBase, NUISystem_CScrollBar
and Codex's bank/file models.

## Includer recheck (`includer_recheck_grades.tsv`)
- **Scope.** 2,709 landed sources and tests reach a changed header.
- **Token-identical preprocess.** 2,683 preprocess token-identically against the final headers, so they give the same
  object.
- **Rechecked: 26 differ.** All 26 are RELOCATION_MATCH with `vland --recheck-landed` on a full mirror identical to the
  final `rebuild/include` (`diff -rq` empty). They include:
  - the STLP_MAP definers 0x00D51520, 0x00DC1770, 0x00E0A820 and 0x00E26E30;
  - 726's siblings 0x006286F0 and 0x00487570.
- **Chunk 723.** Its 6 landed rows and its round 2 and round 3 sibling lists (35 rows) were rechecked on the final tree.
  32 are RELOCATION_MATCH and 3 are MATCH.
- **Chunk 729's three DIFFERs.** 0x00521620, 0x00821010 and 0x0047A9F0 are RELOCATION_MATCH with `--recheck-landed`
  on both HEAD and the final tree (`logs/rc_head.log`, `logs/rc_fin.log`). The DIFFERs chunk 729 saw came from grading
  against a partial overlay, not from the tree.

## Landing edits to the rows
- **0x00989CB0.** The row's `#define FABLE_RETAIL_SHADER_LIGHTING` is dropped, and SetVSConstants is defined inline in
  the TU.
- **0x009DA9F0.**
  - The per-feature macros are replaced by the class macros.
  - It includes `engine/IDirect3DDevice9.h`.
  - Its DrawPrimitiveUP primitive types are commented literals.
  - With the shared header, the bool setters give `setne`.
- **0x0053A0A1 (and 726's other rows).** They take `map::operator[]` from the shared fable_map.h branch.
- **keypayload.py** changed only test text (struct → class keys).

## Names (`NAME_DECISIONS.tsv`)
- **Catalog and manifest.** Names for the 12 rows come from their Leafs.
- **Labels.** `ghidra_out/labels_landing_2026-11-03b.tsv` has 23 labels:
  - the 12 rows;
  - from chunk 726 / 725 / 724 / 723 LABELS where both PDB name sets agree and no landed Leaf conflicts: 0x005379F5,
    0x0053F1C0, 0x0054EAC0, 0x00A9BBE0, 0x00879900, 0x00877840, 0x00B1E380, 0x00B1F200, 0x00B70A90, 0x00B6EDF0,
    0x00BC04F0.
- **Tools run:** `apply_labels.py`, `sync_catalog_names.py`, and `sync_manifest_from_catalog.py --labels`.
- **Queue.** 26 lines were added to `../../naming/NAME_FIXES_QUEUE.tsv`, tagged c726 (11), c725 (4), c724 (7) and
  c723 (4). 8 are PRIORITY and 18 REVIEW.
  - They are the second spellings `spellcheck.py` reports for this landing's bindings, plus the chunk NAME_FIXES not
    already queued.
  - The Codex render rows' second spellings (cx13r) were all already queued under cx13 / c723 / cx11m, so no cx13r line
    was needed.
- **Respellings.** No landed caller was respelled. Each candidate (0x0053FC30, 0x00558B30, 0x00AA4B00..0x00AA5250 and
  others) needs a re-landing against the new headers, so it is queued.
