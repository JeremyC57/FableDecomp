# Hand chunk 530 (CEngineMap .. CEngineInternalPrimitiveAnimatedMesh): 10 of 20 byte-exact

Re-queued chunk (20 rows, 321-397 bytes). 1 MATCH + 9 RELOCATION_MATCH, 10 parked, 0 skipped, no retail_asm row landed
(0x00B439C0 and 0x00B46A30 are retail_asm candidates, both parked). Every src/ row is confirmed by the final gall run;
every test prints PASS under g++ and the lane VC7.1.

## What matched (10)
- CEngineMap::OpenRetailStaticMap 0x00B41E50 (RELOCATION_MATCH): find the map's header offset by StaticMapName, close,
  seek, read the CStaticMapInfoBlock, take CameraMapBounds, bind the CBankFileAsync entry to the file-block manager, open the
  landscape map and (if present) the local-detail cache map, seek past the header, mark open. SHAPE: the
  CCountedPointer operator= copies the source's Info then Data into locals BEFORE the `Info != other.Info` test (retail
  keeps them in ebx/ebp across the release; reading them after the test costs ebp and gives 93 diff lines).
- CEngineLocalDetailGenerator::UpdateLoadingDistanceVideoOptions 0x00B59870 (MATCH, earlier session): fade-distance
  table reset + branch-free highest-set-bit walk.
- CEngineLandscapeRenderer::PostRenderClearRenderStates 0x00B67510 (earlier session): render-core helpers re-read the
  global each time.
- CEngineClothSimulationMeshBase::CalculateRunState 0x00B80940 (earlier session): `!(d^2 < r^2)` form.
- CEngineSimulationManager::GetWindVectorNoise 0x00B80B70: two NFractal::CPerlinNoise::Get3DNoise samples; MinWindMultiplier
  read once into a local.
- CEngineClothSimulationAnimatedMesh::Render 0x00B814B0 / CEngineClothSimulation2DAnimatedMesh::Render 0x00B81640: the
  real callee CalculateFadeOut defined `__declspec(noinline)` earlier in the TU (keeps edx live across the call as retail),
  the FableWin inline `CMatrix3x4::operator*(const C3DVector&) const` (fixes the x87 term order no source order could),
  `CParticleProgram::GetVertexPositionCount()` (Size read first), `C3DMeshInfo::GetAnimationManager()`.
- CEngineBillboardGenerator::CEngineBillboardGenerator 0x00B8B630: inline CEnginePrimitiveRenderUtil base, CEngineComponent
  at +4, VSHADER_STATIC_DIRLIGHT_FOG / PSHADER_TEXTURE_DIFFUSE through GetVShaderResource(1) / GetPShaderResource(). SHAPE:
  the pixel resource is fetched into a local BEFORE the CCharString is built (retail edi across the string ctor), the vertex
  one after; operator= releases through a local copy of Data, the temporaries' dtors through Release().
- CEnginePrimitiveManagerSprites::AddSprite 0x00BAE160: new CEngineInternalPrimitiveSprite (type 7, mask 0x200000), half
  sizes, offsets / half sizes, bitfields Lit/Sprite3D/NonPersistent, AddPrimitive(*sprite, 0), return the handle. SHAPE:
  `__forceinline` handle dtor (else an out-of-line call), the pointee Release() `long count = --IVRefCount; if (count == 0)
  Delete(); return count;` (gives `dec edi ; mov edx,edi ; test edx,edx`), stores in source order WorldPos, sizes, Texture,
  Angle, BlendMode.
- CEngineInternalPrimitiveAnimatedMesh::SetupMesh 0x00BCF2E0: once per render frame, CEnginePrimitiveManagerMeshBase::
  GetAppropriateLOD inlined (`__forceinline`), LODForced override, GetMesh into the render info. SHAPE: the body is ONE
  `if (!Mesh || RenderFrame != frame) { ... } return true;` (shared exit), the tolerance parameter is scaled in place
  (`errorTolerance *= d / (r * S)`: enregistered from the first instruction), the distance clamp is a `>` ternary
  (`test ah,41h ; je`), and C3DMeshInfo::HasLOD carries `lod < 4 &&` -- that bound is what leaves the LOD loop unrotated
  (`cmp edx,3 ; jae` at the top + `jmp` back).

