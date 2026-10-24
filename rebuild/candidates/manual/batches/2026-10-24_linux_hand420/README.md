# Hand chunk 420 (_STL::__copy .. _STL::fill): 35 of 40 byte-exact

All 40 rows sit in the manifest as `_global`. Each has been identified below, using FableWin/Ego_r PDB name sets,
RTTI and callers. 35 rows grade MATCH or RELOCATION_MATCH. Each has a behaviour test that passes under g++ and under
VC7.1/Wine. 5 rows are parked in `differ/`. No row was skipped.

## What matched (35)

- **STLport float sort, /Gr TU (0x00A5A540, 0x00A5A770, 0x00A5A860, 0x00A5A8A0).**
  - `__unguarded_insertion_sort_aux`, `__linear_insert`, `__insertion_sort`, `__partial_sort` over `float*` /
    `less<float>`. They are written as templates in `_STL` and explicitly instantiated.
  - Shapes:
    - The by-value `T(*i)` stays on the x87 stack.
    - `copy_backward` over `float*` is the inline `__copy_trivial_backward` memmove.
    - `__insertion_sort` calls `__linear_insert` out of line.
    - In `__partial_sort`, `make_heap` calls `__make_heap(first, last, comp, (float*)0, (ptrdiff_t*)0)` @ 0x00A5A660.
      `__pop_heap` is inline and `__adjust_heap` @ 0x00A5A580 is called.
- **back_insert_iterator<vector<C2DVector> >::operator= (0x00A5A910).**
  - push_back is inline, with a placement-new null test.
  - `_M_insert_overflow` @ 0x004416E0 takes a `__true_type` with a user ctor, so no store is emitted.
- **Projection functor (0x00A58720, descriptive `CProjectOntoPlane::operator()`).**
  - Plain ctor form: `C3DVector d = p - *Origin; return C2DVector(d * *AxisX, d * *AxisY);`
- **2D/3D maths.**
  - `C2DLineF::GetSquaredDist` (0x00A57770): a named `closest` local, then `(point - closest).GetSquaredMagnitude()`.
  - `NBBBMaths::CLinearSpline::GetLength` (0x00A57EF0): C3DVector has a user copy ctor and
    `GetLength() { return (float)sqrt(GetLengthSquared()); }`. That gives retail's "squares from copies" x87 shape.
  - `NBBBMaths::IsPointInsideTriangleInThePlane` (0x00A58A60, `bool __fastcall`): a `long i != 2` loop with the
    vector `operator[]` written as `*(begin()+n)`.
  - `C3DBoxF::ToC3DBoxI` (0x00A55A80): ctor arguments are evaluated right to left, and C3DBoxI has a user copy ctor.
  - `GFDXGetHermiteSplinePosition` (0x00A565C0): four D3DXVECTOR3 copies, then D3DXVec3Hermite through the
    `__stdcall` thunk 0x00BFEFDA (not dllimport).
- **Strings.**
  - `GFGetSubString` (0x00A60390): a naive nested scan.
  - `GFGetLastWindowsError` (0x00A5ED60, `CCharString __fastcall`): `GetLastError()` must be its own statement before
    `FormatMessageW`. `CWideString(buffer).ConvertToCharString()` goes straight into a named local, and the return
    copies it because VC7.1 has no NRVO.
- **Memory allocators.**
  - `GFCheckTreeNodes` (0x00A644C0).
  - `CMemoryAllocatorVariableSize::DeleteChunk` (0x00A65390): MEMORYSTATUS is a frame local, and the call order is
    GlobalMemoryStatus, VirtualFree, GlobalMemoryStatus. The model derives from engine `CMemoryAllocatorBase.h`.
  - `AllocateMemoryFromExistingChunks` (0x00A656E0): an `Align()` inline.
  - `CMemoryAllocatorFixedSize::NewChunk` (0x00A663D0): engine header base plus a derived method holder. The
    out-of-memory hook is `g_pGlobalMemoryManager->OnOutOfMemory()` @ 0x009F1F00.
