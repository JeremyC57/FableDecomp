# Hand chunk 136 (CEnginePrimitiveManagerAnimatedMeshesBase .. CEngineSceneGrid): 32 of 40 byte-exact

## What matched (32)
- **CEnginePrimitive\* constructors** (0x00B1FE80 ParticleDecal, 0x00AF08D0 ParticleGroup, 0x00B058E0 ParticleMesh, 0x00AFE2A0 ParticleSprite, 0x00AFE220 ParticleLine, 0x00642000 Light, 0x007324A0 Line, 0x0049D000 WeatherMask, 0x004C3120 Text, 0x006558CE Sprite): one shared model. The base `CEnginePrimitive(type, interpolationSet = 0)` stores the type, the set and three zero channel bytes. C3DVector/C2DVector need a **memberwise user copy constructor** (one register per dword; the implicit POD copy goes through `lea` + esi). CRGBColour's copy constructor is the landed `Set(r, g, b, a)` shape (bytes loaded A, B, G, R, B spilled into a dead argument slot, stored R, G, B, A). The 0x30-byte CMatrix3x4 stays a POD (`rep movsd`). The PDB signatures (`??0CEnginePrimitiveX@@QAE@_K...`) give exact parameter orders. The leading `_K` 64-bit handle is never stored. The Sprite constructor is `optimize("y", off)` + `("s", on)`: the two `fldz ; fstp` Offset zeroes are the size optimiser's choice over 7-byte immediates. Text calls the out-of-line CWideString copy constructor.
- **CHandleBasedMemoryPool::CHandleBasedMemoryPool** (0x00A02D20): the vtable 0x0129C964 identifies the class. The base CBase constructor is out of line, the init-list zeroes come from one register, the retail CArray is 12 bytes (FirstUnusedHandle -1 at +0x444), and the body does `memset` of FreeLists (`rep stosd`) plus the defrag defaults.
- **CEnginePrimitiveRenderer**:
  - ResetWorldTransform (0x00B32100): an identity matrix local (landed SetIdentity chained assignment) passed to two global managers.
  - FrameEnd (0x00B32820): `this` sits at the secondary base (+0x268, vtable 0x012A1348 slot 2), so offsets are negative or small (-0x1d0, +0xc, +0xd0). There are three loops: an iterator loop with `end` re-read, then two indexed `GetCount()` loops.
  - AddPrimitive (0x00B32E90): a CEnginePrimitiveHandle temporary with inline constructor and destructor. The destructor does `IsUnique()` then slot 3, then the counted-pointer release through `DecRef(); if (GetRefCount()==0) Destroy();` (the `mov esi,[ecx+4] ; lea eax ; dec esi ; mov edx,esi` shape), one zero store, and the out-of-line base destructor.
- **list<CEnginePrimitiveHandle>::resize(n)** (0x00BBDCB0, catalogued as AddPrimitive): the same handle temporary, as `resize(n, T())`.
- **CTCInGameMenu::IsSaveComponentReceivingInput** (0x0047D6F2): `optimize("y", off)` + `("s", on)`. A frame-local list built by its out-of-line allocator constructor, a by-value CCharString built in its slot, an index loop with `*(begin()+i)`, and an early `return true` that destroys the list.
- **CreateInternalPrimitive** (0x00BBCEB0 ParticleGroup, 0x00BBEA70 SpriteTrailGroup, 0x00BC68B0 Text):
  - The group constructor (two STLport lists with local self-linked nodes, the CArray, the sphere) must be `__forceinline`. The trail group stores its own vtable last.
  - Text is a two-case `switch` (5 / 0x27) with the inline text constructor chain.
- **RenderPrimitive / RenderSubPrimitive**:
  - Decal 0x00BBA340: CEngineSubPrimitiveBase is `__declspec(novtable)` (its Parent/Next stores come before the derived vtable). The pool allocator is `static void* __fastcall operator new` (size in ecx). The distance is X, Y, Z differences summed `dx*dx + dy*dy + dz*dz`. The signature mangles exactly with `ERendered`/`EEngineRenderLayer` enums.
  - AnimatedMeshesBase 0x00BD7110: the float time is a named local homed in a dead argument slot. The cloth primitive must be reached through the counted pointer's `operator->`, because `.Data->` loads the index into edx instead of eax.
- **CEnginePrimitiveManagerParticleGroup::SetUpPointLights** (0x00BBC0B0):
  - `#pragma pack(1)` with explicit pads for the unaligned CurrentWorldPos (+0xe).
  - The channel description is a named local assigned from a default-constructed temporary, which gives the retail self copy `mov eax,[esp+4] ; mov [esp+4],eax`.
  - The count comes back with `movsx`.
