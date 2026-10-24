# Chunk 422 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Same-TU callee bodies make an address-taken local "uncaptured"** (0xA81E20, 0xA82B10, 0xA74EA0).
  - Retail signatures: a field of an address-taken local or by-value argument stays in a register across calls, a
    store into it that a later call could read is dropped, or swap + destructor fold into loads with no stores back.
  - The fix: give every out-of-line callee that receives the object's address a same-TU `__declspec(noinline)` body
    under `#pragma auto_inline(off)`. Here those were list<C2DLineF> operator= 0x517720, ~_List_base 0x5127A0,
    _M_insert_dispatch 0x5152C0, and vector reserve / _M_range_insert / erase. VC7.1 then sees that the address does
    not escape.
  - With declaration-only callees it misses: 86v88 (key re-read every loop iteration), 144v138 (dead proxy zero store
    kept, key spilled), 173v173 (start loads scheduled the other way).
  - Neither /Oa nor the pair's copy-ctor form helps. Try this before calling a row an LTCG gap.
- `CArray<T>::DeallocateUnusedMemory()` is `if (capacity() > size()) { CArray<T> temp; temp.reserve(size());
  temp.insert(temp.begin(), begin(), end()); DeallocateMemory(); swap(temp); }` with STLport vector::swap. Retail's
  "old start loaded first, three words taken, old start freed" comes from swap plus ~temp once the callees have same-TU
  bodies (0xA74EA0).
- The __false_type tag of an inline list copy ctor, placed inside `__unguarded_insertion_sort_aux`'s argument build,
  lands in the comparator's argument slot only when `_M_insert_dispatch` has a same-TU noinline body. Declaration-only,
  VC opens its own `push ecx` frame slot: 110v109 (0xA82960).
- STLport `__copy_ptrs(first, last, result, const __false_type&)` (`ret 8`) and `iter_swap(a, b)` (`/Gr`, plain `ret`)
  over list-bearing elements need `__forceinline` on the inner `__copy` / `__iter_swap`. Plain `inline` leaves an
  out-of-line cdecl call: 25v47 and 13v138 (0xA82AC0, 0xA82B10).
- `__partial_sort` over a 16-byte pair<unsigned __int64, bool | T*> inlines `__pop_heap` even though STLport's pair has
  an explicit copy ctor (0xA828C0 / 0xA829E0). Over pair<long, {CCharString, CCountedPointer}> it stays out of line
  (0xA73330).
- STLport `__linear_insert`: `copy_backward` → inline `__copy_backward_ptrs(__false_type)` → out-of-line random-access
  `__copy_backward(first, last, last + 1, random_access_iterator_tag(), (int*)0)` (`ret 0Ch`). With a declaration-only
  callee the tag is homed in the comparator slot, which is dead on that path. `*first = val` is the pair's trivial
  four-dword assignment; the by-value argument uses the memberwise copy ctor (0xA81C50 / 0xA81CC0).
- `__destroy_aux` over STLport lists with an inline `~_List_base` / `clear()` (0xA827D0):
  - Node frees have no null test, because the node was just dereferenced; the sentinel's `deallocate` keeps its test.
  - The 16-byte loop-top padding (`lea ecx,[ecx]`, `mov edi,edi`) comes out by itself.
- A list's `_List_base` ctor must build the sentinel in a local and publish it to `_M_node._M_data` last. With the
  member-first spelling, VC re-reads the member after the store (0xA78680).
- A memberwise user copy ctor on C2DVector (`X(v.X), Y(v.Y)`) turns a pointer-register block copy of C2DLineF into
  retail's dword moves (0xA74A20 / 0xA74CA0).
- Name the `vector::erase` bound locals (LowerBound first) to get retail's call order. Give the inline erase's `__copy`
  and `__destroy_aux` same-TU noinline bodies so their tags sit on live slots (0xA79290).
- Byte-identical twins:
  - The non-const and const iterator instantiations of a member template (`_M_allocate_and_copy`, 0xA7E9D0 / 0xA7F9F0).
  - The same algorithm over pair<u64,bool> and pair<u64,ptr> (0xA7D570 / 0xA7D610). These differ only in the
    value-word copy when the value is copied by byte.
- GFProcessBreath (0xA762A0): `double now = GFGetTime(); if (lastBreathTime - now >= 0.005) { process.SuspendableProcess();
  lastBreathTime = now; }`. The named double gives the `fst` / reload.
- A `for_each` functor with user ctors returns through the hidden pointer (`mov eax,ecx`, nothing copied when the
  functor is empty) (0xA7CB50).
- Element identification:
  - The CNavigationLayer PDB members name the 4-byte-list containers of the 0xA8xxxx nav TU: LineLists is
    vector<list<C2DLineF> > and SwitchableLineLists is CVectorMap<unsigned __int64, list<C2DLineF> >. The list node is
    0x18 bytes.
  - CNavRegion::Connections is list<CCountedPointer<CNavRegionConnection> >, with 0x10-byte nodes.

Review at landing:
- 0xA74EA0's same-TU `_M_range_insert` and `erase` are minimal stand-ins, not the real bodies. They append, and they
  copy the tail down, and neither touches the tag. Its reserve is the real STLport logic without the out-of-line
  helpers. Precedent: the landed 0xA85420 stand-in. Swap in the real 0xA74AC0 / 0xA748E0 bodies if the lander wants
  them, and regrade.
- 0xA81E20 / 0xA82B10 / 0xA82960 carry hand-written STLport bodies of list<C2DLineF> operator=, destructor and range
  insert as same-TU noinline functions. Only their presence matters for the graded leaf.
- 0xA79240 used 4 grades plus 1 SRC_FAIL (a cast typo) and is parked. Its next lever is the same-TU-bodies trick above.
- The engine header CNavQuadTree.h carries PDB offsets (three or five 16-byte PDB vectors are 12 bytes in retail).
  0xA79240 and 0xA79290 use local CNavQuadTree models.
- Problems in already-landed sources:
  - 0x005127A0 is landed as `CGuiControlTreePane_SortTreeRecursively_005127a0.cpp` (catalog
    `StdList_Clear_005127a0`). It is the STLport list destructor (`clear()` + free the sentinel) for a POD-valued list:
    list<C2DLineF> in this TU.
  - The landed 0xA820E0 (`std_Pushheap`) and 0xA821C0 (`CopyBackward_SPairWithList10`) model the element as {key,
    "8-byte list {head, size}"}. Retail STLport lists are 4 bytes, so the element is pair<unsigned __int64,
    list<C2DLineF> > (+8 list, +0xC padding).
  - The landed 0xA82650 `Copy_SListHandle04` is `__copy` over list<C2DLineF>, and 0xA82730 `SValue16` is C2DLineF.
  - The landed 0xA84FB0 "CRetailVector_CElement16::push_back" is vector<pair<u64, list<C2DLineF> > >::push_back.
    Its "InsertGrow" 0xA84C20 is `_M_insert_overflow(pos, x, __false_type, 1, true)`.
  - The landed 0xA81D90 / 0xA81E80 are named Dinkumware `_Sort_heap`; the retail STL is STLport `sort_heap`.
  - The landed 0xA73840 comment says "20-byte records", but the stride is 16.
  - The landed callee labels of 0xA71B40 / 0xA72AA0 use CNavConnectorNode / CFaction element names, while their callers
    sort the C3DAnimFile chunk records (folded bodies).