- **CChecksumCacheFile and its `map<CWideString, CEntry>` (0x00A60C90, 0x00A60C30, 0x00A608F0, 0x00A609C0, 0x00A61270,
  0x00A61360, 0x00A617E0).**
  - Ctor(long): the same member-init list as the landed 0x00A61150.
  - Close: an inline `IsOpen()` (`Filename.GetLength() != 0`), then Save, the inline STLport `clear()` and
    `Filename = L""`.
  - `_M_find` and `_M_lower_bound`: the textbook STLport bodies in an /O2 TU.
  - `_M_insert`: the landed 0x00990830 skeleton. The pair copy is the CWideString copy ctor plus `rep movsd` for the
    CEntry.
  - `insert_unique` needs same-TU `__declspec(noinline)` bodies for `operator--` (0x00A60890) and `_M_insert`. Only then
    do `j` and the `_M_insert` result temporary share v's dead argument slot. Declaration-only callees add a
    `push ecx` local.
  - `map::operator[]`: the plain STLport `value_type(k, Tp())`. CEntry's implicit default ctor calls
    CDateAndTime() @ 0x009E9AB0.
- **map -> vector copy (0x00A55880).**
  - STLport input-iterator `__copy` under `__fastcall`: the hidden result is in ecx, and the
    `const input_iterator_tag&` lands in edx.
  - `pair<const CCharString,int>` converts to `pair<CCharString,int>` through pair's template converting ctor, which
    gives retail's temporary plus its destructor.
  - `_Rb_global::_M_increment` is inline.
- **CShaderResource TU (0x00A5D7C0, 0x00A5DFF0, 0x00A5E4D0).**
  - `CShaderResource::OpenShaderBank`: the nulling loop runs over a hoisted array pointer, which becomes
    `rep stosd`.
  - `vector<_D3DVERTEXELEMENT9>::_M_allocate_and_copy`: a member template, with `__uninitialized_copy(__false_type)`
    inline.
  - `vector<SElement8>::operator=`: STLport, with `__copy_ptrs` / `__uninitialized_copy` declared out of line as
    `__fastcall`. The `__false_type` tag is homed in x's dead slot.
- **DirectInput device lists, a CInputManagerDX member at +0x2B0 (0x00A61880, 0x00A61A80).**
  - Descriptive `CDirectInputDeviceLists_00a61b10`, the same name as the landed dtor 0x00A61B10.
  - The ctor has two inline `_List_base` ctors (0x10-byte nodes), then `m_enabled(1), m_count(0)`.
  - `ReleaseAll` runs `while (size() > 0) { back().Interface->Release(); pop_back(); }` over the second list, then
    the first.
- **Input devices (0x00A66930 CKeyboard, 0x00A66F20 CMouse; FableWin `(CInputManager&, bool)`).**
  - The sequence: base ctor, the 256 x CInputEvent loop, the inline STLport list ctor (0x40-byte node), then body
    assignments.
  - The two counters are stored in opposite orders in the two classes, so they are body assignments, not
    mem-initialisers. The bool is unused.
- **Texture manager (0x00A698F0, 0x00A6ABE0).**
  - `CTextureManager::CalculateTextureSize` (static `__fastcall`): a `switch` on the D3DFORMAT. An if-chain lays the
    DXT1 arm out first; retail has it last.
  - `ResizePoolTextureCount`: `bool result = false` in bl, with pools 0x54 apart at +0x4C.
- **`_STL::fill<CIVCountedPointer<CLocalDetailPlacementGrid>*>` (0x00A6B000).** The same body as the landed
  0x00A43890, with the intrusive `operator=` inline.

## Parked for Fable 5.1 (5)

