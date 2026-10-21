# Hand chunk 263 (CEnginePrimitiveAttachmentManager .. CExpressionSet): 24 of 40 byte-exact

## What matched (24)
- **CEnginePrimitiveAttachmentManager** 00b3a180 SetAttachedPrimitive: unless the handle already holds an attached primitive
  (type 0x2e), create one, point the handle at it and append it to the manager's list, then Attach (release on failure).
  Shape: the list is STLport-shaped (iterator with a copy ctor, `push_back -> insert(end(), x) -> create_node`), which keeps
  the callee-saved pushes at entry; insert reads `Prev` into a local before linking; the attached pointer is named in its
  own statement so `this` for Attach is loaded before the argument pushes.
- **CEnginePrimitiveManagerOcclusionBase** 00b900b0 IsClippedByNearPlane: the unit-cube corners through box * view; any
  corner in front of the near clip. Shape: full inline CMatrix3x4 product (only the Z column survives).
- **CEnginePrimitiveManagerParticleDecalGroup** 00bb99a0 dtor: compiler-generated (no user `~T`, so no vtable store), members
  in reverse order, the CVertexShader[3] eh-vector loop, root dtor tail-jumped.
- **CEnginePrimitiveManagerParticleGroup** 00bbc130 RenderSubPrimGroups: a type switch with a stack CEngineSubPrimitiveBase
  per case (implicit dtor); the per-function inline budget leaves the fourth ctor out of line, as in retail.
- **CEnginePrimitiveRenderer** 00b32300 InitialiseGlobalLights (forceinline manager SetState, CRGBColour temporaries with the
  second argument built first); 00b32660 FrameStart (the by-value C2DExtentsI result in a named local).
- **CEnginePrimitiveRendererSharedData** 00b36ce0 ctor: base ctor, vtable, then member sub-object ctors in declaration order.
- **CEngineScene** 00b930a0 PreloadPrimitives(const CShadowClipVolume&, const C3DVector&, ulong): every scene map's top grid
  from cell (0,0), then the moving primitives inside the volume's sphere and planes. Shape: the bounding sphere is one
  function-scope local cleared member by member each iteration (a loop-scoped zeroing ctor lets VC7.1 sink the four stores
  below the vptr load); the grid pointer is named before the call.
- **CEngineSceneGrid** 00b92070 PreloadPrimitives (clip-volume overload): cell box clip, per-primitive sphere test, PreloadStat
  2/1, then the 2x2 children. Shape: the `inside` out-bool is declared at function scope (block scope hands it the dead
  volume-argument slot that retail gives the child-row counter). 00b92790 RenderCellClippedOcclusion: as the landed
  RenderCellClipped plus the occluded-layer test, OcclusionLayerBitMask filter and the HasOcclusionResults bit. Shape: the
  null-cell and occluded-cell tests are two separate `return false` statements (one `||` moves the shared exit to the end).
- **CEngineScreenEffectDisplacementRenderer** 00b87680 BeginDisplacementRendering and **CEngineScreenEffectOutlineGlow**
  00b89a20 BeginOutlineRendering: the saved display target is a scoped named CRenderTarget assigned member-wise into
  OldRenderTarget; the state switch / capture block are inline core methods.
- **CEngineScreenEffectGlowRenderer** 00b5f8c0 ComputeGlowColor: CRGBColour locals from `(unsigned char)(x * 255.0f)`.
- **CEngineStateBlockDiffuseOnly** 00b58de0 Apply: four tracked render states set on the core.
- **CTCQuestCompletionUI** 005e4030 ctor and **CTCCreatureOpinionOfHero** 00684420 ctor (catalogued as
  CEngineSubPrimitive2DClockSprite): container ctors as the landed CTCBoastUI/CTCWound; memset byte masks and an inline
  counted list-head loop.
- **CEngineTreeFilerBase** 00bf2e50 AddNodeAutoExpandTree: MIN/MAX macros for the window; the key words are read inside the
  bit loop (the compiler hoists them), which frees retail's registers.
- **CEngineWaterRenderer** 00b6d940 LoadCompressedBuffer (value-returning ReadLong, the range compressor declared first at
  function scope); 00b71fb0 FrameStart (clear() as erase(begin, end) through `__copy_trivial`).
- **CEntity** 00c64fb0 SetMeshComponentsProperty / 00c65690 SetVFXComponentsProperty: `size / 24` entries, a validation pass,
  then an if-chain store pass.
- **CEnvironmentThemeDef** 004732e0 Transfer: members in declaration order, the context in edi.
- **CEnvironmentThemePriorityStackEntry** 00886200 RebalanceWeights: a forceinline erase calling a separate inline
  copy(first, last, result) gives retail's single walking pointer.
- **CExpressionSet** 0081b510 TransferBinaryIn: the Read test is written `size <= remaining`; one shared count slot.

