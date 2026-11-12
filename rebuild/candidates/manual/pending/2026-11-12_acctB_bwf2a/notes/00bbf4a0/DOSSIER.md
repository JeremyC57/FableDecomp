# DOSSIER 0x00BBF4A0 -- CEnginePrimitiveManagerParticleSpriteGroup::RenderSubPrimitive (4169 bytes)

## 1. Identity (PROVEN, 3 sources)
- Leaf: `?RenderSubPrimitive@CEnginePrimitiveManagerParticleSpriteGroup@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z`
  = `virtual void RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass)` (UAE: public virtual thiscall, void, non-const).
  - rtti-vtables.tsv: vtable 0x012A6140 slot 5 -> 0x00BBF4A0 (xref: only reference is .rdata 0x012A6154).
  - Ego_r.pdb order (egor_pdb_names.tsv 29222..29227): RenderPrimitive 0x00A747C0, BuildBuffers 0x00A747D0, **RenderSubPrimitive 0x00A74990**,
    RenderSingleSprite 0x00A759A0, GetSupportedTypes, ctor ... = retail 0x00BBF370, 0x00BBF380, **0x00BBF4A0**, 0x00BC04F0, 0x00BC13A0, 0x00BC14E0 (same order).
  - FableWin.pdb: 0x02EB49B0 RenderSubPrimitive (2728b/697i debug), followed by COMDATs ApplyBillboardPointLightVertexShader(K) 77b/26i,
    ApplyCrossedSpritePointLightVertexShader(K) 77b/26i, Apply3DFaceMePointLightVertexShader(K) 77b/26i, ApplyBillboardDisplacementVertexShader() 22b/10i,
    ApplyCrossedSpriteDisplacementVertexShader() 22b/10i, Apply3DFaceMeDisplacementVertexShader() 22b/10i (all `AAE` = private).
- Manifest label is the full mangled name in the `name` column (functions.tsv 42636) -- correct identity, just unclean formatting.
- It does NOT call RenderSingleSprite (0x00BC04F0) -- that is a separate retail function (called from CEnginePrimitiveManagerSprites::RenderSubPrimitive 0x00BAD2E0).

## 2. Retail shape (xdis / dis_all.txt)
- thiscall, `ret 8` (two exits: early 0x00BBF577, final 0x00BC04E6). No frame pointer (esp-relative). **No EH frame** (no fs:0) although a
  CGraphicFrame local with an inline destructor exists -> TU compiled without /EH, exactly like sibling 0x00BC25F0 (2059, "no EH frame") and
  the landed ctor 0x00BC14E0 (`// Flags: none`). No /GS. Speed-optimised (/O2-style; not "s").
- Prologue: `sub esp,38h ; mov eax,[0x0139B6E0] ; push edi ; xor edi,edi ; cmp ; mov [esp+4],ecx` (this saved at F+0) ;
  `push ebx ; push esi` after the global test; `push ebp` only at 0x00BBF5E0 (after the early-out). Pops: esi, ebx, edi (ebp popped mid-tail at 0x00BC03FA).
- Two separate inline expansions of ~CGraphicFrame (early return 0x00BBF51D..0x00BBF56E, end 0x00BC0490..0x00BC04DD).
- Uses the `sub` / `pass` argument slots as scratch (E+4: `additive` bool, later the shader-manager pointer and the 3-byte theme-channel temp;
  E+8: float `zOffset`, then the list iterator, then fild temps).

