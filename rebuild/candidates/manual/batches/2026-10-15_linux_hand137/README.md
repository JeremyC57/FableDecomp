# Hand chunk 137 (CEngineSceneGrid .. CEngineWeatherRenderer): 35 of 40 byte-exact

## What matched (35)
- **CEngineSceneGrid** (00b91d20 GetCellTotalPrimitiveCount, 00b92250 EnumeratePrimitives, 00b937a0 GetCellCreate): the PDB
  layouts of CEngineSceneGrid / CEngineSceneGridCell match retail exactly (no engine header exists); the recursive 2x2 child
  loops reproduce as plain nested `for` over unsigned indices with `x * 2 + j, y * 2 + i`; VC7.1 itself homes the outer index in
  the dead enumerator argument slot. GetCellCreate keeps `new`'s null test and recomputes the slot address for the store.
- **CEngineSceneGridCell** (00b918b0 MovePrimitivesTo, 00b935c0 ctor, 00b93660 dtor): MovePrimitivesTo inlines the landed
  AddPrimitive + a cache Invalidate (`test al,7 ; and al,0F8h`). The ctor builds the CEnginePrimitiveHandle[2] array by an inline
  counted loop, merges five bit-field assignments into one RMW and inlines the world-seed LCG (`seed*0x24A1+0x24DF`, stored, then
  GFROR13 and stored again, `% range` unless 0). The dtor needed the handle's last-reference test written through a NULL-SAFE
  accessor `long GetRefCount() const { return m_p ? m_p->m_refCount : 0; }` -- the pointer then lands in eax and is copied to ecx
  for the virtual call (as in the retail CEnginePrimitiveHandle `??_G` @ 0x0041C500); `m_p && m_p->count == 1` loads ecx directly
  and even rotates the array-destruction loop.
- **CEngineSceneMap** (00b949a0, really `Initialise`, called by the landed ctor 0x00B94A40): struct assignment of the C2DBoxF
  through `lea`, `resize(1, CCountedPointer())` (default-ctor temporary = immediate zero stores after the pushes), then
  `GridLevels[0].Reset(new CEngineSceneGrid(0))`.
- **CEngineScreenEffectColourFilter** (00b5a160 UpdateUVs, header): retail copies the back-buffer size as ONE 8-byte C2DCoordI local
  (both words to the frame, then `fild` from it); two scalar locals `fild` straight from the member.
