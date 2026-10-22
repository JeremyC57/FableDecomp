# Hand chunk 285 (CEngineInternalPrimitiveAnimatedMesh .. CEnginePrimitiveManagerRepeatedStaticMeshes): 15 of 40 byte-exact

## What matched (15)
- CEngineLocalDetailObjectDef::Transfer 0047769e (RELOCATION_MATCH): persists the meshes, the floats and the eight flag bits. Shape: one transfer per member in declaration order. All eight flags share ONE bool local, homed in a dead slot.
- CEngineMap::OpenDevelopmentStaticMap 00b42530 (RELOCATION_MATCH): reads the header block into a memory stream and parses the map info. Shape: the STLport vector<unsigned char> is resized inline (shrink = finish store, grow = `_M_fill_insert` with the zero byte by reference). It is read through the GetData idiom (`size() ? _M_start : (T*)this`). Storage is released with free().
- CEngineLightingManager::UpdateSIMDVarBlock 00b44540 (MATCH): the global lighting state is splatted into float[4] lanes, written lane by lane in source order.
- CEngineLightingManager::CalcSWLighting (global lights) 00b45510 (RELOCATION_MATCH). Shape:
  - MIN/MAX are argument-repeating macros around an inline Sqr.
  - The colour sum uses by-value CRGBFloatColour operators.
  - Clamp must be `__forceinline`.
- CEngineLightingManager::SetupDefaultFogStates 00b47630 (RELOCATION_MATCH, advisor C1). Shape:
  - `static __forceinline SetState(state, value, mgr)` with the manager parameter LAST. VC7.1 orders the inlined live-range copies (`mov ecx,esi ; mov edi,edx`) by parameter order.
  - The matrix PreMultiply must be `__forceinline`.
- CEngineOcclusionManager ctor 00b8d780 (RELOCATION_MATCH). Shape:
  - Slot and entry arrays are arrays of classes with inline ctors.
  - Shader lookups use implicit CCharString temporaries, as in the landed 0x00BAD040.
  - Retail vectors are 12 bytes, so offsets are shifted against the PDB.
- CEngineInternalPrimitiveStaticMesh::IsInsideFrustum 00b9dc60 (RELOCATION_MATCH). Shape:
  - The world centre is three plain sums; VC canonicalises them to z,y,x.
  - The false exit is duplicated inline.
- CEnginePrimitiveManager2DStaticMeshes::Render2DPrimitive 00b9e950 (RELOCATION_MATCH). Shape:
  - `const long& layer = mesh.Layer;` gives retail's SIB base/index order `[ecx+edx+0x320]`.
  - The mesh pointer must be re-read from memory each time; a named data local breaks it.
  - Sub-primitive padding is placed in the derived class, from 0xc, so that sizeof == 0x24.
- CEnginePrimitiveManagerRepeatedZSprites ctor 00babb50 (RELOCATION_MATCH; the label says 2DSprites). It does the shader lookups, the 256x256 billboard texture and the depth surface. It follows the 2D-sprite ctor model.
- CEnginePrimitiveManager2DSprites ctor 00bad040 (RELOCATION_MATCH): a literal passed to a `const CCharString&` parameter, i.e. an implicit name temporary.
- CEngineInternalPrimitiveSprite::Update 00bad670 (RELOCATION_MATCH): the six flag-word bits are plain bool bitfield assignments after `IsParticle = true`, which VC merges into byte RMWs.
- CEngineInternalPrimitiveSpriteTrail::Update 00bcbc00 (RELOCATION_MATCH). Shape:
  - `LengthSquared() > Sqr(eps)`, where eps is an extern global (a literal would fold).
  - The shift is written per component.
  - The new size is read through a named vector reference.