## 3. Callees (all resolved; landed file / name / PDB)
| addr | call site(s) | name (PDB spelling) | landed row |
|---|---|---|---|
| 0x009FF8D0 | 0xBBF514, 0xBC0030 | `?GetFrame@CGraphicInfo@@QBE_NAAVCGraphicFrame@@KKK@Z` bool GetFrame(CGraphicFrame&, ulong, ulong, ulong) const | 00/9f CGraphicInfo_GetFrame_009ff8d0.cpp |
| 0x009FCBA0 | 0xBBF53B, 0xBC04A7 | CGraphicFrameData::Uninitialise() (inside inline ~CGraphicFrameData) | 00/9f CGraphicFrameData_Uninitialise_009fcba0.cpp |
| 0x00BFE9BC | x4 | operator delete | global_operatordelete_00bfe9bc.cpp |
| 0x00988020 | 0xBBF7F9, 0xBBFA04, 0xBBFDFD, 0xBBFE34 | CShaderRenderManager::ApplyVertexShader(const CVertexShader&, bool) (via CEnginePrimitiveRenderUtil::ApplyVertexShader, pushes `0`) | CShaderRenderManager_ApplyVertexShader_00988020.cpp |
| 0x00988140 | 0xBBF808, 0xBBFA13 | CShaderRenderManager::ApplyPixelShader(const CPixelShader&) | ..._ApplyPixelShader_00988140.cpp |
| 0x00BBC0B0 | 0xBBFCEB | CEnginePrimitiveManagerParticleGroup::SetUpPointLights() -> ulong | 00/bb ..._SetUpPointLights_00bbc0b0.cpp |
| 0x00BBCC70 | 0xBBFE49 | CEnginePrimitiveManagerParticleGroup::SetupPixelShader(ulong, CPixelShader*, CPixelShader*) | 00/bb ..._SetupPixelShader_00bbcc70.cpp |
| 0x00B46C80 | 0xBBFD1E | CEngineLightingManager::UpdateGlobalLightingStates() | (not landed) |
| 0x009896D0 | 0xBBFFD1, 0xBC0213, 0xBC0318 | CShaderRenderManager::SetUserVertexShaderConstants(ulong, long, float*) | ..._SetUserVertexShaderConstants_009896d0.cpp |
| 0x0098B930 | 0xBC0260, 0xBC0369 | SetVSConstants (PrepareForRender expansion) | ..._SetVSConstants_0098b930.cpp |
| 0x00988A20 / 0x00989BF0 / 0x00989760 / 0x009897C0 / 0x0098A760 / 0x0098A540 / 0x00988A50 / 0x00989700 | PrepareForRender x2 | UpdatePixelShader / UpdatePixelShaderConstants / UpdateAmbient / UpdateFogColour / UpdateLightGlobals / UpdateWorldSpaceLighting / UpdateCombinedProjectionTransform / **UpdateCameraPos** | landed; NOTE file `CShaderRenderManager_UpdateAmbient_00989700.cpp` is UpdateCameraPos (manifest + header CShaderRenderManager.h:246 say UpdateCameraPos; filename is stale) |
| 0x00A0AC60 | 0xBC02DE, 0xBC03E8 | CRenderManagerCore::DrawCurrentBuffersAsIndexedTris(ulong,ulong,ulong,ulong) | 00/a0 ..._a0ac60.cpp |
| [dev+0x104] | 0xBBF5AB | IDirect3DDevice9::SetTexture (slot 65) -- inside AttachTextureToStage | |
| [dev+0x190] | 0xBBFE88 | IDirect3DDevice9::SetStreamSource (slot 100) -- CRenderManagerCore::SelectVertexBuffer | |
| [dev+0x1A0] | 0xBBFEB5 | IDirect3DDevice9::SetIndices (slot 104) -- CRenderManagerCore::SelectIndexBuffer | |
No spelling conflicts with the PDB among called rows (UpdateCameraPos file name aside).

