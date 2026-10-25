# Hand chunk 427 (CDynamicObjectHeap .. CEnginePrimitiveRenderer): 33 of 40 byte-exact

All 40 rows carried `_global` catalog labels. Every one was renamed from the Ego_r.pdb / FableWin.pdb name sets plus callers,
vtables and retail layouts (see Label corrections). 33 rows match (10 MATCH, 23 RELOCATION_MATCH). 7 are parked: one
unrolled-loop scheduling residue and six retail_asm candidates (inline `fistp`).

## What matched (33)

- **CEnginePrimitiveRenderer** (7: 00b31e90 SetPrimitiveAnimation, 00b31ec0 SetPrimitivePosition, 00b31ef0 AddChildPrimitive,
  00b31f20 / 00b31f70 SetChildPrimitive (5- and 4-argument), 00b31f50 GetChildPrimitiveHandle, 00b31fa0 ClearChildPrimitive).
  Handle -> internal primitive -> virtual call forwarders (SetAnimation / SetPosition also re-grid the primitive through the
  static `CEngineScene::UpdatePosition` when `GridPos_Level >= 0`). Shape lesson: VC7.1 lays out overloaded virtuals in REVERSE
  declaration order, so the (id, prim, flags) overload is declared before (id, handle, prim, flags) to get slots 7/8.
- **CEngineCamera** (00b2fc10 ConsoleRunDebugScript; 00b31a90 / 00b31b10 the DebugScript vector's `__uninitialized_fill_n` and
  `_M_insert_overflow`). The console command sets DebugScriptRunning, resets the element index and stamps GFGetTime(). Retail
  members are 4 lower than the PDB (DebugScript vector at +0x208, 12 bytes). The fill_n gets retail's second `+0x14` induction
  register from CDebugScriptElement's implicit copy over C3DVector user copy ctors. The overflow is STLport verbatim with
  same-TU noinline copy/fill bodies and an inline `_Construct` for the fill_len == 1 arm.
- **CEngine / CEngineLayerRenderer** (00b262c0 AddRenderLayer, 00b2ac80 AddComponent, 00b29840 vector<CCamera>::_M_insert_overflow).
  AddRenderLayer = DisplayProgress(literal) + push_back + `return *layer`; RenderLayers sits at retail +0x15C. AddComponent
  needed a same-TU noinline `_M_insert_overflow` body so the `__true_type` tag homes on the live argument slot. The CCamera
  overflow calls the out-of-line CCamera copy ctor 0x0049D380 from its inline `_Construct`; CRightHandedSet is Up + Forward
  (24 bytes), and getting it wrong changes the element stride.
- **CEngineSurfaceCompositionManager** (00b2af50 CalcColourDifference, 00b2c460 AddSurface; 00b2e630 / 00b2e830 / 00b2f060 /
  00b2f130 the GeneratedMorphs `map<pair<long,long>, CEngineTextureMorph>` `_M_find`, `_M_lower_bound`, `_M_insert`,
  `insert_unique`). The colour distance is four `fild`s and one fsqrt, and its squares sum from the x87 top. AddSurface needs
  the STLport list iterator's declared copy ctor, or `push edi` is shrink-wrapped. `insert_unique` needs `_M_insert` spelled
  with an explicit iterator out-pointer (`*_M_insert(&j, ...)`) so the result shares j's slot.
- **NParticleEngine parameters** (00b0fb40 CVectorMap::Insert, 00b0f210 / 00b158a0 / 00b1b380 CollectUpdateParameters of
  CPSCUpdateNormal / CPSCEmitterGeneric / CPSCSpline, 00b15b50 GetUpdateParamDesc, 00b059f0 GetVectorParam). The callers need
  `__forceinline` Insert. The CRC key goes in a named local before the descriptor temporary. The lookups need a same-TU noinline
  read-only `LowerBound` body, which lets VC keep the key in ebx across the call (probe without it: DIFFER 87v86).
