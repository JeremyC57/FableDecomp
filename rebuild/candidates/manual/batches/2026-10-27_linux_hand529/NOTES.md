# Chunk 529 -- notes (agent529)

20 rows, 0x00A46B50-0x00B41C30. 11 src (10 RELOCATION_MATCH, 1 MATCH), 3 retail_asm (RELOCATION_MATCH),
6 differ. All 14 matched rows pass their behaviour test under g++ and VC7.1.

## Proven
- 0x00A6D550 `??1CPhysicsMesh@@UAE@XZ` -- RTTI vtable 0x0129DC68 slot 0 (deleting dtor 0x00A73CC0) calls it first; Ego_r.pdb virtual dtor; vptr stores CPhysicsMesh then CIVCountedPointeeBase.
- 0x00A86CD0 `?Create@CIPhysicsFacesSegmentBase@@SIPBV1@ABV?$CArray@VC3DTriangle2@@@@ABV?$CArray@VC3DVector@@@@@Z` -- Ego_r.pdb only static with that signature + donor order; callers CPhysicsMesh ctor / CalculateSegmentation.
- 0x00AD4620 `std::_Rb_tree<pair<long,long>, pair<const pair<long,long>, CCountedPointer<QuadricOptimiserInternals::CVertexPairContraction> >, ...>::_M_insert` (AAE) -- STLport 4.6 _tree.c text verbatim + the only caller insert_unique 0x00AD5630 whose callers build that key/value; landed twin 0x00835270.
- 0x00AE8C30 `?CreateEmitterTemplate@CParticleEmitterDatabase@NParticleEngine@@QAEPAVCParticleEmitter@2@AAJ@Z` -- Ego_r donor 0xAAE5F0 (0x110, between DeleteAllEmitterTemplates and Load) + body (new CParticleEmitter, Emitters.Insert(CurrentHandle), handle out, ++CurrentHandle).
- 0x00B24150 `?ClipSphere@CEngine@@UBE_NABVC3DBoundingSphere@@@Z` -- Ego_r.pdb virtual const; landed CEngine ctor vtable slot 21.
- 0x00B24500 `?SmartRestoreComponentVideoMemory@CEngine@@AAE?AW4ESmartRestoreComponentVideoMemoryResult@1@XZ` -- Ego_r donor 0x009AEC80 (0x1A0) + body (component slots 11/12, video-option quality step-down).
- 0x00B25950 `?RenderFrame@CEngine@@AAEXABVCEngineInterpolationInfo@@@Z` -- Ego_r donor 0x009AFFD0; only caller CEngine::Render 0x00B27D90.
- 0x00B30480 `?ClipSphere@CEngineCamera@@QBE_NABVC3DVector@@MPAK@Z` -- Ego_r donor 0x009D0BF0 (0x1B0), three stack args + bool, same plane order as 0x00B24150.
- 0x00AA3390 `?CalculateSlerpWeights@CQuaternion@@SIXAAV1@ABV1@1M@Z` -- FableWin.pdb only static CQuaternion fn of that shape; body writes exactly the two slerp weights.
- 0x00AA3560 `?SetSlerpFromIdentity@CQuaternion@@QAEXABV1@M@Z` -- FableWin.pdb, `ret 8`, cosine = q.W (dot with identity), FableWin neighbour of CalculateSlerpWeights.
- 0x00B05620 `?GetInterpolatedOrientation@CParticle@NParticleEngine@@QBE?AVCQuaternion@@M@Z` -- Ego_r.pdb, only CQuaternion-returning fn of one float; Orientation +4 * slerp(OrientationChange +0x14).
- 0x00B27D90 (differ) `?Render@CEngine@@UAEXABVCEngineInterpolationInfo@@K@Z` -- Ego_r donor 0x009B39A0; CEngine vtable slot 8; calls RenderFrame.
- 0x00B3BC90 (differ) `?SetupEditorAnimation@CEngineResourceManager@@QAEKABVCWideString@@@Z` -- Ego_r donor 0x009DB270; only caller CEngine::GetEditorAnimation 0x00B23CB0 forwards through resource-manager global 0x01436E98.
- 0x00B41C30 (differ) `?PeekInterpolatedWaterDepth@CEngineMap@@QBEMJJJ@Z` -- Ego_r.pdb; callees PeekHasWaterFast 0x00B40260 / PeekWaterDepth 0x00B40BD0.
- 0x00A90D60 (differ) `?RemapBoneIndices@CDeviceIndependent@N3DPrimitive@@AAEXAAV?$map@KK...@std@@@Z` -- Ego_r.pdb private; only caller C3DPrimitive2::RemapBoneIndices 0x00A90F00.
- 0x00A56670 (differ) `?GFGetHermiteSplinePosition@@YIXAAVC3DVector@@ABV1@111M@Z` -- Ego_r/FableWin pdb, donor pair with GFDXGetHermiteSplinePosition 0x00A565C0.
- Callees proven in this chunk (spellings in NAME_FIXES.tsv): 0x00AF0380, 0x00AE91D0, 0x00AEA880, 0x00B24850, 0x00A05840, 0x00987FE0/0x009880E0/0x00988110/0x00988190, 0x0099B2C0, 0x00998C90, 0x0099EBF0, 0x009CCDC0, 0x009CD620, 0x009CE8D0, 0x009CEA70, 0x009CBA80, 0x00A2D650, 0x0042FA30, 0x00A5C720, 0x00B4FC10 `?ChangeVideoOptionShadowQuality@CEngineShadowRenderer@@QAE_NJM@Z`, 0x00A88B60 `??DCQuaternion@@QBE?AV0@ABV0@@Z`.

