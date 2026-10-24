# Hand chunk 415 (_global .. _global): 36 of 40 byte-exact

## What matched (36)
All 36 grade MATCH or RELOCATION_MATCH under VC7.1 RTM. Every relocation was resolved by hand against the retail image
(`bin/relocs2.py`), and every test passes under g++ (tall.sh) and VC7.1/Wine (tall.py).

- **CArray<CHandleBasedMemoryPool::CHandleInfo> (1)**
  - 0x00A03AF0 `push_back()` appends a null handle slot.
  - Shape: `push_back()` is `push_back(T())` over the inline one-argument push_back. The full path calls the reallocating
    insert 0x00A03900 with a 1-byte empty tag temporary.
  - The source emits the exact Ego_r.pdb symbol `?push_back@?$CArray@VCHandleInfo@CHandleBasedMemoryPool@@@@QAEXXZ`.
- **CInputEvent / CJoystickPositionTracker (2)**
  - 0x00A043A0 `CInputEvent::GetJoystickPosition() const` maps the stick from radial to rectangular limits through the
    anonymous-namespace `__fastcall` helper 0x00A04200.
    - Shape: the helper's hidden result lands in a NAMED local, and C2DVector has a user memberwise copy constructor.
      Returning the call straight passes the hidden pointer through and grades 20v35.
  - 0x00A0DFC0 `CJoystickPositionTracker::Push(const CInputEvent&)` normalises the position, pushes it on the front of
    the deque and pops the back until 30 are left.
    - Shape: STLport `push_front`, `size` and `pop_back` (with `_M_pop_back_aux`) inlined; VC rotates the while loop
      (`ja` at the bottom).
- **CSoundBankLH (5)**
  - The functions: 0x00A0B630 `Load(const CWideString&)` (vtable slot 1), 0x00A0B790 `GetSampleInfo`, 0x00A0B850
    `GetSampleValue`, 0x00A0B900 `GetSampleLength` (float) and 0x00A0BA40 `IsSoundEventValid(const CCharString&)`.
  - Shapes:
    - `Load` builds the bank descriptor through an inline defaulting constructor, then sets four flags in retail's store
      order (+0x0e, +0x0a, +0x0d, +0x0c).
    - The four sample queries hold the sound system in a NAMED local (`CSoundSystemLH* system = m_pSystem;`). That makes
      VC load the vtable into eax and hoist the bank and sample into ecx/edx before `push 0 ; push 0`, as retail does.
    - The result is `unsigned char`, since retail has no `xor eax,eax` before `setne`.
    - The query record constructors are 0x00C08750 (bank, group, sample) and 0x00C08720 (bank, name); its destructor is
      0x00C08780.
- **Locale formatting (3)**
  - 0x00A0D980 `FormatDateTime`: `__fastcall`; the zero time is tested as two word compares; GetUserDefaultLCID is a
    dllimport called twice.
  - 0x00A0DA70 `GetThousandsSeparator`: a switch on PRIMARYLANGID.
    - VC7.1 emits a byte index table plus a three-entry jump table, with the case bodies in source order.
    - I checked both tables case by case against retail.
  - 0x00A0DAC0 `FormatNumberWithSeparators`.
    - The for-header steps `++i, --dest, --src` give retail's `inc ; sub ; sub` order.
    - VC proves `i % 3` non-negative and emits an unsigned `div`.
