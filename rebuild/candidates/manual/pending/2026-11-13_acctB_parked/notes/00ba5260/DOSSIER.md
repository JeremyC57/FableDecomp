# DOSSIER 0x00BA5260 (identity scout, bwf1)

## 1. Retail shape (dis_all.txt / xdis)
- 4056 bytes, __thiscall (ecx = this, stored [ebp-24h] at 0x00BA526C), `ret 8` at 0x00BA5D2C, 0x00BA5E87, 0x00BA6235 (three exits:
  two setup-failure exits + normal end). Args: [ebp+8] = CEngineSubPrimitiveStaticMesh&, [ebp+0Ch] = ERenderPassType.
- EBP frame: `push ebp; mov ebp,esp; sub esp,50h` then push ebx/esi/edi interleaved with the capture-block code (0x00BA526B..0x00BA5284).
  No SEH frame (no fs:0) -> no destructor-bearing EH object; the scoped capture block's RestoreCaptureBlock (0x00A05840) is called
  explicitly at all 3 exits (0x00BA5D21, 0x00BA5E7C, 0x00BA622A).
- Same-TU sibling DisplayMirroredTextures 0x00BA1B00 is an ESP frame, so ebp here is NOT a TU-wide /Oy-; the prior drafts get the
  `push ebp` with no pragma (baseline first diff is at 0x00BA526C, a slot offset), so do not add `#pragma optimize("y",off)`.
- x87 code in the expanded GetAppropriateMipmap (fistp floor site 0x00BA5519..0x00BA5533 = shared engine/GFFloatToLongFloor.h).
- Baseline of $W/differ/00ba5260.cpp under $W/hdr: DIFFER(4060v4056) 65 hunks, 1121v1121 instr (prior: 975 equal, 1003 frame-masked).

## 2. Identity (manifest label is CORRECT)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv, Ego_r 0x00A5C4D0) and FableWin.pdb (naming_stragglers/FableWin.exe.signatures.tsv 0x02E9F5F0,
  1004 bytes debug / 290 instr) both:
  `?RenderMeshPrimitive@CEnginePrimitiveManagerStaticMeshesBase@@QAEXAAVCEngineSubPrimitiveStaticMesh@@W4ERenderPassType@@@Z`
  = public, non-virtual, non-const: `void RenderMeshPrimitive(CEngineSubPrimitiveStaticMesh&, ERenderPassType)`.
- Callers (xref.py): 0x00BA6EA6 in DrawImmediately 0x00BA6DC0 (landed), 0x00BA6FE5 in RenderSubPrimitive 0x00BA6FA0 (landed),
  0x00BA7807 in CEnginePrimitiveManager2DStaticMeshes::RenderSubPrimitive 0x00BA77B0 (landed). Not a vtable slot.
- FLAG: 0x00BA77B0's local declaration is `RenderMeshPrimitive(CEngineSubPrimitiveBase& sub, long pass)` -- disagrees with the PDB
  (should be CEngineSubPrimitiveStaticMesh&, ERenderPassType). Report in NAME_FIXES (signature only, same name).

## 3. Callees (retail address | landed file | PDB name | flag)
- 0x00A05840 CRenderStateManager::RestoreCaptureBlock | 00/a0/CRenderStateManager_RestoreCaptureBlock_00a05840.cpp | ok (via CRenderStateScopedCaptureBlock, StateManager global 0x013CB508)
- 0x00BF1030 CEnginePrimitiveRenderUtil::SetupStencilMask | landed | Ego_r ?SetupStencilMask@CEnginePrimitiveRenderUtil@@QAEXPBVCEngineMeshEffect@@KW4ERenderPassType@@@Z (call at 0x00BA5BCC, ecx=this, arg MeshEffect = [primitive+68h])
- 0x00BA2350 SetupRenderModeShadersAndConstants | NOT landed | Ego_r ?SetupRenderModeShadersAndConstants@CEnginePrimitiveManagerStaticMeshesBase@@AAE_NW4ERenderPassType@@PBVC3DMeshMaterial@@AAVCEngineSubPrimitiveStaticMesh@@AAVC3DTextureStages@@JPAKPA_N@Z
  -> mipmap param is `long` (J), shader flags `unsigned long*`, degenerate `bool*`. Draft passes `unsigned long mipmap_level` (implicit conversion; harmless in codegen, prior tried unsigned/const).
