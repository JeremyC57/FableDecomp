# Hand chunk 222 (CDataBank .. CEngineSubPrimitive2DClockSprite): 19 of 40 byte-exact

## What matched (19)
- CDefClassBase: 009acb20 InternalLoadBinary -- reads the Setup/Template/DefaultValsApplied flag bytes from a CDataInputStream. Three block-scoped bool locals share the dead argument slot.
- CDefinitionManager: 009ae6b0 ClearDefsForRecompile -- for each def whose type name matches (`_stricmp`; an empty name means all defs), clear the def and re-file its counted object in the recompile map under {index, key}. The +1/+1/-1 refcount triple comes from a by-value pass-through copy (param copy, return copy, param dtor). The smart-pointer operator= keeps the old pointer in a local, which gives the short release form.
- CDisplayManager: 009c1050 SetClientWindow -- release the buffer surfaces and the device, recreate the device windowed at the client size (at least 32x32) from the last creation parameters, then rebuild the render manager. A block-scoped `bitDepth` takes the dead argument slot. The params stores follow retail order.
- CDrawGuildSeal: 0065196c Update -- guild-seal draw update (NPlayerGui TU, `#pragma optimize("y",off)` + `("s",on)`). A goto-joined failure store; a one-ternary component lookup loads the component into ecx.
- CEngine: 00b27ee0 GetScratchZBufferD24S8 -- cached power-of-two D24S8 depth surface. The landed HighestSetBit is inlined; a named pixel-format local plus an extents temporary.
- CEngineInternalPrimitiveLine: 00bc7f70 Update -- line descriptor copy. `__forceinline` bank getters, with the frame getter written as `Get(i) ? Get(i)->m_pFrame : 0`.
- CEnginePrimitiveAnimatingMesh: 007088f0 ctor -- descriptor ctor with copied interpolators and a 1-bit Lit init-list member.
- CEnginePrimitiveAnimation (really CEnginePrimitive2DAnimatingMesh): 00550a70 ctor -- built on the model of the landed sibling 0x550940.
- CEnginePrimitiveManagerParticleGroup: 00bbbf70 RenderPrimitive -- the camera delta as a C3DVector local gives the 0x1c frame.
- CEnginePrimitiveManagerRepeatedStaticMeshes: 00b9ab30 CreatePrimitive -- built on the sibling ZSprites CreatePrimitive model, plus the NonPersistent bit.
- CEnginePrimitiveManagerStaticMeshesBase: 00ba7820 CreateInternalPrimitive -- the mesh base's zero members must be member objects (CIVCountedPointer, ThemeChannelDesc) with inline ctors. Plain init-list scalars let VC hoist the vtable stores and cross-jump the bodies.
- CEnginePrimitiveTypeInfo (really DirectShow CBasePin): 00cac070 -- CBasePin ctor with an ANSI object name. Needs `/EHsc`; built from the landed twin 0xCAB370.
- CEngineResourceManager: 00b3c4b0 BuildHWStipplePatterns -- written as a column loop counting up (`src[row*32+col]`, `1<<(31-col)`), so it unrolls by 4; explicit byte swap.
- CEngineScene: 00b93f40 RenderLayerCells and 00b94060 RenderLayerCellsShadow -- a same-TU `auto_inline(off)` stand-in body for map operator[] keeps `layer` in ebp and the camera in edi.
- CEngineScenePrimitiveStatsEnumerator: 00baca70 ProcessPrimitive (plain MATCH) -- if/else plus an inline `Contains` (if / return true / return false) keeps the separate epilogue; named float locals for the overlap.
- CEngineScreenEffectGlowRenderer: 00b5ee80 dtor -- a non-virtual dtor over the inline shader member dtors, then a tail call to ~CEngineComponent.
- CEngineScreenEffectRadialBlurRenderer: 00b5bd70 CaptureEnvironmentParameters -- environment radial-blur record copy.
- CEngineSubPrimitive2DClockSprite (really CTCHeroOnlineScoreboard): 00562ec0 ctor -- STLport map/list/vector models from the landed CTCObjectAugmentations ctor 0x7673C0.

