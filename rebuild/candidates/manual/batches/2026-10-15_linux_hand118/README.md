# Hand chunk 118 (C3DMeshFileTransformationChunk .. CAIBrain): 36 of 40 byte-exact

All 36 sources grade MATCH/RELOCATION_MATCH in `grades.txt` (gall.sh) and every behaviour test prints PASS under
g++ (tall.sh) and the lane's VC7.1 cl.exe (tall.py). Pragmas/flags: 00c6afc0 `/EHsc`; 004eb9e8 `optimize("s")`;
00645f8d (parked) `optimize("y",off)+("s",on)`; everything else default.

## What matched (36)
- **C3DMeshFile\*Chunk family** (shared retail model from the landed chunk sources: 0x2c C3DMeshFileChunk base,
  three-pointer CArray):
  - 00ab9ce0 C3DMeshFileTransformationChunk::CreateNewInstance -- `RegisterNewChunk(new T)` through factory slot 10;
    the landed default ctor (identity CMatrix3x4 via chained-assignment SetIdentity) inlines.
  - 00abbf30 C3DMeshFileTrianglesChunk::WriteToFile -- count, 3x WriteUShort (0x00993E80) per 6-byte triangle,
    super-chunk WriteToFile; `const T& t = Triangles[i]` loop strength-reduces like retail.
  - 00abbdf0 C3DMeshFileVerticesChunk::WriteToFile -- count + 8 WriteFloat per 32-byte C3DVertex2 (PDB layout).
  - 00aa4d80 C3DMeshFileXTextureIndexChunk::ReadFromFile -- the landed TransferBinaryIn inline memory-stream Read
    (64-bit range predicate, uninitialised local stored on the reject tails), engine header used.
  - 00aa58c0 / 00aa59c0 C3DMeshFileXVertexDataChunk::WriteToFile / CreateNewInstance -- version 2 + words +
    `__fastcall` params writer + `WriteData(GetData(), Size())`; factory with member-object ctors (scale 1.0f x4,
    bias 0, empty CArray) after the vtable store. Chunk type literal is "XVBD".
- **CShaderRenderManager** 0098a8e0 UpdateBoneMatrices (label fix) -- three Size() evaluations, one inline
  `GetData()`, and the device pointer as a named local INSIDE the `if` (else it loads after the select branch).
- **C3DMeshInfo** 00a24930 ~C3DMeshInfo -- each LOD slot `if (slot) slot = CIVCountedPointer();` (Release with inner
  null store + second null store), MeshData member dtor, inline empty base dtor storing CMeshDataBankEntry's vtable.
- **C3DMeshLODInfo** 00a24870 ConvertToMeshStats -- the landed Sizeof inlined (two ternaries + sizeof), resource
  list in a named local.
- **CHeroExperienceDef** 004eb9e8 (label fix) -- `/Os` ctor; the vectors must be the real STLport vector
  (`_Vector_base` + `_STLP_alloc_proxy`), three plain zero words schedule the first map's `lea ecx` too early.
- **CSoundOutput00c6afc0** 00c6afc0 (label fix, no-RTTI module) -- `/EHsc`; words before the vtable store are a
  `__declspec(novtable)` root's inline ctor (rate then channels, body order).