- 0x00BA50A0 SetupStaticBlocks | NOT landed (retail neighbour just before this row, same class) | Ego_r ?SetupStaticBlocks@...@@AAE_NAAVC3DPrimitive2@@AAVCEngineInternalPrimitiveStaticMesh@@AAVC3DTextureStages@@PBVC3DMeshMaterial@@PAV?$CVectorMap@JVCCompositeSurfaceInfo@@V?$CKeyPairCompareLess@JVCCompositeSurfaceInfo@@@@@@JPA_N@Z (mipmap `long`)
- 0x00B9D670 SetupFirstTextures | 00/b9/..._SetupFirstTextures_00b9d670.cpp | ?SetupFirstTextures@...@@SIXAAVC3DPrimitive2@@AAVC3DTextureStages@@@Z ok (static fastcall)
- 0x00B905A0 CEnginePrimitiveManagerOcclusionBase::PrepareOcclusionTest | NOT landed | Ego_r ?PrepareOcclusionTest@CEnginePrimitiveManagerOcclusionBase@@IAEXAAVCEngineInternalPrimitiveOcclusionBase@@W4ERenderPassType@@AAK@Z (handle by ref = [ebp-10h])
- 0x00B8D090 / 0x00B8D0A0 | landed as E2_E2_00b8d090.cpp `?M@C00b8d090@@QAEXXZ` / `?M@C00b8d0a0@@QAEXH@Z` (placeholders; manifest Nop_ret0/Nop_ret4) |
  Ego_r + FableWin: ?StartOcclusionTest@CEngineOcclusionManager@@QAEXXZ / ?EndOcclusionTest@CEngineOcclusionManager@@QAEXK@Z (prior NAME_FIXES.tsv already filed; keep it)
- 0x00A0B140 CRenderManagerCore::SetWorldTransform / 0x009881F0 CShaderRenderManager::SetWorldTransform | landed | ok (via inline CEnginePrimitiveRenderUtil::SetWorldTransform, test [GPShaderRenderManager+395h])
- 0x00A058C0 CRenderStateManager::RealiseRenderState | landed | ok
- PrepareForRender expansion (repo CShaderRenderManager.h line ~164, FABLE_FORCEINLINE, header names are PDB-correct):
  SetVSConstants 0x0098B930, UpdatePixelShader 0x00988A20, UpdatePixelShaderConstants 0x00989BF0, UpdateBoneTransform 0x0098A8E0
  (FLAG landed FILE name "UpdateBoneMatrices"), UpdateWorldSpaceTextureTransform 0x009894B0, UpdateObjectSpaceTextureTransform 0x00989230
  (not landed), UpdateWorldSpaceFogTransform 0x00989640, UpdateObjectSpaceFogTransform 0x00989540, UpdateAmbient 0x00989760,
  UpdateFogColour 0x009897C0, UpdateLightGlobals 0x0098A760, UpdateObjectSpaceLighting 0x00989CB0, UpdateWorldSpaceLighting 0x0098A540,
  UpdateLocalisedSphereMapping 0x00988E80 (not landed), ResetWorldTransform 0x00988290, UpdateCombinedProjectionTransform 0x00988A50,
  UpdateCameraPos 0x00989700 (FLAG landed FILE CShaderRenderManager_UpdateAmbient_00989700.cpp; manifest + Ego_r ?UpdateCameraPos@CShaderRenderManager@@AAEXXZ).
  Call through the header; those wrong file names do not affect this row.
- 0x00B9E5B0 RenderMeshPrimitiveBlocksIndexBuffer | NOT landed (manifest `_global` fastcall) | Ego_r/FableWin ?RenderMeshPrimitiveBlocksIndexBuffer@CEnginePrimitiveManagerStaticMeshesBase@@CIXAAVC3DPrimitive2@@ABVC3DTextureStages@@@Z (private static fastcall)
- 0x00B9E750 RenderMeshPrimitiveBlocksDegenerateTriangles | landed | PDB-correct.
- Vtable calls [+190h] SetStreamSource, [+1A0h] SetIndices, [+104h] SetTexture on the D3D device = inline CRenderManagerCore::FlushRender
  (+0x3CF0 PD3DDevice, +0x3CB8 TextureStagesInUse, +0x3D00 CurrentlySelectedTextures); 0x00BA5F49 [+190h] = inline CShaderRenderManager::SelectVertexBuffer ([+1D4h] D3DDevice).
