# Hand chunk 518 (CInputManager .. CNavSwitchableLeafNode): 28 of 40 byte-exact

27 compiler-emitted C++ (8 MATCH, 19 RELOCATION_MATCH) + 1 retail_asm (00a13c20, RELOCATION_MATCH). 12 parked, 0 skipped.
All 28 tests PASS under g++ and VC7.1. The vland dry run (lv/ since deleted) showed 27 WINS and no RELOC_IDENTITY,
RELOC_CONFLICT, RELOC_DUPLICATE or RELOC_NAME lines. It warned SHARED_HEADER_AVAILABLE only; see "Header debt" below.

## What matched (28)
- CInputManager: 009f4c00 IsMouseEventInQueue. A real STLport vector model with the const `operator[]`; CInputEvent is
  0x34 bytes.
- std::list<CVsyncCallbackTimer::CPendingQuery>: 009f7d10 insert. The landed `STLlist_pushback_009f7ec0` model under
  `#pragma optimize("", off)` (the TU is /Od).
- CGraphicDataBank::CGraphicHeader: 009fd6c0, the constructor. Written as an initialiser list. Body assignments put the
  PixelFormat -1 store first.
- CCamera: 00a0bed0 SetFOV. `Flags = 1; if (v > 0) FOVFlags = 3`.
- C3DVector / C2DVector: 00a14440 AccurateNormalise, 00a14480 GetMagnitudeAndNormalise, 00a14510 C2DVector::AccurateNormalise.
  Locals x/y/z, `(float)sqrt(...)` under `#pragma intrinsic(sqrt)`, then `inverse = 1.0f / length`.
- C3DCoordI: 00a13c20 `operator-=`, retail_asm. Retail subtracts X and Y but ADDS Z, then runs `maxps xmm0,xmm1`, which
  VC7.1 cannot emit from this C++. The pure C++ version is 54v57 and differs only by those 3 bytes (probe/00a13c20_pure.cpp).
- CNavigatorManager: 00a15100 GetQuadTreeMemoryUsage. A real STLport list `const_iterator` walk. The old park was a
  QFE-lane artefact; RTM matches straight away.
- CMeshDataBank: 00a23d80 GetEnumerationType (the five animation types, else the base); 00a25420 Sizeof
  (`__forceinline` GFPContainerSizeof plus GFPSizeof).
- C3DMeshInfo: 00a23e40 IsLoaded and 00a23e80 IsLoadedStats. The inline `HasLOD(i)` predicate keeps the loop unrotated.
- C3DMeshLODInfo: 00a254f0 UnloadResource. CanUnload is written with two returns, so the bool stays materialised.
- std::swap<pair<unsigned long, C3DHelperPoint> >: 00a31220. Needs C3DVector's memberwise user copy constructor.
- CVectorMap<unsigned long, long> / CArray<pair<unsigned long, long> >: 00a39900 and 00a39950 DeallocateMemory.
  - STLport `clear()` keeps the counted `__copy` of the empty tail.
  - Then a swap with an empty local array; its destructor frees the old block.
  - 00a39950 tests `capacity() > 0` first. 00a39900 sets `Dirty = false` before the scope-end free.
- C3DAnimationStats: 00a4ef90 GetAllEventNames. Calls `names.reserve(TimingEvents.size())`, then the out-of-line
  `__fastcall` copy 0x00A557E0 (its hidden result goes in the dead `names` slot).
- CCachedVolumes: 00a5ac60 GetNavigationLayerAt. The map key is read into a local before the volume test.
- CFontDataBank: 00a63630, the constructor. Members are built in declaration order with inline constructors, then
  `Filename = "";` in the body (the literal is the pooled tail 0x0122D70E).
- CMemoryAllocatorVariableSize: 00a652d0 GetAllocatedArea. The members are stored straight to the two globals, and the
  header size is read back from the globals. This fixes the old ebx/edi swap.