## Parked for Fable 5.1 (21)
- 009c9170 CDataBank::Validate DIFFER(269v268): this in ebx/index in ebp (retail swaps them); `lea edx,[eax+4]` v `add eax,4` group release | tried: member v local-copy releases, DecRefToZero, getter stand-ins | next: release through a pointee DecRef accessor with the index declared first
- 0067e290 CDegradableDef::Transfer DIFFER(276v273): register rotation in the later Transfer arms (edx/eax v eax/edx); fix-up loop byte load v hoisted DWORD load | tried: if/else vector, enum v long traits, non-static TransferBinaryOut | next: GraphicType read into a local before the loop
- 0064a1ae CDrawPerceivers::Update DIFFER(301v301): two adjacent loads swapped (`mov eax,[esi+38h]` / `mov ecx,[ebp-4]`) | tried: const bitset test, mutable bitset, separate perceiver local | next: lane gap: scheduling
- 0064d6a8 CDrawQuickAccessIcons::Update DIFFER(250v254): the game-event ctor's dead `Player=-1` store is dropped and its other stores sink below the Copy arguments | tried: `>` (jne), `!(<=)` (jnp) | next: out-of-line event ctor stand-in, or the event built in a block before the Copy call
- 0064d92a CDrawQuickAccessIcons::ToggleCharge DIFFER(305v306): hero ebx v edi colouring; sound key in the freed item-def slot v the dead `start` slot | tried: local-copy release, condition temporary (fixes colouring, adds sete), early-declared key | next: block-scoped key in its own scope after the item-def release
- 009e4da0 CDriveManager::CreateDirectoryW DIFFER(294v294): esi/edi swap; `add esi,0FFC0h` (imm32 16-bit) v `add edi,-40h` (imm8 int) | tried: self-subtraction, separate `letter + -0x40`, 32-bit variable with 16-bit views | next: int drive number computed from `(int)letter - 0x40`
- 00b242d0 CEngine::ConvertWorldPosToScreenPos DIFFER(297v293): screen X spilled into the dead transform-result slot with Y kept on the FPU | tried: named C2DVector result, named floats, direct member stores | next: a C2DVector result temporary that reuses the transform result slot
- 00b81050 CEngineClothSimulationMeshBase::UpdateTransform DIFFER(297v297): fmul operand order in 8 cofactor products | tried: swapped source operand order (VC canonicalises) | next: lane gap: x87 operand canonicalisation
- 00bc5c50 CEngineInternalPrimitiveChanger::SetChildPrimitive DIFFER(272v272): descriptor pointer in edx v eax (`mov eax,[esp+1Ch] ; mov eax,[eax]`) | tried: named type local, inline manager getter, enable bit computed first | next: type read through a `const CEnginePrimitive&` reference local
- 00bc2480 CEngineInternalPrimitiveParticleMesh::Update DIFFER(374v294): 0x48 frame with a full orientation-set copy v a 0x24 frame | tried: structural model only | next: drop the set copy (read Up/Forward through a reference), build the right vector in place, negate at the stores
- 00b451d0 CEngineLightingManager::CalculateFogTransformMatrix DIFFER(302v306): fog matrix and by-value parameter copy swap frame blocks (+0 v +0x30) | tried: by-value result + C4DVector assign, out-param product, direct component stores, result declared first | next: parameter taken by reference and copied into a named local declared after the fog matrix
- 00b47fd0 CEngineLightingManager::AddLightPrimitive DIFFER(268v262): `sub esp,8` v `push ecx` (new pointer not packed into the dead arg slot) | tried: named-local form (identical) | next: lane gap: frame packing (push_back pointer/tag in the dead arg slot)
- 00b480e0 CEngineLightingManager::AddLightPrimitive DIFFER(258v255): pointer gets its own `push ecx` slot v sharing the dead `light` slot with the tag; heap entry eax->ecx copy | tried: named-local form | next: lane gap: frame packing (push_back pointer/tag in the dead arg slot)
- 00b39fb0 CEnginePrimitiveAttachmentManager::UpdatePrimitives DIFFER(284v284): `fadd`/`fstp` of the grown radius scheduled after both argument pushes | tried: unsigned refcount, null-safe GetRefCount, type local + switch, decl order | next: `sphere.Radius += radius * 2.0f`
- 00be0cd0 CEnginePrimitiveManager2DParticleGroup::Render2DPrimitive DIFFER(258v258): `[ecx+eax+320h]` v `[eax+ecx+320h]` | tried: subscript order, inline IsLayerEnabled, raw byte offset | next: lane gap: SIB operand order
- 00b905a0 CEnginePrimitiveManagerOcclusionBase::PrepareOcclusionTest DIFFER(272v280): `push ebx` in the prologue v after the early-outs (shrink-wrapped) | tried: the current form only (the allocation helper matches) | next: lane gap: callee-saved pushes after early-outs
- 00b990b0 CEnginePrimitiveManagerRepeatedStaticMeshes::UpdateBoneMatrixForWindSheer DIFFER(276v276): `fld [primitive+0xa0]` hoisted above the wind-record lea | tried: the current statement order | next: lane gap: scheduling
- 00ba6dc0 CEnginePrimitiveManagerStaticMeshesBase::DrawImmediately DIFFER(291v291): select in eax then `mov ebp,eax` v straight into ebp | tried: data local inside GetData, select written in place | next: named `const T* table` local assigned in an if/else
- 00b94bf0 CEngineScene::GetMapAt DIFFER(295v311): `call __ftol2` v bare `fld ; fsub 0.5 ; fistp` floor | tried: C cast (one grade) | next: lane gap: inline-asm fistp (asm_bake FtoL family)
- 00b94d30 CEngineScene::AddPrimitive DIFFER(316v308): extents evaluated before the centre sums, 0x30 v 0x34 frame | tried: named centre/extents, helpers called twice, named centre + explicit copy | next: centre computed into a named local before the extents record, passed by value twice
- 00b947e0 CEngineSceneMap::Update DIFFER(236v246): `call __ftol2` v bare `fstp ; fld ; fistp` | tried: C cast | next: lane gap: inline-asm fistp (asm_bake FtoL family)