## Parked for Fable 5.1 (16)
- 00bc9800 CEnginePrimitiveManagerLines::RenderPrimitive DIFFER(430v428): retail `xor ebx,ebx ; cmp byte ptr [ebp+78h],bl ; setne bl`, ours `mov dl,[ebp+78h] ; test dl,dl ; setne bl` | tried: ?:, != 0, != false, TRUE/FALSE, bool-param helper, char member | next: the value as an inline CRenderStateValue ctor taking the bool
- 00b903d0 CEnginePrimitiveManagerOcclusionBase::RenderUntexturedStaticMeshPrimitive DIFFER(456v464): primitive ebp/stride ebx swapped, index buffer homed in the arg slot in retail | tried: direct body, inlined draw helper (two parameter orders), block by value/reference, cached block count | next: named pointer locals for the primitive and stride (fleet lesson 24p)
- 00b91140 CEnginePrimitiveManagerOcclusionBase::RenderBoundingBox DIFFER(512v510): primitive in ebx (ours) vs edi, the handle copy store sunk below the frame test, the `__true_type` tag in its own frame byte (retail: the live primitive argument slot) | tried: plain push_back, by-value Add wrapper (best), by-value AddTestHandle helper, explicit handle copy, tag without ctor | next: lane gap: empty-tag temporary placed on a live argument slot
- 00bbcc70 CEnginePrimitiveManagerParticleGroup::SetupPixelShader DIFFER(411v415): retail scoped `push ebp ; ... ; pop ebp` temp for the second state value | tried: free core-last SetState for both, manager-method SetState for one/both | next: a CRenderStateValue local per block
- 00b9d730 CEnginePrimitiveManagerStaticMeshesBase::IsInsideFustrum DIFFER(429v429): retail `fld [ecx+18h]` radius first then `fmul [eax+58h]`; ours loads the scale first | tried: both operand orders, radius before/after the camera, `*=`, no camera local, operator- | next: lane gap: single-term x87 commutation
- 00ba4a40 CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksBlankTextures DIFFER(466v463): primitive ebp vs ebx, the bank base reloaded after the imul in retail | tried: compare orders, operator== on the stages, three operator[] forms, block reference | next: named pointer locals (24p)
- 00ba50a0 CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocks DIFFER(441v441): `failed = false` store emitted at the top of the branch (retail after the prim load) | tried: six declaration orders, const flag, named condition | next: the flag pair as a record with an inline ctor
- 00b4b8b0 CEngineShadowRenderer::CalcLocalWorldToBaseShadowSpace DIFFER(421v421): `fld st(2) ; fmul [dir.Z]` vs `fld [dir.Z] ; fmul st(3)` plus result-copy scheduling | tried: operand swaps, ternary pair, literal 0, Cross helper | next: lane gap: single-term x87 commutation
- 00b81820 CEngineSimulationManager::FrameStart DIFFER(433v433): saved-frame store after `push ebx` (retail after `push ebx ; push esi`) | tried: named world local, const local, RAII frame scope | next: lane gap: callee-saved push placement
- 00b61b30 CEngineSkyRenderer::BuildOuterSkyMesh DIFFER(414v410): lower-ring colour 0xffffffff not hoisted into ebx (u lands in ebx, not ebp) | tried: unsigned i, angle declared before u, `vertex++` copies, centre+1 pointer, k-index loop, a `white` local | next: lane gap: register-constant hoisting
- 00b62440 CEngineSkyRenderer::WorldFrameUpdate DIFFER(389v405): layer-0 wind*speed C3DVector built through one more stack copy in retail (0x1c frame vs 0x18) | tried: about ten variants of the scaled-wind delta build (component-wise C2DVector ctor is best; 350v405 to 389v405) | next: C3DVector with a user copy ctor and a by-value `return C3DVector(...)` operator* result
- 00b847d0 CEngineSubPrimitiveRenderer::RadixSort DIFFER(444v444): retail gives the histogram loop its own zero index; ours reuses the memset zero | tried: ++/+=/post-inc, signed/int/scoped index, while loop, clear loop, GetRadix helper | next: lane gap: register allocation
- 00b52990 CEngineWeatherRenderer::UpdateBoxPositions DIFFER(387v406): inline-asm fistp floor helper (ebp frame, fistp/fild per axis) | tried: compiler-emitted float-to-int conversions (go through __ftol2) | next: lane gap: inline asm (not byte-pure reproducible)
- 00b53f60 CEngineWeatherRenderer::FrameStart DIFFER(418v411): retail keeps `this` in ecx across the same-TU inline-asm UpdateBoxPositions | tried: noinline stand-in callee body (415v411) | next: lane gap: callee register knowledge of an asm callee
- 0051eda0 NThingManager::CEntityRuntimePersistence::SaveToGameStateScript DIFFER(485v501): retail keeps the Valid-then-null test pair on the bounds-checked element; ours drops the null test; "\r\n" pushed before the level-UID call | tried: several element-access forms (best kept in differ/) | next: the element access as an out-of-line-looking inline returning a pointer with an explicit null check
- 00520390 NThingManager::CEntityRuntimePersistence::ClearPersistenceDataForSection DIFFER(464v465): the two script loops swap register shapes (base from the size test kept vs reloaded; ebp push placement) | tried: plain loops, then a forceinline ClearScriptsForSection helper plus forceinline GetCRC (best) | next: a block-scoped `const CArray&` for the second loop

## Skipped
- None (every row attempted; no row holds two functions).

## Label corrections
- 005e4030: catalogued CEngineSubPrimitive2DClockSprite ctor; the body is CTCQuestCompletionUI::CTCQuestCompletionUI(CThing*).
- 00684420: catalogued CEngineSubPrimitive2DClockSprite::ctor; the body is CTCCreatureOpinionOfHero::CTCCreatureOpinionOfHero(CThing*).
- 0x00B3B030 (manifest GetIndexBufferUsage) is COMDAT-folded; 00b6d940 calls it as the vertex-buffer usage getter on the
  engine global 0x01436E98.
- 0x00B2FA40 (manifest/landed file "CV_BanditTollScript::Init") is a CEnginePrimitiveOcclusionManager member called on
  0x01436E58 with the occlusion info (00b91140 calls it when the box crosses the near plane).