- **~CEnginePrimitiveManagerFlareSprites** (0x00BCE160): members are destroyed in reverse:
  - the CVertexShader: vtable, then its data released through a **pointer local** (`dec [ecx+10h]`) with a virtual delete, then the CShaderBase vtable;
  - the STLport tree: clear() only when the count is non-zero, then a null-tested free of the header;
  - CBase::~CBase as a tail jump, with CEnginePrimitiveManager `novtable`.
- **UpdateRepeatedZSprite** (0x00BAA780): statement-order assignments. The sphere and box are POD records (re-based pointer copies).
- **UploadShaderConstants** (0x00B99670): engine headers for both classes. `mult / fadeScale` is passed as a float argument, followed by two SetVSConstants calls.
- **CEnginePrimitiveSimulationInfo::AddSimulation** (0x00B82330): `size() > index+1 ? size() : index+1` (re-evaluated in the arm), then `resize(n, CCountedPointer())`, then the landed CCountedPointer::operator= (0x0084F130 family).
- **CEnginePrimitiveTypeInfo::RegisterPrimitiveManager** (0x00B8FAD0): engine header. A local STLport vector filled by virtual slot 2, then an index loop.
- **CEngineResourceManager**:
  - ProcessLoading (0x00B3B7F0): engine header MeshBanks. The `GFGetSystemManager()` result must go into a **system-manager local**, so that the call precedes the argument pushes and the member loads straight into ecx.
  - ResizeHiresTextureMemory (0x00B3B4F0): retail bank offsets +0x954/+0x958, so the class is modelled locally. A `__fastcall` CalculateTextureSize, the saved bank level in ebp, and the bool in bl.
- **CEngineScene::RenderPrimitiveList** (0x00B91760): the manager pointer must be read into a local **before** the sub-primitive chain local is declared, so VC sinks the chain's zero stores below the pushes.
- **CEngineSceneGrid::InvalidateBoundingBoxAndDrawDistance** (0x00B91CA0): a recursive nested 2x2 loop. The outer counter is homed in the dead `bool` argument slot, and the bit-field clear compiles to `and byte ptr [..],0FEh`.
- **list<24-byte POD>::operator=** (0x00471B73, catalogued as AssignCellBoundingBoxes): STLport `operator=` verbatim under `optimize("y", off)` + `("s", on)`. Iterators are copy-constructed in their argument slots, const iterators are pushed as plain records, and the erase result is homed in the dead argument slot.

## Parked for Fable 5.1 (8)
- 007703b0 CEnginePrimitiveWeaponTrail::CEnginePrimitiveWeaponTrail DIFFER(144v144): retail `lea ebx,[esp+28h]` homes the empty `__false_type` tag over the LIVE alpha argument slot; the lane uses the dead interpolation-set slot `[esp+3Ch]`, and the 8 member stores after the call are scheduled differently | tried: init-list order, local self-linked node, iterator copy ctor (fixed the slot build) | next: lane gap: empty tag over a live argument slot (the 0x005384DD / 0x00A76C70 family)
- 00a0e680 UninitCopyDeque (catalogued CEnginePrimitiveRenderer::AddStaticMesh) DIFFER(118v116): retail `lea edx,[esp+58h]` homes the const-ref tag in the dead `result` slot; the lane adds `push ecx` + `lea edx,[esp+37h]` + `pop ecx` | tried: user-constructed tag (best), plain empty tag (adds a zero store), optimize("s") (frame pointer) | next: lane gap: empty tag homed in a dead by-value class argument slot
- 00a0eb40 UninitCopyDeque (catalogued AddStaticMesh) DIFFER(118v116): same as 00a0e680 (callee 0x00A0E790) | tried: as 00a0e680 | next: lane gap: same
- 00a0ef90 UninitCopyDeque (catalogued AddStaticMesh) DIFFER(118v116): same as 00a0e680 (callee 0x00A0ED80) | tried: as 00a0e680 | next: lane gap: same
- 00bc7920 CEnginePrimitiveManagerText::GetSupportedTypes DIFFER(122v122): retail `mov [esp+0Ch],27h` puts the second pushed value in the dead vector-argument slot, shared with the tag; the lane uses the frame word `mov [esp+4],27h` | tried: literal temporaries (best), named locals (8-byte frame) | next: lane gap: value/tag sharing a dead argument slot
- 00bb6b50 CEnginePrimitiveManagerWeatherMask::RenderSubPrimitive DIFFER(136v136): retail pipelines the ten sphere/box dword loads one store ahead and loads Flags before the matrix `rep movsd`; the lane copies sequentially | tried: member-initialising ctor (float by value -> x87; const float& best), default ctor + struct assignments, memberwise C3DVector operator= | next: lane gap: scheduling only
- 00bb6ab0 CEnginePrimitiveManagerWeatherMask::RenderPrimitive DIFFER(152v152): retail squares Y, Z, then X (`fld st(1) ; fmul st(2) ; fld st(1) ; fmul st(2)`); the lane squares Z, Y, X | tried: x+y+z, x+(y+z), y+z+x, by-value Magnitude(x,y,z) (the same source matches sibling 0x00BBA340) | next: lane gap: x87 operand order (or an unidentified helper that keeps Y on top)
- 0042dd28 CNewFrontendGameComponent::CleanUpInterfaces (catalogued CEngineResourceManager::ChangeScene) DIFFER(139v139): retail zero register ebp and manager local ebx; the lane swaps them | tried: speed TU, optimize("s") (best), no manager local, function-scope manager local | next: lane gap: callee-saved register assignment

