# Hand chunk 2060 (CEnginePrimitiveManagerStaticMeshesBase): 0 of 1 byte-exact (row handed over in place)

## Row 0x00BA5260 CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitive -- DIFFER(4060v4056), 1121v1121 instructions
Best source: differ/00ba5260.cpp (= cand/c12E.cpp + header), test differ/00ba5260.test.cpp = test/00ba5260.cpp (PASS g++ and VC7.1).
md.py: 975 equal (1003 with frame offsets masked) of 1121. 62 full-row grades (grades.log), plus free packer / c2prio traces.

### Structure (reusable for the animated twin 0x00BD3070, agent4010)
1. `CRenderStateScopedCaptureBlock` (default repo text) -> StartCaptureBlock at entry, RestoreCaptureBlock 0x00A05840 at every exit.
2. `GPRenderManager->SetSpecularEnable(true); SetFogEnable(true);` (DeviceState +0x1CC / +0x12C = core +0x29E4 / +0x2944).
3. Locals: `mesh_primitive = sub.MeshPrimitive`, `primitive = sub.GetParent()`, `mesh = primitive->PeekMesh()` (RenderInfo.Mesh +0xA4),
   `material = mesh.GetMaterial(mesh_primitive->GetMaterialIndex())`, composite surfaces = &Morph.CompositeSurfaces (+0xBC) when
   GetType() == 10, `graphic_bank = primitive->MeshInfo->PeekTextureBank()`; `if (!material->GetDiffuseMapBankIndex()) return;`.