- CEnginePrimitiveManager2DAnimatedMeshes::Render2DPrimitive 00bd1f70 (RELOCATION_MATCH): the 00b9e950 model plus `const long& primitiveType = mesh.Type;`, which fixes the load-after-store order of the type test.
- CEnginePrimitiveManager2DAnimatedMeshes::CreateInternalPrimitive 00bd6b90 and CEnginePrimitiveManagerAnimatedMeshesBase::CreateInternalPrimitive 00bd6df0 (RELOCATION_MATCH). They use the `__declspec(novtable)` mesh base of the landed static-mesh factory 0x00BA7820. The implicit global operator new is NOT redeclared.

## Parked for Fable 5.1 (12)
- 009ae390 CEngineInternalPrimitiveBase::CompressPrimitive DIFFER(595v600): `this` is kept in ebp (retail spills it to [esp+0x20] only); frame slots this/compressedSize are swapped; the fill-char byte is at +0x10 (retail +0x13); the memset count is copied esi v ebp | tried: CCompressor body 0x54 (fixes the 0x8c frame), `size() > 0` GetData (the `ja`), STLport-order scratch ctor, `__true_type` with a user ctor, pair copy via named values / placement new / scope blocks (8 grades) | next: model the scratch buffer as a real `vector<char>(n, 0, allocator)` with `_Vector_base` + `_STLP_alloc_proxy`, and make Insert take the pair through a `_M_insert_overflow` stand-in on a real vector base.
- 00b45720 CEngineLightingManager::CalcSWLighting(local lights) DIFFER(787v787): the Normalise reciprocal stays on the FPU stack (retail `fstp [r] ; fmul [r]`); colour R/G spill to dead arg slots +8/+0xc (ours +0x18) | tried: A3's levers (Dot member, sum temp, lambert Y,X,Z, FastInvSqrt *=), ~30 forms | next: lane gap: x87 reciprocal spill (A3: 24 spellings, probe/advisor/NOTES_00b45cf0.md).
- 00b45a40 CEngineLightingManager::CalcSWLightingNoClipSeparateMainLight DIFFER(681v683): same Normalise reciprocal spill and colour spill slots as 00b45720 | tried: A3's levers, l1-l3 | next: lane gap: x87 reciprocal spill.
- 00b45cf0 CEngineLightingManager::CalcSWLightingNoClip DIFFER(697v697): the Normalise reciprocal stays on the FPU stack (retail pops it and multiplies from memory) | tried: advisor A3, 24 spellings | next: lane gap: x87 reciprocal spill.
- 00b47390 CEngineLightingManager::SetupShaderSpotLight DIFFER(665v665): x87 term order inside the inlined matrix operator*= | tried: 12 uniform + 3 mixed per-element sum orders (VC7.1 canonicalises by first reference) | next: lane gap: x87 commutative-term canonicalisation (perhaps a different matrix helper shape, e.g. row-vector dot helpers).
- 00b482a0 CEngineLightingManager ctor (labelled CEnginePrimitiveManager::Initialise) DIFFER(528v528): the EmptyLightArray.LightCount store is scheduled before `lea ebp,[esi+0x3c]` (retail after) | tried: ctor-init/body-init at 4 positions, element ctors, memset loops, Clear(), `self` const-ref (A3: 11 forms) | next: lane gap: store scheduling.
- 00b68da0 CEngineLandscapeRenderer::RenderSortedPatches DIFFER(603v607): the compaction count is in eax (retail ecx); the RenderFlag bool is extracted shr/and (retail and 2/shr 1); the shadow bool is sete (retail mov al,1/je/xor) | tried: bool/bitfield forms, count locals | next: an unsigned char bitfield getter for RenderFlag; a named bool with explicit if/else for the shadow mode.
- 00b8d420 CEngineOcclusionManager::RenderOcclusionBox DIFFER(664v664): retail keeps &shaders->Device in esi (`mov ebp,esi ; add esi,1D4h`); ours re-addresses [esi+1D4h] | tried: named device pointer/reference locals | next: lane gap: address CSE / register allocation.
- 00b99300 CEnginePrimitiveManagerRepeatedStaticMeshes::ProcessLightingSW DIFFER(855v871): our two SetVSConstants tails are cross-jumped (-16 bytes); retail loads the bool/count args at entry; frame order of the temporaries | tried: call-in-each-arm, named arg locals | next: make the tails differ (different temporaries per arm) to block cross-jumping.
- 00bd6810 CEnginePrimitiveManagerAnimatedMeshesBase::RenderDebugBoneHierarchy DIFFER(868v894): retail stages the end point through a full 12-byte temporary and keeps the parent/child matrix copies swapped (frame 0x134) | tried: 13 forms (forceinline operator* is worse, 1671 bytes), C3DVector copy-ctor variants | next: a by-value matrix copy with a user copy ctor, plus a line object built from a default ctor and member assignment in a separate scope.
- 00bdc980 CEngineLandscapeMap::GetEdgeStrip DIFFER(552v556): register allocation only (retail testX ebx / side ecx / this eax, spills testY) | tried: named locals, loop forms | next: lane gap: register allocation.
- 00becba0 CEngineInternalPrimitiveMorphInfo::InitialiseCompositeSurfaces DIFFER(538v569): the STLport CVectorMap push_back/_M_insert_overflow, counted-pointer copy and swap/destroy tail are not reproduced | tried: 4 first-draft forms (forceinline containers worse, 663) | next: a real STLport vector base (`_Vector_base` + alloc proxy) and noinline `_M_insert_overflow` / CCountedPointer stand-ins.

