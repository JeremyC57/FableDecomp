# DOSSIER 0x00B33B50 -- CEnginePrimitiveRenderer::CEnginePrimitiveRenderer()  (8858 bytes)

Prior scouting: none. $W/prior/ is empty, agent4027 has no hdr/probe/cand for this row, no other dossier names it
(only bwf2/00bb5040 mentions it as the caller of the cloth renderer ctor). Everything below was scouted from scratch.

## 1. Retail shape (xdis / $W/dis_all.txt)
- `push ebp; mov ebp,esp; and esp,-8; sub esp,18h; push ebx; push ebp; push esi; xor ebx,ebx; push edi; mov esi,ecx`
  -> 8-aligned ebp frame (VC7.1 gives it for 8-byte class temporaries: CPixelShader / CCountedPointer temps; see landed
  0x007AA6B0 "comes out of VC7.1 for this body as written"). ebp is then ALSO pushed and used as a general register.
- NO SEH frame (no fs:0) although `new X(CCharString(...))` temporaries exist -> TU built WITHOUT /EHsc. Every sibling
  row of this TU has no `// Flags:` line or `Flags: none` (0x00B36EC0, 0x00B33520, 0x00B33AE0) -> compile with no flags.
- No /GS cookie. Epilogue `pop edi; mov [0x01436E78],esi; mov eax,esi; pop esi; pop ebp; pop ebx; mov esp,ebp; pop ebp; ret`
  -> __thiscall, `ret` (no args), returns this.
- ebx = 0 for the whole body, esi = this, edi = &SceneList (+0x338) during the manager `new`s.
- Conditional-temporary flag dword at [esp+14h] (prologue stores ebx there) -- the VC7.1 no-EH tracking of the
  CCharString temporary built inside `new CEngineScene(..., CCharString("MainScene"))` (bit 1, cleared before its dtor)
  and the RepMeshScene one (bit 2, not cleared: last use).

## 2. Identity
- Name: `CEnginePrimitiveRenderer::CEnginePrimitiveRenderer()`, leaf `??0CEnginePrimitiveRenderer@@QAE@XZ`.
  Sources: (a) rebuild/include/engine/CEnginePrimitiveRenderer.h line 32 (`CEnginePrimitiveRenderer(); // 0x00B33B50
  ??0CEnginePrimitiveRenderer@@QAE@XZ`, Ego_r.pdb / FableWin.pdb); (b) landed AddPrimitiveManager 0x00B33AE0 header:
  Ego_r donor ??0 at 0x009D4550 directly after AddPrimitiveManager, retail same pair; (c) vtables stored = RTTI
  CEnginePrimitiveRenderer 0x012A1380 (main, slot0 = ??_G 0x00B38D10) and 0x012A1348 (CEngineComponent part at +0x268)
  (rebuild/manifest/rtti-vtables.tsv); (d) sole caller 0x00B2670C in landed CEngine::Initialise 0x00B26360:
  `AddEngineComponent(new CEnginePrimitiveRenderer(), g_pEnginePrimitiveRenderer, 0);` (0x350 bytes).
  The manifest label is CORRECT.
- Class: `CEnginePrimitiveRenderer : public CEnginePrimitiveRendererSharedData (+0), public CEngineComponent (+0x268)`,
  sizeof 0x350 (header retail branch FABLE_RETAIL_CENGINEPRIMITIVERENDERER).
- egor_pdb_names.tsv / egor_signatures.tsv addresses are Ego_r addresses (no hit for 00b33b50 -- expected).