## Skipped
- None. No row spans two functions.

## Label corrections
- 0x00642000 "CEnginePrimitiveShadowedSpotLight" is **CEnginePrimitiveLight::CEnginePrimitiveLight** (type 0x0f; PDB signature `_K, C3DVector&, CRGBColour&, M, M, interp, K`).
- 0x00AFE220 "CEnginePrimitiveShadowedSpotLight" is **CEnginePrimitiveParticleLine::CEnginePrimitiveParticleLine** (type 0x15).
- 0x007324A0 "CEnginePrimitiveParticleLine" is **CEnginePrimitiveLine::CEnginePrimitiveLine** (type 0x10; retail object 0x38 bytes without UseZBuffer).
- 0x00A02D20 "CEnginePrimitiveTypeInfo::CEnginePrimitiveTypeInfo" is **CHandleBasedMemoryPool::CHandleBasedMemoryPool** (vtable 0x0129C964). The landed derived pools 0x00A62E20 and 0x00A148D0 call it as "CEnginePrimitiveTypeInfo"; their base is CHandleBasedMemoryPool.
- 0x00A0E680 / 0x00A0EB40 / 0x00A0EF90 "CEnginePrimitiveRenderer::AddStaticMesh" are SGI `uninitialized_copy` over deque iterators (`__fastcall`, hidden result in ecx).
- 0x00BBDCB0 "CEnginePrimitiveRenderer::AddPrimitive" is STLport **list<CEnginePrimitiveHandle>::resize(size_type)**. Its only caller is ClearAllChildPrimitives 0x00BBD200 on the list at +0x48.
- 0x0047D6F2 "CEnginePrimitiveRenderer::RenderNonsortedPrimitives" is a **CTCInGameMenu** "COMPONENT_SAVE" input query. The name IsSaveComponentReceivingInput is descriptive (no PDB match).
- 0x0042DD28 "CEngineResourceManager::ChangeScene" is a **CNewFrontendGameComponent** member. CleanUpInterfaces is the probable PDB name; it is called from Shutdown 0x0042EBE6.
- 0x00471B73 "CEngineSceneGrid::AssignCellBoundingBoxes" is STLport **list<24-byte record>::operator=**, called from a memberwise assignment at 0x00471AA4.
- Callee labels:
  - 0x0099A2F0 / 0x0099A300 "CBase" are CBase ctor/dtor (root vtables 0x0129A7C4 / 0x01231710).
  - 0x00B94A30 "_Dest_val" is CEngineScene::FrameEnd.
  - 0x00B93DF0 "UnloadBoneConfigs" is a scene list reset.
  - 0x00A0B140 "SetViewTransform" is likely CRenderManagerCore::SetWorldTransform.
  - 0x009D68B0 "_Dest_val" is a CMeshDataBank update.
  - 0x00B2AD80 / 0x00B2AD90 "_Cons_val" / "GiveTutorialForCategory" are CEngineSurfaceCompositionManager methods.
  - 0x00BBDC60 "LinkedList_AddN" is list::resize(n, x).
  - 0x0076FD40 "LinkedList_InsertAll" is the STLport list range insert.

## Header policy (for the lander)
- `this` uses the engine header where the retail layout agrees: 0x00B32100, 0x00B32E90, 0x00BBEA70, 0x00BAA780, 0x00B99670, 0x00B8FAD0, 0x00B3B7F0.
- Where the retail layout differs from the PDB-generated header, the class is modelled locally and the reason is written in the source header: CTCInGameMenu, CHandleBasedMemoryPool, CEngineResourceManager +0x954, CEnginePrimitiveRenderer secondary base, CEngineInternalPrimitiveParticleGroup, CEnginePrimitiveManagerFlareSprites.
- Default (non-strict) mode prints SHARED_HEADER_AVAILABLE warnings for helper/callee structs only: CRenderManagerCore (00b32100), CEngineComponent (00b32820), CEngineInternalPrimitiveRepeatedZSprite (00baa780: its base fields are opaque pads in the header), CEngineCamera (00bba340: WorldPosition is inside an opaque CameraDesc).
