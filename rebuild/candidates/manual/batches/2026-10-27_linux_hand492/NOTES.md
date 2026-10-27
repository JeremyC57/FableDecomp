# Hand chunk 492 (std::vector<CASuspendableProcess*> .. CEngineSimulationBase): 37 of 40 byte-exact

37 rows graded MATCH / RELOCATION_MATCH from the file in src/ (20 MATCH, 17 RELOCATION_MATCH, 0 retail_asm), 0 parked,
3 skipped (fragments). Every src/ row has a behaviour test that passes under g++ (tall.sh) and VC7.1 (tall.py).

## Names

### Proven
Retail evidence (vtable slot, caller binding, layout match) plus a PDB symbol (Ego_r.pdb / FableWin.pdb name sets):
00a44c90 C3DSkeletalBoneMorph::CompareCRCs (static fastcall)
00a4be80 C3DAnimationSequenceData::GetDuration(bool) const
00a553f0 C3DAnimation2::CanRelocateData(long) const
00a629c0 CPackedUIntArray::CPackedUIntArray()
00a6be40 CArray<CIVCountedPointer<CManagedTextureData> >::DeallocateMemory()
00a6ca00 CPhysicsMesh::GetDummyObjectPositionStatic(const CCharString&, long) const
00a6f2f0 CVectorMap<CCharString,C3DDummyObject,CCompareString<C3DDummyObject> >::LowerBound(const CCharString&) const
00a75a70 CNavNavigableLeafNode::GetRegionID() const
00a76280 CNavBlockedQuadTreeNode::AddToIndexTable(long&, map&) const
00a76520 CNavNavigableLeafNode::HasNeighbours() const
00a8ece0 N3DPrimitive::CDeviceIndependent::GetTotalTriangleCount() const
00a8ed30 N3DPrimitive::CDeviceIndependent::GetTotalIndexCount() const
00a90510 C3DPrimitive2::AddMirroredTextureVertexIndex(const unsigned long*, unsigned long)
00a96270 GFExplicitNew<N3DPrimitive::CAnimatedBlock>
00a9def0 C3DAnimationManager::IsAnimationLoaded(const C3DAnimationStateBase&) const
00a9df50 C3DAnimationManager::BeginLoadingAnimation(const C3DAnimationStateBase&)
00aa7550 CChunkedFileSaver::CChunkedFileSaver(const char*)
00ac3910 GFPSizeof<C3DMeshPartitionElement* const>
00ac4f30 CMirroredTextureHighlightsChunk::GetList()
00ac6b40 QuadricOptimiserInternals::COptimisedVertex::CGroup::CompareWeight (static fastcall)
00addf70 C3DBoundingSphere::AccurateBuildFromPoints(const std::vector<C3DVector>&, float)
00adeea0 C3DBoundingBox::BuildFromPoints(const std::vector<C3DVector>&)
00af2ab0 NParticleEngine::CPSCBase::Initialise(CIEngine*, CParticleSystem&, const CParticleUpdateParameters<..>&, CEnginePrimitiveHandle&)
00b15af0 NParticleEngine::CPSCEmitterGeneric::EnterTimelineEnd()
00b1ae60 NParticleEngine::CPSCLight::EnterTimelineEnd()
00b5ebf0 CEngineScreenEffectGlowRenderer::SetConfiguration(int)
00b806d0 CEngineSimulationBase::GetMaximumDistanceError() const

### Probable
STLport members (the PDBs were built against Dinkumware, so no name set spells them) or a name chosen by elimination:
00a44930 std::vector<CASuspendableProcess*>::empty() const -- STLport body; type from the caller 0x00A44880 on g_ProcessStack
00a44a80 std::vector<CASuspendableProcess*>::pop_back() -- STLport body; same caller, right after back() 0x00A44A70
00a80100 std::__pop_heap_aux<pair<unsigned __int64,bool>*, .., CKeyPairCompareLess<..> > -- STLport _heap.h; element/comparator shared with the landed __make_heap 0x00A80000
00ad01f0 std::_Rb_tree_node_base::_S_minimum -- STLport _tree.h body verbatim; no callers
00ae60d0 CVectorMap<CCharString,unsigned long,CKeyPairCompareLess<CCharString,unsigned long> >::LowerBound -- the only 8-byte-entry CCharString map with an operator< comparator in Ego_r.pdb; callers 0x00AE16C0 (returns entry.second or -1) and Find 0x00AE6380
00af7250 std::vector<pair<NParticleEngine::EPSCType,NParticleEngine::CComponentInstance> >::erase(iterator) -- element type proven by its __copy callee binding 0x00AF6ED0; erase is the STLport body
00b676f0 CEngineLandscapeRenderer::Clear() -- Ego_r.pdb method order (OnReleaseDefaultPoolResources, Clear, three console commands) matches retail 0x00B676A0 / this / 0x00B67740..60, whose three commands tail-jump here

### Descriptive
Placeholder types (CUnproven*_<addr>); the function shape is certain but nothing in retail names the instantiation:
00abe910 CCountedPointer<CUnprovenTarget_00abe910>::SetStaticCast<CUnprovenSource_00abe910> (no callers; method name probable)
00ae5730 std::__uninitialized_copy<const CUnprovenElement_00ae5730*, CUnprovenElement_00ae5730*> (16-byte element; its vector copy ctor 0x00AE5C60 is called only from the unreferenced 0x00AE69D0)
00b2e900 std::_Rb_tree<CUnprovenKey_00b2e900, ..>::_M_find (byte twin of the landed 0x008EE850; no callers)