## Probable
- 0x00A46B50 `?insert@?$vector@VC3DSkeletalBoneMorph@@...@QAEPAVC3DSkeletalBoneMorph@@PAV3@ABV3@@Z` -- Ego_r lists vector<C3DSkeletalBoneMorph>::insert; only caller C3DSkeletalMorphTarget::CombineMorph (BoneMorphs is CArray<C3DSkeletalBoneMorph>); 20-byte element layout. (Template, no donor address.)
- 0x00A46E00 `std::__introsort_loop<C3DSkeletalBoneMorph*, C3DSkeletalBoneMorph, int, bool(__fastcall*)(...)>` -- STLport text + landed siblings __unguarded_partition 0x00A461D0 / __partial_sort 0x00A46940 for this element; comparator C3DSkeletalBoneMorph::CompareCRCs.
- 0x00A54CB0 `??4?$vector@VC3DVectorWord@C3DAnimationSequenceData@@...@QAEAAV01@ABV01@@Z` -- Ego_r name + 6-byte element; callers in the sequence compressor 0x00A4EFC0.
- 0x00A82BE0 (differ) `std::__adjust_heap<pair<unsigned __int64, list<C2DLineF> >*, int, ..., CKeyPairCompareLess<...> >` -- callers __pop_heap / __make_heap and callee __push_heap all landed for this element; unsigned 64-bit key compare; Ego_r CNavigationLayer::SwitchableLineLists value_type.
- 0x00AEA780 / 0x00AEA680 -- the __introsort_loop / __final_insertion_sort called by std::sort 0x00AEA880 (STLport call order); exact spellings not produced here.

## Descriptive
- 0x01436EA0 `?g_pEngineCamera_01436ea0@@3PAVCEngineCamera@@A` -- no PDB data name; type proven by layout (0x00B24150 / 0x00B30480 reads). Eight landed spellings -- Review at landing.
- 0x01436E60 `?g_pShadowRenderer_01436e60@@3PAVCEngineShadowRenderer@@A`, 0x01436E38 `?g_pDisplacementRenderer_01436e38@@3PAVCEngineScreenEffectDisplacementRenderer@@A`, 0x01436E34 `?g_pGlowRenderer_01436e34@@3PAVCEngineScreenEffectGlowRenderer@@A`, 0x01436E30 `?g_pRadialBlurRenderer_01436e30@@3PAVCEngineScreenEffectRadialBlurRenderer@@A` -- no PDB data names; types proven by the landed AddEngineComponent instantiations that publish them.
- 0x013CB508 `?g_pRenderStateManager_013cb508@@3PAVCRenderStateManager@@A` (bound spelling), 0x01436E18 `?g_pRenderManagerCore@@3PAVCRenderManagerCore@@A`, 0x013BC470 `?g_ShaderRenderManager@@3VCShaderRenderManager@@A` (bound spellings, types proven by layouts).
- Inline helpers that are never emitted (names are descriptive, they do not reach any object): CRenderStateManager::CreateCaptureBlock (0x00B25950), CRenderManagerCore::ReleaseSelectedDeviceObjects (0x00B27D90), GetEditorAnimationPathname (0x00B3BC90), CEngineComponent slot 10 PeekSceneFilterFlags (landed name, not in the PDB).
- Arc-cosine table 0x013CB530 `g_ArcCosTable[1026]`, cosine table 0x013CD550 `g_CosTable[1025]` (landed names).