- 00a56aa0 CCardinalSpline::CCardinalSpline DIFFER(167v167): register swap only: the first inline push_back copies through edx/ecx and pushes the tag from ecx (retail ecx/edx, tag from edx); the second push_back and the rest match | tried: _Vector_base + _STLP_alloc_proxy CArray ctor with two inline push_backs; an inline AppendControlPoint wrapper (not inlined, 57v167) | next: a __forceinline AppendControlPoint wrapper, or push the start point through a named const reference; else "lane gap: register allocation"
- 00a57730 C2DLineF::GetDist DIFFER(63v63): x87 stack lifetime only: retail squares Y then X from copies (fld st(0) ; fmul st(1) ; fld st(2) ; fmul st(3)) and stores the root over the differences (fstp st(2) ; fstp st(0)); ours squares X in place and ends fxch ; fsqrt ; fstp st(1) | tried: sqrt(X*X+Y*Y), sqrt(Y*Y+X*X), a named offset local, a C2DVector user copy ctor | next: GetMagnitude() { return (float)sqrt(GetSquaredMagnitude()); } with a separate inline GetSquaredMagnitude (the form that matched 0x00A57EF0 here and the landed 0x009E2460 / 0x00661CD0)
- 00a5a9d0 _STL::transform DIFFER(211v203): the X dot product is stored to the named local r and copied into the push_back temporary by an integer move (frame 0x14); retail stores it straight into the temporary (frame 0x0c) | tried: ctor form (Y first plus spills), C3DVector user copy ctor, member-assignment form, C2DVector user copy ctor | next: a named float x computed before return C2DVector(x, d * *AxisY), or __forceinline on the functor's operator()
- 00a5c2b0 NVectorMath::GFFinishPartialSlerp DIFFER(98v67): retail is hand SSE (shufps broadcasts, strides added from [ebp+N] each pass, dec ecx ; jne, an ebp frame without and esp,-16); the intrinsic reference body gets an aligned frame and spills the count | tried: one intrinsic reference body (one grade, retail_asm rule) | next: "lane gap: retail_asm candidate" (land as grade retail_asm: a minimal inline-asm body plus a Retail-asm evidence line)
- 00a5c300 NVectorMath::GFClear4DVectors DIFFER(95v89): the retail SSE arm is hand asm (movups xmm0,[eax] then a movaps [edx],xmm0 ; add edx,10h ; dec ecx ; jne loop) in a plain ebp frame whose C++ fallback addresses its 16-byte copy as [ebp-10h]; the intrinsic body adds and esp,-16 | tried: one intrinsic reference body with the fill_n (0x00A5C510) fallback | next: "lane gap: retail_asm candidate"

## Skipped

None. No row covered two functions, and none was an int3-after-noreturn span.

## Label corrections

Manifest labels for this chunk, all propagated or descriptive:

| Address | Manifest label | Corrected identity | Basis |
|---|---|---|---|
| 0x00A55880 | BinaryTreeTraversal_InsertPair | `_STL::__copy(map iter, map iter, back_insert_iterator<vector<pair<CCharString,int> > >, const input_iterator_tag&, int*)` | |
| 0x00A55A80 | MakePath | `C3DBoxF::ToC3DBoxI() const` | |
| 0x00A56AA0 | CPSCSplineBase_Initialize | `CCardinalSpline::CCardinalSpline(const C3DVector&, const C3DVector&, float, bool)` | |
| 0x00A57730 | C2DLineF_DistanceToPoint | `C2DLineF::GetDist(const C2DVector&)` | |
| 0x00A57770 | C2DVector_DistanceToLineSquared | `C2DLineF::GetSquaredDist(const C2DVector&)` | |
| 0x00A57EF0 | CPSCSplineBase_GetPathLength | `NBBBMaths::CLinearSpline::GetLength() const` | |
| 0x00A58720 | Matrix3x3_TransformPoint | the plane-projection functor's `operator()` | descriptive class name |
| 0x00A5A540 | `_Fill<CHeroMarriageInfo*,...>` | `__unguarded_insertion_sort_aux<float>` | |
| 0x00A5A770 | Array_InsertionSort_Float | `__linear_insert` | |
| 0x00A5A860 | Vector_ForEach_ApplyFunc | `__insertion_sort` | |
| 0x00A5A8A0 | Quicksort_Float_Partial | `__partial_sort` | |
| 0x00A5A910 | Vector_Pair_Append | `back_insert_iterator<vector<C2DVector> >::operator=` | |
| 0x00A5A9D0 | Vector_InsertMany_PairFloat | `transform` | |
| 0x00A5D7C0 | CResourceIndex_InitializeFromBank | `CShaderResource::OpenShaderBank(const CShaderDataBank*)` | FableWin.pdb |
| 0x00A5DFF0 | AllocAndCopyRange_u64 | `vector<_D3DVERTEXELEMENT9>::_M_allocate_and_copy` | 0x00A5DEE0 is the byte-identical copy for the SElement8 vector |
| 0x00A5E4D0 | StdVector_Assign_PairFloatEnum_v2 | STLport `vector<8-byte element>::operator=` | the pair<float,EConversationAttitude> name is Ego_r propagation |
| 0x00A608F0 | BinaryTree_FindByKey_LeftBias | `_Rb_tree::_M_find` | |
| 0x00A609C0 | BinaryTree_FindByKey_RBTree | `_Rb_tree::_M_lower_bound` | |
| 0x00A60C30 | CChecksumCacheFile_Save_WithTreeRebuild | `CChecksumCacheFile::Close()` | |
| 0x00A60C90 | CBankFile_ParseHeader | `CChecksumCacheFile::CChecksumCacheFile(long)` | |
| 0x00A61270 | ListNode_Insert | `_Rb_tree::_M_insert` | |
| 0x00A61360 | LinkedList_FindInsertionPoint | `_Rb_tree::insert_unique(const value_type&)` | |
| 0x00A617E0 | TreeNode_GetByKey | `map<CWideString, CChecksumCacheFile::CEntry>::operator[]` | |
| 0x00A61880 | DoubleLinkedList_Initialize | `CDirectInputDeviceLists_00a61b10` ctor | descriptive |
| 0x00A61A80 | DoubleLinkedList_ClearAndFree | `CDirectInputDeviceLists_00a61b10::ReleaseAll` | descriptive |
| 0x00A65390 | CMemoryAllocatorVariableSize_Deallocate | `CMemoryAllocatorVariableSize::DeleteChunk` | |
| 0x00A656E0 | MemoryAllocator_AllocateFromPool | `CMemoryAllocatorVariableSize::AllocateMemoryFromExistingChunks` | |
| 0x00A663D0 | MemoryAllocatorFixed_CreateNewChunk | `CMemoryAllocatorFixedSize::NewChunk` | |
| 0x00A66930 | NHeroInformationScreens_Keyboard_Constructor | `CKeyboard::CKeyboard(CInputManager&, bool)` | RTTI vtable 0x0129DBA8 |
| 0x00A66F20 | NHeroInformationScreens_Mouse_Constructor | `CMouse::CMouse(CInputManager&, bool)` | RTTI vtable 0x0129DBC4 |
| 0x00A6ABE0 | TextureManager_ResizePoolByFormat | `CTextureManager::ResizePoolTextureCount(unsigned long, unsigned long)` | FableWin.pdb |
| 0x00A6B000 | `_Fill<CIVCountedPointer<...>>` | STLport `fill` | |