## What matched (37)
- STLport vector members (0x00A44930 empty, 0x00A44A80 pop_back, 0x00AF7250 erase, 0x00A6BE40 CArray::DeallocateMemory):
  the out-of-line COMDAT keeps the header's optimisation state; erase is `__copy_ptrs(.., __false_type)` -> forward
  `__copy(.., random_access_iterator_tag(), (int*)0)` with the tag in the dead position slot, then the element's
  implicit destructor inline (counted-pointer release, then the out-of-line ~CBaseClass).
- Comparators (0x00A44C90 CompareCRCs `cmp ; sbb ; neg`, 0x00AC6B40 CompareWeight `fcomp ; test ah,41h`): static
  `__fastcall` members returning the comparison directly.
- Heaps / trees / searches (0x00A80100, 0x00AD01F0, 0x00B2E900, 0x00A6F2F0, 0x00AE60D0, 0x00AE5730): STLport 4.6 text
  in namespace std; `__pop_heap` must be `__forceinline` into `__pop_heap_aux`; the CCharString operator< LowerBound needs
  CBasicString<char>::operator< compiled noinline in the TU so VC hoists the key's data pointer (ebp).
- Vector-overload forwarders (0x00ADDF70, 0x00ADEEA0): `F(&v[0], v.size(), ..)`; the box's pointer overload really takes
  a float count, which gives the unsigned->float `fild` + 2^32 fixup.
- Virtual one-liners (0x00A553F0, 0x00A75A70, 0x00A76280, 0x00A76520, 0x00AF2AB0, 0x00B15AF0, 0x00B1AE60, 0x00B806D0):
  bit fields as `bool:1` for byte ops and `unsigned long:N` for the dword `and`; secondary-base override with `this`
  already adjusted.
- Engine methods (0x00A4BE80, 0x00A629C0, 0x00A6CA00, 0x00A8ECE0, 0x00A8ED30, 0x00A90510, 0x00A96270, 0x00A9DEF0,
  0x00A9DF50, 0x00AA7550, 0x00AC3910, 0x00AC4F30, 0x00B5EBF0, 0x00B676F0): natural source over PDB layouts with retail
  12-byte STLport CArrays; GFPSizeof needed the `if (!null) return sizeof(T) + GFSizeof(*p); return sizeof(T);` form
  (3 grades: the ternary and accumulator forms were 43v40 / 38v40).

## Parked for Fable 5.1 (0)

## Skipped
- 00a446ce NScript_CIngredientOwner_RunThread -- fragment: uses the parent's ebp frame ([ebp-0x14]), not a function.
- 00a44719 NScript_MicroThread_Idle -- fragment: the tail of the same frame (ebp-relative locals, pops it never pushed).
- 00a46594 GetDefaultVal<CObjectAugmentationParticleSet> -- fragment: `sub esp,14h` then pops of registers it never pushed.

## Label corrections
- 0x00A44930 manifest "CMessageEventFilter_HasExtraData::operator()" -> std::vector<CASuspendableProcess*>::empty() const.
- 0x00A44A80 manifest "CGameScriptThing::DecrementScriptCounter" -> std::vector<CASuspendableProcess*>::pop_back().
- 0x00A553F0 "C3DAnimation2::vfunc_2" -> C3DAnimation2::CanRelocateData(long) const.
- 0x00AF2AB0 "CPSCBase::vfunc_28" -> CPSCBase::Initialise; 0x00B15AF0 / 0x00B1AE60 "vfunc_25" -> EnterTimelineEnd.
- All 26 `_gapscan::sub_*` rows now carry the names above.
- Landed 0x00A44A70 is named `C00a44a70::M` / DoSizeof; it is std::vector<CASuspendableProcess*>::back().
- Landed 0x00A44880 models the process stack with a global-namespace `vector` stand-in (should be std::vector).
- Landed 0x00A45150 binds 0x00A44C90 as a free `BoneMorphLess`; the real name is C3DSkeletalBoneMorph::CompareCRCs.
- Landed __make_heap 0x00A80000 binds pair as `V` (class); STLport's pair is a struct (`U`).
- 0x00A242C0 has 9 spellings in reloc_bindings; Ego_r.pdb names it CMeshDataBank::PeekAnimation(unsigned long) const.
- 0x009E6530 (CCharString::FastCompare) carries conflicting bindings.
- 0x00BDC450 is bound as `cl_h_00bdc4f0` (wrong address in the name); it is probably CEngineLandscapeMap::Clear().
- 0x01436E8C carries seven global spellings (CEngineLandscapeMapList, g_pEngineEnvironment, g_pEngineState, ...); Ego_r
  layout (MapList at +0x20 holding CEngineMap*, LandscapeMap at +0) says it is g_pEngineWorldMap (CEngineWorldMap*).
- Landed 0x00B676A0 (OnReleaseDefaultPoolResources) uses a candidates/ header with a `CEngineLandscapeMapList` stand-in
  and pointer-cast arithmetic; the 0x00B676F0 source shows the natural form (`for (long i = 0; i < (long)MapList.size(); ++i)`).

## Header debt
Local models used because the engine header cannot host the body:
- CNavNavigableLeafNode, CNavBlockedQuadTreeNode, C3DAnimation2, C3DAnimationTransitionState -- the headers flatten the
  vptr into a data member, so the polymorphic class is re-modelled with the same offsets.
- CMirroredTextureHighlightsChunk -- TriangleVertexIndexList is opaque bytes in the header.
- CEngineScreenEffectGlowRenderer -- a plain layout struct; SetConfiguration cannot be declared on it (the landed
  0x00B5EE00 works around this with a derived shell, which mangles under the wrong class).
- CEngineWorldMap -- MapList is opaque bytes (`unsigned char MapList[0xc]`).
- C3DBoundingSphere / C3DBoundingBox / C3DVector / NParticleEngine::CComponentInstance -- no engine header exists.