## Label corrections
- 0x00A46B50 Vector_Insert_CParticleMorphEntry -> vector<C3DSkeletalBoneMorph>::insert.
- 0x00A46E00 Introsort_Partition_With_Predicate -> std::__introsort_loop<C3DSkeletalBoneMorph*...>.
- 0x00A54CB0 Array_C3DVectorWord_Assign -> vector<C3DVectorWord>::operator=.
- 0x00A6D550 C3DMeshVolume_Destructor -> CPhysicsMesh::~CPhysicsMesh.
- 0x00A82BE0 _Adjust_heap<pair<pair<long,long>,CEngineTextureDecal>...> -> __adjust_heap<pair<unsigned __int64, list<C2DLineF> >*...> (propagated label, wrong element).
- 0x00A86CD0 bare "Create" -> CIPhysicsFacesSegmentBase::Create.
- 0x00A90D60 AnimationSequence_RemapBoneRefs -> N3DPrimitive::CDeviceIndependent::RemapBoneIndices.
- 0x00AA3390 / 0x00AA3560 / 0x00B05620 "ApplyPartialAnimationAlpha" (propagated across all three) -> CalculateSlerpWeights / SetSlerpFromIdentity / CParticle::GetInterpolatedOrientation.
- 0x00AD4620 BTree_InsertPair -> _Rb_tree::_M_insert.
- 0x00AE8C30 CParticleEmitterManager_EmplaceEmitter -> NParticleEngine::CParticleEmitterDatabase::CreateEmitterTemplate.
- 0x00B24150 catalog name was the raw mangled symbol -> CEngine::ClipSphere.
- 0x00B27D90 "CEngine::CEngine::Render" (doubled module) -> CEngine::Render.
- 0x00B3BC90 "CEngine::GetEditorAnimation" -> CEngineResourceManager::SetupEditorAnimation (GetEditorAnimation is the caller 0x00B23CB0).
- 0x009CBA80 Nop_ret4_009cba80 -> CBankFile::UpdateEntry(unsigned long) (callee).
- 0x00AE91D0 DeleteViaVtable0_00ae91d0 -> CCountedPointer<NParticleEngine::CParticleEmitter>::DeleteData (callee).

## Header debt
- engine/CEngine.h: no virtuals or methods, members flattened; local CEngine models used (00b24150, 00b24500, 00b25950, 00b27d90). Note MSVC pads CEngine's vptr slot to 8 (the class holds doubles), so members start at +0x08.
- engine/CEngineCamera.h: CameraDesc / FrustrumPlanes flattened to byte arrays and no methods; local view used (00b24150, 00b30480).
- engine/CEngineResourceManager.h: no methods; local model (00b3bc90).
- NParticleEngine::CParticleEmitterDatabase: retail Emitters +0x28 / CurrentHandle +0x40 (Ego_r +0x30: retail CSymbolMap is 8 bytes shorter).
- N3DPrimitive::CDeviceIndependent: Groups at +0x1C (Ego_r +0x24, Dinkumware vector sizes in the PDB).
- vland SHARED_HEADER_AVAILABLE warnings (CIVCountedPointeeBase in 00a6d550, CParticleEmitter in 00ae8c30, CEngineComponent in 00b24500 / 00b25950, CSimpleMemoryPool / CRenderStateManager / CEngine in 00b25950): the generated engine/<Class>.h headers have no virtuals (no virtual members in any) and no methods, while these rows need the virtual destructor / vtable slots 1-13 (CEngineComponent), the vptr (CIVCountedPointeeBase), or out-of-line method declarations (CSimpleMemoryPool::FreeAll, CRenderStateManager::RestoreCaptureBlock, CParticleEmitter ctor). Local models kept; the headers need virtual / method declarations before these rows can include them.
- CShaderRenderManager: VertexShadersEnabled +0x390 / PixelShadersEnabled +0x391 (Ego_r +0x3A0/+0x3A1).

## Review at landing
- 0x00A86CD0: the same-TU CIPhysicsFacesSegmentBase::Calculate body is a STAND-IN (not retail) used only to stop the face array escaping; it must not be landed as 0x00A868C0's body.
- 0x01436EA0 has eight landed spellings; this chunk uses `?g_pEngineCamera_01436ea0@@3PAVCEngineCamera@@A`. The coordinator should pick one.
- 0x0099B6B0 CWideString(const wchar_t*) spelling: VC7.1 here mangles `PBG` (wchar_t as unsigned short, used in 00b3bc90) vs Ego_r `PB_W`; unresolved.
- 0x01436E38 is bound in the repo as a CEngineShadowRenderer* (`?g_pShadowRenderer@@3PAVCEngineShadowRenderer@@A`) and as g_pEngineSettings_01436e38 -- both wrong; it is the displacement renderer (NAME_FIXES).
- Descriptive inline helper names above are never emitted.
- vland dry run (tmp/vland.log): WINS 11 (10 RELOCATION_MATCH + 0x00B30480 MATCH), all behaviour PASS; no RELOC_CONFLICT, RELOC_DUPLICATE or RELOC_IDENTITY. 14 WARNs: RELOC_NAME for 0x00A46E00 (own label), 0x00A52040, 0x00A70EE0, 0x00A70E80, 0x00A73AE0, 0x00AE91D0, 0x00B4FC10 (all respelled correctly here, listed in NAME_FIXES.tsv), and SHARED_HEADER_AVAILABLE (Header debt above).
- retail_asm rows are not in payload.json: retail_asm/payload_retail_asm.json + retail_asm/oracle_retail_asm.tsv (purity.classify = retail_asm for all three).
