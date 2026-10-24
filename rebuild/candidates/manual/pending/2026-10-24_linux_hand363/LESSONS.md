# Chunk 363 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport `__uninitialized_copy`: if a direct `for (...; ++first, ++result) new (result) T(*first)` loop keeps a second, strength-reduced destination pointer, use STLport's literal `T* cur = result; for (; first != last; ++first, ++cur) _Construct(cur, *first); return cur;` with `template <class T1, class T2> inline void _Construct(T1* p, const T2& v) { new (p) T1(v); }` (004cfa10 96v83 -> 83). Some instantiations match the direct form (00cba3e0), so try both.
- Pair elements reproduce the nested-copy register shapes only with STLport's explicit `pair(const pair<T1,T2>& o) : first(o.first), second(o.second) {}` over the real nested member records (004b8210, 00a6fed0, 00bedb60). A flattened struct differs in scheduling (004b8210 flat 73v73 DIFFER).
- `_Rb_tree` lookups with a CCharString key: put the `CBasicString<char>::operator<` body (0x00429950 shape, `__declspec(noinline)`) in the TU. VC7.1 then sees that the compare writes no memory and hoists the key's data pointer out of the loop (007acd30 65v67 -> 67).
- Dinkumware `_Transform` into a `back_insert_iterator`: give the iterator its `explicit back_insert_iterator(C&)` user ctor. Without it the 4-byte inserter comes back in eax and first/last shift to ecx/edx (007896af 50v55).
- A `std::find_if` that forwards to `__find_if(first, last, pred, random_access_iterator_tag())` homes the empty tag in the predicate's own argument slot only when the `__find_if` body is in the TU (`__declspec(noinline)`). Use `// Leaf: ??$find_if@`, because the leaf `find_if` also matches the larger `__find_if` (0080e2a0 77v75 / 291v75 -> 75).
- A persist wrapper `Transfer(name, value) { Transfer(name, value, T()); }` over a POD: value-initialisation `T()` schedules the value-slot load after the zero stores. Retail's early `mov edx,[esp+20h]` comes from a user default ctor that zeroes each field (006861c0).
- Callee clobber knowledge cuts both ways in one image: 00c18b20 calls same-TU rotations defined above under `#pragma auto_inline(off)` and never reloads ecx. The twin 00c1b470 keeps `this` in edi and does `mov ecx,edi` before each call, which is the shape when the rotations are only declared.
- VC7-era Dinkumware `vector<T>::_Ucopy` for a scalar T is `memmove(ptr, first, n*sizeof(T)) + n` through `extern "C" __declspec(dllimport) memmove` (IAT 0x01440144): thiscall, `ret 0Ch` (00c13ec0). The catalog's `_Umove` is the VC8 name.
- STLport `sort_heap` over a polymorphic 16-byte pair matched on the first try (0076b650) with pop_heap, `__pop_heap_aux` and `__pop_heap` all inline, reusing the landed `__pop_heap_aux` 0x0076AC70 element model:
  - The implicit copy ctor stores the vtable immediate into the 16-byte frame temporary.
  - The implicit assignment skips the vptr.
  - The argument-area copy uses the explicit pair copy ctor.
- A parameter block built from arguments (006b8550, CRadialBlurParams) lays its stores out in source order. The in-place ctor init list follows declaration order (colours first); retail's order (scalars, intensity, threshold, then the 1.0f alphas deferred past the pushes) comes from member-wise assignment in that order.

Review at landing:
- 0080e2a0 took 5 grades. The 5th only changed the `// Leaf:` line from `find_if` to `??$find_if@`, because the 4th had already matched `__find_if`.
- 006b8550 (parked) is probably one statement swap (`Fade` before `Length`) from a match. Re-grade it first.
- Local polymorphic models sit next to existing engine headers: CScriptThing.h (008ac140), CVertexShader.h (00b6bab0), CMeshDataBankEntry.h (00a2ae80). The headers flatten the vptr as `void* __vftable`, which cannot reproduce a vtable-storing implicit copy ctor.
- Already-landed sources:
  - The landed `CTreeNode_00c194f0::CTreeNode` (0x00C194F0) is actually the libpk PKCTRRedblackTree constructor: vtable 0x012B2F0C, the allocator at +4/+8, the nil sentinel at +0xC (self-linked, black), the root at +0x20 = &sentinel, the count at +0x24. Its "tree node" naming should be corrected.
  - The landed 0x0076A4E0 (`_Adjust_heap`, Dinkumware model) and 0x0076AC70 (STLport `__pop_heap_aux`, which calls it `__adjust_heap`) name the same callee in two library dialects. The algorithm TUs here are STLport.
  - The landed `_Lbound` sources 0x007ACC00 / 0x007ACBB0 are old-style fastcall-with-dummy-edx models. Re-model them like 007acd30 (STLport `_M_lower_bound`, thiscall, `ret 4`).
- Catalog rows holding several functions: none. 004cfac0 is ICF-folded (one body shared with the CActionInputControl vector TU), not a multi-function row.
