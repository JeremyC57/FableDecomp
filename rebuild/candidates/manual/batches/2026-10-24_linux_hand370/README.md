# Hand chunk 370 (_global .. _global): 35 of 40 byte-exact

All 40 rows are catalog `_global`. Most are STLport 4.x container members and algorithms (named with STLport spelling
on line 1), plus a few Ego_r.pdb-named free functions. Every matched source grades MATCH/RELOCATION_MATCH in
`grades.txt` and its test prints PASS under g++ (`tests_gxx.txt`) and VC7.1 (`tests_vc.txt`).

## What matched (35)

- **STLport vector members, CHeroLogBook (speed TU)**
  - 0x0047CDE0 / 0x0047C420 / 0x0047C470 `vector<CQuestDesc|CQuestObjectiveDesc|CHeroLogBookEntry>::push_back`.
    STLport `push_back`: a placement copy under VC's null test, `++_M_finish` re-read, else
    `_M_insert_overflow(finish, x, __false_type(), 1, true)`. `__false_type` has a user ctor (so there is no zero
    store) and is homed in the dead `x` slot.
  - 0x0047C2E0 `__uninitialized_fill_n<CQuestDesc*>`: /Gr `__fastcall`, with the implicit memberwise copy ctor.
- **STLport map `CHeroLogBook::LogEntries` (map<ECategory, vector<CHeroLogBookEntry>>, speed TU)**
  - 0x0047C500 `_Rb_tree::_M_insert`. The iterator needs a user ctor so the result goes through the hidden pointer
    (`ret 14h`).
  - 0x0047C6A0 `_M_erase`: the pair's ~vector (the __destroy_aux loop) and ~_Vector_base are inlined.
  - 0x0047CB50 `map::operator[]`: lower_bound is inlined. It matches only with the callees (__destroy_aux 0x47BAF0,
    vector copy 0x47BB90, `_M_insert` / insert_unique) defined in the TU and kept out of line. VC7.1 then knows that
    the temporaries' addresses do not escape: it propagates the `T()` temporary's null members
    (`xor edx,edx ; xor ecx,ecx`, no free) and keeps the pair's start in esi.
  - 0x0047C350: the non-virtual `??_G` of `pair<const ECategory, vector<CHeroLogBookEntry>>`, emitted by `delete p`
    (no vtable store, no reference in the image). It matches with the same-TU out-of-line `__destroy_aux`, which
    homes the empty tag over the flags slot (`lea eax,[esp+8]`).
- **STLport vector members in the def TUs (frame-pointer size TUs)**
  - 0x00477428 `vector<T*>` copy ctor: it goes through the 3-argument `uninitialized_copy` wrapper (the tag is built
    inside it, so `_M_start` loads first).
  - 0x004774D6 / 0x0047876B copy ctors and 0x00477530 / 0x004787BE `_Vector_base(n, a)` for CEngineLocalDetailObjectDef
    and CAbilityTrackLevelDef.
  - 0x00478DD6 / 0x0047A1E9 non-POD `_M_insert_overflow`, written verbatim with `__forceinline` helpers.
  - 0x00485B6C `vector<uchar>::_M_insert_overflow` (POD).
  - 0x0048608C `vector<CHeroMarriageInfo>::_M_fill_insert`: same-TU stand-ins that clobber edx.
  - 0x0047CE40 `fill(uchar*, uchar*, const uchar&)`, the memset overload.
- **CQuickAccessItemButtonGuiDef algorithms (/Os, frame pointer)**
  - 0x004761AA `__copy_ptrs` and 0x00476497 `__copy_backward_ptrs`. The `ret 8` forms have the SGI
    `__copy` / `__copy_backward` inlined as `__forceinline`. The element's memberwise `operator=` must be spelled
    out as `__forceinline` (CParentDefClassBase::operator= 0x431ED0, then ButtonGraphic, Position, Offset): the
    /Os TU calls the compiler-generated one out of line (55v104).
- **STLport `map<CCharString, long>` (CPersistContext::TransferMap TU)**
  - 0x00475BF6 `erase(const key&)`: `equal_range`, then `__cdecl distance(const It&, const It&)` (STLport
    `_STLP_CALL`), then `erase(first, last)`. The by-value iterators are built in their slots, the first in the
    not-yet-popped distance slot.
- **STLport `map<CWideString, bool>` (the CTCInGameMenu / CFrontEndManager Signatures cache)**
  - 0x004856CD `_M_find` (/Os, `__forceinline` less).
  - 0x00485BF0 `_M_insert` (frame-pointer /Os; the root and rightmost stores are tail-merged).
  - 0x00485E5C `insert_unique(v)`, the STLport 4.x form with `_M_insert(/*__x*/ __y, __y, __v)` on the begin()
    path. The result iterator is homed in the dead `v` slot.
- **Lists and filtered copies**
  - 0x00479449 `list<COpinionTransientOffset>::push_back = insert(end(), x)` (/Os, `__forceinline` insert/end).
  - 0x0048D5C0: a filtering copy of `list<CThing*>` into `back_inserter(vector<CThing*>)`, keeping the things whose
    +0xD4 flag is set. `_M_insert_overflow` 0x48D0D0 must be same-TU and `__declspec(noinline)`: speed TUs inline
    template members despite `auto_inline(off)` (235v122 before the fix).