## Skipped
None.

## Label corrections
- 0x00550A70: the manifest's "CEnginePrimitiveAnimation::CEnginePrimitiveAnimation" is CEnginePrimitive2DAnimatingMesh::CEnginePrimitive2DAnimatingMesh. Primitive type 0x25; the stores match the PDB layout of 0x80 bytes.
- 0x00CAC070: the manifest's "CEnginePrimitiveTypeInfo::InitializeEntry" (propagated) is DirectShow amfilter.cpp CBasePin::CBasePin with an ANSI name. Callee 0x00CB1D00, labelled "CEntry", is CMediaType::CMediaType().
- 0x00562EC0: the manifest's "CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite" (propagated) is CTCHeroOnlineScoreboard::CTCHeroOnlineScoreboard.
- Callee labels:
  - 0x00464931 "push_back" is CArray<unsigned long>::resize(n, fill).
  - 0x00443F90 "CEnginePrimitiveRenderer::AddPrimitive" is vector<CEnginePrimitiveHandle>::Resize.
  - 0x00B94540 "CEngineScene::ResizeGrid" is CEngineSceneMap::ResizeGrid.
  - 0x00BEC8D0 "CEngineInternalPrimitive_UpdateVisibilityVector" is the morph info's hidden-material builder.
  - 0x00632470 "CPersistTraits<float>::TransferBinaryIn" is an ICF-shared 4-byte body.
  - 0x009C9070 is also labelled "GetPGroupEntry"; it is the text-entry getter.
  - The ba7820 Morph-member ctor is labelled "CTCOpinionSource::CTCOpinionSource" (propagated).
