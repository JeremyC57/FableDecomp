# Hand chunk 180 (C2DLineF .. C3DQuadricBasedMeshOptimiser): 29 of 40 byte-exact

## What matched (29)
- C3DMeshFile*/C3DAnimFile* chunk ReadFromFile (0xAA52B0, 0xAA4570, 0xAC02D0, 0xAA5370, 0xA98AF0, 0xAB0130, 0xAC03F0, 0xAB0790): the inlined stream Read wrapped in by-value `ReadULong()/ReadLong()/ReadFloat()/ReadBool()` helpers (locals land in the dead stream slot; an unused version read loses its fast-path copy); ReadBool reads an `unsigned char` and returns `!= 0`; base reader gets `chunkSize - headerSize` with headerSize a named local; strings assigned from the by-value slot-6 ReadNullTerminatedString temporary.
- C3DAnimationSequence::ReadCompressedFromFile 0xA4CDD0: same helpers, unused GetPosition kept, `(short)ReadLong()`.
- COpinionSet::TransferBinaryIn 0x7020D0 / CSimVoice::TransferBinaryIn 0x680950 (catalog said XTriangleStrip ReadFromFile): count read, resize, landed 0xAA48D0 block Read into `GetData()` with `size() > 0` (ja).
- C3DMeshFileXUVTangentChunk / XAnimatedPrimitiveBlockChunk WriteToFile (0xAA57F0, 0xAA5720): landed chunk model; weights written `(long)(float)unsigned`.
- C3DMeshFileRootChunk::CountVertices 0xA9B460 / CountTriangles 0xA9B550: GetFirst/GetNext bodies defined above (auto_inline off) so VC keeps the counted pointer in ebx/edi; op= reads the source's count/object into locals first.
- C3DAnimFile() 0xA98E60 / C3DMeshFile() 0xA9AD80: base from a CCountedPointer temporary over `new Root` (raw ctor re-reads Object for the block), comment string as a named local in its own `{}` block.
- C3DAnimFileLoader::LoadAnimFile 0xA99420: C3DAnimFile(const CCountedPointer&) body defined above keeps the root count in ebp.
- C3DAnimFileRootChunk::GetNonLoopingDuration 0xA99610: GetFirstObject defined above makes its out pointer non-escaping (register release, zero stores dropped) while the other TU's sequence keeps the re-read release.
- C3DAnimation2() 0xA4EE10: init list + body order, list sentinel ctor, Allocator.Reset(new pool).
- C3DBoundingBox::IntersectsSolid 0xADECC0: Arvo test, `if (d <= r*r) return true; return false;`.
- CLipSyncDataBank::~CLipSyncDataBank 0xA42CE0 (catalog ~C3DClothPrimitive): inline STLport clear (landed 0x5EC5C0 shape) + member dtors.
- C3DMeshInfo copy ctor 0xA2A400: implicit copy; C3DVector memberwise user copy ctor flattens the bounds, arrays copy via lea.
- C3DMeshInfo::RemoveLOD 0xA249A0: CIVCountedPointer `= 0` / `= other` as `__forceinline` operators.
- C3DPrimitive2 RemapBoneIndices 0xA90F00, UploadToVertexBufferAnimated 0xA914B0, UploadToVertexBufferStatic 0xA931F0 (vertexCount named local), LoadVertexBuffer 0xA91560 (`AnimatedBlockCount > 0` for jbe).
- C3DQuadricBasedMeshOptimiser::BeginOptimisation 0xAA4210: CCountedPointer::Reset body defined above (same TU) keeps the mesh words in esi.