- **Deque**
  - 0x00493875 `_Deque_base<CSetTextInfo>::_M_initialize_map`: STLport buffer size 128/28 = 4; the `(max)` by-reference
    temporaries go in a frame word and in the dead `n` slot (frame-pointer /Os).
- **Small algorithms**
  - 0x004936BA STLport `__copy` for a 16-byte POD (/Os `movsd` x4).
  - 0x0048DAE0 `template <class T> void __fastcall GFLimit(T&, const T&, const T&)` for float.
- **Ego_r.pdb-named free functions and CSubtitleRenderer**
  - 0x00486C40 GFFloatToWideString (speed).
  - 0x0048DBF9 GetNumberOfCharactersOfTypeInString: the CSubtitleRenderer TU is /Os, and only `optimize("s")` fixes
    the esi/edi swap.
  - 0x0048E012 CSubtitleRenderer::GetCharWidthW: a static `__fastcall`, frame pointer + /Os.
  - 0x0048E072 CSubtitleRenderer::IsTextReadyForClickPast (/Os, IsCutsceneSkippable under `auto_inline(off)`).
  - 0x0047D9E3 VideoOptionsChangedConfirmed and 0x0047E941 VideoOptionsNotAcceptedConfirmed (/Os, the GetInterface
    model with LowerBound / IS_THING_ALIVE stand-ins).

## Parked for Fable 5.1 (5)

- 00475d07 std::_Rb_tree<CCharString, pair<const CCharString, long> >::_M_lower_bound DIFFER(73v73): register swap only. Retail holds y in edi (`mov edi,[ecx]`) and k's hoisted data pointer in ebx (pushed after the root test); ours has y=ebx and key=edi | tried: y-off+s (frame, operator< called), s + __forceinline operators (bottom-test loop, key re-read), speed + compare 0x411570 body in the TU (73v73 swap) | next: lane gap: register allocation. With the compare only declared, y does get edi but the key is re-read per iteration, so retail needs a hoist without the body knowledge; look for a CCharString::operator< spelling that caches `other.PStringData`.
- 00475d50 std::_Rb_tree<CCharString, pair<const CCharString, long> >::_M_upper_bound DIFFER(73v73): the same edi/ebx swap as 00475d07 (y in edi, key data in ebx) | tried: speed + compare body (swap), compare declared only (y right, key re-read per iteration: `mov ebx,[esp+10h] ; mov ecx,[ebx]`), compare body after the caller (swap), a register-resident key copy (worse) | next: lane gap: register allocation (fix together with 00475d07).
- 00475dfa GFSerialiseVectorBinaryIn<C2DVector> DIFFER(152v152): loop registers swapped. Retail has the index in edi and the reloaded `_M_start` in esi (`mov [esi+edi*8],ecx`); ours has the index in esi and the base in edi | tried: `*(_M_start+n)` operator[] (materialises `lea esi,[eax+edi*8]`, and resize(n) called out of line), `__forceinline resize(n)` + `begin()[i]` (swap), STLport `*(begin()+n)` operator[] (swap), CPersistTraits/ReadCount template-family shape of the landed 0x5C2AD8 (141v152) | next: lane gap: register allocation. Try a same-TU body for ReadC2DVector 0x995030 or resize 0x475E92 (callee clobber knowledge).
- 004792cf CPersistContext::Transfer<std::list<COpinionTransientOffset> > DIFFER(82v79): retail forms `lea ecx,[ebp-4]` (the traits object) once before the mode switch's `je` and reuses it for the In call, the Out call and the default path's ~list; ours re-forms it before each call and the destructor | tried: declared callees (the allocator temporary also in its own slot), a same-TU list ctor body (the allocator now homed at [ebp+0Bh] as retail), same-TU stand-ins for TransferBinaryIn/Out (no change) | next: try an if/else-if chain or a by-reference traits accessor so that `this` is formed before the argument pushes; otherwise lane gap: code hoisting.
- 0048d940 GFFloatToLongFloor DIFFER(45v58): retail is inline asm: `fld [x] ; fsub [half] ; fistp dword [r] ; fild [r] ; fadd [one] ; fstp [chk]` with 0.5f/1.0f in frame words, then an integer compare of x's bits with chk; the C++ cast calls _ftol | tried: plain C++ (the cast) | next: lane gap: retail inline asm (a bare dword `fistp`). It is a `retail_asm` candidate under the owner's 2026-10-24 exception (minimal `__asm` + `// Retail-asm:` line), which this brief forbids.

## Skipped

None. No row holds two functions, and every row's extent ends at its `ret` with int3 padding after it.

## Label corrections