- CManagedTextureData: 00a69da0 Uninitialise.
  - Needs a real `CCountedPointer<CTexture>` member with inline ReleaseData and `operator->`.
  - Retail re-reads Texture.Data after clearing it and tail-calls CTexture::Uninitialise on null, a harmless retail bug.
    With the flattened header members, VC folds that load to `xor ecx,ecx`.
- CPreallocTexturePool: 00a6a330, the constructor. Uses the landed CTextureManager pool model: STLport
  `_Vector_base(n, a)` with n = 0 gives the +0x50 / +0x48 / +0x50 / +0x4C store order.
- CNavBlockedQuadTreeNode: 00a76900, the copy constructor. It goes through `CANavQuadTreeNode(position, level, layer)`,
  then `Blocked = true`.
- CNavBlockedQuadTreeNode: 00a76950 SaveToFile. If asked: WriteVar of the type byte, the flag argument, then Blocked.
- CNavNavigableLeafNode:
  - 00a76a30, the constructor (grade 4). The base constructor body sets LeafNode = true and Blocked = false (both stores
    land before the derived vtable store), and C2DVector has a memberwise user copy constructor.
  - 00a76c10 RemoveNeighbours. The inline `CSmallVector::clear()`, then PRegion = 0.
  - 00a779b0 ConvertIndexesToPointers. STLport `map::find` inline.

## Parked for Fable 5.1 (12)
- 009fd3a0 CGraphicDataBank::UpdateAvailableMemory DIFFER(59v59): clamp in edx (`and edx,eax`), and the ReduceSizeFrameDelay load + push hoisted above the AvailableMemory store | tried: 4 forms around the inline `ResourceList.GetFreeMemory()` clamp (`x <= 0 ? 0 : x`) | next: the PDB getter GetReduceSizeFrameDelay or a Max<> template
- 009fd660 CGraphicDataBank::FrameEnd DIFFER(91v91): the same clamp / store order as 009fd3a0 (the inlined UpdateAvailableMemory) | tried: the shared 009fd3a0 model | next: the same as 009fd3a0
- 009fdba0 CGraphicInfoFrame::UploadToVolumeTexture DIFFER(61v61): after GetEntryDataType, retail holds header=ecx, level=eax, bool=dl; we hold edx, ecx, al | tried: 3 RTM forms + 1 QFE grade | next: lane gap: register choice
- 00a0be30 CCamera::SetView DIFFER(70v68): the two vector copies go through ecx in retail (no edi push); ours use edx/edi | tried: 4 forms; best is direct member assignments plus an inline Normalise | next: v4 plus an inline user operator= on C3DVector
- 00a0d300 CProcessedInput::Clear DIFFER(43v48): /Os encodings (`or -1`, `push 4 ; pop ecx`) where retail has `mov -1` / `mov ecx,4` | tried: /O2 (unrolls to 66 bytes), optimize("s"), optimize("g", off) | next: lane gap: the rolled loop with /O2 encodings
- 00a24cd0 C3DMeshInfo::Reset DIFFER(82v80): the LODErrors zero register (retail `xor ecx,ecx` with direct stores, the LODDataSizes `lea ; xor edx` scheduled inside) | tried: 4 forms; best is two memsets (a counted loop gives 74 bytes) | next: a C3DMeshInfo inline helper that clears both arrays
- 00a2ea20 C3DMeshStats::HasHelperPoint DIFFER(63v63): the second end() loads Size and Elements into edx and ecx the other way round | tried: 4 forms; the `? true : false` ternary fixes the setne, leaving only the end() registers | next: lane gap: register choice
- 00a50bf0 C3DAnimationStats::GetAllEvents DIFFER(50v48): the inline copy's back_insert_iterator result gets a `push ecx` frame slot, where retail homes the result and the category tag in the dead `events` slot | tried: a bare tag, a by-value `__iterator_category`, a named inserter, no `__copy_aux` hop | next: lane gap: temporary slot sharing
- 00a57140 C2DLineF::GetPointOnInfiniteLine DIFFER(82v90): x87 order. Retail loads point.Y early, spills (p.X - P1.X) to [esp], forms Y products first, and pops the d*t temporary with `fstp st(2)` / `fstp st(0)` | tried: inline Dot/GetSquaredMagnitude, a named offset, explicit X-first and Y-first (84v90) | next: a C2DVector `operator-` / Dot taking a named temporary through memory
- 00a76c30 CNavNavigableLeafNode::IsNeighbourOf DIFFER(55v53): the find's `random_access_iterator_tag` gets its own frame slot; retail homes it over the live `node` slot | tried: the inline compare, a named result (fixes the end() re-read), POD tags, a key copy (moves the tag onto the slot, 63v53) | next: lane gap: tag over a live argument slot
- 00a76c70 CNavSwitchableLeafNode::IsNeighbourOf DIFFER(77v72): the inlined base's by-value node copy gets a frame slot; retail homes the copy and the tag in the node slot | tried: early return, the then-arm call (fixes the branch) | next: lane gap: as 00a76c30
- 00a77420 CNavSwitchableLeafNode::Unlock DIFFER(98v96): `mov ecx,esi` before the recursive Unlock; retail skips it (LTCG knows IsEveryUniqueIDOpen keeps ecx) | tried: the natural loop, the overload order (fixes slot 1) | next: lane gap: LTCG ecx-preserved callee