## Parked for Fable 5.1 (11)
- 00a57960 C2DLineF::GetInfiniteLineLineIntersection DIFFER(210v210): frame 0x14 v 0x20, y2/x4 copy slots, x2*y1 / x4*y3 load order | tried: float locals, member expressions, C2DVector p1..p4, p2/p4 only (retail frame, full copies) | next: p2/p4 with a __forceinline memberwise copy ctor
- 0098b860 CShaderRenderManager::CreateResource DIFFER(242v205): __false_type tag in the live hidden-result slot (retail sub esp,8), counted words kept in edi/esi | tried: inline push_back model | next: lane gap: empty tag at a still-live argument slot
- 00a4e250 C3DAnimation2::GetRotationAtTime DIFFER(171v171): quaternion X,Y,Z,W in edi,edx,ecx,esi v ours ecx,edx,esi,edi | tried: rotation local before/after duration, named identity | next: lane gap: register rotation
- 00a3c1a0 C3DAnimationBlendState::MakeOwnCopyOfData DIFFER(182v189): retail re-reads and null-tests Data before `IVRefCount > 1`, pushes the copy source via edx | tried: `Data && n > 1`, null-safe GetRefCount, reinterpret re-read | next: CIVCountedPointer member accessor VC cannot value-number
- 00a26c60 C3DAnimationInfo::GetAnimation DIFFER(224v224): key store before Data reload, `lea esi,[eax+1Ch]` v `add esi,1Ch` | tried: map reference, inline GetAnimations(), by-value Find | next: lane gap: scheduling
- 00c6adb0 CReverbSettings00c6adb0::SetDefaults DIFFER(170v173): 0.04f kept as two immediates, zero stores in address order | tried: (float)0.04, optimize("s") | next: lane gap: constant CSE scheduling
- 00a8b5f0 C3DMesh2::GetUsedBones DIFFER(183v194): ebx pushed at entry, second induction pointer for &it->BoneIndex | tried: begin/end walk | next: key/value pair walk (it->second.BoneIndex)
- 00ac09a0 C3DMeshFileSubMeshChunk::FindNextBone DIFFER(207v209): found object loaded before the null-count je (ours sinks it) | tried: cast ctor, AdoptCast, count/object locals, else-arm | next: lane gap: load sinking
- 00a361a0 C3DMeshObject::GetHelperPointPositionStatic DIFFER(187v193): X row first, (z*Z + y*Y) + x*X + P, x kept on FPU | tried: reordered named rows, member-by-member result | next: lane gap: x87 term order
- 00a2e760 C3DMeshStats::AllocateBuffers DIFFER(183v183): `add ebp,eax` v `add eax,ebp` for the volume pointer | tried: direct stores, cast sum, inline SetBuffer (fixed scheduling) | next: SetBuffer + `(unsigned char*)(dummyBytes + (unsigned long)base)`
- 00a8eda0 C3DPrimitive2::SetupVertexBuffer DIFFER(166v165): `test byte ptr [esp+1Ch],1` v `mov bl ; mov edx,1 ; test dl,bl` | tried: chained/separate scale stores, buffer local | next: lane gap: in-place flag test

## Skipped
- none

## Label corrections
- 0x0098B860 "C3DAnimFile::C3DAnimFile" is CShaderRenderManager::CreateResource (Ego_r.pdb; returns CCountedPointer<CShaderResource>).
- 0x00A9AD80 "C3DAnimFile::C3DAnimFile" is C3DMeshFile::C3DMeshFile (vtable 0x0129E0D8).
- 0x00A9B460 "CountTriangles" is C3DMeshFileRootChunk::CountVertices (calls 0xABC080 CountVertices); 0x00A9B550 is CountTriangles.
- 0x00680950 "C3DMeshFileXTriangleStripChunk::ReadFromFile" is CSimVoice::TransferBinaryIn; 0x007020D0 is COpinionSet::TransferBinaryIn.
- 0x00A42CE0 "~C3DClothPrimitive" is ~CLipSyncDataBank (vtable 0x0129D3D4).
- 0x00C6ADB0 "C3DDummyObject::C3DDummyObject" is a void SetDefaults of an unnamed 0x68-byte audio reverb parameter block (no RTTI).
- 0x00A9B030/0x00A9B0C0 "FindFirstBone/FindNextBone" are C3DMeshFileRootChunk GetFirstSubMesh/GetNextSubMesh ("SUBM"); 0x00A99110/0x00AB03D0 are GetFirstObject ("AOBJ") / GetFirstSequence ("ASEQ").
- 0x00A4EB00 "CRegionDef" is C3DAnimationStats::C3DAnimationStats; 0x00AA8360 "CShopItemDef" is C3DMeshFileChunk(const char*).
- 0x00AB3B10 "CAIStateGroup_HoverHornetQueen" is N3DPrimitive::CVertexShaderInput's ctor; 0x00A986D0/0x00A98640 "~CPatchTesselationEdgeStrip" are ~CVertexShaderInputAnimated/~CVertexShaderInputStatic; 0x00AC20A0/0x00AC20B0 "CThingFilter_IsValid::operator()" are CVertexBufferWin32 Lock/Unlock; 0x00AB37B0 "CPlayerManager::PrepareForLoad" is the vertex-size getter.
- 0x00AA4460 / 0x00A52BE0 "CCountedPointer<CIPhysicsFacesSegmentBase const>::Reset" are the Reset of CCountedPointer<C3DOptimisedMeshBase> / <CSimpleMemoryPool>; 0x00ACD500 "CollapseSegments" takes the optimisation parameter block.
- 0x00AC3AF0 "_Cons_val<...CParticleMorphs...>" is the cloth primitive's RemapBoneIndices; 0x00553E60 "std::map::insert" is map<ulong,ulong>::operator[]; 0x00A5D4D0 "CObservable::AddExclusiveObserver" is CShaderResource::Initialise(CShaderRenderManager&).
- C3DPrimitive2 +0x38 is VertexBufferStatus (PDB +0x3c), +0x68 InitFlags.