- Globals: GPRenderManager 0x01436E18, GPShaderRenderManager 0x01436E14, g_pOcclusionManager 0x01436E5C, g_pEngineLightingManager 0x01436E9C,
  g_pPrimitiveRenderer 0x01436E78, g_pEngineCamera 0x01436EA0, capture StateManager 0x013CB508. Run RELOC_IDENTITY before landing.

## 4. Members
- CEngineSubPrimitiveBase (struct_layouts_egor): RenderFlags +4 (ulong), Alpha +8 (uchar), Parent +0xC; CEngineSubPrimitiveMeshBase MeshPrimitive +0x18.
  Accessors PeekRenderFlags / PeekAlpha / GetParent / PeekObjectTransform (Parent+0x70 = CEngineInternalPrimitiveStaticMesh::ObjectTransform, 0x00BA5F52/61) /
  GetScale: hdr-only opt-in FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ACCESSORS (hdr/engine/CEngineSubPrimitiveBase.h, CEngineSubPrimitiveStaticMesh.h; repo=0).
- CEngineInternalPrimitiveBase AlphaRef +0x28 (uchar); primitive MeshEffect +0x68 (0x00BA5BC6); RenderInfo.Mesh +0xA4 (PeekMesh, hdr-only
  FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH_PEEKMESH); Morph.CompositeSurfaces +0xBC (CEngineInternalPrimitiveMorphedStaticMesh, type 10).
- C3DPrimitive2: Ego_r layout has VertexBuffer +0x34, but retail reads [mesh_primitive+30h] (0x00BA5F2A) -> TLC layout is 4 smaller before it;
  hdr/engine/C3DPrimitive2.h (STALE, FABLE_RETAIL_C3DPRIMITIVE2_RENDER, hdr-only) is what makes it right. Re-base by hand onto the current repo header.
- C3DBoundingSphere {Center, Radius}, C3DTextureStages {DiffuseTextureStage, BumpTextureStage, UseBlankTextures} (12 bytes; [ebp-40h]); ctor -1,-1,false = hdr-only FABLE_RETAIL_C3DTEXTURESTAGES_CTOR.
- CRenderStateManager DeviceState setters (SetSpecularEnable/SetFogEnable/SetPolyCullMode/SetAlphaRef, FABLE_FORCEINLINE) are in REPO
  CRenderStateManager.h; hdr copy is STALE (re-base).
- CEngineLightingManager::RestoreDefaultPrimitiveParameters is REPO FABLE_FORCEINLINE (+0xCE, +0xE0..E2, +0xE4, +0xE8).
- Opt-ins the draft uses that exist ONLY in $W/hdr (repo=0): C3DPRIMITIVE2_RENDER, C3DTEXTURESTAGES_CTOR, C3DVECTOR_MAGNITUDE_NAMED_SQUARED,
  C3DVECTOR_NAMED_RESULT_SUB, CENGINEINTERNALPRIMITIVESTATICMESH_PEEKMESH, CENGINEOCCLUSIONMANAGER_OCCLUSIONTEST,
  CENGINEPRIMITIVEMANAGERMESHBASE_GETAPPROPRIATEMIPMAP, ..OCCLUSIONBASE_PREPAREOCCLUSIONTEST, ..STATICMESHESBASE_RENDERMESHPRIMITIVE,
  CENGINEPRIMITIVERENDERERSHAREDDATA_MIPMAP, CENGINEPRIMITIVERENDERUTIL_MESHRENDER, CENGINESUBPRIMITIVEBASE_ACCESSORS, CGRAPHICINFO_GETWIDTH,
  CIVCOUNTEDPOINTER_GET. All others exist in repo headers.
