# Hand chunk 276 (C2DLineF .. C3DMeshMaterial): 17 of 40 byte-exact

## What matched (17)
- **C2DLineF**
  - 00a57430 GetDist(const C2DLineF&, float*, float*, bool*): the distance between two segments, with an embedded 4-entry jump table. Shape: the case-2 vector `other.Point1 - c2` is not a named local, because VC CSEs it out of the squared distance and stores it to the frame with `fst`. Each exit takes a named `float d = (float)sqrt(best)`, so fsqrt is scheduled before the pops.
  - 00a57cc0 GetDist(const C2DBoxF*, ...): the distance from a segment to a box, through four edge C2DLineF temporaries that share one frame slot. An explicit `else` for the side flag sinks the flag load into that arm.
- **C3DMeshInfo**
  - 00a24a80 BeginLoadingStats: matched with the inline IsLODPresent(i) predicate (unrotated loops) and the landed CMeshData model.
  - 00a256f0 BeginLoading: matched with the inline Touch (resource-list move-to-tail) and a reference local for the LOD slot.
  - 00a25e70 GetMesh: needed the counted-pointer operator= out of line at four sites (a same-TU auto_inline(off) stand-in) plus an inline Info-first Assign at the blocking-load site.
- **C3DAnimationManager**
  - 00aa09f0 CalcBlendedMeshTransformations: a pool guard ternary, then a 3x4 narrowing loop. The named destination reference is bound before the source.
- **Chunk readers.** These all use inlined 4-byte stream Reads through by-value helpers whose local reuses the dead stream-argument slot. The retail C3DMeshFileChunk base is 0x2c (PDB 0x34) and retail vectors are 12 bytes.
  - 00aa5030 XBoundingBox
  - 00aa66b0 XVertexData
  - 00aa6a60 XUVTangent: `resize(n, const C3DVector&)` takes an uninitialised fill value in the frame.
  - 00aa6c70 XAnimatedPrimitiveBlock: inline STLport resize.
  - 00aa9e60 DummyObject: inline SetIdentity chained assignment.
  - 00aaf850 AnimFileSequence: inline Clear plus push_back(ReadMatrix3x4()).
  - 00abcc80 Group: inline erase, plus push_back with a merge of repeated vertices.
  - 00ac0010 Material
  - 00ac6750 Cloth
- **C3DClothPrimitive**
  - 00ac3bd0 LoadBinary: a scoped pool handle {pool, -1}, then a memory stream over the locked block.
- **C3DMeshMaterial**
  - 00abf840 InitialiseFromFileData: each chunk string goes through an inline by-value getter, so every use is a CCharString copy in the dead argument slot.