## Parked for Fable 5.1 (10)
- 00b43720 CEngineMap::PeekThemeId DIFFER(327v324): retail `push ecx` reserves a frame slot for mapY and keeps x/y in edi/ebp, writing the argument slots only on the loop back-edge; ours homes mapY in y's argument slot | tried: 4 forms (239/348/350/327 bytes: early return, explicit loop, F) | next: a source shape that keeps mapY out of y's slot (separately scoped neighbour block / different tail-call form)
- 00b43870 CEngineMap::PeekThemeBlend DIFFER(327v324): same mapY slot residue as 0x00B43720 | tried: 4 grades incl. the F form | next: as 0x00B43720
- 00b439c0 CEngineMap::PeekLandscapeHeight DIFFER(345v356): same mapY slot residue (+4 on every frame offset); the GFFloatToLongNear asm tail matches | tried: F, explicit loop, asm helper with early returns (378/395), one-call-site form (345) | next: fix the mapY slot, then it lands as retail_asm (`// Retail-asm:` line already in differ/)
- 00b46a30 CEngineLightingManager::LookupSmallObjectLightCache DIFFER(322v350): retail keeps both C3DVector temporaries (frame 0x40, built max-first) and copies them into the box; ours builds min straight into the box | tried: inline/forceinline box + vector ctors, temporaries vs locals, asm in helper vs body | next: non-inline C3DVector copy ctor with a forceinline box ctor (retail_asm candidate)
- 00b677d0 CEngineLandscapeRenderer::SetupForegroundStates DIFFER(333v333): retail copies the core pointer (`mov ecx,eax` / `mov edx,eax`) before `mov edi,esi` in blocks 2-3 | tried: mask local vs member, compare orders, state reference local, enum tables | next: lane gap (scheduling)
- 00b812f0 CEngineClothSimulationStaticMesh::Render DIFFER(343v343): retail reads InitialSimulation.Size before NonSimCount | tried: both sum orders, transform forms, owner local | next: CParticleProgram::GetVertexPositionCount() + CMatrix3x4::operator* (both fixed the animated siblings)
- 00b9dfc0 CEngineInternalPrimitive2DStaticMesh::Update DIFFER(341v341): MODEL ERROR in the graded build (virtual on a flattened-vptr header -> second vptr, every member +4) | tried: probe NA vptr-free model = RELOCATION_MATCH | next: regrade differ/00b9dfc0.cpp once (expected RELOCATION_MATCH)
- 00b9e120 CEngineInternalPrimitive2DStaticMesh::Initialise DIFFER(373v365): WorldPos copy eax/edx swapped, retail keeps screen Y via `fst`, scale `c0*Scale*d/d4` reassociated | tried: C2DVector halves, Min template, camera local, operator* forms, split scale | next: two-argument TransformViewToWorld(const C3DVector&, C3DVector&) overload
- 00bcc1c0 CEngineInternalPrimitiveSpriteTrail::GetWorldPosition DIFFER(334v334): frame-slot order only (retail box at [esp+8], point copy above) | tried: GetCenter forms, min/max templates, no copy ctor, declaration orders, AddSphereToBox helper (354) | next: lane gap (frame-slot ordering) or box as two C3DVector locals
- 00bcee20 CEngineInternalPrimitiveAnimatedMesh::GetBoundingBoxWorldSpace DIFFER(342v351): retail sums each component Z-term first, loads Radius,X,Y,Z of the sphere onto the x87 stack (enregistered copy) and stores the magnitude before adding, frame 0x50 vs 0x4C | tried: 4 term orders, Radius+mag vs mag+Radius, GetBoundingSphere() by-value copies (worse) | next: GetSafeBoundingRadius over an enregistered (not frame-copied) C3DBoundingSphere local

## Skipped
None.