- **CEngineScreenEffectDisplacementRenderer** (0048d7c0, 00b86a70 InitialiseTextures, 00b88ed0 Render): InitialiseTextures/Render
  use the header through a derived shell (the enabled flag is the header's `_pad_0x08[0]`); Render inlines the landed
  ReleaseDynamicTextures (0x00B86A30). `return handle >= 0;` from a bool gives the retail `xor ecx,ecx ; setge cl ; mov al,cl`.
  0048d7c0 is not this class (see label corrections): an implicit destructor of {STLport vector of pairs, STLport map}.
- **CEngineScreenEffectGlowRenderer / RadialBlurRenderer** (00b5ee00, 00b5bfd0 SetEffectQuality; 00b5ea30 Render): header shells;
  the page free is an inline `if (handle >= 0)` helper; the twins differ only in the disabled arm (glow clears the flag first).
  Render's fade test is `Fade > 1.0f / 255.0f`; `this` is still in ecx for the first call.
- **CEngineScreenEffectOutlineGlow** (00b89140 FinishOutlineRendering, header shell, landed FinishDisplacementRendering shape)
  plus four propagated-label rows: 0052c4b0 and 0098d9f0 (STLport container record destructors: members die in reverse order,
  `clear()` re-reads the tree header per store, null-tested frees, an inline `_Destroy` pointer loop for CCharString elements),
  009f7330 (an UNOPTIMISED record destructor: `#pragma optimize("", off)` + `__forceinline` member destructors, which VC7.1 still
  inlines at /Od and homes `this`/locals in frame words), 00c5d610 (a waveOut device Close: `__declspec(dllimport)` WINMM imports,
  the unprepare import hoisted into ebp for the loop).
- **CEngineShadowRenderer** (00b4f390 UpdateShadowScene, 00b50510 SetupShadowFuncRenderStates) plus 0082b750 (CMiniMapDisplay).
  UpdateShadowScene uses engine/CEngineShadowRenderer.h + engine/CEngineCamera.h, viewing the opaque composites through
  C3DVector / CMatrix3x4 casts (vector copies via `lea`, the matrix via `rep movsd`). SetupShadowFuncRenderStates' identity
  CMatrix3x4 stores the nine zeros highest-address-first, then the diagonal: two CHAINED assignments (`a = b = ... = 0.0f;
  Right.X = Up.Y = Look.Z = 1.0f;`), which VC evaluates right to left. 0082b750 passes an empty predicate BY VALUE: it needs a
  user (empty) constructor, otherwise `T()` value-initialises its byte (`mov byte ptr [esp+4],0`).
- **CEngineSimulationManager** (00b80840 PausedWorldFrameUpdate): retail STLport lists are 4 bytes, so WindVector/PausedStepsPending
  sit 0x1c below the PDB; the direction copy needs a memberwise C3DVector copy constructor (the implicit copy forms `add eax,14h`
  before the first load); `direction * strength` through an inline `operator*(float)` returning a constructed temporary.
- **CEngineSkyRenderer** (00b61aa0, 00be9530 KeepHistoryInRange): byte-identical bodies; a `__thiscall` that never reads `this`.
- **CEngineSubPrimitive2DClockSprite** (006ddae0): really the CTCTrophy constructor (vtable 0x0125FC54) -- initialiser list in
  declaration order, two inline 4-byte sentinel lists, `add esp,8` of the two mallocs merged after the body's first load.
- **CEngineSubPrimitiveRenderer** (00b84320 RenderListTwoPass, 00b84400 DeleteList): the landed RenderList model; DeleteList's
  per-sub virtual `Destroy(1)` has no null test (not `delete`) and the clear is the inlined SGI `erase(begin(), end())`.
- **CEngineSurfaceCompositionManager** (00b2ae60, 00b2b790): FindIndexInCompletedBank reads both morph ids into locals before the
  first GFIntToCharString (retail loads both up front); GetGraphicInfo needs two getters over vectors of DIFFERENT record types
  with STLport `*(begin() + n)` -- one shared getter is tail-merged into a single block.
- **CEngineVSConstantLayoutLightsBones** (00bdb260 ctor): the landed CEngineVSConstantLayoutParticles template (two
  CShaderConstantRange temporaries assigned to the base's ranges).
- **CEngineWaterBackgroundSubPatch** (00bfd3b0 OnPostRelocation, 00bfd480 dtor, 00bfd5d0 RelocateData): the header matches;
  OnPostRelocation/RelocateData attach through a derived shell, the dtor needs a local model (a CMovableResource base with an
  inline destructor and a non-virtual derived destructor) because the header's vptr is a data word.
- **CEngineWaterRenderer** (00b6d630 FinishUsingDynamicTextures): pool local; four (texture, slot) pairs 8 bytes below the PDB block.

## Parked for Fable 5.1 (5)
- 00b5bbc0 CEngineShadowRenderer::SetShadowClipVolumesToNeverShadow DIFFER(117v117): retail `mov eax,3F000000h ; mov edx,3F4CCCCDh` (0.5f stays in eax, edx reused for the zero) vs lane `mov eax,3F4CCCCDh ; xor edx,edx` hoisted and eax reloaded with 0.5f | tried: retail store order, member order, long-typed zeros, `optimize("s")` (x87 path) | next: lane gap: constant-register allocation (same stores, same order)
- 00b50e50 CEngineWeatherRenderer::GetBoxBoundingSphereRadiusSquared DIFFER(113v114): retail homes y/z at [esp]/[esp+4] and the if/else maximum at [esp+8]; the lane puts the maximum at [esp] | tried: C3DVector copy + ternaries, scalar locals + if/else (FP code identical), maximum declared first, struct copy + if/else | next: lane gap: frame-slot ordering
- 00bf2ad0 CEngineTreeFilerBase::ConvertTreeToList DIFFER(115v115): retail list tail in esi and climbing parent in edi, lane the reverse (every other instruction identical) | tried: separate cursor local, `root` reused as cursor, head/tail declaration swap, parent declared at function scope first | next: lane gap: callee-saved register colouring
- 00bf3030 CEngineTreeFilerBase::AddNewKey DIFFER(122v122): retail `lea eax,[eax*4+1Ch]` and a copy loop with dest in eax, `key - dest` in edx, index in ecx (`[edx+eax]`); lane `lea ecx` and `[edx+eax*4]` with the index in eax | tried: indexed copy, pointer-walk copy, key words addressed as `(node + 1)` | next: an `unsigned long` index or a `memcpy`-style inline helper; else lane gap: induction-variable strength reduction
- 00c0a790 CAnimatedMeshCreateParams::CAnimatedMeshCreateParams (catalogued CEngineWaterBackgroundSubPatch ctor) DIFFER(106v106): retail loads `mov edx,3F800000h` right before the first 1.0f store (after the +0x10 byte store), the lane hoists it to the top; all stores and their order match with body assignments | tried: initialiser list, C2DVector scale member, body assignments in retail order | next: lane gap: constant-load placement (the 24b "1.0f register" family)

## Skipped
- none (no row spans two functions).

## Label corrections
- 0082b750: catalogued CEngineShadowRenderer::OnReleaseDefaultPoolResources; every callee is a CMiniMapDisplay member (remove_if<CIsMarkerBoundToThing>, RemoveAllDeadMarkers, UpdateMarkersInternal, Initialise*Markers) and the sole caller is CMiniMapDisplay::UpdateMarkers(bool) @ 0x0082B7C0 -> CMiniMapDisplay::UpdateMarkersNow (`// Leaf: UpdateMarkersNow`). Retail CMiniMapDisplay: Markers +0x20, UpdateRequired +0xbc (PDB +0x28/+0xd8).
- 006ddae0: catalogued CEngineSubPrimitive2DClockSprite::Constructor; stores vtable 0x0125FC54 (CTCTrophy) over CTCBase(CThing*) @ 0x00686800 -> CTCTrophy::CTCTrophy (`// Leaf: ??0CTCTrophy`). Callee 0x00686800 is catalogued as "CEngineSubPrimitive2DClockSprite" too: it is CTCBase::CTCBase.
- 00b949a0: catalogued CEngineSceneMap ctor; it is CEngineSceneMap::Initialise (the landed ctor 0x00B94A40 calls it) (`// Leaf: Initialise`).
- 0048d7c0, 0052c4b0, 0098d9f0, 009f7330 (all "FinishWithDynamicTextures"): implicit destructors of non-polymorphic container records (callers are their deleting destructors / an owner destructor at member +0x3b4 of 0x0098ABF0); named CKeyedDefStringTable, CKeyedTablePair, CNamedShaderTable, CPairedNodeLists by contents (`// Leaf: ??1<name>`).
- 00c5d610 ("FinishWithDynamicTextures"): slot 7 of the RTTI-less vtable 0x012B8FC4, a WINMM waveOut close -> CWaveOutDevice::Close (`// Leaf: Close`).
- 00c0a790: catalogued CEngineWaterBackgroundSubPatch ctor; no vtable, no argument, built on the stack by CEngineSubPrimitiveAnimatedMesh::Create @ 0x00AABD50 -> a 0x5c-byte create-parameter block ctor (parked).
- Callee labels seen wrong: 0x009CCDC0 "CQuestManager::OnQuestStartScreenDisplayed" is the graphic bank's find-index-by-name; 0x0047C800 "_Move_backward<pair<ulong,CDefString>*>" is a `__fastcall` range destroy (first, last, const tag&); 0x00B93EB0 "CV_SingingStonesScript" is the 0x20-byte CEngineSceneGrid constructor (one argument, 0 here; retail CArray is 12 bytes); 0x00B95A30 "CCountedPointer<CIPhysicsFacesSegmentBase>::Reset" is CCountedPointer<CEngineSceneGrid>::Reset; 0x00B98170 "vector<unsigned int>::resize" is the GridLevels resize; 0x00C3C370 "CTCHeroStats::InformOfEvent" is a one-argument free wrapper; 0x00B8FEE0 ClearPrimitiveList and 0x00AC1F60 (empty CIndexBuffer relocation hook) carry `$E2`/global labels.
