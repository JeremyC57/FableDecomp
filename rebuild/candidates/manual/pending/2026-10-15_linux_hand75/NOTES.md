# Hand chunk 75 (CShaderRenderManager .. CSkeletalMorphResourceManager): 34 of 40 byte-exact

Authored and graded on the Linux Wine lane (DDK 13.10.4035 cl.exe) with `q.py`. All 34 `src/` files grade
MATCH/RELOCATION_MATCH in `grades.txt`. All 34 tests PASS under g++ (`tall.sh`) and under the lane's VC7.1 (`tall.py`).
`payload.json` and `oracle.tsv` come from `finish.py`. No row needed a `// Flags:` line. Two rows carry
`#pragma optimize("s", on)` in the source (004e069f, 00454b5f).

## What matched (34)
- **CShaderRenderManager** (10 at 0x0098xxxx): UpdateWorldTransform 00988e30 (`count < 4 ? count : 4`, with the
  clamp arm placed after the epilogue), SetShadowTextureTransform 009896b0 (a member forwarder, `call ; ret 4`), and
  SetUserVertexShaderConstants 009896d0. Seven one-vector uploads match only through an **inline helper
  `SetVertexShaderConstant(reg, x, y, z, w)` that takes the register FIRST and the four components by value**:
  UpdateAmbient 00989700 (really CameraPos with w = 0), SetShadowFadeTransform 00989910, SetShadowFadeFactors
  00989960, and SetUserVertexShaderConstant 00989a60 / 00989ab0 / 00989b00 / 00989b50. Writing the frame vector
  inline puts the register push late in the schedule. So do a device local, a register local, a `float[4]`,
  `/Oa`, and the landed twin 0x00989760's own source, which DIFFERs in the lane. Layout:
  `engine/CVertexShaderConstantLayout.h` matches retail. Its 12-byte CShaderConstantRange members are read as
  {vptr, Offset +4, Count +8}.
- **0x00C4 setters** (manifest says CShaderRenderManager): SetProjection 00c40830 (a 27-dword block copied with
  `rep movsd`, `or [eax+78h],0FFFFFh`, then the global flags word through load/or/store) and CRangedObject::SetRange
  00c417a0 (the plain source reproduces retail's load/or/store flag update).
- **CShaderResource**: CloseShaderBank 00a5d750 (`if (Shaders[i]) { delete Shaders[i]; Shaders[i] = 0; }` with an
  unsigned index); SetVertexShaderInput 00a5d8b0 (the implicit CVertexShaderInput `operator=`, then ONE inline
  `GetData()` written `Size() > 0 ? first : (T*)this`, because `!= 0` folds into `test edx,0FFFFFFF8h`); the
  constructor 00a5d870 and the 00a46f30 record pair. **Both constructors need STLport's real vector structure**:
  `vector(const allocator_type& = allocator_type())` -> `_Vector_base(alloc)` with the end-of-storage pointer in an
  `_STLP_alloc_proxy` over the empty allocator. That puts `lea ecx,[esi+18h]` / `mov ecx,edi` between the zero
  stores as retail does. A plain three-pointer class hoists it, which is the "24i lane gap". The script constructor
  00dbf190 (vtable 0x012D9E18) has two argument stores before the vtable. That is an inlined
  `__declspec(novtable)` intermediate base `CScriptPairBase(first, second) : m_second(second), m_first(first)`,
  plus the STLport array. This is the F15 family shape: on its own, the novtable base fixes the vtable placement and
  only the argument load order is left.
- **NShape::CShape::UpdateDisplacePoints** 00761200: `moved = true; **it += delta;` over the selected-point
  pointers, with the end pointer re-read from the member each iteration.
- **C3DAnimationBlendState::CSharedData** copy constructor 00a3d330: CIVCountedPointeeBase's implicit copy (its
  vtable, then IVRefCount), the intrusive pointer add-ref'ed through an inline `AddRef()` (`inc [eax+4]`), the
  CArray copy called out of line, and the counted pointer shared (`inc [edi]`).