- **STLport 4.6 deque<C2DVector> (18)**
  - Iterator and algorithms:
    - Iterator members: 0x00A0E2A0 `_M_advance`, 0x00A0E9E0 and 0x00A0EE00 `operator[]` (Nonconst and Const traits),
      0x00A0EA30 `operator-(int)`.
    - Algorithms: 0x00A0E3D0 `__uninitialized_copy`, 0x00A0E460 `__copy`, 0x00A0E850 `__copy_backward`, 0x00A0E700
      `fill`, 0x00A0E740 `__uninitialized_fill`, 0x00A0EBC0 `__uninitialized_copy_fill`, 0x00A0EC80
      `__uninitialized_fill_copy`.
  - deque members: 0x00A0E940 `pop_back`, 0x00A0EA90 `clear`, 0x00A0F300 `_M_new_elements_at_front`, 0x00A0F350
    `_M_new_elements_at_back`, 0x00A0F3B0 `_M_push_front_aux_v`, 0x00A0F430 `_M_reserve_elements_at_front`, 0x00A0FC80
    `resize`.
  - Shape: the STLport 4.6 bodies, verbatim, in namespace `_STL`.
    - Layout: the deque is 0x28 bytes {start +0, finish +0x10, map +0x20, map_size +0x24}; nodes hold 0x80 bytes
      (16 elements).
    - Allocation: malloc 0x00BFEA0E and free 0x00BFEA14.
    - Calling convention: free functions are `__fastcall` (/Gr), and 16-byte iterators are passed by value on the stack.
  - Specific matches:
    - `resize` matches only with `operator+` declared without a body (retail calls 0x00A0E980).
    - `__uninitialized_copy_fill` and `__uninitialized_fill_copy` match only with direct `__uninitialized_copy(...,
      __false_type())` and `__uninitialized_fill(..., __false_type())` calls. The inline forwarders drop the tag's
      `lea edx` (187v191).
- **DirectX-sample word wrap (7)**
  - The functions:
    - 0x00A0FDB0 `WordWrap_SetOption` (/GS, CPINFOEXA buffer).
    - 0x00A10310 `WordWrap_FindNonWhiteSpaceForward` and 0x00A10350 `WordWrap_FindNonWhiteSpaceBackward`, both with a
      branchless `neg/sbb/and` or `cmp/sbb/not/and` result.
    - 0x00A10960 and 0x00A10990 `WordWrap_FreeTokenArrayW/A`.
    - 0x00A109C0 and 0x00A109F0 `WordWrap_AllocTokenArrayW/A`, where the array `new` binds to the scalar operator new
      0x00BFEA1A because there is no `operator new[]` in the TU.
  - All are `__fastcall`. WCHAR is `unsigned short`.
  - These names are descriptive: there is no PDB name.

## Parked for Fable 5.1 (4)
- 00a10170 _global::IsNonEndingChar DIFFER(202v197): retail tests the char already in ax; ours adds `mov ax,word ptr [esp+4]` and every other instruction matches (60 of 60) | tried: `bool IsNonEndingChar(unsigned short)` (cdecl; `__fastcall` would put the char in cx, not ax) | next: lane gap: LTCG custom register convention (argument in ax; needs a /GL + /LTCG link-and-extract grading mode)
- 00a10240 _global::IsEastAsianChar DIFFER(110v105): retail compares the char already in ax; ours adds `mov ax,word ptr [esp+4]` and every other instruction matches (36 of 36) | tried: `bool IsEastAsianChar(unsigned short)` (cdecl) | next: lane gap: LTCG custom register convention (argument in ax)
- 00a102b0 _global::WordWrap_CanBreakLineAt DIFFER(118v92): retail calls the helpers with the char in ax (`mov eax,edx ; call` / `mov ax,[ecx-2] ; call`) and keeps ecx/edx live across the calls with no saves; ours pushes each char, saves esi and pops the arguments | tried: `bool __fastcall WordWrap_CanBreakLineAt(const unsigned short*, const unsigned short*)` with the sample's white-space macro | next: lane gap: LTCG custom register convention (callees take ax, caller-saved registers kept live)
- 00a10720 _global::WordWrap_TokenizeW DIFFER(149v222): retail inlines WordWrap_CanBreakLineAt(cursor, 0) (`test edi,edi`) and calls the helpers with the char in ax; ours calls 0x00A102B0 out of line | tried: `long __fastcall WordWrap_TokenizeW(const unsigned short*, unsigned short**)` with CanBreakLineAt as a same-TU inline | next: lane gap: LTCG cross-module inlining + custom register convention

## Skipped
None.

## Label corrections
Propagated catalog names that are wrong (catalog name -> real identity):

