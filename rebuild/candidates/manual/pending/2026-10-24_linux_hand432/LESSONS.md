# Chunk 432 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport map clear loop: `iterator next = it; ++next; erase(it); it = next;` keeps the erased node in ecx and advances the copy in esi (0x00BE05D0).
- `_Rb_tree::_M_insert`: an explicit `template class _Rb_tree<...>;` instantiation is what lets VC7.1 inline `_M_create_node` in the left-link branch and call it out of line in the right-link branch, as retail does (0x00BE0B20).
- A NAMED `C3DVector v(x, y, z)` local copied member-wise reproduces the z-through-frame-word integer copy and the `fxch st(2)` + 3 `fstp` order (0x00BE66F0 / 0x00BFDEC0).
- if/else into an `unsigned long` local gives one `movzx` per arm, and a ternary widens once after the merge. A separate counter local takes a different register than incrementing the parameter (0x00BEBBE0).
- Reading a float member into a local before a flag test hoists the `fld` above the test. `member * global` written inline loads the global first (0x00BEB660).
- The /Gr STLport sort family of CVectorMap<long, CCompositeSurfaceInfo> and CVectorMap<pair<long,long>, CEngineTextureDecal> (0xBED790..0xBEF830) compiles from VERBATIM STLport 4.6 bodies:
  - Write them as templates in `namespace _STL`, with the comparator's `operator()` inline. Make it declaration-only in `__unguarded_insertion_sort_aux`.
  - Keep `__unguarded_linear_insert` / `__linear_insert` / `__adjust_heap` / `__pop_heap` / `sort_heap` / `iter_swap` declared out of line.
  - VC itself calls the 16-byte pair's implicit operator= out of line inside `__linear_insert` (0x00BEE8F0).
  - VC itself calls the decal vector's copy ctor out of line in `iter_swap` / `__linear_insert` / `_M_insert_overflow` and inlines it elsewhere, given an inline-defined STLport copy ctor (`_Vector_base(n, a)` + `__uninitialized_copy(..., __false_type())`).
- `vector::reserve` with `_M_allocate_and_copy` (0x00BEEB80): retail homes the new block in the dead n slot. That needs a same-TU `__declspec(noinline)` body for `__uninitialized_copy`. With it declaration-only, the block and the saved finish swap slots.
- `iter_swap` of a 28-byte entry (0x00BEE280): VC7.1 keeps `__iter_swap` out of line unless it is `__forceinline`.
- Decal-entry copies (0x00BEE660): STLport's explicit memberwise `pair(const pair&)` copy ctor is needed. With the implicit one, the key's second word is copied through the base pointers instead of the biased ones. This matches the landed sort_heap 0x00BEF350 note.
- Decal entries (pair<pair<long,long>, Decal>) passed by value: retail keeps the value's two key words in ebx/ebp across the vector operator= calls, but verbatim VC7.1 re-reads them from the argument slot.
  - The model that matches 0x00BEE320 reads `const K key = val.first;` once, compares with a `(const K&, const pair&)` comparator overload, and stores `last->first = key; last->second = val.second;`.
  - The same trick leaves 0x00BEF2B0 two bytes long (register swap). The landed __push_heap 0x00BEE420 needed the same family of trick.
- STLport `__unguarded_partition` keeps the pivot key in registers verbatim (0x00BEEC60), because nothing takes the pivot's address.
- A Leaf naming `V` (class) while the source declares a `struct` (U) matches no symbol, and q.py silently grades the largest function. 0x00BE66F0 / 0x00BFDEC0 still "matched" that way until the vertex types became `class`. Check every leaf with `objdump -t`.

Review at landing:
- Rows that needed 4 grades:
  - 0x00BE66F0, 0x00BEB660, 0x00BEE320 (matched at grade 4).
  - 0x00BEC8D0 and 0x00BEF2B0 (parked).
- Tests:
  - The 19 STLport rows share two generated test models: `gen/testA_common.txt` and `gen/testB_common.txt`, built by `gen/mk_testsA.py` and `gen/mk_testsB.py`.
  - The tree tests come from `gen/tree_tests.py`.
- Local models stand in where no engine header exists: CCountedPointer, CCompositeSurfaceInfo, CEngineTextureDecal, the STLport containers. Check whether `rebuild/include/engine/CCompositeSurfaceInfo.h` / `CEngineTextureDecal.h` exist with retail (STLport) sizes before landing. The generated headers may carry Ego_r Dinkumware layouts (16-byte vectors).
- 0x00BE66F0 (and the parked 0x00BE6800) declare a local `class CLandscapeBackgroundPatch`, even though `engine/CLandscapeBackgroundPatch.h` exists. The reason: that generated struct declares no member functions, and these static members need the `?...@CLandscapeBackgroundPatch@@SI...` mangling. The bodies never touch `this`, so the local class has no data members. Decide at landing whether the header should grow the static declarations.
- Already-landed sources with wrong identities:
  - 0x00BEE710 "CRandomAppearanceMorph::UninitializedFillN" is the decal-entry `__uninitialized_fill_n`.
  - 0x00BED9A0 "CopyBackward_PairLongPointerHolder" is `__copy_backward<pair<long, CCompositeSurfaceInfo>*>`.
  - 0x00BEDD70's file name and line 1 name pair<long, CCountedPointer<CNavConnectorNode>>, but its body is the CCompositeSurfaceInfo `__adjust_heap`.
  - 0x00BEF5C0 "Sort_SElement16" is `_STL::sort` over the CCompositeSurfaceInfo entries.
  - The landed std_sortheap_00bee990 / 00bef350 line 1s say `std::`, but retail is STLport `_STL`.
- The CRT tail (0x00BFE9C8 report_failure .. 0x00BFEBB0 __aullrem) is MSVCRT hand asm / seccook.c and was not authored.
- 0x00BE6800 / 0x00BFDFD0 are retail_asm candidates (bare dword fistp ROUND helper, as the landed 0x00BE6790 / 0x00BFDF60).