| Address | Catalog label | Correct name |
|---|---|---|
| 0x00475BF6 | RBTree_TraverseAndCompare_Copy | `_Rb_tree<CCharString, pair<const CCharString,long>>::erase(const key&)` |
| 0x00475D07 | RBTree_FindNode_ReverseOrder | `_M_lower_bound` (same tree) |
| 0x00475D50 | RBTree_FindNode_Reverse | `_M_upper_bound` (same tree) |
| 0x004761AA | Vector_ShiftCopyElements | STLport `__copy_ptrs<CQuickAccessItemButtonGuiDef*>` |
| 0x00476497 | Vector_ShiftQuickAccessButtonsReverse | `__copy_backward_ptrs<CQuickAccessItemButtonGuiDef*>` |
| 0x00477428 | lua_stackspace | STLport `vector<T*>::vector(const vector&)` (COMDAT-shared by five pointer vectors) |
| 0x004774D6 / 0x0047876B | *_vector_CopyAndResize | vector copy constructors |
| 0x00477530 / 0x004787BE | *_vector_Reserve | `_Vector_base(size_t, const allocator&)` |
| 0x00478DD6 | CAbilityTrackLevelDef_vector_Reallocate | `vector<CAbilityTrackLevelDef>::_M_insert_overflow` |
| 0x0047A1E9 | CAbilityTrackLevelDef_vector_Reallocate | `vector<CAppearanceModifierScalingDef>::_M_insert_overflow` |
| 0x004792CF | COpinionDeedEffectsDef_Transfer | `CPersistContext::Transfer<list<COpinionTransientOffset>>`; the catalog names the caller |
| 0x00479449 | LinkedList_Insert_CDanyalTestScriptsData | `list<COpinionTransientOffset>::push_back` (COMDAT-shared) |
| 0x0047C2E0 | _Fill_n<unsigned_long*,...> | `__uninitialized_fill_n<CHeroLogBook::CQuestDesc*>` |
| 0x0047C350 | QuestManager_PersistInfo_Destructor | `pair<const ECategory, vector<CHeroLogBookEntry>>` scalar deleting destructor |
| 0x0047C420 | Vector_PushBack_PersistInfo | `vector<CHeroLogBook::CQuestObjectiveDesc>::push_back` |
| 0x0047C470 | Vector_PushBack_DialogueLayerDef | `vector<CHeroLogBook::CHeroLogBookEntry>::push_back` |
| 0x0047C500 | ListNode_Insert | `_Rb_tree<ECategory, ...>::_M_insert` |
| 0x0047C6A0 | TreeNode_RecursiveDelete | `_Rb_tree<ECategory, ...>::_M_erase` |
| 0x0047CB50 | CHeroLogBook_InsertEntry | `map<ECategory, vector<CHeroLogBookEntry>>::operator[]` (LogEntries) |
| 0x0047CDE0 | Vector_PushBack_DialogueNarrative | `vector<CHeroLogBook::CQuestDesc>::push_back` |
| 0x0047CE40 | Memory_FillBytes | STLport `fill(unsigned char*, unsigned char*, const unsigned char&)` |
| 0x0047E941 | VideoOptionsChangedConfirmed (duplicate) | `VideoOptionsNotAcceptedConfirmed` (Ego_r.pdb name set) |
| 0x004856CD | RBTree_FindNode_Iterator | `_Rb_tree<CWideString, pair<const CWideString,bool>>::_M_find` |
| 0x00485B6C | vector_reallocate_with_padding | `vector<unsigned char>::_M_insert_overflow` (POD) |
| 0x00485BF0 | CMemoryAllocatorVariableSize_InsertNode | `_Rb_tree<CWideString, ...>::_M_insert` |
| 0x00485E5C | RBTree_LowerBound_Iterator | `_Rb_tree<CWideString, ...>::insert_unique(const value_type&)` |
| 0x0048608C | Std_Vector_Insert_Copies | `vector<CHeroMarriageInfo>::_M_fill_insert` |
| 0x0048D5C0 | LocationList_InsertActiveCharacters | a filtering copy of `list<CThing*>` into `back_inserter(vector<CThing*>)`, named by behaviour |
| 0x0048DAE0 | GFLimitReturn<double> | `GFLimit<float>(T&, const T&, const T&)` |
| 0x0048E072 | CSubtitleRenderer_EvaluateCutsceneSkippability | `CSubtitleRenderer::IsTextReadyForClickPast` (Ego_r name, inferred) |
| 0x004936BA | _Move<CMusicEntry*,CMusicEntry*> | STLport `__copy` for a 16-byte POD (COMDAT-folded) |
| 0x00493875 | CEngineInterpolationInfo_Initialize | `_Deque_base<CSubtitleRenderer::CSetTextInfo>::_M_initialize_map` (element inferred by size) |

Callees seen along the way that are also mislabelled:
- 0x00475E92 (CArray<pair<long,long>>::push_back) is `vector<C2DVector>::resize(size_type, const T&)`.
- 0x00475CDD is the CCharString tree's `equal_range`.
- 0x00475C3B is `distance`.
- 0x00475C5E is `erase(first, last)`.
- 0x0047BAF0 is `__destroy_aux<CHeroLogBookEntry*>`.
- 0x0047BB90 is `vector<CHeroLogBookEntry>` copy constructor.
- 0x00450D90 is `list(const allocator&)`.
- 0x00493BF4 is `_Deque_base<CSetTextInfo>(const allocator&, size_t)`.
