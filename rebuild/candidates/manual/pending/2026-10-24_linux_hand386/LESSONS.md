# Chunk 386 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **`return a || b;` in a bool function** gives full-width exits (`mov eax,1` / `xor eax,eax`) (0x006B9460, CFaction::IsAlliedWithFaction).
- **STLport `set::count` vs `find != end()`:** `count` is `_M_find(k) == _M_header ? 0 : 1`, which reads the header after the call. A `find(k) != end()` written by hand puts the end read before the call (0x006B9460).
- **VC7.1 `<math.h>` float `floor` overload:** `inline float floor(float x) { return (float)floor((double)x); }` gives the float-typed `fsubr dword` / `fmul dword` and a 4-byte frame float (0x006BB300).
- **Same-TU `__declspec(noinline)` pair `operator=` body:** lets VC keep a by-value argument's word in a register across the call. Declared-only gives 128v113 (0x006BA7B0).
- **STLport `iter_swap`:** needs `__forceinline __iter_swap`; plain `inline` gives 13v185 (0x006BE870).
- **`_M_insert_overflow` frame:** fixed by same-TU `auto_inline(off)` bodies of `__uninitialized_copy` and `__uninitialized_fill_n`. Declared-only gives `sub esp,8` instead of `push ecx` (0x006B7F70).
- **Found-arm-first lookup:** `if (it != end()) { out = it->second; return true; } return false;` reproduces retail's found-arm-first branch layout; the early `== end() return false` form does not (0x006BC350).
- **High register pressure in `__partial_sort`:** one source turns the pointer loop into a trip count held in the dead `last` slot (0x006BF340, same source as 0x006BADB0).
- **CArray push_back:** a `CArray<T>::push_back` that calls the in-class STLport `vector::push_back` is NOT inlined (a 5-byte `jmp`). Write the STLport body in the CArray member (0x006BF410).
- **CIntelligentPointer copy ctor:** write it through typed inline `T* GetPItem() const` / `SetPItem(T*)` wrappers in `CIntelligentPointer<T>`. The extra inline layer raises the data pointer's register weight and fixes a this/data ebp<->ebx swap (0x006C1E70, back_insert_iterator over a list).
- **Emitting an implicit copy assignment out of line:** take its address in a NON-const global member pointer (`T& (T::*g)(const T&) = &T::operator=;`). A `const` one has internal linkage and is discarded, leaving no symbol (0x006C2940).
- **Local vector copy:** a local `vector` copy walked by index keeps its bounds in registers (trip count hoisted into a `dec` counter; destroy loop against a held `finish`) only when the vector copy ctor and `_Vector_base(n, a)` have same-TU `auto_inline(off)` bodies. Reconfirms the 0094cb40 lesson (0x006C3E80, 203v184 -> match).
- **Empty predicate functor:** needs a user constructor. A POD `CIsThingDead()` temporary zero-stores its byte (`mov byte ptr [esp+18h],0`) (0x006C3E80).
- **STLport 4.5 map iteration:** `_M_node = _Rb_global<bool>::_M_increment(_M_node)` (static, returns the node) walks in eax/ecx and copies back (`cmp eax,edi ; mov esi,eax`). The SGI member-walking `_M_increment` walks the iterator register itself. With a call in the loop body, `end()` must be a named local (header kept in edi); `it != map.end()` re-reads the header after every call (0x006C5780 / 0x006C5800). Without a call both forms match (0x006C5710).
- **`find(..) == v.end()` around a by-value temporary:** VC7.1 reads `end()` once for both the argument and the compare, even across the out-of-line `find` call. The temporary dies before the branch (`sete bl`), and the random-access tag takes the dead argument slot (0x006C3DF0).

Review at landing:
- 0x006B9460 needed 5 grades (4 to match, plus a bool-return re-check against the PDB signature).
- Local models sit next to existing engine headers:
  - **CRegion (0x006BC290 / 0x006BC350):** engine/CRegion.h uses the PDB's 16-byte containers.
  - **CLevelLoader (0x006C2120 / 0x006C2940):** engine/CLevelLoader.h has the PDB 12-byte list at +0x14, so the members after it sit high. It also cannot host the nested `CLevelLoader::CObject` that the mangled leaf needs.
  - **CTCBase (0x006C3DF0, 0x006C3E80, 0x006C5710, 0x006C5780, 0x006C5800):** engine/CTCBase.h forward-declares `struct CThing;`, which would mangle the PDB's `VCThing` parameters as `UCThing`.
- Errors noticed in already-landed sources:
  - CFaction_SetAlliedToFaction 0x006B94E0 models the set as a VC6 `_Tree` (comp +0, head +4). Retail STLport has the header at +0.
  - CFactionManager_GetPFaction 0x006B93D0 sits in compiled/ with a "Grade: DIFFER" header.
  - Landed 0x006B6FF0 models CBlendedEnvironmentTheme as an opaque "SThemeBlock".
  - Landed CObject_OnAdd 0x006C20B0 declares `OnAdd(CLevelLoader*)` on a global `CObject`. Ego_r.pdb has `?OnAdd@CObject@CLevelLoader@@QAEXABV2@@Z`, i.e. `CLevelLoader::CObject::OnAdd(const CLevelLoader&)`.
  - Landed CTCFadeOutAndIn::FrameUpdate 0x006C3F40 names its 0x006C3E80 callee `RemoveDeadFadingThings`. Ego_r.pdb names it `UpdateFadingThings`.
  - Landed 0x0062ED40 carries the descriptive class `CRetailVector_CIntelligentPointer_0062ed40`. It is `vector<CIntelligentPointer<CThing>>::vector(const vector&)`.
- Tests that include standard headers: 0x006C3DF0 and 0x006C3E80 use plain C++98 `<vector>`/`<algorithm>`, and 0x006C5710/0x006C5780/0x006C5800 use `<map>`. All five pass under g++ and the VC7.1 lane.
