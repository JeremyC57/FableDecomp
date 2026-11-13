# DOSSIER 0x00A91640 -- C3DPrimitive2::InitialiseFromMeshFileData (4344 bytes)

Normalised listing (every `[esp+X]` rewritten to `[B+X]`, B = esp right after the prologue's 4 pushes; args at B+0xb4..):
`$W/dis_norm.txt` (script `$W/norm.py`). Use it with OUTLINE.md.

## 1. Retail shape
- `sub esp,0A0h ; push ebx ; push ebp ; push esi ; mov ebx,ecx ; push edi` -- NO frame pointer (/Oy, base /O2 /Oy /W3),
  frame 0xA0, saved order ebx, ebp, esi, edi. `this` lives in ebx (also spilled to B+0x58 at 0xA9164C).
- NO SEH prologue, no /GS cookie, despite ~10 CCountedPointer locals with destructors => TU built WITHOUT /EHsc (the
  lane default). Every early `return false` replays the destructors of the live locals inline.
- `ret 14h` (thiscall + 5 dword args). Returns `bool`: `mov al,1` (0xA926B8) / `xor al,al` (0xA9272C).
- Two epilogues: success 0xA926B5, failure 0xA92729. Cold block after the success `ret`: 0xA926C4..0xA92726 (size-guard
  failure cleanup) that falls into the failure epilogue.
- Not size-optimised (no `/Os` idioms); FPU code is the plain /O2 x87 form.

## 2. Identity (manifest label `_gapscan::sub_00a91640` is WRONG -- generic gap-scan placeholder)
- Name: `C3DPrimitive2::InitialiseFromMeshFileData`
- Mangled: `?InitialiseFromMeshFileData@C3DPrimitive2@@QAE_NABVC3DMeshFilePrimitiveChunk@@_NKKAAVCScopedMemoryHandle@@@Z`
- Signature: `public: bool __thiscall C3DPrimitive2::InitialiseFromMeshFileData(const C3DMeshFilePrimitiveChunk& chunk, bool b, unsigned long reps, unsigned long initFlags, CScopedMemoryHandle& memoryHandle)`
  (parameter names unknown in every source -- descriptive).
- Evidence (3 sources): ghidra_out/egor_pdb_names.tsv `00bd39b0 ?InitialiseFromMeshFileData@C3DPrimitive2@@...` (Ego_r);
  ghidra_out/_fablewin_api.tsv `032f5dd0` same mangled name (FableWin debug, 6131 B, in the C3DPrimitive2 block 0x032F5A80..);
  retail shape: `ret 14h` = 5 args; arg5 is passed as ecx to `CScopedMemoryHandle::Initialise` 0x00A039F0 (landed,
  whose header comment already says "Caller 0x00A91BA5"); arg1 is used as a C3DMeshFilePrimitiveChunk (PeekVertices,
  FindFirstTriangles/Group/Cloth); first call is C3DPrimitive2::FreeData 0x00A901E0 on `this`; it calls the sibling
  N3DPrimitive::CAnimatedBlock::InitialiseFromMeshFileData 0x00A90AE0. Landed rows FreeData 0x00A901E0 and
  AddMirroredTextureVertexIndex 0x00A90510 already name 0x00A91640 / 0x00A92476 as "the C3DPrimitive2 loader".
- Caller: 0x00A8C2C4 (one call; in the C3DMesh2 TU, unlanded).
- Arg use: arg1 B+0xb4 chunk; arg2 B+0xb8 bool (CalculateVertexSize's 2nd arg, CAnimatedBlock::InitialiseFromMeshFileData's
  bool, SetupVertexStreamFlags' bool); arg3 B+0xbc -> RepeatingMeshReps (+0x08); arg4 B+0xc0 -> InitFlags (+0x68);
  arg5 B+0xc4 CScopedMemoryHandle&.

## 3. Callees (retail addr | landed file | landed spelling | PDB name / remark)
| addr | landed row | landed spelling | PDB / notes |
|---|---|---|---|
| 0x00A901E0 | 00/a9/C3DPrimitive2_FreeData_00a901e0.cpp | C3DPrimitive2::FreeData() | same (Ego_r FreeData) -- OK |
| 0x00BFEA1A | global_operatornew | operator new | -- |
| 0x00A98100 | 00/a9/N3DPrimitiveCDeviceIndependent_CDeviceIndependent_00a98100.cpp | N3DPrimitive::CDeviceIndependent::CDeviceIndependent() | ??0CDeviceIndependent@N3DPrimitive@@QAE@XZ OK |
| vtbl slot0 (push 1) | -- | scoped_ptr reset deletes old Data via scalar deleting dtor | ??_ECDeviceIndependent |
| 0x00AA7F40 | 00/aa/CChunkedFileSuperChunk_FindFirstSubChunkOfType_00aa7f40.cpp | `CChunkPointer FindFirstSubChunkOfType(const char*)` (non-const) | **NAME FIX**: 0x00AA7F40 is the CONST overload `?FindFirstSubChunkOfType@CChunkedFileSuperChunk@@QBE?BV?$CCountedPointer@VCChunkedFileChunk@@@@PBD@Z` (Ego_r order QBE First, QBE Next, QAE First, QAE Next = retail 0xAA7F40/0xAA7FF0/0xAA80E0/0xAA8190; PeekVertices row 0x00ABAFE0 already says so). Called 8x here (chunk is const&). |
| 0x00AA7FF0 | 00/aa/..._FindNextSubChunkOfType_00aa7ff0.cpp | non-const spelling | **NAME FIX**: const `?FindNextSubChunkOfType@CChunkedFileSuperChunk@@QBE?BV?$CCountedPointer@VCChunkedFileChunk@@@@ABV?$CCountedPointer@$$CBVCChunkedFileChunk@@@@@Z` |
| 0x00AA7EA0 | 00/aa/CChunkedFileSuperChunk_CountSubChunksOfType_00aa7ea0.cpp | `long CountSubChunksOfType(const char*) const` | PDB `?CountSubChunksOfType@CChunkedFileSuperChunk@@QBEKPBD@Z` returns **unsigned long** (landed says long) |
| 0x00A910E0 | 00/a9/C3DPrimitive2_CalculateVertexSize_00a910e0.cpp | static __fastcall CalculateVertexSize(unsigned long, bool, bool) | ?CalculateVertexSize@C3DPrimitive2@@SIKK_N0@Z OK |
| 0x00A8EDA0 | 00/a8/C3DPrimitive2_SetupVertexBuffer_00a8eda0.cpp | `bool SetupVertexBuffer(long,long,unsigned long,unsigned long)` | PDB `?SetupVertexBuffer@C3DPrimitive2@@AAE_NW4EVertexType@@KKK@Z` (private; arg1 EVertexType, landed uses long) |
| 0x00AC20A0 | 00/ac/CVertexBuffer_Lock_00ac20a0.cpp | generic `T` struct | CVertexBufferWin32 lock (non-virtual wrapper tail-jumping slot 1, 3 args, returns the data pointer). Landed spelling is a placeholder struct |
| 0x00AC20B0 | 00/ac/CVertexBuffer_Unlock_00ac20b0.cpp | generic | Unlock (slot 2) |
| 0x00ABAFE0 | 00/ab/C3DMeshFilePrimitiveChunk_PeekVertices_00abafe0.cpp | PeekVertices() const | OK |
| 0x00ABC420 | 00/ab/C3DMeshFileGroupChunk_GetList_00abc420.cpp | C3DMeshFileGroupChunk::GetList | ICF fold; here it is `C3DMeshFileVerticesChunk::GetList() const` (`?GetList@C3DMeshFileVerticesChunk@@QBEPBVC3DVertex2@@XZ`, FableWin) -- called on the VERT chunk |
| 0x00ABA360 | 00/ab/CShaderPreParser_GetConstantCount_00aba360.cpp | CShaderPreParser::GetConstantCount | ICF fold; here `?GetCount@C3DMeshFileVerticesChunk@@QBEKXZ` |
| 0x00A96A90 | 00/a9/STLvectorC3DVertex2_erase_00a96a90.cpp | vector<C3DVertex2>::erase(first,last) | from inline resize |
| 0x00A97040 | 00/a9/stdvectorC3DVertex2_Mfillinsert_00a97040.cpp | vector<C3DVertex2>::_M_fill_insert | from inline resize |
| 0x00AC20A0/B0 | see above | | |
| 0x00ABB300 / 0x00ABB390 | C3DMeshFilePrimitiveChunk_FindFirstTriangles / FindNextTriangles | const forms | OK |
| 0x00A90900 | **not landed** | -- | `?AddTriangleList@CDeviceIndependent@N3DPrimitive@@AAEXABVC3DMeshFileTrianglesChunk@@@Z` (private!; thiscall ret 4; ecx = Data, arg = chunk ptr). Called twice (0xA919D6, 0xA9202A) |
| 0x00A62E40 | (AllocateResource) | `bool AllocateResource(long*, CMovableResource*, unsigned long, unsigned long)` | `?AllocateResource@CMovableResourceMemoryPool@@QAE_NPAJPAVCMovableResource@@KK@Z` OK |
| 0x00A039F0 | 00/a0/CScopedMemoryHandle_Initialise_00a039f0.cpp | Initialise(CHandleBasedMemoryPool*, long) | OK |
| 0x00A026B0 | 00/a0/CHandleBasedMemoryPool_GetPointerFromHandle_00a026b0.cpp | GetPointerFromHandle(long) | reached through inline `CScopedMemoryHandle::GetPointer() const` (FableWin `?GetPointer@CScopedMemoryHandle@@QBEPAXXZ` @0x032F7F10, inside the C3DPrimitive2 debug block): ecx = Pool (+0), push Handle (+4) |
| 0x00A94150 | 00/a9/global_GFExplicitNew_00a94150.cpp | GFExplicitNew<CStaticBlock> | __fastcall (ecx ptr, edx count); `??$GFExplicitNew@VCStaticBlock@N3DPrimitive@@@@YIXPAVCStaticBlock@N3DPrimitive@@K@Z` (Ego_r label at that addr is a wrong propagated one) |
| 0x00A96270 | 00/a9/global_GFExplicitNew_00a96270.cpp | GFExplicitNew<CAnimatedBlock> | __fastcall, OK |
| 0x00ABB920 / 0x00ABB9B0 | FindFirstGroup / FindNextGroup | const forms | OK |
| 0x00AC4150 | 00/ac/C3DGroup2_C3DGroup2_00ac4150.cpp | C3DGroup2(const C3DMeshFileGroupChunk&) | new(0x10) |
| 0x00A982D0 (address stored) | 00/a9/CCountedPointerC3DGroup2_DeleteData_00a982d0.cpp | CCountedPointer<C3DGroup2>::DeleteData | `?DeleteData@?$CCountedPointer@VC3DGroup2@@@@SIXPAX@Z` |
| 0x00BFEA0E / 0x00BFEA14 | malloc / free | STLport malloc allocator inside inline _M_insert_overflow | |
| 0x00A969D0 | 00/a9/std_destroyauxCCountedPointerC3DGroup2_00a969d0.cpp (`// Flags: /Gr`) | __destroy_aux<CCountedPointer<C3DGroup2>*> | fastcall, tag ptr pushed |
| 0x0075C040 | 00/75/stdvector_resize_0075c040.cpp | vector<C3DVector>::resize(n, const T&) | OUT OF LINE (foreign COMDAT), called twice for UTangents / VTangents |
| 0x00A964C0 | 00/a9/std_copy_00a964c0.cpp | std::__copy<CIVCountedPointer<CTriangleList>*> | fastcall, ret 0Ch (from inline TriangleLists.clear()) |
| 0x00A96990 | 00/a9/std_destroyaux_00a96990.cpp | __destroy_aux<CIVCountedPointer<CTriangleList>*> | fastcall |
| 0x00A90AE0 | **not landed** | -- | `?InitialiseFromMeshFileData@CAnimatedBlock@N3DPrimitive@@QAEXABVC3DMeshFileXAnimatedPrimitiveBlockChunk@@_N@Z` (ret 8) |
| 0x00A8ECE0 / 0x00A8ED30 | N3DPrimitiveCDeviceIndependent_GetTotalTriangleCount / GetTotalIndexCount | QBEKXZ | OK |
| 0x00A975E0 | 00/a9/stdvectorCCountedPointerC3DClothPrimitive_reserve_00a975e0.cpp | vector<CCountedPointer<C3DClothPrimitive>>::reserve | OUT OF LINE |
| 0x00ABBC00 / 0x00ABBC90 | FindFirstCloth / FindNextCloth | const forms | OK |
| 0x00AC3E00 | 00/ac/C3DClothPrimitive_C3DClothPrimitive_00ac3e00.cpp | C3DClothPrimitive(const C3DMeshFileClothChunk*) | new(0x14) |
| 0x00A93A60 (address stored) | 00/a9/gapscan_sub00a93a60_00a93a60.cpp | `DeleteIfSet` (generic!) | **NAME FIX**: `?DeleteData@?$CCountedPointer@VC3DClothPrimitive@@@@SIXPAX@Z` (it is stored as the cloth count block's DeleteFunc at 0xA92298) |
| 0x00A96940 | 00/a9/std_destroyauxCCountedPointerC3DClothPrimitive_00a96940.cpp (`/Gr`) | __destroy_aux<CCountedPointer<C3DClothPrimitive>*> | |
| 0x00AC4F30 | CMirroredTextureHighlightsChunk_GetList_00ac4f30.cpp | `unsigned long* GetList()` (QAE, non-const) | OK |
| 0x00AC4F20 | 00/ac/unattributed_GetCount00ac4f20_00ac4f20.cpp | CClass_00ac4f20::GetCount_00ac4f20 | **NAME FIX**: `?GetCount@CMirroredTextureHighlightsChunk@@QBEKXZ` (ICF; called with the MTEH chunk) |
| 0x00A90510 | C3DPrimitive2_AddMirroredTextureVertexIndex_00a90510.cpp | private AddMirroredTextureVertexIndex(const unsigned long*, unsigned long) | OK |
| 0x00BFE9BC | operator delete | count-block free (CCountedPointer release) | |
Indirect: `call [Info+4]` with ecx = Info->Data = CCPPointerInfo::DeleteFunc (fastcall). `call [vtbl]` push 1 = scalar deleting dtor (scoped_ptr reset).

Globals:
- 0x013D2D6C `CMovableResourceMemoryPool*` -- the current mesh-data pool. Name NOT found (no data symbols in the PDB tsvs). Other refs: 0x00A24581/91/0x00A2476C (CMeshDataBank loader saves it, sets it to CMeshDataBank+0x370 = MeshDataMemoryPool.Data, restores; ghidra_out/decomp_mesh_bank_inflate_buildmesh.c:67). 0x013D2D70 next to it = cloth pool. Descriptive name needed (e.g. `GCurrentMeshDataMemoryPool`), mark descriptive.
- 0x013D2D74 `bool` -- gates the MTEH (mirrored-texture highlights) load; only ref is this row. Descriptive name.
- 0x0129BA3C DELTA_NOTIONAL_ZERO (1e-4f), 0x013CE558 GFastOneOverSqrtTable, 0x0122DED4 3.0f, 0x0122F59C 0.5f -- all from engine/C3DVector.h (FABLE_RETAIL_C3DVECTOR).
- String literals (this TU's .rdata, in this order): 0x0129DF28 "TRIS", DF30 "CLTH", DF38 "XUVT", DF40 "XVBD", DF48 "XMIF", DF50 "XAPR", DF58 "XAPB", DF60 "XTST", DF68 "MTEH"; 0x0129CEF0 "XBDS" (earlier TU's copy, folded). FableWin has `?GetStaticChunkType@<Chunk>@@SIPBDXZ` for C3DMeshFileXVertexDataChunk, XAnimatedPrimitive(Block)Chunk, XUVTangentChunk, XBoundingSphereChunk, XMappingInfoChunk, TrianglesChunk, ClothChunk, CMirroredTextureHighlightsChunk -- spell lookups as `X::GetStaticChunkType()`.

## 4. Members touched
`this` = C3DPrimitive2 (rebuild/include/engine/C3DPrimitive2.h; retail 0xA0, Ego_r names, 12-byte CArrays):
+0x04 MaterialIndex (long) = chunk.GetMaterialIndex(0); +0x08 RepeatingMeshReps = arg3; +0x0C BoundingSphere
(C3DBoundingSphere: Center +0x0C, Radius +0x18; header has `unsigned char BoundingSphere[16]` only); +0x1C
AverageTextureStretch (float); +0x20 ClothPrimitives CArray<CCountedPointer<C3DClothPrimitive>> (header: opaque [12]);
+0x2C Data scoped_ptr<N3DPrimitive::CDeviceIndependent> (header: opaque [4]); +0x30 VertexBuffer
scoped_ptr<CVertexBufferWin32> (opaque); +0x38 VertexBufferStatus (|= 1); +0x40 StaticBlockCount; +0x44 StaticBlocks
(CStaticBlock*); +0x48 AnimatedBlockCount; +0x4C AnimatedBlocks; +0x58 VertexCount; +0x5C TriangleCount; +0x60
IndexCount; +0x65 VertexStreamFlags (uchar); +0x68 InitFlags.
Header opt-ins: FABLE_RETAIL_C3DPRIMITIVE2_HIERARCHY (CMovableResource base + virtual dtor -- needed: `this` is passed
as CMovableResource* to AllocateResource, no adjustment), FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS (types
AnimatedBlocks + inline GetAnimatedBlock(i)), FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER (needs FABLE_RETAIL_SCOPED_PTR(+_RESET)).
MISSING in the header (author needs an `#ifdef FABLE_RETAIL_*` overlay in $W/hdr): typed ClothPrimitives, Data,
VertexBuffer, BoundingSphere; declarations of InitialiseFromMeshFileData, FreeData (0xA901E0), SetupVertexBuffer
(0xA8EDA0, private), CalculateVertexSize (static fastcall), AddMirroredTextureVertexIndex (private), inline
SetupVertexStreamFlags(unsigned long, bool) (Ego_r `?SetupVertexStreamFlags@C3DPrimitive2@@AAEXK_N@Z`, expanded here
0xA92578..0xA9259F).
Other classes (retail layouts):
- N3DPrimitive::CDeviceIndependent (0x50, landed ctor row 0x00A98100 models it locally; no shared header): vptr,
  Vertices CArray<C3DVertex2> +0x04, TriangleLists CArray<CIVCountedPointer<CTriangleList>> +0x10, Groups
  CArray<CCountedPointer<C3DGroup2>> +0x1C, BoundingSphere +0x28, UTangents CArray<C3DVector> +0x38, VTangents +0x44.
- N3DPrimitive::CStaticBlock (engine/N3DPrimitive_CStaticBlock.h, 0x20): MaterialIndex +0x1C (written per block).
- CAnimatedBlock 0x30 (C3DPrimitive2.h ANIMATEDBLOCKS opt-in).
- C3DMeshFilePrimitiveChunk (engine header, FABLE_RETAIL_C3DMESHFILEPRIMITIVECHUNK): MaterialIndices CArray<long> +0x2C
  (FableWin inline `?GetMaterialIndex@C3DMeshFilePrimitiveChunk@@QBEJK@Z` -> `MaterialIndices[i]`).
- C3DMeshFileXVertexDataChunk (header exists): VertexCount +0x14, VertexSize +0x18, InitFlags +0x1C, VBCParams
  (CVertexCompressionParams, 32 B) +0x20, VertexData CArray<char> +0x40 (FableWin inline PeekVertexData: begin, or
  &VertexData when empty).
- C3DMeshFileVerticesChunk VertexList CArray<C3DVertex2> +0x10; C3DVertex2 (engine/C3DVertex2.h) Position/Normal/U/V.
- C3DMeshFileXUVTangentChunk UTangents +0x10, VTangents +0x1C (FableWin inline GetCount / GetUTangent(K) /
  GetVTangent(K) BY VALUE -> the temp C3DVector copies at B+0x6C / B+0x1C).
- C3DMeshFileXBoundingSphereChunk Center +0x10 (GetCenter by value -> temp B+0x1C), Radius +0x1C (GetRadius).
- C3DMeshFileXMappingInfoChunk AverageTextureStretch +0x10 (GetAverageTextureStretch).
- CVertexBufferWin32/CVertexBufferBase CompressionParams +0x14 (32 B, copied by `rep movsd` x8).
- CCountedPointer<T> {Data, Info}, CCPPointerInfo {RefCount, DeleteFunc, Data} (engine/CCountedPointer.h); CScopedMemoryHandle
  {Pool +0, Handle +4} (no shared header; landed 0xA039F0 models it locally).
Shared headers available for locally-modelled classes in the landed siblings: CArray.h, CCountedPointer.h, C3DVector.h,
C3DVertex2.h, C3DBoundingSphere.h, C3DPrimitive2.h, CChunkedFileSuperChunk.h, C3DMeshFile*Chunk.h, CMirroredTextureHighlightsChunk.h,
CMovableResourceMemoryPool.h, CHandleBasedMemoryPool.h, CVertexBufferWin32.h, boost/scoped_ptr.h.

## 5. Visibility / inline decisions (what retail expanded vs called)
EXPANDED inline in retail: scoped_ptr::reset (Boost swap form: new value stored, then old deleted via slot 0 --
FABLE_RETAIL_SCOPED_PTR + FABLE_RETAIL_SCOPED_PTR_RESET); every CCountedPointer ctor/copy/dtor/ShareData/SetStaticCast
(engine/CCountedPointer.h default bodies; SetStaticCast is the PeekVertices idiom); vector<C3DVertex2>::resize(n) (calls
erase 0xA96A90 / _M_fill_insert 0xA97040); vector<CCountedPointer<C3DGroup2>>::push_back AND vector<CCountedPointer<
C3DClothPrimitive>>::push_back INCLUDING _M_insert_overflow (malloc/free, copy loop, `len = size + max(size,1)`; only
__destroy_aux called) -- note out-of-line _M_insert_overflow copies exist at 0xA97CE0 (Group2) / 0xA97AB0 (Cloth) but this
row does not call them; TriangleLists.clear() (erase(begin,end) -> __copy + __destroy_aux calls); IsNonZero+Normalise
(C3DVector.h FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN: `fld st0; fadd st2; fadd st3` then `fadd st1; fadd st2`, the
two leftover pops around the exponent shift -- same as landed 0x00AC8C80); GFFastOneOverSqrt default body; memcpy
intrinsic (`rep movsd` + `rep movsb`, size VertexCount*vertexSize); CScopedMemoryHandle::GetPointer; SetupVertexStreamFlags;
all chunk getters; IsValidSize (__int64 0..0x7fffffff guard) + AllocateResource (CContainmentVolume_Initialise_00a755c0 idiom).
CALLED out of line: vector<C3DVector>::resize(n, x) 0x75C040 (=> the one-arg resize(n) wrapper expanded but resize(n,x) not;
FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE is per-TU and would also outline the C3DVertex2 resize -- needs a type-local
trick, e.g. call `UTangents.resize(n, C3DVector())`-style through a declared-only specialisation, author to test);
vector<CCountedPointer<C3DClothPrimitive>>::reserve 0xA975E0; __destroy_aux/__copy helpers (fastcall /Gr in their rows:
use FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL); FindFirst/Next*, CountSubChunksOfType; GFExplicitNew x2.
Neighbouring same-TU landed rows (defined earlier, bodies visible to retail's compiler): FreeData 0xA901E0,
AddMirroredTextureVertexIndex 0xA90510, CalculateVertexSize 0xA910E0, ConvertToVBFormatAnimated 0xA91180, LoadVertexBuffer
0xA91560 (immediately before this row), CAnimatedBlock::InitialiseFromMeshFileData 0xA90AE0, AddTriangleList 0xA90900 --
all are CALLED here, so declare them (no bodies, or noinline).

## 6. Sibling rows to copy idioms from
- 00/a9/C3DPrimitive2_FreeData_00a901e0.cpp (local model of the full retail layout incl. scoped_ptr, CArray).
- 00/a9/C3DPrimitive2_AddMirroredTextureVertexIndex_00a90510.cpp (inline STLport push_back with CArray.h).
- 00/a9/N3DPrimitiveCAnimatedBlock_ConvertToVBFormatBump_00a90040.cpp (`// Flags: none`; defines
  FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS, FABLE_RETAIL_CANIMATEDBLOCK_DATA, includes engine/C3DPrimitive2.h).
- 00/ab/C3DMeshFilePrimitiveChunk_PeekVertices_00abafe0.cpp (SetStaticCast + const FindFirstSubChunkOfType model).
- 00/a7/CContainmentVolume_Initialise_00a755c0.cpp (IsValidSize + AllocateChecked forceinline idiom).
- 00/a2/C3DMeshStats_AllocateBuffers_00a2e760.cpp / 00/a8/C3DMesh2_PackBoneNames_00a8b6c0.cpp (pool AllocateResource).
- Flags: lane default /O2 /Oy /W3 (no /EHsc, no /GS). Siblings use `#pragma auto_inline(off/on)` around declared helpers
  (ConvertToVBFormatAnimated 0xA91180).