## Names
### Proven (two sources each; exact PDB spelling in src/differ and NAME_FIXES.tsv)
- 0x00B41E50 ?OpenRetailStaticMap@CEngineMap@@QAEXAAVCStaticMapBankFile@@AAVCDataInputStream@@AAV?$map@VCCharString@@J...@Z -- FableWin name set + catalog label + Ego_r CEngineMap offsets
- 0x00B43720 ?PeekThemeId@CEngineMap@@QBEJJJJ@Z -- FableWin + CIDrawMap slot 6 DrawGetEngineThemeAt
- 0x00B43870 ?PeekThemeBlend@CEngineMap@@QBEEJJJ@Z -- FableWin + CIDrawMap slot 7 DrawGetEngineBlendAt
- 0x00B439C0 ?PeekLandscapeHeight@CEngineMap@@QBEMJJ@Z -- FableWin + slot 5 DrawBlockGetSizeZAt + QuantiseLandscapeHeight tail
- 0x00B46A30 ?LookupSmallObjectLightCache@CEngineLightingManager@@AAE... -- FableWin + Ego_r (private)
- 0x00B59870 ?UpdateLoadingDistanceVideoOptions@CEngineLocalDetailGenerator@@QAEXABV...@Z -- FableWin + Ego_r
- 0x00B67510 / 0x00B677D0 ?PostRenderClearRenderStates / ?SetupForegroundStates@CEngineLandscapeRenderer@@AAEXXZ -- FableWin + Ego_r (private)
- 0x00B80940 ?CalculateRunState@CEngineClothSimulationMeshBase@@IBE?AW4ERunState@CEngineSimulationBase@@ABVC3DVector@@M@Z -- FableWin + Ego_r
- 0x00B80B70 ?GetWindVectorNoise@CEngineSimulationManager@@QBE?AVC3DVector@@ABV2@@Z -- FableWin + Ego_r + member offsets
- 0x00B812F0 / 0x00B814B0 / 0x00B81640 the three cloth Render overloads -- FableWin + vtable slots
- 0x00B8B630 ??0CEngineBillboardGenerator@@QAE@XZ -- FableWin + its two vtables 0x012A3BF4/0x012A3BFC
- 0x00B9DFC0 / 0x00B9E120 ?Update / ?Initialise@CEngineInternalPrimitive2DStaticMesh -- FableWin + Ego_r
- 0x00BAE160 ?AddSprite@CEnginePrimitiveManagerSprites@@QAE?AVCEnginePrimitiveHandle@@PAVCTexture@@ABVC3DVector@@MMMW4EEnginePrimitiveBlendMode@@E_N3MM3@Z -- FableWin + ret 34h + RTTI CEngineInternalPrimitiveSprite members
- 0x00BCC1C0 ?GetWorldPosition@CEngineInternalPrimitiveSpriteTrail@@UAE_NAAVC3DVector@@@Z -- FableWin + Ego_r
- 0x00BCEE20 ?GetBoundingBoxWorldSpace@CEngineInternalPrimitiveAnimatedMesh@@UAE_NAAVC3DBoundingBox@@@Z -- FableWin + Ego_r
- 0x00BCF2E0 ?SetupMesh@CEngineInternalPrimitiveAnimatedMesh@@QAE_NMM@Z -- FableWin + sibling SetupMesh2D layout
- Callees (all FableWin name set + call-site types): 0x00A25E70 C3DMeshInfo::GetMesh (const, by reference), 0x00A5D5F0
  CShaderResource::GetVertexShader, 0x00A5D720 GetPixelShader (const), 0x00A62FB0 ??0CVertexShader, 0x00A5EC00 ??0CPixelShader,
  0x00B59710 ??0CEngineComponent, 0x0099A2F0 ??0CBaseClassNonCopyable@@IAE@XZ, 0x0099EBF0/0x0099EAE0 CCharString ctor/dtor,
  0x009D58D0 CBankFileAsync::GetAsyncEntry, 0x00B3EFA0 CEngineMap::ReadMapInfoBlock (private static __fastcall),
  0x00BE03A0 SetBankFileEntry, 0x00BDD0E0 / 0x00BDF010 OpenStaticMap (stream by reference), 0x00BACDD0
  CEnginePrimitiveManager::AddPrimitive, 0x00B8FF00 ??0CEngineInternalPrimitiveBase@@QAE@JK@Z, 0x00AE1600 / 0x00AE1260
  CParticleProgram::PrepareForRendering (simulation by reference), 0x00B83750 GetBoneScalings (const, mesh by reference),
  0x00B4A6B0 Layer2DToDistance and 0x00B4A6A0 Layer2DToSortDistance (Layer2DDistances +0x88 vs Layer2DSortDistances +0x1D4),
  0x00F361D0 NFractal::CPerlinNoise::Get3DNoise.
- Inlines used under PDB names: CEnginePrimitiveManagerMeshBase::GetAppropriateLOD, C3DMeshInfo::HasLOD/GetLODError/
  GetSafeBoundingRadius/GetBoundingSphere/GetAnimationManager, CEngineResourceManager::GetVShaderResource/GetPShaderResource,
  CEngineMap::QuantiseLandscapeHeight, GFFloatToLongNear, CIVCountedPointeeBase::AddRef/Release/GetRefCount,
  CIVCountedPointer::Reset/IsNull/GetRefCount/Release, CEnginePrimitiveHandle::Clear, CVertexShaderData/CPixelShaderData::
  AddReference/Release/CountReferences, CEnginePersistentPrimitiveBase::NotifyReleased, CMatrix3x4::operator*.

