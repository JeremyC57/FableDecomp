# acctB hand-back, row 0x00BC25F0 CEnginePrimitiveManagerParticleMeshGroup::RenderSubPrimitive: matched

| address | result | notes |
|---|---|---|
| 0x00BC25F0 `?RenderSubPrimitive@CEnginePrimitiveManagerParticleMeshGroup@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z` | RELOCATION_MATCH 3480v3480 (960 of 960), test PASS (g++ and VC7.1) | no flags. 62 logged compiles, 22 authoring grades (`grades.log`) |

Re-verified by the acctB local coordinator (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` holds the grade, both test runs and the read-only vland log. Purity and padded-struct greps clean. Nothing is landed. vland: 1 win, no RELOC_CONFLICT / RELOC_IDENTITY / NOT_GENUINE; two `WARN RELOC_NAME` lines (0x00A23C50
`C3DMeshInfo::BeginLoadingTextures` and 0x00B86A00 `CEngineScreenEffectDisplacementRenderer::SetupShadowHWAsDepthBuffer`
carry `Nop_ret4_*` catalog labels). The agent was cut off by a usage limit after finishing; its notes are complete.

## What closed it (first draft 751 / 960)

`ApplyMeshPointLightVertexShader` forced inline, the `failed` flag in its own block (frame 0x6C to 0x68),
`repeats < 10 ? repeats : 10`, `meshes` computed after SetWorldTransform, `batch_size * 4` as the upload count, the
colour block reference before the CRGBFloatColour conversion. Last residue (the order of SetRenderState's
bookmark test / or): pass 6's blend states written through the same helpers the other pass uses, with constant
arguments (`SetupBlendOpState(ENGINE_PRIMITIVE_BLEND_OP_ADD); SetupAlphaBlendingStatesFromBlendMode(ENGINE_PRIMITIVE_BLEND_ALPHA);`).
Still matches with 1, 2, 4 and 8 dead locals: not on an edge.

## Review items

- **FITTED:** the items above, and `ApplyMeshPointLightVertexShader`'s body (the line-group twin's text) with
  `FABLE_FORCEINLINE` (score 144 against 74; `// Inline-budget:` line in the source; no retail copy).
- **PROVEN:** the helper set (FableWin COMDAT run after the row: GetReadZBuffer, ApplyMeshPointLightVertexShader,
  ApplyDisplacementVertexShader, GetRepeatingMeshBatchSize, HasValidDiffuseMap) and the getter sizes.
- **Made-up names:** the five render switches `g_particleMeshRenderEnabled_0139b7b0`,
  `g_particleMeshBlendOpEnabled_0139b7ac`, `g_particleMeshBlendModeEnabled_0139b7a8`,
  `g_particleMeshBatchDrawEnabled_0139b7a4`, `g_particleMeshTailDrawEnabled_0139b7a0` (no data name anywhere).
- **Headers:** chunk 2045's overlay plus eleven opt-ins (`hdr/README.md`); chunk 2045's row 0x00BB8EF0 regraded
  under it by the agent: RELOCATION_MATCH. `NAME_FIXES.tsv` has 3 entries.