- **CSharerParserOutputToDebugContextMapper** 006bab80: a static `__fastcall` function that returns a CEntry by
  value (hidden result in ecx): `CEntry entry; return entry;`. With no NRVO this gives a default build, an inlined
  member-wise copy and a destructor.
- **CShopItemDef::Copy** 004e069f (`engine/CShopItemDef.h` + `_Methods`): the base `operator=` (0x00431F10), Type,
  and the seven flattened CShopItem fields viewed as one record, so `push 7 ; pop ecx ; rep movsd` comes out under
  `optimize("s")`.
- **Mislabelled constructors** (also listed under Label corrections):
  - 00aa8360 CChunkedFileSuperChunk(const char*): the CChunkedFileChunk base inlined. `memset(ChunkType,0,5)` +
    `memcpy(ChunkType,type,4)` gives the `mov edi,ecx` destination copy.
  - 00c3c3b0 CVtable_012b4f74: a 28-byte record whose inline constructor is `memset(this,0,sizeof *this)`. That
    gives the `lea ecx,[esi+10h]` addressing; member-wise initialisers fold into esi.
  - 00454b5f CThingBuildingDef: `/Os` fldz floats in an inline-constructed record.
- **CSimVoice** constructor 00455e39: the CInitBaseClass base, then the vector map's inline constructor (three
  pointers and the dirty byte).
- **CSplineData_00b20b60::CopyFrom** 00b20b60 (manifest says CSimVoice::operator=): it returns nothing, copies two
  flag bytes in source order and makes two vector `operator=` calls.
- **CSimpleMemoryPool** (`engine/CSimpleMemoryPool.h` + `_Methods`):
  - ClearMemoryBlocks 00a5c600: one `FirstMemoryBlock = 0` after the if/else, which VC tail-duplicates into both
    arms.
  - Reset 00a5c790: the same inline keeps one shared store.
  - CalculateMemoryAllocatedByClient 00a5c850: the pool total minus `area->Size + 16` over the free list.
- **STLport `vector(n)` constructors** (manifest says CSimpleMemoryPool::New<...>):
  - 009d2af0: 8-byte elements filled from `T()`. Its single `ecx` zero register needs a **POD element whose `T()`
    temporary is value-initialised**; a constructor zeroing two members gives ecx and edx.
  - 00a6b680 / 00b2ec80: byte-identical 4-byte fill.
  - 00a6b620: a default-construct loop, `new (p) T`, which gives the ebx zero and the aligned loop.

  All four need `_Vector_base(n, allocator_type())` with the alloc proxy. A flat class schedules `mov esi,ecx`
  late.
- **CSkeletalMorphResourceManager::UnloadBoneConfigs** 00b93df0: STLport `_Rb_tree::clear()` inlined, then
  `m_record = SBoneConfigRecord()`. A constructor zeroing three members gives the three separate `xor` registers.