### Probable (one source + consistent shape)
- 0x00BCF2E0 C3DMeshInfo::HasLOD body `lod < 4 && LODDataSizes[lod] != 0` -- the name is PDB, the bound is inferred from the match only.
- CDataInputStream::SetPosition(unsigned long) as virtual slot 1 (+0x04) in 0x00B41E50 -- PDB virtual exists, slot order not proven.
- CEnginePrimitiveHandle holding the new sprite without AddRef (`Data.Reset(sprite)`) in 0x00BAE160 -- matches the landed SetData 0x00B38780 adopt semantics.

### Descriptive (no real name recoverable)
- Globals: 0x01436E80 g_pPrimitiveRenderer_01436e80, 0x01436E78 g_pPrimitiveRendererSharedData, 0x01436E98
  g_pEngineResourceManager_01436e98, 0x01436EA4 g_pEngine_01436ea4, 0x01436E7C g_pPrimitiveRenderer2D_01436e7c,
  0x01436E8C g_pEngineWorldMap (several landed spellings exist for each; one should be chosen at landing).
- Parameter names of AddSprite, SetupMesh, OpenRetailStaticMap (the PDB name sets carry none).
- Enumerators: EShaderBankNo 1, EEnginePrimitiveType 7, STATIC_MAP_OPEN_STATE 1, ERunState values (cast literals).
- Vtable placeholders: CEnginePersistentPrimitiveBase slot 2 (`v002`), the 32 leading slots of CEngineInternalPrimitiveAnimatedMesh in differ/00bcee20.cpp.

## Label corrections
- 0x00BAE160 catalog `_global::CParticleMorphs_CEntry_Constructor` -> CEnginePrimitiveManagerSprites::AddSprite.
- 0x00B80B70 catalog `_global::ComputeWindVector_Impl` -> CEngineSimulationManager::GetWindVectorNoise.
- 0x00B43720 / 0x00B43870 catalog `CEngineMap::PeekLandscapeHeight` (propagated) -> PeekThemeId / PeekThemeBlend.
- 0x009D58D0 `CEngineWorldMap::CreateNewBankMap` -> CBankFileAsync::GetAsyncEntry (the landed file says "real name unknown"; it is in FableWin).
- 0x00A5D5F0 `CShaderResource::GetPixelShader` -> GetVertexShader.
- 0x00A62FB0 / 0x00A5EC00 `CBaseIntelligentPointer::CBaseIntelligentPointer` -> ??0CVertexShader / ??0CPixelShader.
- 0x00B59710 `CNavigationLayer::CNavigationLayer` -> ??0CEngineComponent.
- 0x0099A2F0 `CBase::CBase` -> ??0CBaseClassNonCopyable@@IAE@XZ.
- 0x0099EAE0 and 0x00BACDD0 `_Cons_val<...CParticleMorphs::CEntry...>` -> ??1CCharString / CEnginePrimitiveManager::AddPrimitive.
- 0x00B4A6A0 `CEnginePrimitiveRenderer2D::Layer2DToDistance` -> Layer2DToSortDistance (0x00B4A6B0 is Layer2DToDistance).
- 0x00B3EFA0 `_global::ReadMapInfoBlock` -> CEngineMap::ReadMapInfoBlock (private static __fastcall).

## Header debt
- engine/CEngineResourceManager.h: VShaderResources / ShaderResourcePixel are 0x230 too high (header 0x2BF0 / 0x2CC8, retail
  0x29C0 / 0x2A98 = Ego_r less 0xF34); modelled locally in 0x00B8B630.
- engine/CEnginePrimitiveRenderer.h pads +0x9C..+0xA8, +0x23C, +0x264 (CharacterLODErrorTolerance, LODVirtualScreenSizeFOVAdjusted,
  BoundingRadiusFix, LODForced are CEnginePrimitiveRendererSharedData members; no header exists for the shared data).
- engine/CEngineBillboardGenerator.h / CEngineComponent.h / C3DMeshInfo.h flatten the vptr into a data member, so polymorphic
  classes are modelled locally (deriving and adding a virtual creates a second vptr -- the 0x00B9DFC0 model error).
- No engine/ header for CEngineMap, CEngineInternalPrimitiveAnimatedMesh, CEngineInternalPrimitiveSprite, CEnginePrimitiveHandle.