- **NParticleEngine components** (00b215e0 CPSCSplineBase::StartUpdate, 00b216f0 CSplinePosParamData::GetScaleFactor,
  00b10360 CPSCEmitterGeneric::ChangeOrientation, 00b081a0 CPSCSingleSprite::AddTrailPosition, 00b0f6f0
  CParticle::CreateRandomAccelerationData). GetScaleFactor is a ratio of two inline-sqrt lengths. ChangeOrientation and
  AddTrailPosition both needed C3DVector's memberwise USER copy ctor: with the implicit one VC keeps the local in registers
  instead of retail's frame words. AddTrailPosition is STLport list push_back + `while (size() > Max) pop_front()`. The
  scoped_ptr reset ends in a `jmp [edx]` deleting-dtor tail.
- **CArray<CArray<long>> growth** (00afe4b0 `__uninitialized_fill_n`, 00afe5c0 `__uninitialized_copy`, both `/Gr`).
  CParticle::PrimIDList's element copies, with the STLport vector copy ctor (`_Vector_base(n)` + `__copy_trivial`) inline.
  Twins of the landed CVertexGroupBlock 0x00ADC3B0.
- **CRGBFloatColour** (00b09b90 Interpolate). `*this * (1 - t) + colour * t`: the operator+ argument is evaluated first.

## Parked for Fable 5.1 (7)

- 00af87c0 CDynamicObjectHeap::CDynamicObjectHeap DIFFER(115v111): in the 6-way unrolled free-list loop ours folds the first link store into `mov [eax-9Ch],edx` and stores entry 2 before `lea edx,[eax+138h]`; retail does `lea ecx,[eax-9Ch] ; mov [ecx],edx` | tried: long index loop, unsigned index, inline Free(entry) helper, pointer walk (breaks the unroll) | next: try the QFE-4035 toolset on this TU, else lane gap: unroller store scheduling
- 00af8d50 GFPackFloat DIFFER(135v110): two inline `fld ; fistp dword` GFFloatToLong conversions (ours: C++ floor/_ftol) | tried: C++ reference body (1 grade) | next: lane gap: retail_asm candidate (land under the retail_asm grade with a minimal `fistp` __asm and a `// Retail-asm:` line)
- 00afe380 CParticle::GetRecordedPos DIFFER(161v178): inline-asm ceiling (`fadd 0.5 ; fistp [ebp+0Ch]` + float-bits compare) and the ebp frame it forces | tried: C++ reference body with ceil (1 grade) | next: lane gap: retail_asm candidate
- 00b05c20 NParticleEngine::GFInterpolate DIFFER(61v55): inline `fld ; fistp dword [esp+4]` rounding of `(1-t)*a + b*t` | tried: C++ reference body (1 grade) | next: lane gap: retail_asm candidate
- 00b09aa0 CRGBFloatColour::GetRGBColourClamped DIFFER(252v233): four inline `fstp ; fld ; fistp dword` channel conversions before the 0..255 clamps | tried: C++ reference body (1 grade) | next: lane gap: retail_asm candidate
- 00b0a420 CParticle::SetAngleChange DIFFER(45v46): inline GFFloatToLong(float, long*) `lea eax,[esp+0Ch] ; mov edx,[esp] ; fistp dword ptr [edx]` | tried: C++ reference body (1 grade) | next: lane gap: retail_asm candidate
- 00b0a4a0 CParticle::SetCreationTime DIFFER(45v46): same inline `fistp dword ptr [edx]` helper, x 255.0f into the CreationTime byte | tried: C++ reference body (1 grade) | next: lane gap: retail_asm candidate

## Skipped

None. No row holds two functions.

## Label corrections