- Note: $W/hdr/engine/CRenderManagerCore.h carries two blocks the repo header no longer has (VOLUMETEXTURE_INLINE, DRAWINDEXED_TEXTURES) -- check before hand-back.
- Local models: none of the class layouts are modelled locally (all via headers); the draft defines inline BODIES in the row (PeekGraphic,
  IsFixedFunctionVertexMode, SelectVertexBuffer, FlushRender, GetMipmapSizeMultiplier, SetWorldTransform, GetAppropriateMipmap,
  RenderMeshPrimitiveBlocks, SetupAlphaBlendingStatesFromRenderFlags, GFGetTopBitInByte). Same idiom as landed sibling DisplayMirroredTextures 0x00BA1B00
  (defines `FABLE_FORCEINLINE void CRenderManagerCore::FlushRender()` with the identical text in-row).

## 5. Visibility
- FableWin-only inlines (no retail copy): GetAppropriateMipmap (FableWin 269 b), SetupAlphaBlendingStatesFromRenderFlags (268 b), RenderMeshPrimitiveBlocks
  (48 b), FlushRender (FableWin 0x02CABE80), PrepareForRender, SetWorldTransform (util), SelectVertexBuffer. All expanded in retail -> forceinline is required
  (prior grades 6/23: plain inline leaves two called).
- Called out of line though same class / same TU: SetupStaticBlocks 0x00BA50A0 (immediately before), SetupRenderModeShadersAndConstants 0x00BA2350,
  SetupFirstTextures, RenderMeshPrimitiveBlocksIndexBuffer / DegenerateTriangles -- keep them declaration-only (no bodies in the TU).
- RestoreCaptureBlock 0x00A05840 called (not expanded); StartCaptureBlock expanded (0x00BA5266..0x00BA5294).
- No STLport helper calls (CVectorMap only address-taken). PeekGraphic is a vector index with bounds test (expanded).

## 6. Siblings
- DisplayMirroredTextures 0x00BA1B00 (same class/TU, MATCH): FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS, CENGINEPRIMITIVERENDERUTIL_SETTEXTUREFACTOR,
  CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS, CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_DISPLAYMIRRORED, CENGINEPRIMITIVERENDERER; in-row forceinline helper bodies; no pragma.
- SetupStaticBlocksNoMorph 0x00BA48F0: STATICBLOCKS_TEXTURE_HELPERS, STATICBLOCKS_ORIGINAL_ACCESSORS, C3DPRIMITIVE2_HIERARCHY, CIVCOUNTEDPOINTER_RESET_ADOPT,
  CENGINEPRIMITIVEMANAGER{MESHBASE,STATICMESHESBASE}_STATICBLOCKS; empty `// Flags:`.
- RenderMeshPrimitiveBlocksDegenerateTriangles 0x00B9E750, SetupFirstTextures 0x00B9D670 (static fastcall), DrawImmediately 0x00BA6DC0, RenderSubPrimitive 0x00BA6FA0.
- Family: /opt/fable_lane/w22/agent2059 ParticleMeshGroup::RenderSubPrimitive 0x00BC25F0 (matched; alpha/blend helper idioms), agent2048 RenderClouds/RenderSunFlare.
- None of the siblings use #pragma optimize or extra Flags.

## 7. Draft model vs disassembly
- Control flow matches retail: capture block, spec/fog, early return on !DiffuseMapBankIndex, mipmap, alpha states, `flags&0x80` stencil (0x00BA5BB4 `test al,al; jns`),
  failure exits Restore-lighting THEN FlushRender THEN RestoreCaptureBlock (0x00BA5C00..0x00BA5D21), normal tail FlushRender THEN Restore-lighting (0x00BA6117..0x00BA6224) -- draft order right.
- Retail arg order at 0x00BA5BD1: mipmap reloaded from [ebp-14h]; stages [ebp-40h]; shader_flags [ebp-1Ch]; degenerate flag [ebp-4] zeroed right before the call (0x00BA5BEF).
- Remaining residue (prior NOTES): register allocation only -- mipmap piece coloured into ebx in ours; retail keeps ebx = mesh_primitive 0x00BA5D35..0x00BA5F2A,
  hence all frame-slot shifts (this at -24h vs ours -20h etc.). Block scoping/occlusion-handle placement FITTED, not proven. GetAppropriateMipmap text is 75 /Od instr vs FableWin 81 (fwsize): the
  six missing instructions = wrong text there; fitting it is the most promising untried lever.
- Header debt: re-base STALE C3DPrimitive2.h / CIVCountedPointer.h / CRenderStateManager.h; ASM_RULES per-site table for GFFloatToLongFloor 0x00BA5519 not built.
