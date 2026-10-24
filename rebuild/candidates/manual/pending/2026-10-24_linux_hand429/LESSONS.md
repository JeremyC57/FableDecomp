# Chunk 429 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Out-of-line STLport `__copy_backward_ptrs` / `__copy_backward` inside `vector::insert`: VC7.1 leaves them as calls
  unless both are `__forceinline` (0x00B8F650).
- An inline overload that only forwards (CShaderRenderManager::DrawBuffersIndexed 6-arg, or
  `CRenderManagerCore::SetTexture(long, const CTexture*)`) is not inlined into a large caller. `__forceinline` it. Keep
  the `-1` stage tests outside it, or VC hoists the texture load (0x00B8EB00, 0x00BA7DB0).
- A key built in the frame and passed by reference to an out-of-line callee is re-read from the frame after the call.
  A same-TU `auto_inline(off)` + `__declspec(noinline)` body of that callee keeps the arguments in registers
  (CVectorMap::LowerBound in 0x00BD8800).
- A `__false_type` tag homed on a LIVE argument slot (`n` in vector::reserve) needs a same-TU noinline stand-in body of
  the tag-taking `__uninitialized_copy` (0x00BC5E90).
- A `C3DVector GetPos() const` that block-copies `Pos` gives three integer moves. Returning
  `C3DVector(Pos.X, Pos.Y, Pos.Z)` gives scalar replacement: direct loads and one frame copy (0x00BB6450).
- x87 term order:
  - `sqrt(X*X + Y*Y + Z*Z)` as one expression squares Z, Y, X;
  - a dot `X*v.X + Y*v.Y + Z*v.Z` evaluates Y, X, Z;
  - retail ClipSphere's dot is Z, X, Y and is still open (0x00B96350).
- A clamp that copies the high bound to the frame is the by-value template
  `template<class T> T Clamp(T v, T lo, T hi) { return v < lo ? lo : (v > hi ? hi : v); }` (0x00BAA460).
- An 8-byte CEngineFadeDistance comes back through the hidden pointer only when it has a user constructor. There is no
  NRVO: build a local, modify it, `return` it (0x00BAA460).
- Multiple-inheritance constructor (0x00BD9FD0):
  - the first base with an IMPLICIT constructor (CFailedAllocationHandler) emits no vtable store of its own;
  - the second base with a user constructor (CMovableResource) keeps its store;
  - the derived constructor then stores both final vtables.
- A recursive inline helper (FindNode in FindID 0x00BC9B80) is inlined only under `#pragma inline_recursion(on)`. Adding
  `inline_depth(1)` to limit it to one level also blocks the nested helpers (still open).
- CVectorMap::LowerBound is a hand-written `count / 2` (signed: `cdq ; sub eax,edx ; sar eax,1`), not STLport's `>> 1`
  (0x00BD8480 and its inlined copy in 0x00B8D3C0).
- STLport `__lower_bound` / `__introsort_loop` rows put `__fastcall` on the template definition. The empty `less<_Tp>`
  needs `less() { }`; an inline `CSortElement::operator<` comparator inlines (0x00B867A0, 0x00B8F1C0).
- `#include` of a scratch header outside rebuild/include hangs cl until the 200 s timeout (0x00BD8480 grade 1). Paste
  the text into the source.
- A bare `fld [f] ; mov edx,[p] ; fistp dword ptr [edx]`, with the int homed in a dead argument slot, is a Lionhead
  inline-asm float-to-int helper. It is a retail_asm candidate; the C++ cast calls `_ftol2` (0x00BA7B30).
- Retail offsets, which run high in Ego_r because retail CArray/vector is 12 bytes:
  - CShaderRenderManager: RenderManager +0x1D0, D3DDevice +0x1D4 (Ego_r +0x1E0/+0x1E4);
  - CEngineClothRenderer: Ego_r less 0x18;
  - CEngineOcclusionManager: PrevFrameHandleMapper +0x24048, FlareVertexBuffers +0x24054;
  - CEngineLandscapeMemoryManager: STLport lists are 4 bytes.

Review at landing:
- Rows that needed more than 4 grades: none. Parked after 4: 00b96350, 00b9d670, 00bba1e0, 00bc9b80. Parked after 1
  (retail_asm candidate): 00ba7b30.
- Local base models next to an existing engine header:
  - 00ba7b30 (differ) models CEngineInternalPrimitiveBase polymorphically: the header keeps the vptr as data and the
    +0x34 bitfield opaque, and the row needs virtual slot 13 GetFadeDistance;
  - 00b91840 models the primitive locally (the header leaves +0x08 Type unnamed);
  - 00bd9fd0 models CEngineLandscapeMemoryManager locally (its header carries Ego_r 12-byte list offsets);
  - 00baa460 and 00bb6450 model CEngineFadeDistance / C3DVector / CMatrix3x4 locally (no member-function headers).
- Errors in ALREADY-LANDED sources:
  - 0x00BC9F50 "CRenderDevice_00bc9f50::DrawVertexBuffer" is CShaderRenderManager::DrawBuffersNonIndexed (manifest label
    CBowerstoneIntroGuard::Init is also wrong).
  - 0x00BAA4E0 (IsInsideFrustum) names its callee 0x00BAA460 "GetFadeInRange"; it is GetTransitionFade. Its global
    0x01436E80 is CEnginePrimitiveRendererSharedData (+0x8C ZSpriteDrawDistanceFactor).
  - CLocalDetailCacheMap Update 0x00BDED90 calls 0x00BE04F0 "RemoveFinished"; it is
    CEngineStaticMapFileBlockManager::DeleteUnusedBlocks.
  - 0x004400C0 (`__uninitialized_copy<CEnginePrimitiveHandle*>`) uses `namespace std`; retail STLport mangles `_STL`.
- Catalog and manifest labels found wrong outside the chunk:
  - 0x009B6300 "vector_insert" is `vector<pair<ulong,ulong> >::_M_insert_overflow`;
  - 0x0042971D / 0x0042A34E are `_Rb_global<bool>::_Rebalance` / `_Rebalance_for_erase`, not
    CMemoryAllocatorVariableSize;
  - 0x0042AB73 is `list<long>::~list`, not CActiveFile::OnReadFinished;
  - 0x00B8F220 is labelled a CFaction pair deleting destructor, which is suspect next to the occlusion-manager vectors.
- Catalog rows that hold several functions: none found; every row's bounds matched its object.