| address | catalog label | real name (evidence) |
|---|---|---|
| 00af87c0 | CDynamicObjectHeapBase_Construct_Large | CDynamicObjectHeap<156,2100>::CDynamicObjectHeap(bool) (vtable 0x0129F644, landed CreateMoreBlocks 0x00AF88F0) |
| 00af8d50 | Audio_CalculateFilterParameter | GFPackFloat (FableWin `?GFPackFloat@@YIKMKMMK@Z`; ecx/edx = the two ulongs) |
| 00afe380 | Audio_InterpolateFilterCoefficient | NParticleEngine::CParticle::GetRecordedPos(float, float) const (PositionBuffer +0x29) |
| 00afe4b0 | Vector_AllocateFromTemplate | std::__uninitialized_fill_n<CArray<long>*, unsigned int, CArray<long>> |
| 00afe5c0 | Vector_CopyVectors | std::__uninitialized_copy<CArray<long>*, CArray<long>*> |
| 00b059f0 | CVectorMap_LookupAndCopyVolume | CParticleUpdateParameters<K,K>::GetVectorParam |
| 00b05c20 | GFInterpolate (_global) | NParticleEngine::GFInterpolate(long, long, float) (`YIJJJM`) |
| 00b081a0 | EventQueue_PushEvent | NParticleEngine::CPSCSingleSprite::AddTrailPosition(const C3DVector&, float) |
| 00b09aa0 | Color_FloatToRGBA_Clamp | CRGBFloatColour::GetRGBColourClamped() const |
| 00b09b90 | InterpolateRadialBlurPositionParams (__fastcall) | CRGBFloatColour::Interpolate(const CRGBFloatColour&, float) const (thiscall) |
| 00b0a420 | PackUVForVertexBuffer | NParticleEngine::CParticle::SetAngleChange(float) (AngleChange +0x28, x 63.5) |
| 00b0a4a0 | PackUVForVertexBuffer | NParticleEngine::CParticle::SetCreationTime(float) (CreationTime +0x74, x 255) |
| 00b0f210 | NHeroInformationScreens_CSystemOffsetEntry_Add | NParticleEngine::CPSCUpdateNormal::CollectUpdateParameters |
| 00b0f6f0 | NParticleEngine_CDecal_SetRandomAcceleration | NParticleEngine::CParticle::CreateRandomAccelerationData |
| 00b0fb40 | NParticleEngine_CParticle_InsertEffect | CVectorMap<K, CParticleUpdateParamDesc<K,K>, ...>::Insert |
| 00b10360 | CParticleDataSource_ApplyAxisRotation | NParticleEngine::CPSCEmitterGeneric::ChangeOrientation |
| 00b158a0 | NHeroInformationScreens_InsertCentre | NParticleEngine::CPSCEmitterGeneric::CollectUpdateParameters |
| 00b15b50 | CVectorMap_FindByKey | CParticleUpdateParameters<K,K>::GetUpdateParamDesc |
| 00b1b380 | Vector_InsertElement | NParticleEngine::CPSCSpline::CollectUpdateParameters |
| 00b215e0 | CPSCSplineBase_UpdateSplineData | NParticleEngine::CPSCSplineBase::StartUpdate (vtable slot 8) |
| 00b216f0 | CSplinePosParamData_CalcDistanceAlpha | CPSCSplineBase::CSplinePosParamData::GetScaleFactor |
| 00b262c0 | Engine_AddRenderLayer | CEngine::AddRenderLayer |
| 00b29840 | CTCCameraPointScriptedSpline_InsertKeyCamera | std::vector<CCamera>::_M_insert_overflow |
| 00b2ac80 | Vector_PushBack | CEngineLayerRenderer::AddComponent |
| 00b2af50 | $E2 | CEngineSurfaceCompositionManager::CalcColourDifference |
| 00b2c460 | CResourceBank_InitializeCompositeSurface | CEngineSurfaceCompositionManager::AddSurface |
| 00b2e630 | RBTree_FindLE | _Rb_tree<pair<long,long>, ...CEngineTextureMorph...>::_M_find |
| 00b2e830 | RBTree_FindLE | _Rb_tree<...>::_M_lower_bound |
| 00b2f060 | CCreatureAction_NavToPos_Insert | _Rb_tree<...>::_M_insert |
| 00b2f130 | RBTree_FindInsertPosition | _Rb_tree<...>::insert_unique |
| 00b2fc10 | ConsoleRunDebugScript | CEngineCamera::ConsoleRunDebugScript (static __fastcall; label OK, class added) |
| 00b31a90 | fill<C3DAnimationStateAlpha*,C3DAnimationStateAlpha> | std::__uninitialized_fill_n<CEngineCamera::CDebugScriptElement*, ...> |
| 00b31b10 | Vector_InsertResizeAnimation | std::vector<CEngineCamera::CDebugScriptElement>::_M_insert_overflow |
| 00b31e90 | CEnginePrimitiveHandle_SetAnimation | CEnginePrimitiveRenderer::SetPrimitiveAnimation |
| 00b31ec0 | CEnginePrimitiveHandle_SetPosition | CEnginePrimitiveRenderer::SetPrimitivePosition |
| 00b31ef0 | CEnginePrimitiveHandle_AddChild | CEnginePrimitiveRenderer::AddChildPrimitive |
| 00b31f20 | CEnginePrimitiveHandle_SetChild | CEnginePrimitiveRenderer::SetChildPrimitive (handle, id, handle, prim, flags) |
| 00b31f50 | CEnginePrimitiveHandle_GetChildHandle | CEnginePrimitiveRenderer::GetChildPrimitiveHandle |
| 00b31f70 | CEnginePrimitiveHandle_RemoveChild | CEnginePrimitiveRenderer::SetChildPrimitive (handle, id, prim, flags) |
| 00b31fa0 | ChildPrimitive_ClearViaVtable | CEnginePrimitiveRenderer::ClearChildPrimitive |