Callees:

| Address | Manifest label | Corrected identity |
|---|---|---|
| 0x00A5A580 | `_Adjust_heap<CAStarState...>` | `__adjust_heap<float>` |
| 0x00A64C10 | AllocateMemoryFromExistingChunks | `GetTreeAreaWithSizeGreaterThanOrEqualTo` |
| 0x00A5C510 | `vector<u64>::_Ufill` | `fill_n<C4DVector*>` |
| 0x009F1F00 | "$E2" / Nop_ret0 | the global memory manager's empty out-of-memory hook |
| 0x00A60890 | TreeNode_Successor_RBTree | the map iterator's `operator--` (predecessor, `_M_decrement` inline) |
| 0x00A614B0 | BinaryTree_Insert_RBTree | `_Rb_tree::insert_unique(iterator, const value_type&)` (hinted insert) |
| 0x00A5DF30 | `_Copy_impl<pair<float,EConversationAttitude>*>` | STLport `__copy_ptrs(..., __false_type)` |
| 0x00A5E6A0 | `vector<unsigned int>::resize` | `vector<CShaderBase*>::resize(n, const T&)` |

Already-landed rows whose names look wrong:

- **0x0099B220 `CCharString_GetLength`.** The body is `(end - begin) >> 1` and it is called on
  `CChecksumCacheFile::Filename`. It is `CWideString::GetLength`.
- **0x00A04410 (file `CInputEvent_CInputEvent`).** The class in the source is still `CDefaultEngineMemoryInit_00a04410`.
  It is `CInputEvent::CInputEvent()`: the CKeyboard/CMouse ctors build their `CInputEvent EventStore[256]` with it at a
  0x34 stride.
- **0x00A57F70 (`CLinearSpline_GetPointWithParam`).** It calls 0x00A57EF0 "GetPathLength". The PDB name is
  `NBBBMaths::CLinearSpline::GetLength`.
- **0x00A5DDE0 (`_Uninit_copy<SElement8*>`).** The landed comment calls the second stack argument a "by-value
  allocator". It is STLport `__uninitialized_copy(first, last, result, const __false_type&)`, and the stack word is
  the tag reference.
- **0x004856CD.** It is modelled in `namespace std`, but retail is STLport `_STL`, so its mangled name differs from the
  retail symbol.
