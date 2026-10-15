# Hand chunk 134 (CEngineInternalPrimitive2DStaticMesh .. CEngineLandscapeMemoryManager): 30 of 40 byte-exact

All 30 matched sources grade MATCH/RELOCATION_MATCH with no pragma or flag; every behaviour test prints PASS under g++ (tall.sh) and the
lane's VC7.1 cl.exe (tall.py).

## What matched (30)
- **CEngineInternalPrimitive2DStaticMesh** 00ac6dc0 GetObjectTransform: `this` is a row-major 3x3 *double* matrix narrowed into a by-value
  CMatrix3x4 (named local + `rep movsd` copy-out; source cells reused, which is why retail keeps loads on the x87 stack). Not the PDB class.
- **"2DViewport"** 00af70d0 / 00af7170 ClearChildPrimitive (owner not the viewport, no xrefs): handle Clear (agent-61 `IsUnique` +
  `Data = 0`) then STLport `vector<POD>::clear()` whose `__copy_trivial(last, finish, first)` compares finish with itself (`cmp eax,eax`)
  and memmoves 0 bytes; `size() > index` order gives `cmp eax,edi ; jbe`.
- **CEngineInternalPrimitive2DViewport** 00be21f0 ctor: the 16 zeroed-and-never-read frame bytes are the C4DVector TEMPORARY of
  `AlphaMaskPositionFactors = C4DVector(0,0,0,0)` in the C2DAlphaMaskInfo constructor body; the two bools must be body assignments after it.
- **CEngineInternalPrimitiveAnimatedMesh** 00bceb40 GetWorldPosition: `(end - start) * t + start` with t = the global
  CEngineInterpolationInfo (0x0143B8E0) `InterpolationSetList[set].WFInterpolate` read into a named float first.
- **CEngineInternalPrimitiveBase** 00b8fdf0 AddToList (mask local first; walker declared in the `for`, head re-read as `*list` so the head
  rides in edx), 00b8fe60 RemoveFromList (straight PDB-member code; esi zero register).
- **CEngineInternalPrimitiveChanger** 00bc58e0 ClearChildPrimitive, 00bc5ae0 Update: handle Clear / counted copy with the release on the
  pointee and the old pointer in a local; the landed ComputeRenderLayerMask helper; descriptor = PDB CEnginePrimitiveChanger.
- **CEngineInternalPrimitiveDecal** 00bba290 Update: struct members copy through `lea`; Lit is bit 7 of an UNSIGNED SHORT bit field
  (`mov al ; shr al,7 ; and al,1` -- unsigned char drops the `and`, unsigned long loads the dword).
- **CEngineInternalPrimitiveFlareSprite** 00bcdfb0 Update: 15-argument CEnginePrimitiveSprite frame local (blend 4, white + flare alpha),
  qualified non-virtual `CEngineInternalPrimitiveSprite::Update(sprite, flags)`.
- **CEngineInternalPrimitiveLight** 00b453d0 Update: byte colour -> float colour through an inline `Set(r, g, b, a)` (arguments evaluated
  right to left: A, B, G, R) storing straight into the member; the Radius copy is written before it.
- **CEngineInternalPrimitiveLine** 00bc7d40 GetWorldPosition: inline C3DVector operators, `(end - start) * 0.5f + start`.
- **CTCHeroCentre** 0077ee70 (manifest "~CEngineInternalPrimitiveMeshBase"): destructor with no vtable store = no virtual in the model;
  STLport `_Destroy(first, last)` by value (finish cached in ebx), `if (start) free(start)`, CDefPointer release, out-of-line ~CTCBase.
- **CEngineInternalPrimitiveMeshBase** 00beb5c0 GetFadeDistance: 8-byte result with a user ctor by hidden pointer; ONE result local
  written field by field (a constructed temporary adds a frame copy).
- **CEnginePrimitiveHandle** 0043f9b0 operator= (manifest "ParticleGroup::SetChildPrimitive"); **CEngineInternalPrimitiveParticleGroup**
  00bbc310 SetChildPrimitive, 00bbc3a0 GetChildPrimitiveHandle: the handle assignment = Clear + counted copy, all `__forceinline`.