- **C3DMeshObject**: 004c3380 operator= (memberwise, inlined landed CCountedPointer::operator=, POD matrices via
  `rep movsd`); 00a35f80 GetBoundingCentre (the WHOLE sphere copied into the frame -> 16-byte frame, products read
  from the sphere local, named result + memberwise copy ctor); 00a36010 SetMesh (`Mesh = info->GetMeshStats()`:
  inlined counted operator= + the temporary's inline dtor).
- **C3DMeshVolume** 00a2e9a0 ctor(chunk, pool) -- Name =, `Reset(new CContainmentVolume)`, Initialise with the
  `Size() > 0 ? first : this` (`ja`) GetData idiom.
- **C3DOrientationInterpolator** 005508d0 ctor(start, end) -- two CRightHandedSet copies (memberwise C3DVector).
- **NCameraModes::CScriptModuleGoToLookInDirection** 005ed190 ctor (label fix) -- vectors copied, inline
  `Normalise()` = `1.0f / (float)sqrt(x*x+y*y+z*z)` with the sqrt intrinsic.
- **C3DPrimitive2**: 00a8e5c0 CalculateTextureChangeFlags (material array + PREVIOUS block's index as named
  locals before the block reference); 00a90700 ctor (root class: vtable first, init list + member-object ctors);
  00a90860 Sizeof (GetSizeof helpers accumulated statement by statement); 00a92870 UploadAnimatedBuffers
  (scoped_ptr detach+delete, blocks' pointers assigned an empty CIVCountedPointer).
- **CEnginePrimitiveSpriteTrail** 00b09f90 ctor (label fix) -- inline VC6 list ctor (malloc'd self-linked
  sentinel), OffsetVector as a member object with a zeroing ctor (body assignments sink below the matrix ctor).
- **C3DSkeletalMorph** 00a44eb0 dtor -- body loop clears the parameters' back pointers; counted-pointer member
  dtor (nulls kept) then the landed CRetailArray dtor (`_Destroy_range` + `free`).
- **C3DSkeletalMorphParameter**: 00a44d80 copy ctor (default SharedData then body operator=; VC keeps the release
  test on the known-null block); 00a45370 SetCurrentValue (inline `SignedFlag ? -1.0f : 0.0f` evaluated twice,
  immediate 1.0f clamp, inline counted Clear()); 00a455a0 ctor(target) (Reset(new CShared) with the inline CShared
  ctor; STLport CArray model again fixes the second name's `lea` scheduling).
- **C3DVector** 00a13c60 Rotate -- axis-angle CMatrix3x4 ctor, a copy of *this (kept on the x87 stack), products
  written component-first (`v.X * m.XAxis.X`).
- **CAC_\***: 00e33c90 / 00e33e00 Init -- `game->B(m_thing, game->A(name, 0))` (one vtable fetch, CScriptThing
  temporary dtor inline); 00e33660 OnPredicateFail -- `&&` over two IMPLICIT CCharString conversions (no EH: flag
  register, frame flag home).
- **CAFile** 00516180 / 00516d90 ReadVar<int>, 00516e10 ReadVar<unsigned char> -- the vector<unsigned char>(n)
  ctor (0x00514E20) DEFINED out of line earlier in the TU (non-template, `#pragma auto_inline(off)`): VC7.1 then
  keeps the buffer pointer in edi across the virtual Read and reuses it for the inline dtor's free.
- **CAIBrain** 0088b6f0 OnKill, 0088ba40 ForceReset, 0088bac0 IsReadyToRunStateGroupWithHigherScriptPriorityThan --
  local retail model (header 4 bytes high from +0x28); rotated pump loops, bool local `xor bl,bl`, landed
  update-frame arithmetic + list walk.

## Parked for Fable 5.1 (4)
- 00abbe90 C3DMeshFileTrianglesChunk::GetChunkInfo DIFFER(105v103): `mov eax,ecx ; mov ecx,[eax+30h] ; sub ecx,[eax+2Ch] ; imul ecx` vs retail `mov edx,[ecx+30h] ; sub edx,[ecx+2Ch] ; imul edx`, hidden result pointer loaded before the sprintf call instead of after | tried: plain sprintf+return, count local, optimize("s"), inline GetNumTriangles | next: lane gap: register allocation under /GS (try a `CCharString::Format`-style helper or an out-of-line count getter)
- 00a36df0 C3DMeshObject::GetEventTime DIFFER(113v113): `lea ecx,[esp] ; push ecx` (hidden result of GetAnimationStats) pushed before the GetInfo call instead of after it | tried: named stats local, full temp chain, animation-id local, info local | next: an inline accessor wrapping `GetInfo(id)->GetAnimationStats()` so the inner call completes before the hidden pointer is formed
- 00ac2160 C3DMeshPartitionElement::BuildSubElement DIFFER(123v125): null arm of `new` stores the zero register `mov [ebx+ebp*4+20h],edi` where retail does `xor eax,eax ; mov [..],eax` | tried: plain new, element local, placement-new ternary | next: lane gap: zero-register coalescing at the new-null merge (one attempt left: a non-inline factory helper defined earlier in the TU)
- 00645f8d NPlayerGui::CStatBar::ClearGraphics DIFFER(124v131): `lea ecx,[eax+ebx*8]` per use vs retail `mov edi,ebx ; shl edi,3 ; lea ecx,[edi+eax]` / `add ecx,edi` (scaled index kept in edi across the calls) | tried: `*(begin() + i)` with inline begin(), long/int index, iterator object | next: lane gap: RTM vector slot address materialisation (wave 24l), or a byte-offset loop variable

## Skipped
- none (no row spans two functions).

## Label corrections
- 0098a8e0 "C3DMeshFileXUVTangentChunk::WriteToFile" -> CShaderRenderManager bone-matrix upload (callers: PrepareForRender
  0x009E12FA, the water renderers; between SetBoneTransform and CreatePixelShader); named UpdateBoneMatrices.
- 004eb9e8 "C3DMeshMaterial::C3DMeshMaterial" -> CHeroExperienceDef::CHeroExperienceDef (vtable 0x0124390C).
- 00c6afc0 "C3DMeshMaterial::C3DMeshMaterial" -> constructor of a no-RTTI audio object (2 ch / 48 kHz), member +0x2f8
  of the object built at 0x00C54090; named CSoundOutput00c6afc0.
- 005ed190 "C3DPositionInterpolator::C3DPositionInterpolator" -> NCameraModes::CScriptModuleGoToLookInDirection ctor (0x01255F9C).
- 00645f8d "C3DPrimitive2::GetUsedBones" -> virtual slot 1 of NPlayerGui::CStatBar (0x0125A738) and three sibling
  stat-bar vtables; named ClearGraphics.
- 00b09f90 "C3DPrimitive2::C3DPrimitive2" -> CEnginePrimitiveSpriteTrail ctor (member +0xac of CPSCSingleSprite).
- Callees: 0x009FC440 CTCActivationTrigger::OnKill -> CResource::RemoveFromList; 0x00A242C0 GetAbilityLevel ->
  animation bank info lookup; 0x00A4EAE0 CRegion::SetMiniMapRegionExitTextOffsetY -> C3DAnimationStats::GetEventTime;
  0x00A752D0 C3DGroup2 -> CContainmentVolume ctor; 0x00A46790 CCombatWheel::operator_assign ->
  CArray<C3DSkeletalBoneMorph>::operator=; 0x0088B910 CEngineWorldMap::BuildAndSaveSea -> CAIBrain::GetUpdateZone;
  0x008FCF40 / 0x008FCF50 CXMVPlaybackInfo -> CAIStateGroupBase::Terminate / Activate; 0x00C77770
  CGameEventPackage::Clear -> a 0x2c-byte record ctor; 0x00C6A650 CTCKraken -> audio stream ctor; 0x00514E20
  std::vector::_Init -> vector<unsigned char>(size_type); 0x0044C0C0 CShoveHaPennyRegionDef -> definition base ctor;
  0x0043F360 _For_each<pair<long,CCharString>..> is a COMDAT-folded for_each body (here CEnginePrimitiveHandle + mem_fun_ref).