## 3. Callees (retail target -> landed row -> name; NAME issues flagged)
| target | n | landed row | name to use | note |
|---|---|---|---|---|
| 0x00B36CE0 | 1 | b3/CEnginePrimitiveRendererSharedData_CEnginePrimitiveRendererSharedData_00b36ce0.cpp | CEnginePrimitiveRendererSharedData::CEnginePrimitiveRendererSharedData() | base ctor, called (COMDAT after us) |
| 0x00B59710 | 1 | b5/CEngineComponent_CEngineComponent_00b59710.cpp | CEngineComponent::CEngineComponent() | 2nd base, ecx=this+0x268 |
| 0x00BFEA1A | 24 | global_operatornew_00bfea1a.cpp | operator new | |
| 0x00BFE9BC | 21 | global_operatordelete_00bfe9bc.cpp | operator delete | CCPPointerInfo free in temp dtor |
| 0x0099EBF0 | 43 | CCharString_Constructor_0099ebf0.cpp | CCharString::CCharString(const char*, long = -1) | `push -1` = default arg |
| 0x0099EAE0 | 43 | CCharString_CCharString_0099eae0.cpp | CCharString::~CCharString | |
| 0x00B94A80 | 2 | b9/CEngineScene_CEngineScene_00b94a80.cpp | CEngineScene::CEngineScene(float mapSize, unsigned long maxMapLevels, float maxPrimitiveCellSizeRatio, float maxAveragePrimitivesPerCell, const CCharString& name) | FableWin ??0CEngineScene@@QAE@MKMMABVCCharString@@@Z; no engine header |
| 0x00B389D0 | 2 | b3/stdvector_Minsertoverflow_00b389d0.cpp | std::vector<CEngineScene*>::_M_insert_overflow(pos, const T&, const __true_type&, size_type, bool) | NAME_FIX: landed element type `CUnprovenElement_00b389d0*` is CEngineScene* (this row's SceneList.push_back proves it) |
| 0x00B33AE0 | 21 | b3/CEnginePrimitiveRenderer_AddPrimitiveManager_00b33ae0.cpp | CEnginePrimitiveRenderer::AddPrimitiveManager(const CCountedPointer<CEnginePrimitiveManager>&) | private (`AAEX`) |
| 0x00B37470..0x00B38320 | 1 each (23) | b3/CCountedPointer00b360x0_Reset_00b37xxx.cpp | CCountedPointer<T>::Reset(T*) | landed names are per-deleter placeholders (`CCountedPointer_00b36060::Reset`); the real spelling is ?Reset@?$CCountedPointer@V<T>@@@@QAEXPAV<T>@@@Z (CCountedPointer.h comment: Ego_r keeps Reset out of line) |
| 0x00BABB50 | 1 | ..._CEnginePrimitiveManagerRepeatedZSprites_00babb50.cpp | ctor(CArray<CEngineScene*>&) | 0xc4 |
| 0x00BA1A70 | 1 | ..._CEnginePrimitiveManagerStaticMeshes_00ba1a70.cpp | ctor | 0x318 |
| 0x00BD1EF0 | 1 | ..._CEnginePrimitiveManagerAnimatedMeshes_00bd1ef0.cpp | ctor (??0..@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z) | 0x690 |
| 0x00B99D90 | 1 | ..._CEnginePrimitiveManagerRepeatedStaticMeshes_00b99d90.cpp | ctor | 0x94 |
| 0x00BACF70 | 1 | ..._CEnginePrimitiveManagerSprites_00bacf70.cpp | ctor | 0xc |
| 0x00BCE070 | 1 | CEnginePrimitiveManagerFlareSprites_CEnginePrimitiveManagerLines_00bce070.cpp | CEnginePrimitiveManagerFlareSprites ctor | 0x24; FILE NAME says ..Lines (wrong leaf in filename) |
| 0x00BCC5F0 | 1 | CEnginePrimitiveManagerWeaponTrails_CEnginePrimitiveManagerSpriteTrails_00bcc5f0.cpp | CEnginePrimitiveManagerWeaponTrails ctor | 0x30; filename leaf wrong |
| 0x00BCA770 | 1 | ..SpriteTrails_00bca770.cpp | CEnginePrimitiveManagerSpriteTrails ctor | 0x30 |
| 0x00BC7E90 | 1 | ..Lines_00bc7e90.cpp | CEnginePrimitiveManagerLines ctor | 0x14 |
| 0x00BC6B50 | 1 | ..Text_00bc6b50.cpp | CEnginePrimitiveManagerText ctor | 0x2c |
| 0x00B90270 | 1 | ..OcclusionBase_00b90270.cpp | CEnginePrimitiveManagerOcclusionBase(CArray<CEngineScene*>&) | called from the INLINE ctor of CEnginePrimitiveOcclusionGroupManager (0x20), which then stores vtable 0x012A1308 |
| 0x00BC61F0 | 1 | ..PrimitiveGroupManager_00bc61f0.cpp | CEnginePrimitiveGroupManager ctor | 0x20 |
| 0x00BC5570 | 1 | ..ChangerManager_00bc5570.cpp | CEnginePrimitiveChangerManager ctor | 0xc |
| 0x00BC4D90 | 1 | ..ParticleLineGroup_00bc4d90.cpp | ctor | 0x260 |
| 0x00BC34D0 | 1 | ..ParticleMeshGroup_00bc34d0.cpp | ctor | 0x2dc |
| 0x00BC14E0 | 1 | ..ParticleSpriteGroup_00bc14e0.cpp | ctor | 0x328 |
| 0x00BBEAF0 | 1 | ..ParticleSpriteTrailGroup_00bbeaf0.cpp | ctor(CArray<CEngineScene*>&, const CCountedPointer<CEnginePrimitiveManagerSpriteTrails>&) | 0x40; 2nd arg = &SpriteTrailManager (+0x2b8) |
| 0x00BBBF40 | 1 | ..ParticleGroup_00bbbf40.cpp | ctor | 0x28 |
| 0x00BBA480 | 1 | ..Decal_00bba480.cpp | ctor | 0x124 |
| 0x00BB6E00 | 1 | ..ParticleDecalGroup_00bb6e00.cpp | ctor | 0x124 |
| 0x00BB6A30 | 1 | ..WeatherMask_00bb6a30.cpp | ctor | 0xc |
| 0x00BB5040 | 1 | NOT LANDED (bwf2/00bb5040 has a dossier) | CEngineClothRenderer::CEngineClothRenderer() | 0xf0, no args |
| 0x009886F0 | 2 | 98/CNavigator_EnableNavigator_009886f0.cpp (manifest SetMember_byte0x393) | CShaderRenderManager::SetSoftwareVertexShaderEnable(bool) | NAME_FIX. FableWin ?SetSoftwareVertexShaderEnable@CShaderRenderManager@@QAEX_N@Z + Ego_r SoftwareVertexShaderEnable +0x3a3 (retail +0x393, next bool +0x394). Header CShaderRenderManager.h:141 declares it. On NEngineInternal::GPShaderRenderManager 0x01436E14 |
| 0x00988700 | 1 | 98/EnableNavigator_EnableNavigator_00988700.cpp | CShaderRenderManager::SetSoftwareFixedFunctionEnable(bool) | NAME_FIX (byte +0x394; Ego_r +0x3a4). CShaderRenderManager.h:142 |
| 0x009BEDE0 | 1 | 9b/CDisplayManager_GetVertexShaderVersion_009bede0.cpp | CDisplayManager::GetVertexShaderVersion() const (unsigned long) | on NEngineInternal::GPDisplayManager 0x01436E1C |
| 0x009BEE00 | 1 | 9b/CDisplayManager_GetPixelShaderVersion_009bee00.cpp | CDisplayManager::GetPixelShaderVersion() const | |
| 0x00A23B60 | 1 | a2/EnableNavigator_EnableNavigator_00a23b60.cpp (`C00a23b60::M`) | CMeshDataBank::SetDP3BumpEnable(bool) | NAME_FIX, PLAUSIBLE: FableWin ?SetDP3BumpEnable@CMeshDataBank@@QAEX_N@Z is the only CMeshDataBank bool setter besides SetFastBankBuildMode; byte +0x3d8 (Ego_r DP3BumpEnable +0x464, retail bank is 0x460 v Ego_r 0x4f8); this = GPResourceManager->MeshBanks[0] (+0x10, CEngineResourceManager.h), null-tested, arg BumpMappingEnable |
| 0x00A5D720 | 39 | a5/CShaderResource_GetPixelShader_00a5d720.cpp | CShaderResource::GetPixelShader(const CCharString&) const -> CPixelShader | CShaderResource.h:22 |
| 0x00B84290 | 1 | b8/CEngineSubPrimitiveRenderer_SetMaxSortDistance_00b84290.cpp | CEngineSubPrimitiveRenderer::SetMaxSortDistance(float) | on GPSubPrimitiveRenderer 0x01436E74, arg 10000.0f |
| 0x00BACD80 | 2 | ba/E2_E2_00bacd80.cpp (`C00bacd80::M`, `ret 4` empty body) | CEnginePrimitiveManager::AddEnableConsoleVariable(const CCharString&) | NAME_FIX: FableWin ?AddEnableConsoleVariable@CEnginePrimitiveManager@@QAEXABVCCharString@@@Z; CEnginePrimitiveManager.h already declares it with 0x00BACD80. (Decal row 0x00BBA480 spells it RegisterEnable -- wrong) |
| `call [Info+4]` | 21 | -- | CCPPointerInfo::DeleteFunc (__fastcall, ecx = Info->Data) | inline ~CCountedPointer of the AddPrimitiveManager temp |
| `call [vptr]` with push 1 | 39+39 | -- | CPixelShaderData deleting dtor | inline CPixelShader::operator= and ~CPixelShader |

Globals (no PDB data symbols on disk; spellings as landed):
- 0x01436E14 `NEngineInternal::GPShaderRenderManager` (bound ?GPShaderRenderManager@NEngineInternal@@...; CShaderRenderManager.h:10).
- 0x01436E1C `NEngineInternal::GPDisplayManager` (?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A, 15 rows).
- 0x01436E20 the ENGINE def: landed `g_pEngineDef_01436e20` (CEngine::Initialise, `const CEngineDef*`), also
  `g_pRenderSettings`. Read here: +0x3C LODErrorTolerance, +0x40 CharacterLODErrorTolerance (CEngineDef retail offsets,
  Ego_r +0x40/+0x44). Use CEngine::Initialise's spelling and CEngineDef type.
- 0x01436E74 `NEngineInternal::GPSubPrimitiveRenderer` (labels: ?GPSubPrimitiveRenderer@NEngineInternal@@3PAVCEngineSubPrimitiveRenderer@@A,
  row 00bca6b0); landed rows still say g_pEngineSubPrimitiveRenderer.
- 0x01436E78 = this (written at the end; zeroed by the landed dtor 0x00B33520 as `g_pPrimitiveRenderer`). Landed
  spellings disagree (g_pPrimitiveRenderer, g_pEngineRenderSettings, g_pFadeSettings, g_pPrimitiveRendererSharedData);
  most likely PDB name NEngineInternal::GPPrimitiveRenderer (0x01436E7C is proven GPPrimitiveRenderer2D) but unproven --
  use the dtor's `CEnginePrimitiveRenderer* g_pPrimitiveRenderer` to stay consistent with the same TU.
- 0x01436E98 `NEngineInternal::GPResourceManager` (CEngineResourceManager*; landed g_pResourceManager in 0x00BC14E0).
  +0x10 MeshBanks[0]; +0x2A98 ShaderResourcePixel (CCountedPointer<CShaderResource>, `GetPShaderResource()` inline).

## 4. Members written (this = esi)
Own (CEnginePrimitiveRenderer, Ego_r names, retail offsets from the generated half of CEnginePrimitiveRenderer.h and the
landed dtor 0x00B33520 model): PrimitiveManagers CArray<CCountedPointer<CEnginePrimitiveManager> > +0x274;
StaticMeshManager +0x280, AnimatedMeshManager +0x288, RepeatedStaticMeshManager +0x290, RepeatedZSpriteManager +0x298,
SpriteManager +0x2a0, FlareSpriteManager +0x2a8, WeaponTrailManager +0x2b0, SpriteTrailManager +0x2b8, LineManager +0x2c0,
TextManager +0x2c8, GameMeshManager +0x2d0 (never set), OcclusionGroupManager +0x2d8, PrimitiveGroupManager +0x2e0,
PrimitiveChangerManager +0x2e8, ClothRenderer +0x2f0, ParticleLineGroupManager +0x2f8, ParticleMeshGroupManager +0x300,
ParticleSpriteGroupManager +0x308, ParticleSpriteTrailGroupManager +0x310, ParticleGroupManager +0x318, DecalManager +0x320,
DecalGroupManager +0x328, WeatherMaskManager +0x330 (all CCountedPointer<T>, 8 bytes); SceneList CArray<CEngineScene*> +0x338;
MaxPreloadRadius +0x344, MinPreloadRadius +0x348, MaxPreloadMipmapTolerance +0x34c (float).
Header status: retail branch keeps +0x274..+0x350 OPAQUE; only FABLE_RETAIL_CENGINEPRIMITIVERENDERER_SCENELIST exposes
SceneList (+0x338) and FABLE_RETAIL_CENGINEPRIMITIVERENDERER_ADDSTATICMESH a declaration. AddPrimitiveManager is NOT
declared -> the author must add an hdr/ overlay block (#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERER_MEMBERS or similar)
with the full member list above and `private: void AddPrimitiveManager(const CCountedPointer<CEnginePrimitiveManager>&);`.
The landed dtor 0x00B33520 and AddPrimitiveManager 0x00B33AE0 model these LOCALLY (CManagerPointerArray /
CScenePointerArray / local CEnginePrimitiveRendererSharedData / local CEngineComponent) -- do not copy those; use headers.

Shared data (CEnginePrimitiveRendererSharedData.h; retail = Ego_r, no containers): header retail opt-in
FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS only names +0xd0..+0xf4 and +0x262. The full list (struct_layouts_egor.tsv,
also written out in landed 0x00B36EC0's local model) the author must add to the overlay:
+0x04 BlockOnLoad bool; +0x08 CurrentInterpolationInfo (0x70); +0x78 CurrentOcclusionLayer EEngineOcclusionLayer;
+0x7c DrawDistanceFactor, +0x80 DrawDistanceMinimum, +0x84 DrawDistanceMaximum, +0x88 RepeatedMeshDrawDistanceFactor,
+0x8c ZSpriteDrawDistanceFactor, +0x90 MinimumZSpriteAsMeshDistance, +0x94 MaximumZSpriteAsMeshDistance (float);
+0x98 DrawDistanceInvalid bool; +0x9c LODErrorTolerance, +0xa0 CharacterLODErrorTolerance, +0xa4 LODVirtualScreenSize,
+0xa8 LODVirtualScreenSizeFOVAdjusted, +0xac MipmapQuality, +0xb0 MipmapSizeMultiplier (float); +0xb4 PreloadMipmapTolerance,
+0xb8 PreloadMipmapMaxSize (long); +0xbc BumpMappingEnable, +0xbd OcclusionEnable, +0xbe OcclusionHierarchyEnable,
+0xbf OcclusionHierarchyEnableConsole (bool); +0xc0 GlobalPrimitiveInitFlags, +0xc4 DynamicVBUsage (ulong); +0xc8 DynamicVBPool
EVertexBufferPool; +0xcc DynamicVBLockFlags, +0xd0 DynamicIBUsage; +0xd4 DynamicIBPool EIndexBufferPool; +0xd8 DynamicIBLockFlags;
+0xdc..+0x214 forty CPixelShader (names in OUTLINE); +0x21c..+0x238 eight CPixelShader* (PixelShaderShadowBufferRender,
...RenderAlpha, PixelShaderDiffuseFogShadow, ...OutputFactor, PixelShaderDiffuseEnvFogShadow, PixelShaderDiffuseEnvBumpFogShadow,
PixelShaderDiffuseDP3BumpFogShadow, ...OutputFactor); +0x23c BoundingRadiusFix float; +0x240 BoundingSphereViewEnabled,
+0x241 BoundingBoxViewEnabled bool; +0x244 ShowMeshInfo long; +0x248 ShowPreloadColours bool; +0x24c ShowVisibilityInfo,
+0x250 ShowSilhouettes, +0x254 ShowSilhouettesConsole long; +0x258 VisibilityRatiosUpdate, +0x259 VisibilityRatiosReceiveUpdate,
+0x25a VisibilityRatiosValid, +0x25b BoneViewEnabled, +0x25c Show2DBoundingRect, +0x25d Show2DBoundingSphere,
+0x25e ShowUser2DBoundingSphere, +0x25f Show2DRotationPos, +0x260 OldPixelShadersEnable, +0x261 OldVertexShadersEnable,
+0x262 HighlightMirroredTexturesEnabled (bool); +0x264 LODForced long. (+0x260/+0x261 and +0x114 PixelShaderDiffuseDP3BumpFog
are NOT written by this ctor.)

Other headers needed: engine/CPixelShader.h (operator= and ~CPixelShader inline exactly as retail expands them -- same shape
matched in landed 0x00B36EC0 and 0x00BC14E0), engine/CShaderResource.h, engine/CEngineResourceManager.h (FABLE_RETAIL_CENGINERESOURCEMANAGER:
GetPShaderResource(), MeshBanks[3] +0x10), engine/CCountedPointer.h, engine/CArray.h (+ FABLE_RETAIL_CARRAY),
engine/CCharString (FABLE_RETAIL_CCHARSTRING), engine/CShaderRenderManager.h, engine/CDisplayManager (FABLE_RETAIL_CDISPLAYMANAGER),
engine/CEngineSubPrimitiveRenderer.h, engine/CEnginePrimitiveManager.h (AddEnableConsoleVariable decl).
Manager headers: Sprites/FlareSprites/RepeatedZSprites/RepeatedStaticMeshes/ParticleLine/ParticleMesh/ParticleSpriteTrail/
ParticleDecal/OcclusionGroupManager are GENERATED flattened headers with no ctor (only *_VIRTUAL_DTOR macros); SpriteTrails.h and
ParticleSpriteGroup.h (FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLESPRITEGROUP) declare the CArray<CEngineScene*>& ctor. No header at
all: StaticMeshes, AnimatedMeshes, WeaponTrails, Lines, Text, PrimitiveGroupManager, ChangerManager, ParticleGroup, Decal,
WeatherMask, CEngineClothRenderer, CEngineScene (local declarations OK for those). For the headered ones add an overlay opt-in
`CLASS(CArray<CEngineScene*>&)` declaration (size asserts already match the `new` sizes: 0x94, 0xc4, 0xc, 0x24, 0x20, 0x260, 0x2dc,
0x328, 0x40, 0x124). OcclusionGroupManager needs an overlay that derives it from CEnginePrimitiveManagerOcclusionBase
(header declares `CEnginePrimitiveManagerOcclusionBase(CArray<CEngineScene*>&)`) with an INLINE ctor
`: CEnginePrimitiveManagerOcclusionBase(scenes) { }` and a virtual so it gets vtable 0x012A1308 (its inline virtuals 0x00B38810..
0x00B38870 sit right after this TU's code -- they are this TU's COMDATs).

## 5. Callee-body visibility
- Out of line in retail and emitted in THIS TU (addresses just after the ctor): SharedData ctor 0x00B36CE0 / dtor 0x00B36EC0,
  CCountedPointer DeleteData 0x00B36060..0x00B361B0, Reset 0x00B37470..0x00B38320 (23, called), vector<CEngineScene*>::
  _M_insert_overflow 0x00B389D0, __destroy_aux 0x00B38930, CPrimitiveOcclusionGroupManager virtuals 0x00B38810.., ??_G 0x00B38D10.
  -> Reset must be CALLED: try the header's default visible body first; if VC7.1 inlines it, use
  FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL (declaration-only Reset in CCountedPointer.h line 204).
- Expanded inline: CCountedPointer converting copy ctor (template <U>) + dtor for the AddPrimitiveManager argument temp;
  CPixelShader::operator= and ~CPixelShader (+~CShaderBase vtable 0x0129A030 store); vector::push_back (fast path + call
  _M_insert_overflow __true_type overload); CArray/vector default ctor zeros; CCountedPointer() zeros; inline
  CEnginePrimitiveOcclusionGroupManager ctor; GetPShaderResource(); CCountedPointer::operator-> for SpriteManager/FlareSpriteManager.
- STLport: fable_containers.h vector::push_back line 479 (inline) calls _M_insert_overflow (template body visible; out of line).

## 6. Sibling rows (idioms)
- 0x00BC14E0 CEnginePrimitiveManagerParticleSpriteGroup ctor (Flags: none; defines FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLESPRITEGROUP,
  FABLE_RETAIL_CENGINERESOURCEMANAGER, FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_CARRAY): `X = g_pResourceManager->GetPShaderResource()
  ->GetPixelShader("PSHADER_...");` -- exactly our 39 shader statements.
- 0x00B36EC0 ~CEnginePrimitiveRendererSharedData (Flags: none, hdr overlay): full SharedData member model, CPixelShader dtor shape.
- 0x00B33520 ~CEnginePrimitiveRenderer (no flags, CCountedPointer.h): member order, g_pPrimitiveRenderer.
- 0x00B33AE0 AddPrimitiveManager (no flags): CArray<CCountedPointer<CEnginePrimitiveManager> > push_back.
- 0x00BBA480 CEnginePrimitiveManagerDecal ctor: no EH frame, `ret 4`, AddEnableConsoleVariable spelled RegisterEnable (local model).
- 0x00B26360 CEngine::Initialise: global spellings for 0x01436E1C..0x01436EAC.