## 4. Globals / data
- 0x0139B6E0 dword render switch (tested first; no PDB/label name anywhere: descriptive, e.g. `g_particleSpriteRenderEnabled_0139b6e0`, twin of 2059's `g_particleMeshRenderEnabled_0139b7b0`).
- 0x01436E98 `NEngineInternal::GPResourceManager` (CEngineResourceManager*; landed spelling in 0x00BBCC70 / 0x00B2D9F0 / 0x00BCFB00; other rows use g_pEngine_01436e98 etc.). GraphicBanks[0] +0x954 via inline `PeekGraphicBank(EGraphicBankNo 0)` (CEngineResourceManager.h:56).
- 0x01436E18 NEngineInternal::GPRenderManager; 0x01436E14 NEngineInternal::GPShaderRenderManager; 0x01436E84 g_pEnginePrimitiveTypeInfo (PrimitiveManagers +0x10, so +0x70 = GetPrimitiveManager(24)); 0x01436E38 g_pDisplacementRenderer_01436e38 (2059 spelling; also landed as g_pEngineSettings_01436e38) -> Enable3DDisplacement +0xEF (Ego_r member; FableWin inline `?GetEnable3DDisplacement@CEngineScreenEffectDisplacementRenderer@@QBE_NXZ`, 20b); 0x01436E9C lighting manager (header comment: `NEngineInternal::GPLightingManager`; 2059 used g_pEngineLightingManager); 0x01436EA0 g_pEngineCamera.
- Render-state tables (CRenderStateManager.h statics): D3DBlendOpModeTable 0x01396ED0 [0]/[1]=0x1396ED4/[2]=0x1396ED8; D3DAlphaBlendModeTable 0x01396F68: [1]=0x1396F6C ONE, [3]=0x1396F74 INV_SRC_COLOR, [4]=0x1396F78 SRC_ALPHA, [5]=0x1396F7C INV_SRC_ALPHA; D3DPolyCullModeTable 0x01396FB0 [2]=0x1396FB8 (CULL_NONE); D3DAlphaFuncModeTable 0x01396DD8 [4]=0x1396DE8 (GREATER).
- Floats: 0x0122DEDC 0.0f, 0x0122DED8 1.0f, 0x0122DEE0 -1.0f, 0x01231724 1/255 (CRGBFloatColour), 0x01230010 1024.0f, 0x0122F59C 0.5f, 0x0129C81C 1/32767, GFastCosTable 0x013CD550.

## 5. Members (retail offsets)
- this (CEnginePrimitiveManagerParticleSpriteGroup, header engine/CEnginePrimitiveManagerParticleSpriteGroup.h, opt-in FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLESPRITEGROUP, typed already):
  VertexShadersBillboardSpritePointLights +0x0C, VertexShadersCrossedSpritePointLights +0x18, VertexShaders3DFaceMeSpritePointLights +0x24 (CArray<CVertexShader>, begin +0/end +4, elem 8),
  *Displacement +0x30 (Billboard) / +0x38 (Crossed) / +0x40 (3DFaceMe), PixelShader +0x48, PixelShaderStipple +0x50, PixelShaderDisplacement +0x58,
  VertexBuffer +0x60 (CVertexBufferWin32: PVertexBuffer +4, Stride byte +0x10), IndexBuffer +0x94 (PD3DIndexBuffer +4), ConstantBlock[40] +0xA8.
  MISSING in header: Apply3DFaceMePointLightVertexShader(unsigned long) and Apply3DFaceMeDisplacementVertexShader(); the four declared Apply* have no bodies. Add inline bodies in the overlay (text of 2059's ApplyMeshPointLightVertexShader / ApplyDisplacementVertexShader; FableWin sizes identical 77b/22b).
- group = sub.GetParentBase() (sub +0x0C; FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_PARENTBASE -- only in 2059's overlay, NOT in repo) as **CEngineInternalPrimitiveParticleSpriteGroup** (NO shared header exists; create overlay header like 2059's CEngineInternalPrimitiveParticleMeshGroup.h): base CEngineInternalPrimitiveParticleGroup (sizeof 0xAC, Primitives std::list<CEnginePrimitiveHandle> +0x48), own members Ego_r minus 0x14: SpriteBankIndex ulong +0xAC, Flags ushort +0xB0, BlendMode EEnginePrimitiveBlendMode +0xB4, BlendOp EEnginePrimitiveBlendOp +0xB8 (sizeof 0xBC; verify with CreateInternalPrimitive 0x00BC1420 alloc size).
  Flags bits used: 0x01 3DFaceMe, 0x40 Crossed (cull none), 0x80 point lights, 0x02/0x04/0x08/0x10 offset sign selectors, 0x20 constant 3.Z = 1.
- sprite = list node value's data (node +0xC = CEnginePrimitiveHandle data pointer) as **CEngineInternalPrimitiveParticleSprite** (no header; Ego_r = retail offsets): WorldPos C3DVector +0x48, Size C2DVector +0x54, Angle float +0x5C, Colour CRGBColour +0x60 (bytes B,G,R,A), Frame ulong +0x64.
- CGraphicInfo (engine/CGraphicInfo.h, 0x2C, FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS): Header words read +0, +2, +6, +8 (CGraphicDataBank::CGraphicHeader -- check its member names; constant 1.Z = (float)w[+6] / (float)w[+0], 1.W = w[+8] / w[+2], fild/fidiv on ints).
  CGraphicDataBank::GraphicList CArray<CGraphicInfo> at +0x1E0; inline PeekGraphic(index) with bounds check `index < size ? &list[index] : 0` (size via /0x2C magic 0x2E8BA2E9).
- CGraphicFrameData (CGraphicFrame.h, STATICBLOCKS branch): ReferenceCount +0, PTexture {Data +8, Info +0xC}, TLX +0x10, TLY +0x12, flag byte +0x1A (IsInitialised bit 0x10). The repo header has no ~CGraphicFrame / ~CGraphicFrameData: model text = landed 0x009DCA80 Draw2DGraphic (local) / 0x00B2ED60 -- put it in the overlay as FABLE_FORCEINLINE dtors.
- CRenderManagerCore/CRenderStateManager (FABLE_RETAIL_CRENDERMANAGERCORE + _FORCEINLINE_HELPERS): DeviceState +0x2818 => BlendOpMode +0x2C3C, SourceAlphaBlendMode +0x291C, DestAlphaBlendMode +0x2930, PolyCullMode +0x2890, AlphaTestEnable +0x28CC, AlphaRef +0x28E0, AlphaTestFunc +0x28A4; TextureStagesInUse +0x3CB8, PD3DDevice +0x3CF0, PSelectedIndexBuffer +0x3CF4, CurrentlySelectedTextures +0x3D00.
- CShaderRenderManager: UpdateFlags +0x3D8, CurrentVSConstantLayout +4 (PrepareForRender inline in repo header).
- CEngineLightingManager (FABLE_RETAIL_CENGINELIGHTINGMANAGER(_MEMBERS)): FadedShadowDiffuseFactorEnabled +0xCE, CurrentThemeChannel +0xE0..E2, StateUpdateFlags +0xE4, SelfIlluminationAmbientPercentage +0xE8; inline SetSelfIlluminationAmbient(long), RestoreDefaultPrimitiveParameters() in repo header.
- CEngineCamera +0x144 InverseNoScaleViewTransform (CMatrix3x4, opaque pad in repo header line 100; needs overlay member): reads +0x144,+0x150 / +0x148,+0x154 / +0x14C,+0x158.

## 6. Visibility / helper bodies
- All helpers expanded (no out-of-line copies called): PeekGraphicBank, PeekGraphic, GetParentBase, ~CGraphicFrame, AttachTextureToStage (FORCEINLINE_HELPERS opt-in), SetRenderState family, SetupBlendOpState / SetupAlphaBlendingStatesFromBlendMode (2059 overlay FORCEINLINE texts), GetEnable3DDisplacement, the six Apply* sprite helpers, SetSelfIlluminationAmbient, SelectVertexBuffer / SelectIndexBuffer (CRenderManagerCore, null-tested), GFFastCos + GFFastSin, CRGBFloatColour(const CRGBColour&), PrepareForRender(0x15) x2, RestoreDefaultPrimitiveParameters.
- PrepareForRender flag = **0x15** (bits seen: SetVSConstants, &1 UpdatePixelShader, &2 consts, &4 Ambient, &0x20000 Fog, &0x400 LightGlobals, &0x200 WorldSpaceLighting(=flags&0x10), &0x80 CombinedProjection, &8 CameraPos(=flags&4); no ResetWorldTransform (flags&1)). 2059 used 0x11.
- GFFastSin is not in a shared header (only GFFastCos.h); landed text (identical): rebuild/src/compiled/00/a1/global_GFFastSin_00a147e0.cpp and C2DVector_GetRandomUnitVector_00a13eb0.cpp (FORCEINLINE copies). Two fistp sites go through engine/GFFloatToLongNear.h: row needs `// Retail-asm:` lines for 0x00BC010D `fistp dword ptr [esp+24h]` and 0x00BC0159 `fistp dword ptr [esp+2Ch]` (ASM_RULES per-site table).
- Retail CSEs `angle*1024` once (0xBC00E3, fst to two slots) for cos and sin.

## 7. Siblings / idioms to copy
- /opt/fable_lane/w22/agent2059/src/00bc25f0.cpp (ParticleMeshGroup::RenderSubPrimitive, RELOCATION_MATCH): SAME skeleton (render switch, GetParentBase, group_manager = GFDebugPDynamicCast(GetPrimitiveManager(24)), pass 6/7/else, SetupBlendOpState/SetupAlphaBlendingStatesFromBlendMode FORCEINLINE with constant args in pass 6 (lesson: this fixed the SetRenderState bookmark-test operand order), SetupPixelShader, ConstantBlock batching, PrepareForRender, DrawCurrentBuffersAsIndexedTris). Copy its whole hdr/ overlay into $W/hdr and its #define list.
- 0x00BC14E0 ctor (landed, `// Flags: none`, FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLESPRITEGROUP / CENGINERESOURCEMANAGER / CCHARSTRING / CARRAY).
- 0x00BBCC70 SetupPixelShader / 0x00BBC0B0 SetUpPointLights (landed, same particle TU family, NEngineInternal::GPResourceManager spelling).
- 0x009DCA80 CRenderManager2D::Draw2DGraphic (GetFrame + inline ~CGraphicFrame text that matched).
- 0x00BB8EF0 (chunk 2045, regraded under 2059 overlay) for SetupBlendOpState/AlphaBlending idioms.