## Parked for Fable 5.1 (6)
- 00c40450 CTrackedTransform_00c40450::SetPosition DIFFER(79v75): retail `or dword ptr [ecx+48h],8000h` after the 12-byte copy; the lane emits `mov eax,[ecx+48h] ; or eax,8000h ; mov [ecx+48h],eax` interleaved with the third copy | tried: volatile flags, inline SetDirty, component-wise copy, CTrackedVector-typed member with operator!=, optimize("s")/("t"), /Oa, /GS | next: lane gap: RMW split by the scheduler
- 00c404a0 CTrackedTransform_00c404a0::SetDirection DIFFER(79v75): same as 00c40450 (`or [ecx+48h],10000h` split into load/or/store) | tried: the 00c40450 forms | next: lane gap: RMW split by the scheduler
- 0046313e CSimVoice::operator= DIFFER(43v43): retail `mov al,[edi+0Dh] ; mov [ebx+0Dh],al ; pop edi ; mov eax,esi`, the lane `mov dl,[edi+0Dh] ; pop edi ; mov eax,esi ; pop esi ; mov [ebx+0Dh],dl` | tried: explicit operator= with inline map op=, implicit (COMDAT) op=, STLport vector member / base for the map, optimize("s") | next: lane gap: epilogue scheduling of the last byte store
- 006808c0 CSimVoice::TransferBinaryOut DIFFER(61v73): after WriteLong, retail re-reads first/last for the byte count behind the GetData() join (`mov ecx,[esi+4] ; mov ebx,[esi] ; sub ; sar 3 ; shl 3`, ebx saved); the lane reuses the GetData count (`shl eax,3`) | tried: ternary / if-local / void* GetData, signed and unsigned Size, separate byte-count accessor, data local, iterator-object vector, STLport vector base | next: model the map as the real CVectorMap<..> template over the VC7.1 std::vector with checked iterators (the PDB names `_Vector_iterator`), or lane gap: cross-join CSE
- 00a5c570 CSimpleMemoryPool::CSimpleMemoryPool DIFFER(74v74): one instruction placement: retail `xor ecx,ecx` before `mov [esi+0Ch],eax`, the lane after it | tried: granularity in the init list with body assignments, everything in the body, everything in the init list | next: an inline SetGrowthGranularity / CArea constructor variant, else lane gap: scheduling
- 00a533f0 CSimpleMemoryPool::New<SSequenceKey> DIFFER(82v80): the lane keeps a dead `jb` after `test edx,edx ; ja` in the unsigned 64-bit size guard; retail drops it because `test` clears CF | tried: `<= 0xFFFFFFFF`, inverted `>`, `< 2^32`, a named 64-bit local, optimize("s"), /Oa, /GS | next: lane gap: dead branch after test (RTM peephole)

## Skipped
- none (no row spans two functions).

## Label corrections
- 00454b5f "CSimVoice::CSimVoice": CThingBuildingDef::CThingBuildingDef (vtable 0x01233144, base CThingBaseDef 0x006772D0).
- 00aa8360 "CShopItemDef::CShopItemDef": CChunkedFileSuperChunk::CChunkedFileSuperChunk(const char*) (vtables 0x0129E474 / 0x0129E4F0).
- 00c3c3b0 "CShopItemDef::CShopItemDef": CVtable_012b4f74 constructor (no-RTTI vtable 0x012B4F74, base 0x00C3C590).
- 00a46f30 "CShaderResource::CShaderResource": not CShaderResource (no vtable, no callers); a pair of {CCharString, vector} records.
- 00dbf190 "CShaderResource::CShaderResource": a script-module class constructor (vtable 0x012D9E18, CScriptBase base 0x00CB8110).
- 00b20b60 "CSimVoice::operator=": a void CopyFrom on a spline-data object (caller 0x00B22467); no `return *this`.
- 006bab80 "CSharerParserOutputToDebugContextMapper::AddEntry" (4 params): a static `__fastcall` CEntry-by-value factory.
- 009d2af0 / 00a6b620 / 00a6b680 / 00b2ec80 "CSimpleMemoryPool::New<C3DAnimationSequence>": STLport `vector<T>(n)` constructors (`this` is the vector).
- 00a533f0 "New<C3DAnimationSequence>": it is a CSimpleMemoryPool member (Allocate 0x00A5C910), but the element is an 8-byte {0, -1, 0} record, not C3DAnimationSequence.
- 00989700 "UpdateAmbient": uploads CameraPos (retail +0x380) with w = 0; the label is propagated from 0x00989760.
- 00c40450 / 00c404a0 "SetCameraPos", 00c40830 / 00c417a0 "SetProjectionTransform": not CShaderRenderManager. These are CTrackedTransform SetPosition / SetDirection, a projection-block owner, and CRangedObject::SetRange, following the landed callers 0x00C338C0 / 0x00C338F0 / 0x00C33F20.
- 0x00AB3B10 "CAIStateGroup_HoverHornetQueen" (callee): CVertexShaderInput::CVertexShaderInput (vtable 0x0129EA08).