## Parked for Fable 5.1 (22)
- 00a56c80 C2DLineF::IntersectsWith DIFFER(865v867): retail emits the first projection dot's X term first and does `fld A.Y ; fmul st(2) ; .. fstp st(0)`, where ours does `fxch ; fmul [A.Y]` | tried: 25 spellings (term order, named `along` before and after the denominator, negated t, member-assignment declarations, C2DVector temporaries) | next: lane gap: x87 operand order is canonicalised by VC7.1, not driven by the source.
- 00a571a0 C2DLineF::GetPointOnLine (label GetDistanceToPoint) DIFFER(653v654): the length-square x87 order (retail X*X first), and p/before swapped between esi and edx | tried: GetLength, GetLengthSquared, inline sqrt and named-local forms | next: in-context matrix spelling search (probe/mx/ctx.py) over the length square.
- 00a50c20 C3DAnimation2::InitialiseAnimationInstance DIFFER(611v611): in the allocation-failure exit, retail emits `xor al,al ; mov [ebp+74h],al` before the pops | tried: return-value forms | next: lane gap: scheduling.
- 00a53860 C3DAnimation2::CalcLocalBoneTransformation DIFFER(922v1005): the inlined GetKeyframeInterpolate's table acos and sin use the inline-asm `fsub 0.5 ; fstp ; fld ; fistp` rounding helper | tried: first draft with an (int) cast | next: lane gap: inline-asm fistp rounding helper.
- 00a3c3b0 C3DAnimationBlendState::SetInterpolate DIFFER(777v777): `fstp st(0)` placement in the 4x-unrolled loop, and `mov ecx,eax` one slot later in the temporary release | tried: 11 variants (dst-first declaration, `== 0 ?` count, `T* old` / DecRef release, loop forms); an ADVISOR REQUEST was sent | next: lane gap: scheduling.
- 00ab1740 C3DAnimationBuilder::AssignAnimation DIFFER(605v592): retail reloads source into esi after the prefix loop; ours keeps it in edi and spills the index | tried: own forms plus advisor c1..c11 (accessor ternaries, loop-local source pointer) | next: an advisor pass on the prefix-loop register pressure.
- 00ab2160 C3DAnimationBuilder::GetIntersection DIFFER(1097v960): the AssignAnimation result stays in edi rather than the frame; the loop's counted-pointer copies schedule differently; the inlined array destructors are larger | tried: first draft, with and without CArray destructors (949v960 without) | next: model STLport vectors with _Vector_base and alloc_proxy, and the CCountedPointer dtor without zero stores.
- 00a9d750 C3DAnimationManager::ApplyBoneMorphScaling DIFFER(492v551): the original inline-assembly SSE block (fixed xmm3, row pointer re-read through the frame) | tried: intrinsics | next: lane gap: inline-asm SSE.
- 00a9dfa0 C3DAnimationManager::ApplyBoneMorphDisplacement DIFFER(565v565): SIB base/index order `[ecx+edx+4]` against ours `[edx+ecx+4]`, plus one `add edx,3Ch` slot | tried: index and pointer forms, const-ref locals | next: lane gap: SIB order after strength reduction.
- 00a4c1f0 C3DAnimationSequenceData::GetKeyframeInterpolate DIFFER(922v1006): the inline-asm fistp rounding helper in the inlined slerp (table acos and sin) | tried: first draft | next: lane gap: inline-asm fistp rounding helper.
- 00a52760 C3DAnimationSequenceData::GetKeyframeInterpolate DIFFER(922v1004): a twin of 00a4c1f0 from another TU, differing only in x87 scheduling of the dot and the blend | tried: the same draft | next: lane gap: inline-asm fistp rounding helper.
- 00a4ebb0 C3DAnimationStats::InitialiseStats DIFFER(606v604): retail's locals sit 8 bytes higher, and it builds the pair<CCharString,float> by storing the time first | tried: v1..v4 (zero-vector ctor, member store order Z,Y,X, pair construction forms) | next: build the pair via make_pair or a value_type ctor from (name, time).
- 00add690 C3DBoundingSphere::FastBuildFromPoints DIFFER(646v648): in the remainder loop, retail does `mov esi,ebx ; dec esi` with the base lea after it | tried: 19 variants (loop guards, `!=` loops, compare forms, componentwise centre) | next: lane gap: unroll remainder register copy.
- 00add920 C3DBoundingSphere::AccurateBuildFromPoints DIFFER(637v690): retail keeps this in ecx and tail-duplicates the centre step into both arms, with frame 0x40 against ours 0x28 | tried: first draft | next: write the centre step out in both arms, each with its own C3DVector temporaries.
- 00ac53f0 C3DMeshFileEngineMaterialChunk ctor DIFFER(703v755): retail uses an `and esp,-8` frame with ebp as the frame pointer and immediate zero stores | tried: switch and if-chain forms; without the switch the lane gives the aligned frame | next: find what in the switch's CCharString temporaries triggers the 8-byte alignment.
- 00aaf1e0 C3DKeyframe::InitialiseFromMatrix DIFFER(839v904): frame 0x78 against 0x84, this and the argument swapped between ebx and ebp, and the quaternion-to-matrix x87 order | tried: three drafts (sum orders, a by-value quaternion copy, a C3DVector position temporary) | next: model the inline CMatrix3x4-from-keyframe constructor (quaternion, position by value, scale args right to left) as one inline function.
- 00a89a70 C3DMesh2::GetDummyObjectPositionAnimated DIFFER(970v970): fld/fmul operand and term order in both inlined 3x4 multiplies | tried: 96 per-element spellings in context (probe/mx) | next: lane gap: x87 operand order.
- 00a89e40 C3DMesh2::GetDummyObjectPositionAnimated (blended) DIFFER(986v986): the same as 00a89a70 | tried: the a89a70 source | next: lane gap: x87 operand order.
- 00a8a220 C3DMesh2::GetDummyObjectTransformAnimated DIFFER(966v965): retail computes &Bones[bone] once into edi before the call | tried: first draft | next: a named bone reference before the call, then the a89a70 multiply.
- 00a8a5f0 C3DMesh2::GetDummyObjectTransformAnimated (blended) DIFFER(982v981): the same as 00a8a220 | tried: first draft | next: as 00a8a220.
- 00a8a9d0 C3DMesh2::CalculateBoneKeyframes DIFFER(876v874): this and the offset swapped between ebx and ebp, the cofactor product order, and c11 reuse | tried: first draft | next: GetInverse with c11 named and reused, and the loop over a byte offset.
- 00a24520 C3DMeshLODInfo::BuildMesh DIFFER(849v847): retail spills the primitive index to the dead flag slot | tried: own forms plus advisor c1 (ok declared first) | next: lane gap: register allocation.