## Skipped
13 rows hold inline asm:
- Hand-written SSE inline asm:
  - 00b44820 CalcSWLightingNoClipVector
  - 00b44a60 CalcSWLightingNoClipSeparateMainLightVector
  - 00b98e10 ClipSubSectionsSIMD
  - 00bd2d90 GetBoneTransformations (`and esp,-16`)
  - 00bf7bc0 DebugCalcInForgroundRange
- The ebp-frame inline-asm fistp floor/ceil helper (known lane gap):
  - 00b45fb0 GetLightsInSphere
  - 00b46280 GetLightsInArea
  - 00b46660 AddLightsToLookupGrid
  - 00b3fc40 Peek/GetInterpolatedMapNormal
  - 00b67270 BuildBlendingTables
- The fstp/fld/fistp float-to-int helper:
  - 00b41770 Peek/GetInterpolatedWaterHeight
  - 00b8dcd0 SelectFlareTestVertexBuffer
  - 00bbddd0 ParticleSpriteTrailGroup::RenderPrimitive

## Label corrections
| Address | Label | Correct |
|---|---|---|
| 00babb50 | CEnginePrimitiveManager2DSprites ctor | CEnginePrimitiveManagerRepeatedZSprites ctor (z-sprite shaders, EnableZSprites, billboard texture) |
| 00b482a0 | CEnginePrimitiveManager::Initialise | CEngineLightingManager ctor (vtables 0x012A2274 / 0x012A223C) |
| 0x00B59710 | CNavigationLayer | CEngineComponent ctor |
| 0x00A5D5F0 | GetPixelShader | GetVertexShader |
| 0x00988600 | SetFogColour | CShaderRenderManager::SetFogTransform (copies into +0x370) |
| 0x00A3AA10 | CVO_OracleMarker::Init | CCompressorZlib::Init |
| 0x00A3AEA0 | CPreallocTexturePool | CCompressor ctor |
| 0x00994B40 | GetFramesRemaining | CMemoryDataOutputStream::GetSize |
| 0x00A23C60 | CThingFilter_IsUnconscious | C3DMeshInfo is-built check |
| 0x00A36EC0 | _Dest_val | CCountedPointer<const C3DMesh2> dtor |
| 0x00B4A4C0 | vector<CBrainBehaviour>::resize | vector<CThemeChannel>::resize |
| 0x00BF12A0 / 0x00BF18B0 | (as catalogued) | CEnginePrimitiveRenderUtil::AddGeneratedObjects, called on the managers |
| 009ae390 | CEngineInternalPrimitiveBase::CompressPrimitive | class doubtful: the data vector is at +0x88 and the chunk map at +0x94 |