## Skipped
None. No split fragments in this chunk.

## Names

### Proven
Each name is a PDB spelling backed by a caller, a vtable slot or a matched layout.

009f4c00
009fd3a0
009fd660
009fd6c0
009fdba0
00a0be30
00a0bed0
00a0d300
00a13c20
00a14440
00a14480
00a14510
00a15100
00a23d80
00a23e40
00a23e80
00a24cd0
00a25420
00a254f0
00a2ea20
00a4ef90
00a50bf0
00a57140
00a5ac60
00a63630
00a652d0
00a69da0
00a6a330
00a76900
00a76950
00a76a30
00a76c10
00a76c30
00a76c70
00a77420
00a779b0

### Probable
These are template instances; the body shape and the callers fix them.

009f7d10
00a31220
00a39900
00a39950

### Descriptive
These are callees or data bound by this batch under a non-PDB spelling.

013d28d8
013d28d4
00a557e0

- 013d28d8 / 013d28d4 use the landed `g_LastAllocatorChunkCounter_013d28d8` / `g_LastAllocatorUsage_013d28d4`, but they
  hold AlignmentShift / Alignment.
- 00a557e0 is bound as `std::copy` over `multimap<CCharString, float>::const_iterator`. It is a key-only copy; the real
  template is not in the PDB dumps.

Evidence for the less obvious names:
- 00a4ef90 / 00a50bf0 are `?GetAllEventNames` / `?GetAllEvents@C3DAnimationStats@@QBE...` (Ego_r.pdb and FableWin.pdb).
  Their callers are the NAnimationEvents queries 0x006F9820 / 0x006F9240 / 0x006F91E0. TimingEvents is a
  `multimap<CCharString, float>` at +0x20.
- 00a6a330 stores vtable 0x0129DC50, which is CPreallocTexturePool (rtti-vtables.tsv). The landed CTextureManager
  constructor 0x00A6A360 inlines the same body.