Neighbours (not chunk rows) seen while naming. These are propagated or behavioural labels:
- 0x00B2E5E0 "RBTree_FindNext" = `_Rb_tree_base_iterator::_M_decrement`.
- 0x00B2EF70 "CCreatureAction_NavToPos::Clone" = `_Rb_tree::_M_create_node`.
- 0x005BB346 "std::vector::operator=" = vector<CCompositeBlendDesc> copy ctor.
- 0x0042971D = `_Rb_global<bool>::_Rebalance`.
- 0x00B31A30 / 0x00B28ED0 "_Move<CTCCameraPointScriptedSpline::CKeyCamera*>" = `__uninitialized_copy` for CDebugScriptElement / CCamera.
- 0x00B28F60 "GFExplicitNew<N3DPrimitive::CAnimatedBlock>" = `__uninitialized_fill_n<CCamera*>`.
- 0x0049D380 "CCamera" = `CCamera::CCamera(const CCamera&)`.
- 0x00B2AA70 "CTCCameraPointScriptedSpline_InsertKeyCamera" = `vector<CCamera>::push_back`.
- 0x00B2E290 "Reset CCountedPointer<CIPhysicsFacesSegmentBase const>" = `CCountedPointer<CCompositeSurfaceCache>::Reset`.
- 0x00B31700 "CAnimInit::Clear" = the CEngineCamera constructor (stores vtable 0x012A12A4).
- 0x00B317A0 "CTCActionUseReadable::OnCreate" = `CEngineCamera::ConsoleAddDebugScript`.
- 0x00B31C10 "CScriptModuleTransitionBetweenPoints::OnInitLeaf" = `vector<CDebugScriptElement>::push_back`.
- 0x00B31C70 "CLandscapeBackgroundPatch vector_deleting_destructor" = CEngineCamera's deleting destructor (slot 0 of 0x012A12A4).
- 0x00AFE910 "resize vector<CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph...>>" = `vector<CArray<long>>::resize`, called as `PrimIDList.resize(1)` by the particle renderers.
- 0x00AFE660 "fill<CArray<unsigned short>*>": its element is the 12-byte CArray<long>.
- 0x00B23BE0.. are the CEngine forwarders (`mov ecx,[0x01436E80] ; jmp`) to the CEnginePrimitiveRenderer rows.
