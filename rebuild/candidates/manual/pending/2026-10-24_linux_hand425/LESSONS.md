# Chunk 425 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- An empty STLport tag (`__true_type` / `__false_type` / `random_access_iterator_tag`) that retail homes on a LIVE
  argument or local slot (no frame of its own) needs the tag-taking callee DEFINED in the TU, as a
  `__declspec(noinline)` body under `#pragma auto_inline(off)`. A declaration-only callee gives the tag its own frame
  slot. Seen in:
  - 0x00AD91F0: `_M_insert_overflow`, tag on the `group` slot.
  - 0x00ADBC50: `__copy_backward`, tag on the comparator slot.
  - 0x00ADCB90: `__uninitialized_copy` + `__destroy_aux`, both tags on `n`.
  - 0x00ADD020: `_M_insert_dispatch`.
- An empty functor passed by value (`less<T>()`) needs a user default constructor, `less() { }`. Without one VC7.1
  zero-stores each temporary (`mov byte ptr [esp+8],0`). With one it pushes the uninitialised frame word, as retail does
  (0x00ADC890 `sort<CBlockInfo*>`).
- Model STLport `_List_iterator<T, _Traits>` with its only copy-like constructor, `_List_iterator(const iterator&)`.
  That constructor is the user copy constructor of `iterator`, so an iterator argument is built in its slot
  (`push ecx ; mov ecx,esp ; mov [ecx],eax`), while `const_iterator` keeps the implicit copy and is pushed from its
  register (0x00ADD020 `list::operator=`, 0x00ADCC80 `_M_insert_dispatch`).
- A recursive `_Rb_tree::_M_erase` whose body is visible in the TU is inlined ONE level into `clear()` /
  `~_Rb_tree()`; the recursion stays a call (0x004507A1). The retail shape `test ; je ; call _M_erase(root)` + the
  header reset comes from the full body, not from a declaration (0x00ADC190, 0x00ADC250).
- STLport `vector::_M_allocate_and_copy` is a member template. reserve / `_M_insert_overflow` instantiate
  `__uninitialized_copy<T*, T*>` (0x00ADC330), while operator= instantiates `<const T*, T*>` (0x00ADC440). The bodies
  are byte twins, and line 1 / the leaf must name the right pointer constness.
- `if (c) x = A; else x = B; f(desc); return true;`: VC7.1 duplicates the call + return tail into both arms. Writing
  the call in each arm by hand schedules the second arm differently (0x00ADD240 `CLHFile::Read`, priority select
  before `AddObject`).
- A null-guarded inline accessor reproduces retail's branch order only when written null arm first:
  `return p == 0 ? 0 : p->Length;`. `p ? p->Length : 0` inverts both branches (0x00ADD650 `C3DBone2::SetName`, the
  CCharString GetString / Length pair).
- An explicit specialisation DECLARATION of an inner template callee (`template <> T* __uninitialized_copy<const
  COptimisedBone*, COptimisedBone*>(...);`) keeps that callee out of line while the outer instance inlines everything
  else (0x00AD75F0, 0x00AD7DC0).
- Leaf checks:
  - A member-template instance mangles as `??$name@...`, not `?name@...`, so a leaf written `?_M_insert_dispatch@`
    matches no symbol and silently grades the largest function (0x00ADCC80).
  - An overloaded member (`list::erase(iterator)` vs `erase(iterator, iterator)`) needs the leaf extended past the
    first argument (0x00ADC4E0).
- Lane gap (park after 2 grades) in `map::operator[]`, `value_type(k, T())` + `insert(it, value)`: retail stores
  some pair words AFTER the `push edx` of the hidden iterator result; the lane stores them before the iterator copy.
  `__default_constructed`, `_Tp()` and the full map -> _Rb_tree layering do not move them (0x00AD6420, 0x00AD64C0,
  0x00ADBFF0).
- `pair<iterator, bool>` returned from a local `set::insert` needs user constructors on the pair, or it comes back in
  eax:edx instead of through the hidden pointer (0x00AD9120).

Review at landing:
- 0x00ADC330 and 0x00ADC440 are byte twins (the `<T*, T*>` and `<const T*, T*>` instances of `__uninitialized_copy`
  over CVertexGroupBlock). Keep both with distinct line-1 names.
- Already-landed 0x00ADF1E0 "CLandscapeBackgroundPatch_ClearTextures" is really `CParticleSimulation::Clear()`. The
  0x00AE0600 constructor calls it, and its stores hit CParticleSimulation members (vtable 0x0129F0C4 RTTI).
- Already-landed 0x00ADBE70 (`__insertion_sort`) and 0x00ADC080 model the 12-byte element as SLongKeyAt4Record /
  SElement12. It is `CVertexGroupBlockOptimiser::CBlockInfo { BlockIndex, Cost, Benefit }` with `operator<` on Cost.
  Retype them to share one model with the chunk-425 sort family.
- Already-landed 0x00ADD190 "GetMode" returns the block size (0x200 / 0x800 / 1). Rename it to GetBlockSize; 0x00ADD240
  calls it twice.
- The 0x00ADD240 name `CLHFile::Read` is inferred (CLHFile vtable 0x0129F094 slot 1, an async block read with a
  CLHFileCompletionCallback); no PDB symbol confirms it.
- All rows use local models. No `rebuild/include/engine/` header exists for QuadricOptimiserInternals::*,
  CVertexGroupBlock(Optimiser), CLHFile / CLHFileCompletionCallback, C3DBone2 or CParticleSimulation. Switch to
  shared headers if they are generated.
- The 00ad71b0 name `COptimisedVertex::Remove` is descriptive: the body is Exists=false + FaceList.clear() +
  PairContractions.clear(), and no PDB symbol was found at the address.
- No row needed more than 4 grades and no catalog row holds a second function.