- 00a76900 is `??0CNavBlockedQuadTreeNode@@QAE@ABV0@@Z`, the copy constructor (`ret 4`, it reads the other node's fields).
- 00a652d0: Ego_r.pdb CMemoryAllocatorBase has Alignment +0xC14 and AlignmentShift +0xC18. CMemoryAreaChunk has
  PNextChunk +0x20 and PFirstAllocatedArea +0x30.
- 00a39900 / 00a39950 are `?DeallocateMemory@?$CVectorMap@KJV?$CKeyPairCompareLess@KJ@@@@QAEXXZ` /
  `?DeallocateMemory@?$CArray@U?$pair@KJ@std@@@@QAEXXZ`. Their callers are CCRCSymbolMap (SLONGMap at +4) and
  CArray::DeallocateUnusedMemory 0x00A39A00.

## Label corrections
| Address | Catalog label | Real name |
|---|---|---|
| 009f7d10 | CKinematic::operator= | std::list<CVsyncCallbackTimer::CPendingQuery>::insert |
| 009fd6c0 | C3DAnimationSequenceData ctor | CGraphicDataBank::CGraphicHeader::CGraphicHeader() |
| 009fdba0 | CKilledThing::ModifyHeroStats | CGraphicInfoFrame::UploadToVolumeTexture |
| 00a0d300 | CGameEventPackage ctor | CProcessedInput::Clear |
| 00a14440 | GetScaled | C3DVector::AccurateNormalise |
| 00a14480 | GetScaled | C3DVector::GetMagnitudeAndNormalise |
| 00a14510 | C2DVector::Dot | C2DVector::AccurateNormalise |
| 00a2ea20 | C3DMeshStats::HasDummyObject | C3DMeshStats::HasHelperPoint (0x00A2EA60 is HasDummyObject) |
| 00a31220 | CDebugScriptElement | std::swap<pair<unsigned long, C3DHelperPoint> > |
| 00a39900 | CActiveFile::OnReadFinished | CVectorMap<unsigned long, long, CKeyPairCompareLess<...> >::DeallocateMemory |
| 00a39950 | CArray<unsigned_short>::DeallocateMemory | CArray<std::pair<unsigned long, long> >::DeallocateMemory |
| 00a4ef90 | CGraphicDataBank::ChangeScene | C3DAnimationStats::GetAllEventNames |
| 00a50bf0 | CGraphicDataBank::ChangeScene | C3DAnimationStats::GetAllEvents |
| 00a6a330 | CCompressedMemoryDataInputStream ctor | CPreallocTexturePool::CPreallocTexturePool() |

## Header debt
These are local models. The verifier warns SHARED_HEADER_AVAILABLE; strict mode would reject them.
- Flattened headers that drop the real C++ structure:
  - CNavigatorManager: QuadTrees is opaque bytes; the STLport list is needed.
  - CMeshDataBank: CBankFile is a polymorphic base, plus the CArray of CMeshDataBankEntry.
  - C3DMeshInfo / C3DMeshLODInfo: the CResource base, counted pointers, nested CMeshData.
  - CFontDataBank: CBankFile base, CFontBankStateBlock vptr.
  - CManagedTextureData: the flattened Texture_Data / Texture_Info members change codegen (see 00a69da0).
  - CANavQuadTreeNode / CNavNavigableLeafNode / CNavBlockedQuadTreeNode / CNavSwitchableLeafNode: there is no
    CBaseClassNonCopyable base, Position is flattened, and there are no methods.
- Classes with no header:
  - C3DAnimationStats: the STLport multimap TimingEvents at +0x20.
  - CCachedVolumes: the CVectorMap at +0; CContainmentVolume's header is unused here.
  - CPreallocTexturePool: retail 0x54 bytes against 0x58 in the PDB.
  - CMemoryAllocatorVariableSize: the nested CMemoryArea / CMemoryAreaChunk.
- The header generator needs base classes, vptr-bearing members and real STLport containers before these can move to
  `#include "engine/<Class>.h"`.

## Why the earlier attempts stalled
- Most 2026-10-14 parks (hand46/55/56/57/59/60/28) were graded on the QFE-4035 DDK lane. Under the RTM 13.10.3077
  lane, GetQuadTreeMemoryUsage, GetEnumerationType, CFontDataBank and the nav constructors match.
- Propagated labels led to invented types:
  - The two "ChangeScene" rows are C3DAnimationStats event-name queries.
  - "CCompressedMemoryDataInputStream" is CPreallocTexturePool.
  - "CDebugScriptElement" is std::swap; its old "register allocation" note was really a missing C3DVector user copy
    constructor.
- Shape details the old models lacked:
  - User copy constructors (C3DVector, C2DVector).
  - A real CCountedPointer member instead of a flattened pair (00a69da0).
  - Reading the published globals back instead of locals (00a652d0).
  - Base-constructor flag stores (00a76a30, 00a76900).