- **CPhysicsMeshObject** 00a11ba0 GetBoundingBoxZEdge, 00a11c90 GetBoundingBoxYEdge (manifest "ParticleLine::GetWorldPosition"):
  `PositionMatrix.Rotate(C3DVector(0, 0, half))` with the landed Rotate (the two zero terms fold to `(a + b) * 0.0f`).
- **CEngineInternalPrimitiveSprite** 00bacf00 GetBoundingSphereWorldSpace: `sphere = C3DBoundingSphere(position, sqrtf(w*w + h*h))`;
  C3DVector needs a memberwise user copy ctor so the centre is really copied into the frame temporary.
- **CEngineInternalPrimitiveStaticMesh** 00b9d8e0 Update: engine resource record through an inline accessor (global loaded before the
  argument), `ShadowMeshInfo = MeshInfo = ...` (MeshInfo stored first), `Size * 0.01f`, `__fastcall` CalcObjectMatrix.
- **CEngineInterpolationInfo** 0042fa30 ctor, 0069ed40 operator=(const CInterpolationInfo&), 0069ede0 converting ctor: PDB layouts
  (12-byte engine sets vs 8-byte game sets); first-try matches.
- **CEngineLandscapeMap** 00bdc450 Clear (CCountedPointer reset through an inline member keeps the zero stores in place),
  00bdc3c0 OnMapVisibilityChanged (`if (Patches) { delete[] Patches; Patches = 0; }` -- the store sits in the non-null arm here).
- **CEngineLandscapeMemoryManager** 00bd9b90 FreeVirtual (`delete g` through a class `__fastcall operator delete`), 00bda360
  AllocateIndexBuffer / 00bda3d0 AllocateVertexBuffer (class operator new = landed AllocVirtual body; inlined vertex-buffer ctor stores
  the vtable before PVertexBuffer/Stride, so those are derived-ctor body assignments).

## Parked for Fable 5.1 (10)
- 00b9e4e0 CEngineInternalPrimitive2DStaticMesh::GetBoundingSphereWorldSpace DIFFER(130v130): `fld [ecx+58h] ; fmul [edx+34h]` vs retail `fld [edx+34h] ; fmul [ecx+58h]` (radius * scale tail only) | tried: r*s, s*r, float local `*=`, float-returning inline helper, `*r = R; *r *= s` (radius first but an extra fst) | next: lander pragma sweep; else lane gap: x87 operand order
- 00b9dbd0 CEngineInternalPrimitiveStaticMesh::GetBoundingSphereWorldSpace DIFFER(130v130): same radius tail as 00b9e4e0 (byte-identical twin) | tried: as 00b9e4e0 | next: as 00b9e4e0
- 00be2120 CEngineInternalPrimitive2DViewport::ClearChildPrimitive DIFFER(107v106): `mov ecx,[edx+eax*8+4] ; test ; push esi ; lea esi,[edx+eax*8] ; mov eax,ecx` vs retail `push esi ; lea esi,[edx+eax*8] ; mov eax,[esi+4] ; test eax,eax` | tried: reference, pointer, iterator-object and plain-pointer element access, bool ClearIfSet helper | next: lane gap: vector slot address materialisation (GOTCHAS 24l)
- 00bc62d0 CEngineInternalPrimitiveGroup::ClearChildPrimitive DIFFER(98v97): same as 00be2120 (vector at +0x50) | tried: reference, iterator local, ClearIfSet | next: lane gap: vector slot address materialisation
- 00b8ff00 CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase DIFFER(102v102): retail stores +0x1C/+4 before the vtable and +0x0C/+0x10/+0x20/GridPos after it, bitfield read-modify before Alpha; ours schedules them across | tried: all initialisers, all body assignments, OcclusionLayerBitMask initialiser + body | next: try GridPos/FadeDistance as member objects with ctors and the zero members in an embedded struct (24i rule)
- 00bbce10 CEngineInternalPrimitiveParticleGroup::ClearChildPrimitive DIFFER(175v154): `lea ebx,[ebp*8]` (id*8 CSE, id in ebp) vs retail id in edi with `[ecx+edi*8]` re-formed and this in ebx | tried: plain, inline helper (not inlined), __forceinline ReleaseID (fixes the arg-slot store), iterator-object operator[] | next: lane gap: scaled-index CSE / register allocation
- 00bc7db0 CEngineInternalPrimitiveLine::GetBoundingSphereWorldSpace DIFFER(141v154): sum X `fld [ecx+54h]` vs retail `fld [ecx+48h]`; retail copies the centre's Z through edx into a frame word and spills dX to [esp] before the length | tried: member operators + Length, `(end+start)` + named centre copy, by-value operator- operand, named offset local | next: a by-value C3DVector taken from `sphere.Center` after its stores (re-read from the argument)
- 00a11c20 CPhysicsMeshObject::GetBoundingBoxXEdge DIFFER(108v103): `sub esp,0Ch ; fstp [esp] ; ... ; fld [esp]` vs retail `push ecx ; fst [esp] ; fxch` | tried: temporary vector, by-value Rotate, named half-width float, no user copy ctor | next: lane gap: frame trimming of a partly-constant temporary (the Y/Z twins match)
- 00b83750 CEngineInternalPrimitiveAnimatedMesh::GetBoneScalings DIFFER(167v155): second `find` into a temp + `mov eax,[eax] ; mov [esp+0Ch],eax` (8-byte frame, ebx = this) vs retail one frame word reused and edi = this, ebx = mesh pushed late | tried: key local + reassigned iterator, two scoped lookups, user iterator copy/assignment, long key converted at the call | next: lane gap: call-result temporary coalescing
- 006a2370 CTCCarrying::OnSetInLimbo DIFFER(97v97): vector start re-read from the frame for `first` and for the destructor vs retail `mov esi,[esp+0Ch]` once (push esi frame) | tried: direct begin()/end(), begin() local, for_each defined above under auto_inline(off) | next: lane gap: local cached in esi across a call (GOTCHAS 24k)