| Address | Catalog name | Real identity |
|---|---|---|
| 0x00A03AF0 | Vector_PushBack_Lazy | `CArray<CHandleBasedMemoryPool::CHandleInfo>::push_back()` (Ego_r.pdb) |
| 0x00A043A0 | GetDefaultVal<CEngineGraphic> | `CInputEvent::GetJoystickPosition() const` (Ego_r.pdb) |
| 0x00A0B630 | PathConversion_InitializeTemp | `CSoundBankLH::Load(const CWideString&)` (vtable 0x0129C9DC slot 1) |
| 0x00A0B790 | QuadTree_QueryBounds | `CSoundBankLH::GetSampleInfo` (descriptive) |
| 0x00A0B850 | QuadTree_CountInBounds | `CSoundBankLH::GetSampleValue` (descriptive) |
| 0x00A0B900 | QuadTree_QueryMaxDistance | `CSoundBankLH::GetSampleLength` (descriptive) |
| 0x00A0BA40 | UISystem_TriggerActionParam | `CSoundBankLH::IsSoundEventValid` (descriptive) |
| 0x00A0DAC0 | FormatNumber_AddThousandsSeparators | FormatNumberWithSeparators (descriptive, cosmetic) |
| 0x00A0DFC0 | InputProcessor_ManageDequeCapacity | `CJoystickPositionTracker::Push(const CInputEvent&)` (descriptive) |
| 0x00A0E2A0 | Vector_ReserveAndSeek | STLport `_Deque_iterator_base<C2DVector>::_M_advance` |
| 0x00A0E3D0 | DequeIterator_CopySegments | `__uninitialized_copy` |
| 0x00A0E460 | DequeIterator_CopyForward | `__copy` |
| 0x00A0E700 | List_CopyRange | `fill` |
| 0x00A0E740 | DequeIterator_CopyRange | `__uninitialized_fill` |
| 0x00A0E850 | DequeIterator_CopyBackward | `__copy_backward` |
| 0x00A0E940 | Deque_FreeBlock | `deque<C2DVector>::pop_back` |
| 0x00A0E9E0 | operator_new | `_Deque_iterator<_Nonconst_traits>::operator[]` |
| 0x00A0EE00 | operator_new | `_Deque_iterator<_Const_traits>::operator[]` |
| 0x00A0EA30 | GFToScreen | `_Deque_iterator::operator-(int)` |
| 0x00A0EA90 | Deque_Clear | `deque::clear` |
| 0x00A0EBC0 | DequeIterator_Insert | `__uninitialized_copy_fill` |
| 0x00A0EC80 | DequeIterator_Insert_Reverse | `__uninitialized_fill_copy` |
| 0x00A0F300 | Deque_AllocateBlocks | `deque::_M_new_elements_at_front` |
| 0x00A0F350 | MemoryPool_EnsureCapacity | `deque::_M_new_elements_at_back` |
| 0x00A0F3B0 | Deque_AllocateNewSegment | `deque::_M_push_front_aux_v` |
| 0x00A0F430 | Deque_CalcIterationCount | `deque::_M_reserve_elements_at_front` |
| 0x00A0FC80 | RenderList_Insert_Wrapper | `deque::resize` |
| 0x00A10350 | FindNonWhiteSpaceBackward | WordWrap_FindNonWhiteSpaceBackward (prefix only) |
| 0x00A10960 / 0x00A10990 | LinkedList_DeleteChain(_Alt) | WordWrap_FreeTokenArrayW / A (descriptive) |
| 0x00A109C0 / 0x00A109F0 | Array_AllocateTerminated(_Alt) | WordWrap_AllocTokenArrayW / A (descriptive) |

Errors in already-landed sources:
- Landed 0x00A0ED80 (`std_copyaux`, "SPair8") is STLport `__copy_backward_aux`, and its callee is 0x00A0E850, not
  0x00A0E860.
- Landed 0x00A0EF90 (`std_copy`) is `copy_backward`.
- The landed CDequeIterator8 0x00A0E980 comment names 0x00A0E2A0 `operator+=`; it is `_M_advance`.