4. `mipmap_level = CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap(camera->PeekCameraPos(), sub.PeekObjectTransform(), *mesh_primitive,
   sub.GetScale(), material, graphic_bank)` -- FableWin.pdb inline (81 debug instr). Body that reaches retail's x87 code:
   `C3DBoundingSphere sphere = prim.GetBoundingSphere(); transform.TransformPoint(sphere.Center);` (IN PLACE on the sphere copy: this is
   what gives retail's int-copied Z / radius and memory xx / yy), `unsigned long mipmap = 0;` BEFORE the distance (else `fstp; fld`),
   `float distance = ((camera - sphere.Center).GetMagnitude() - sphere.GetRadius() * scale) * prim.GetAverageTextureStretch();`,
   `if (distance > DELTA_NOTIONAL_ZERO) { index > 0 -> PeekGraphic(index)->GetWidth() / (GetMipmapSizeMultiplier() / distance * scale),
   GFFloatToLongFloor, GFLimit(size, 1, 255), GFGetTopBitInByte }`. C3DVector operator- named result, GetMagnitude named X Z Y sum.
5. Inner block A: `C3DTextureStages texture_stages;` (FableWin ctor: -1, -1, false), render flags, `shader_flags = 0`, then the
   FableWin.pdb inline `SetupAlphaBlendingStatesFromRenderFlags(render_flags, pass, sub.PeekAlpha(), sub.GetParent()->AlphaRef)`
   (body size-PROVEN 71i/268b: pass 1 -> ZWrite on, blend off, test on, ref, ColourWrite off; else flags&2 test on + ref*alpha>>8 /
   off + 0; flags&1 -> ZWrite off, blend on, SRC_ALPHA / INV_SRC_ALPHA, test on; else ZWrite on, blend off). The AlphaRef argument fed
   from the raw member makes the byte parameter 4-aligned ([ebp-4], r3013 lesson). `if (render_flags & 0x80) SetupStencilMask(MeshEffect.Get(), ...)`.
6. Inner block B: `bool render_degenerate = false; if (!SetupRenderModeShadersAndConstants(...)) { lighting->RestoreDefaultPrimitiveParameters();
   GPRenderManager->FlushRender(); return; } bool used_blank; if (!SetupStaticBlocks(...)) { same; return; }`
   then SetupFirstTextures, SetPolyCullMode(CCW), PrepareOcclusionTest(*primitive, pass, occlusion_handle), StartOcclusionTest when
   handle != -1, SelectVertexBuffer(*mesh_primitive->GetVertexBuffer()), CEnginePrimitiveRenderUtil::SetWorldTransform (FableWin inline:
   fixed-function -> render core, else shader manager), RealiseRenderState, PrepareForRender(shader_flags), the inline
   RenderMeshPrimitiveBlocks(IndexBuffer; Degenerate when the flag), EndOcclusionTest, FlushRender, RestoreDefaultPrimitiveParameters.
7. `occlusion_handle` is declared at function scope (top). Blocks A / B and that placement are FITTED by frame-packer traces (pk.sh):
   c2 treats an address-taken local as live over its whole scope; this nesting is the only one found that reproduces retail's slot
   sharing (frame 0x50; all slots except the two mipmap-temporary slots now map 1:1, fmap.py).

### Exact remaining difference (retail addresses)
- Root: c2 colours the mipmap level's call1->call2 piece into ebx (c2prio: piece #3 of mipmap, prio 16, refs 2). Retail reloads
  [ebp-14h] at both calls (0x00BA5BD1 `mov ecx`, 0x00BA5D2F `mov edx`) and gives ebx to the mesh primitive from 0x00BA5D35 to 0x00BA5F2A
  (`mov ecx,ebx` SetupFirstTextures 0x00BA5E8D, `mov eax,[ebx+30h]` 0x00BA5F2A); ours reloads mesh_primitive there.
- Consequences: mipmap gets one frame reference less -> packer groups {xx, distance, shader_flags, half} and {mipmap, yy, one}
  (retail {xx, mipmap, half} at -14h and {yy, distance, one, shader_flags} at -1Ch; sim2.py `--ref 10@0 --rewalk` reproduces retail's
  grouping), so: mipmap zero store at 0x00BA544B (ours 3 instr earlier), 0.5 / 1.0 stores swapped (0x00BA54FA / 0x00BA550A), primitive /
  this / scale slots shifted by 4 (retail -18h / -24h / -20h), cull value kept in ebx instead of spilled to [ebp-34h] (0x00BA5EAE /
  0x00BA5EE2), PrepareForRender register choice (0x00BA5F94..0x00BA5FEB, ours spills edi), `xor edi,edi` / stage byte / shader-flag
  store order at 0x00BA5582..0x00BA558F, alpha-ref multiply registers 0x00BA57C1..0x00BA57CB, and ecx/edx rotation in the two failure
  exits (0x00BA5C4F..0x00BA5D0F, 0x00BA5DA6..0x00BA5E6A).
- Tried without effect on the ebx piece: const / unsigned mipmap_level (unsigned: +2), reference locals for mesh_primitive / primitive,
  named bool results for the two setup calls, declaration order of stages / flags / render flags (4 orders), block boundaries
  (stencil inside B, used_blank / shader_flags in B), FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED, early-return
  GetAppropriateMipmap (worse), `sphere.GetCenter()` by value in the subtraction (worse, 908; /Od 78i), named screen_size / texture_size / unsigned width (/Od 75-76i, no release change).
- Next ideas: (1) find what raises the mesh-primitive call2..VB piece above the mipmap piece (c2prio_b.py --blocks: K(b) of the cull
  SetRenderState blocks and of the call2 block; retail's piece needs prio > 16); (2) r3009 arm swaps on the two failure exits and on the
  `occlusion_handle != -1` tests; (3) a natural spelling for the block scoping (e.g. a code-free constant-true `if`), which changes the
  CFG the split pass sees; (4) fit GetAppropriateMipmap to FableWin's 81 instructions (ours 75: the six missing /Od instructions are a
  named temporary or two-statement form somewhere in the distance / ratio code) -- it changes IL counts and candidate keys.

### Names (two sources each)
- Row: Ego_r.pdb + FableWin.pdb ?RenderMeshPrimitive@CEnginePrimitiveManagerStaticMeshesBase@@QAEXAAVCEngineSubPrimitiveStaticMesh@@W4ERenderPassType@@@Z.
- Callees: manifest + Ego_r names for SetupRenderModeShadersAndConstants / SetupStaticBlocks / SetupFirstTextures / PrepareOcclusionTest /
  SetupStencilMask; StartOcclusionTest / EndOcclusionTest = Ego_r.pdb names with Ego_r body sizes 1 / 3 bytes = retail's empty `ret` /
  `ret 4` at 0x00B8D090 / 0x00B8D0A0 (landed as BeginQuery / EndQuery: NAME_FIXES.tsv).
- Inlines: FableWin.pdb names (GetAppropriateMipmap, SetupAlphaBlendingStatesFromRenderFlags, GetMipmapSizeMultiplier, SetWorldTransform,
  GFGetTopBitInByte, PeekRenderFlags, PeekAlpha, GetParent, PeekObjectTransform, GetScale, PeekMesh, GetBoundingSphere,
  GetAverageTextureStretch, GetVertexBuffer, GetWidth, C3DTextureStages ctor).
- Descriptive: none. Globals as the landed rows spell them (NEngineInternal::GPRenderManager / GPShaderRenderManager, g_pEngineCamera,
  g_pPrimitiveRenderer, g_pEngineLightingManager, g_pOcclusionManager, g_highestSetBitTable, DELTA_NOTIONAL_ZERO) -- run vland's
  RELOC_IDENTITY check before landing (0x01436E78 / 0x01436E9C / 0x01436E5C have several bound spellings).

### Header debt
- ASM_RULES per-site attempts table (30 plain + 10 intrinsic) for the GFFloatToLongFloor site 0x00BA5519 not built yet.
- C3DTextureStages.h / CEngineSubPrimitiveBase.h etc.: new opt-ins only (hdr/README.md).