## Skipped
None: no row spans two functions.

## Label corrections
- 0x0043F9B0 "CEngineInternalPrimitiveParticleGroup::SetChildPrimitive" -> CEnginePrimitiveHandle::operator=(const CEnginePrimitiveHandle&).
- 0x00A11BA0 / 0x00A11C20 / 0x00A11C90 "CEngineInternalPrimitiveParticleLine::GetWorldPosition" -> CPhysicsMeshObject::GetBoundingBoxZEdge /
  XEdge / YEdge (PDB names; callers at 0x004BC9A5.. use the three in a row).
- 0x0077EE70 "CEngineInternalPrimitiveMeshBase::~CEngineInternalPrimitiveMeshBase" -> CTCHeroCentre::~CTCHeroCentre (its `??_G` 0x0077EE50
  is slot 0 of vtable 0x0126AF94).
- 0x006A2370 "CEngineLandscapeMap::RenderBackground" -> CTCCarrying::OnSetInLimbo (vtable 0x012433F4 slot 16).
- 0x00AC6DC0 "CEngineInternalPrimitive2DStaticMesh::GetObjectTransform": `this` is a double 3x3 matrix, not the PDB class (owner unknown).
- 0x00AF70D0 / 0x00AF7170 "CEngineInternalPrimitive2DViewport::ClearChildPrimitive": owner is not the viewport (handle vector at +0x90 /
  +0x68 with a parallel POD-vector array); no xrefs.
- Callees: 0x006A2220 "COptimisedPrimitive::RemapGroups" -> CTCCarrying::GetAllCarriedThings; 0x00528720 "_For_each<CTargetPoint*,..>" is a
  byte-identical SGI for_each; 0x00A24280 "CTCInventoryAbilities::GetAbilityLevel" is called as the engine mesh-bank lookup;
  0x009F1F10 "CWorld::DrawGetEnvironment" is the CMemoryAllocator allocation-count query; 0x0084ACE0 "list<CTCBase*>::remove" serves
  list<unsigned long>; 0x00B83520 "RBTree_LowerBound" is map::find; 0x00686830 "~CChunkedFileChunk" is CTCBase::~CTCBase.
