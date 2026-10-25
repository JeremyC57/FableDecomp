# Chunk 390 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Retail STLport 4.x lives in namespace `std`, not `_STLP`/`std` (RTTI `.?AU?$pair@...@std@@`). It has 12-byte
  vectors and 4-byte lists (sentinel `_M_node`, node {next, prev, data}), and its allocator is
  `n != 0 ? malloc(n*sz) : 0` / `if (p) free(p)`. Name algorithm rows with the STLport spelling (`__merge_sort_loop`,
  `__stable_sort_adaptive`, `_M_insert_overflow`, `_M_allocate_and_copy`) (0x007107D0 .. 0x00711B80).
- If the retail TU holds the callee's body (a COMDAT later in the TU, or a header-inline), put a same-TU stand-in
  body under `#pragma auto_inline(off)` + `__declspec(noinline)`. VC7.1 then knows the callee leaves the caller's
  escaped locals alone, which can register-cache temporary members across the call, drop dead zero stores, hoist
  loop-invariant stores, and put tag / hidden-return temporaries in dead argument slots. A declaration alone gives
  extra frame words or a reload. Seen at 0x0070B240, 0x0070D0A0, 0x0070D2F0, 0x0070EC30, 0x00711B80, 0x007196D0,
  0x0071CF70 and 0x0071DE20.
- VC7.1 drops the `if (p != 0)` before `free(p)` when p was already dereferenced in the same function: the inline
  list clear / `~_List_base` frees the sentinel with no null test (0x0070EC30, the callee 0x0070BA00).
- STLport `_List_base(const allocator_type&)`: build the sentinel in a local and store it to `_M_node._M_data` last.
  Allocating straight into the member gives a store plus a reload (2 bytes longer, 0x00711D50).
- STLport tag types (`__true_type`, `__false_type`, `random_access_iterator_tag`) need a user constructor
  (`__false_type() {}`). Without one, VC7.1 writes `mov byte ptr [esp+N],0` for each tag temporary (0x00711E30,
  0x0070D2F0).
- STLport `pair` has an explicit `pair(const pair<_T1,_T2>&)` copy constructor next to its converting template.
  With only the implicit copy, the second word of an 8-byte pair is register-allocated differently in the placement
  copy loops (0x00718FB0).
- In list `insert(pos, x)`, link through the locals `_Node* n = pos._M_node; prev = n->_M_prev`, and give the
  iterator a user copy constructor (`_List_iterator(const iterator&)`). Reading `pos._M_node->_M_prev` twice
  reloads it (0x007119C0, 0x00706800). The const_iterator copy then pushes plainly, while a by-value iterator
  argument is built with `push ecx; mov ecx,esp` (0x00711E30).
- STLport `(min)` / `(max)` take const references. The computed operand is stored in a dead argument slot, and the
  result is the selected address (`lea ecx,[..] ; jb`). Write `(min)(a, b)` over references, not `a < b ? a : b`
  (0x007107D0, 0x00710980, 0x0070B240, 0x007196D0).
- VC7.1 /O2 unrolls a simple counted `for (i = 0; i < n; ++i) if (...) x *= m;` four times, then adds a remainder
  loop. The plain loop is the source (0x0071C120).
- `__merge_backward`: write the real STLport `copy_backward -> __copy_backward_aux(__true_type) ->
  __copy_backward_ptrs(__false_type) -> __copy_backward(random_access_iterator_tag, (int*)0)` chain. VC7.1's
  inlining budget then inlines the two early-out copies and keeps the two in-loop copies as calls to 0x007108A0
  (0x00710D00).
- An anonymous-namespace function with no caller in the TU is not emitted. To keep it as its own symbol, declare it,
  place a `_Use` caller, and only then give the definition (0x0070C770, 0x0071C120, 0x0071CF70).
- The FableWin and Ego_r PDB names beat the propagated labels. For 0x0070BD80 the PDB name is
  `?GFGetInterpolatedLoopingValue@@YIMMMMM_N@Z`: `__fastcall (float, float, float, float, bool)`, so the bool
  travels in cl.
- The Ego_r layout is not the retail layout for containers: Ego_r containers are 16 bytes, retail ones 12 (vector) or
  4 (list). Subtract 4 per vector and 12 per list from the PDB offsets. For example, CAnimSet is 0x54 bytes and
  CCompleteAnimation 0x60 in retail.

Review at landing:
- 0x00706800 took 5 grades (over the cap of 4). The source is final and graded RELOCATION_MATCH.
- The landed 0x0070C800 declares its callee as `AnimationInterpolation_CalcValue(bool, float x4)`. Its real form is
  `GFGetInterpolatedLoopingValue(float x4, bool)` (`__fastcall`, the bool in cl). The calling sequence is the same,
  so only the name and parameter order need fixing.
- The landed 0x00711CF0 "CAnimSet::CAnimSet" is CAnimateThing::CCompleteAnimation::CCompleteAnimation().
- The landed 0x00711520 "CNavigatorOwner_00711520::ResetBuffers" is CAnimSet::Clear (Ego_r `?Clear@CAnimSet@@QAEXXZ`).
- The landed 0x0071DD50 GetTattoos models the erase as Dinkumware. It is STLport (`__copy` + `__destroy_aux`
  0x005B26E9).
- Every row uses a local layout model. `rebuild/include/engine/CTCHeroAttachableAppearanceModifiers.h` exists but
  carries the PDB 16-byte containers, so it cannot model retail `this`. Retyping these rows needs retail-layout
  STLport headers.
- The bytes cannot tell which T 0x0071EC20 `std::swap` is for (any trivially copied 12-byte T folds to it). The source
  uses `pair<CSkeletalMorphs::CEntry, float>`, the only 12-byte T in that TU.
- The landed CTCHeroAttachableAppearanceModifiers code calls a member "Active" that the PDB names
  CModifierEntry::Rendered.
- The `__true_type` / `__false_type` tag model differs per row: some rows need the user constructor (0x00711E30,
  0x0070D2F0) and some rows match either way. A shared STLport header should give them the user constructor.
