# Chunk 362 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Two sort_heap shapes.** Both are /Gr TUs: `first` in ecx, `last` in edx, comparator by value on the stack.
  - SGI: `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap / __pop_heap_aux inline. The value is
    copied into the argument area, then `__fastcall __pop_heap(first, last-1, last-1, value, comp, (int*)0)`
    (0x005E2FB0).
  - Dinkumware: `for (; 1 < last - first; --last) _Pop_heap_0(first, last, pred, (T*)0)`. The value is a frame
    temporary, then copied again into _Adjust_heap's argument (0x0076AF10).
- **Dinkumware _Sort_heap: no pop_heap guard.** Call `_Pop_heap_0` straight from the loop. A `pop_heap` wrapper with
  `if (1 < last - first)` is not folded into the loop test (+6 bytes) (0x007590C0).
- **Dinkumware _Sort_heap: `__forceinline` when the temporary is big or non-trivial.** Plain `inline` on `_Pop_heap`
  leaves it as a local call that pushes `(long*)0` and the comparator. `__forceinline _Pop_heap` / `_Pop_heap_0` gives the
  retail inline body (0x00A338C0, 0x006FCB90).
- **CSmallVector temporary destructor.** An inline `~CSmallVector() { delete[] Start; Start = 0; Size = 0; }` emits the
  `call operator delete[]` and then two zero stores that VC schedules down to the loop latch (0x006FCB90).
- **Empty `__false_type` tag by const& to an out-of-line callee.** The tag needs a user ctor (`__false_type() {}`).
  Without one there is an extra zero-byte store. With a same-TU `__declspec(noinline)` body for the callee, the tag
  overlays the dead comparator parameter slot as in retail (0x00A84470 `_M_insert_dispatch`, 0x00BEF350
  `__uninitialized_copy`).
- **STLport list copy ctor.** It is `_List_base(a)` (malloc'd 0x18 sentinel, self-linked) +
  `insert(begin(), x.begin(), x.end())`. `_List_const_iterator` must have a converting ctor from `_List_iterator` and
  no user copy ctor, or the iterators are copy-constructed through the stack (0x00A84470).
- **STLport `pair` explicit memberwise copy ctor.** `pair(const pair&)` fixes the scheduling where an implicit copy
  hoists `lea ebp` early. Use it when the key is itself a pair<long,long> (0x00BEF350).
- **Pair copy split shapes.**
  - Key dword, then `rep movsd`: a user memberwise pair copy ctor over a POD second (0x007590C0, 0x0081C700). A
    smaller POD second (five dwords) copies as a block through its own address instead (`add eax,4`) (0x0081C670).
  - `lea ecx,[edi+4]` source re-basing: a flat struct.
  - `add eax,4` / `add edi,4` re-basing: a nested struct copied through its own address.
- **A big comparator copied memberwise into the argument area** (dwords through ecx/edx, the bool byte from a register
  hoisted out of the loop) is the implicit copy of a comparator whose C3DVector members have a user copy ctor, not a
  `rep movsd` (0x007FA980, 0x0080A3F0).
- **A polymorphic member's implicit copy** shows as a single vtable store before the members (0x00BEE990,
  CCompositeSurfaceInfo : CBase).
- **A 16-byte element whose +0xC is never copied** is an `unsigned __int64` key (8-byte aligned) plus a 4-byte value
  plus padding (0x00A81D90, 0x00A81E80, 0x00A84470).
- **An `and esp,-8` frame** comes on its own from an 8-byte-aligned pair temporary. Do not try to force it
  (0x009B8170).
- **STLport random-access `__copy`.** The shape is `__copy(first, last, result, const random_access_iterator_tag&,
  Distance*)` with `for (n = last - first; n > 0; --n)`, `ret 0Ch`, and `result` returned on both paths. The count is
  kept in the dead tag argument slot. Such a body can be mislabelled as a sort_heap through skeleton propagation
  (0x005E18B0).

Review at landing:
- **Same-TU stand-in bodies.** 00a84470 (`_M_insert_dispatch`, real one at 0x005152C0) and 00bef350
  (`__uninitialized_copy<CCompositeBlendDesc>`) define a `__declspec(noinline)` callee body in the TU. The calls resolve
  locally, so they grade RELOCATION_MATCH. The lander should confirm that the stand-in's own function is excluded and
  the call is treated as a relocation to the retail address.
- **Local models next to an existing engine header:**
  - 009b8ca0 models `CDefinitionManager` (nested CDefClassInfo) locally.
  - 005e18b0 models `CIVCountedPointeeBase` locally, because the generated header flattens the vptr and cannot make the
    slot-1 virtual Destroy call. The landed 0x005E1820 does the same.
  - No `this` is modelled anywhere (all rows are free templates).
- **005e18b0 is a `std::__copy`, not a sort_heap.** Retitle it at landing (line 1 already says `std::__copy`, with leaf
  `__copy`).
- **Callee manifest labels found wrong** (list in NOTES.md, Label corrections): 0x005E1B60, 0x006BA180, 0x00A81F10,
  0x00758910, 0x007FA7B0, 0x00808DB0, 0x0081B910, 0x0081AB40, 0x009B8770, 0x00A32500, 0x00A41630, 0x00A71BE0,
  0x00A83570, 0x00BEE070 and 0x00BEEDA0.
- **Rows that needed more than one grade:**

  | Row | Grades | Fix |
  |---|---|---|
  | 00a84470 | 3 | same-TU noinline `_M_insert_dispatch` |
  | 007590c0 | 2 | no guard |
  | 00a338c0 | 2 | `__forceinline` |
  | 00bef350 | 2 | pair copy ctor |
  | 006fcb90 | 2 | `__forceinline` |

  None needed more than 4.