## Skipped
- 00c5acb0 "C3DMeshFileXBoundingBoxChunk::LoadData": wrong label. The bytes are D3DX-style library code (vtable calls at +0x58 on a flagged object) and are not a chunk method.

## Label corrections
- 0x00C5ACB0 is not C3DMeshFileXBoundingBoxChunk::LoadData (library code).
- 0x00A571A0 is C2DLineF::GetPointOnLine (PDB `?GetPointOnLine@C2DLineF@@QBEMABVC2DVector@@AAV2@MPAV2@PA_N@Z`), not GetDistanceToPoint.
- 0x00A57030 is C2DLineF::Intersects2D.
- 0x00A14510 is C2DVector::Normalise, not Dot.
- 0x00A56BE0 is C2DLineF::GetLowestY, labelled GetHighestY. The others: GetLowestX 0xA56B80, GetHighestX 0xA56BB0, GetHighestY 0xA56C10.
- 0x00A99390 is C3DAnimFileRootChunk::GetHelpers, not FindFirstBone.
- 0x00AAE4D0 HasMovementVector, 0x00AAE5B0 GetMovementVector, 0x00AAE7B0 FindFirstEvent, 0x00AAE840 FindNextEvent (C3DAnimFile helpers chunk).
- 0x00A537F0 is C3DAnimFile::GetRootChunk, not SharedPtr_Copy.
- 0x00A54B50 is the multimap<CCharString,float> insert, not _Ubound. 0x00A54A90 is the multimap operator=.
- 0x0099EAE0 is ~CCharString, not _Cons_val.
- 0x00A3D500 is vector<C3DAnimationStateAlpha>::resize, not CVectorMap.
- 0x00A2ACC0 is CCountedPointer<const C3DMesh2>::operator= (manifest: CCountedPointer<CDiskFileWin32>::operator=, a byte-identical COMDAT). 0x00A299C0 is propagated the same way.
- 0x009FC440 is CResource::AddToResourceList.
- 0x00AA8360 is the chunk ctor.
- 0x00AC0B70, 0x00AC0E50 and 0x00AC0EE0 are material-chunk finders.
- 0x00ABFDC0 is GetSelfIllumination.
- 0x009F4980 / 0x009F49A0 are the scratch buffer Lock / Unlock.
- 0x009D5620 is CBankFileAsyncData::Sizeof.
- 0x0075C040 is vector<C3DVector>::resize (labelled vector<CBrainBehaviour>).
- 0x004122D0 is vector<char>::resize(n, const char&) (labelled vector<CEditRegion>); 0x00993B10 is ~CMemoryDataInputStream.
- In GetIntersection's callees:
  - 0x00AB1F30 is C3DAnimationBuilder::BuildBoneInfoTree (CArray overload), not GetIntersection_CollectEntities.
  - 0x00AAF590 is a C3DAnimFileSequenceChunk parent-index setter, not NUISystem::CObservable::AddExclusiveObserver.
  - 0x00AB33E0 is vector<CCountedPointer<C3DAnimFileSequenceChunk>>::_M_insert_overflow, not Vector_InsertN_uint64Entity_v2.
  - 0x00AB30A0 is that vector's operator=.
  - 0x00AB2BD0 and 0x00AB2C20 are the CCountedPointer vector _Destroy_range copies (labelled pair<CCharString,...>).
- 0x00A88DE0 CQuaternion::SetFromRotationMatrix and 0x00A56180 CMatrix3x4::Equals are correct.

## Review at landing
- 00a57430 has an embedded 4-entry jump table, so the lander may need verify_land_jumptable.py.
- Rows that took more than 4 grades: 00a57430 (10 probes), 00a3c3b0, 00add690, 00a56c80, 00a89a70 (searched spellings).
- Several sources model local bases next to engine headers, with the evidence in their `// Layout:` lines: the C3DMeshFileChunk base is 0x2c (PDB 0x34), retail vectors are 12 bytes, and C3DMesh2 is 8 bytes below the PDB.
- The advisor request for 00a3c3b0 (item 2 in advisor a02820cb300b7bf2e's queue) had no result after a 20-minute wait; the advisor was serving chunk 278's priority rows. Any answer lands in $S/w22/retry_shared/advisor/00a3c3b0/. Check it at landing. The row is scheduling-only, so it is parked as is.
