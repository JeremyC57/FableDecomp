# Hand chunk 2059 (CEnginePrimitiveManagerParticleMeshGroup): 1 of 1 byte-exact

## What matched (1)
- 0x00BC25F0 CEnginePrimitiveManagerParticleMeshGroup::RenderSubPrimitive (3480 bytes, RELOCATION_MATCH, test PASS under g++ and VC7.1).
  When the render switch 0x0139B7B0 is on, fetch the group's mesh LOD 0 (flags 0x11 when the primitive renderer blocks on loads); if it is
  not there start its texture load (BeginLoadingTextures(1)) and return. Pass 6 (displacement buffer): the shadow-HW hook when the
  particle-group manager reads the z-buffer, blend ADD / SRCALPHA / INVSRCALPHA, displacement shaders and vertex constants 0 (zero) and 1..3
  (the camera's non-homogenous view matrix, transposed, copied to a local); pass 7 (displacement stencil): alpha test on, ref 16, GREATER,
  then the same shaders and constants; other passes: point lights -> the clamped point-light vertex shader, blend op / blend mode behind
  their switches 0x0139B7AC / 0x0139B7A8, and the additive or the normal pixel-shader pair. Constant 4 = (4, 0, 0, 0). Then per mesh
  primitive with a diffuse map whose texture lookup succeeds: texture stage 0, self illumination + global lighting, the primitive's vertex
  and index buffers, then the group's particle meshes in batches of min(reps, 10): per mesh three transposed object-matrix rows and the
  colour / 255 with doubled alpha into ConstantBlock; a full batch (and the tail) sets the group world transform, uploads the constants at
  5, PrepareForRender(0x11) and draws (switches 0x0139B7A4 / 0x0139B7A0); the lighting defaults are restored per primitive and the world
  transform reset at the end.
- How it closed (grades.log): first full draft 751 / 960 equal instructions (grade 3). Then: ApplyMeshPointLightVertexShader forced
  (786), `failed` flag in its own block (frame 0x6C -> 0x68, 842), `repeats < 10 ? repeats : 10` (919), meshes computed after
  SetWorldTransform (920), `batch_size * 4` as the upload count (921), the colour block reference before the CRGBFloatColour conversion
  (943: only the SetRenderState bookmark test / or orders left, pattern ours BBBBBBBAAAAA v retail BAAAAAAAAAAA). Dead-local rulers (1..32
  `long` / `const long`, top or end of the function) did not move that pattern at all. Writing pass 6's blend states through the same
  helpers the other pass uses, with constant arguments (`SetupBlendOpState(ENGINE_PRIMITIVE_BLEND_OP_ADD);
  SetupAlphaBlendingStatesFromBlendMode(ENGINE_PRIMITIVE_BLEND_ALPHA);`), gave RELOCATION_MATCH at grade 49 (direct SetSource +
  helper-free SetBlendOp: BBBAAAAAAAAA, 954). Edge check: still RELOCATION_MATCH with 1, 2, 4, 8 dead `const long`s added (diagnostic).
- FITTED v PROVEN:
  - PROVEN (two sources each): every callee / global name and address (PDB name sets + retail call sites; donor order for the two empty
    stubs, LABELS.tsv); member layouts (Ego_r.pdb names, retail offsets read in the row); the helper set (FableWin.pdb COMDAT run after the
    row: GetReadZBuffer, ApplyMeshPointLightVertexShader, ApplyDisplacementVertexShader, GetRepeatingMeshBatchSize, HasValidDiffuseMap);
    FableWin debug sizes that make GetRepeatingMeshBatchSize / GetReadZBuffer / the C3DPrimitive2 getters plain one-member getters.
  - FITTED: pass 6 through SetupBlendOpState / SetupAlphaBlendingStatesFromBlendMode with constant arguments (natural: the same helpers as
    the other pass, bytes identical to direct setters; chosen by the bookmark or-order); the `failed` block scope; the ternary clamp of
    `repeats`; `meshes = count / 4` after SetWorldTransform; `batch_size * 4` as the upload count; the colour reference before the
    conversion; ApplyMeshPointLightVertexShader's body (the line-group twin's text) and its FABLE_FORCEINLINE (c2 score 144 > 74; no
    Ego_r / retail copy).
- Descriptive (made-up) names: the five render switches g_particleMeshRenderEnabled_0139b7b0, g_particleMeshBlendOpEnabled_0139b7ac,
  g_particleMeshBlendModeEnabled_0139b7a8, g_particleMeshBatchDrawEnabled_0139b7a4, g_particleMeshTailDrawEnabled_0139b7a0 (dwords; no
  data name in the PDB name sets, labels or sibling repos; the line group's twin is the pending g_particleLineRenderEnabled_0139b864).
  The row's local names (group, mesh, group_manager, texture_bank, primitive, batch_size, ...) are descriptive.

## Parked for Fable 5.1 (0)

## Skipped
None. The row starts at its real entry (vtable slot 5) and ends at `ret 8` 0x00BC3385; length 3480 confirmed.

## Label corrections
- 0x00A23C50 is ?BeginLoadingTextures@C3DMeshInfo@@QBEXW4EBankLoadPriority@@@Z (Ego_r donor order), not SetLoadPriority /
  UnknownRet4 / Nop_ret4 (landed 0x00B9E950, 0x00BD1F70, 0x00B991D0 bind the wrong spellings; NAME_FIXES.tsv; also pending r3013).
- 0x00A23C60 is ?AreTexturesLoaded@C3DMeshInfo@@QBE_NXZ, not IsBuilt / IsValid (landed 0x00B9E950, 0x00B9CE10).
- 0x00B86A00 is ?SetupShadowHWAsDepthBuffer@CEngineScreenEffectDisplacementRenderer@@QAEXPBVCMatrix3x4@@@Z (manifest Nop_ret4_00b86a00).

## Header overlay
hdr/README.md (chunk 2045's overlay plus eleven opt-ins of this chunk; every changed header's default text preprocesses identically to
the repo copy). Chunk 2045's row 0x00BB8EF0 regraded under this overlay: RELOCATION_MATCH.
