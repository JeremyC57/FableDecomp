# Chunk 454 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- VC7.1 names a TU's dynamic initializer and atexit thunk `_$E1` / `_$E2`. Use that as the `// Leaf:`; a catalog
  "$E2" can really be `_$E1` (0x0121AEC0).
- When an atexit thunk tears down a static array of a class with a dtor, it is one `vector destructor iterator` call
  (`??_I`, 0x00401400, mislabelled "constructor iterator"). Reproducing it takes a USER-DECLARED out-of-line element
  dtor plus `#pragma optimize("s", on)`; /O2 or an implicit dtor gives an inline loop (0x0122A20A).
- An unaligned function address means a size-optimised TU. Add `// Flags: /Os` (or the pragma) before trying other
  shapes: deque `_M_pop_front_aux` at 0x00493C25 / 0x00655CA7, `set::insert` at 0x005BAF94.
- PDB types are `class` (mangled `V`), STLport `pair` / `_Deque_iterator` are `struct` (`U`). A wrong letter makes the
  leaf prefix match no symbol, and the grade fails silently with a "no block" (0x00493C25).
- A leaf prefix that stops at the class name can also match the implicit copy ctor of a template class. Use the full
  mangled ctor name, and check with calls.py that the relocations line up (0x00E7E020).
- To get a mangled-prefix leaf for a member, write `?Name@Class@@`; the bare "Name" matches nothing (0x00C21750).
- Explicitly instantiate only the member being graded
  (`template void _STL::deque<T>::_M_pop_front_aux();`). A whole-class instantiation lets VC7.1 inline the aux into
  pop_front (0x00493C25).
- STLport tag dispatch:
  - `__false_type` / `__true_type` need a user ctor `{ }`, otherwise VC7.1 zero-stores the tag temporary.
  - Define the out-of-line callee's real body in the same TU, `__declspec(noinline)` under `#pragma auto_inline(off)`.
    This homes the tag temporary in the dead `last` argument slot (0x0048D490 `_Destroy`, 0x00A0EAF0
    `uninitialized_fill`).
  - A declaration-only callee takes a fresh frame slot instead.
- The STLport deque iterator needs its user copy ctor `_Deque_iterator(const iterator&)`. It gives the
  single-register member-wise outgoing copies of by-value iterators (0x0048D490, 0x00A0EAF0).
- `set::insert` returns `pair<iterator,bool>(__p.first, __p.second)`, built from the tree's non-const pair through the
  pair converting ctor and the iterator's converting ctor (0x005BAF94).
- `basic_string(const char*)` is `_M_range_initialize(s, s + strlen(s), forward_iterator_tag())` with
  `#pragma intrinsic(strlen)`. The tag goes by const reference (0x00568440).
- The operand order of a self-assignment test picks the cmp operand order: `if (&other != this)` gives `cmp ebx,edi`
  (0x006FBBD0 CSmallVector).
- C3DVector with a memberwise USER copy ctor lets VC7.1 forward a unary-minus temporary's X/Y straight from x87. The
  implicit copy goes through integer moves (0x0121AEC0).
- Putting a call result in a named local makes it run before the next call's argument pushes; the nested form pushes
  the later arguments first (0x00C21750).
- Def and refcount atexit thunks show `add eax,N ; dec [eax] ; cmp [eax],0`, or a register decrement plus an explicit
  `test`, through a formed pointer. That is the volatile-count signature. Park these after one or two grades as
  "volatile candidate" (0x01228B70, 0x0122AE7E, likely 0x0122A550).
- finish.py's line-1 parser: write `_STL::fill(T* first, ...)` / `_STL::deque::_M_pop_front_aux()`, with the
  instantiation in the parameters or the description. An `operator()` line keeps its name only when nothing before it
  contains whitespace.

Review at landing:
- Parked volatile candidates 01228b70 and 0122ae7e, and 0122a550 (the same `add eax,N` formation); all three are
  `_$E2` atexit thunks.
- Rows that used the full 4-grade budget: 0122a20a, 0048d490, 00a0eaf0 (matched), and 01228b70, 0122ae7e, 0122a550
  (parked).
- Unproven identities:
  - 00655ca7: the deque owner, probably NPlayerGui::CDrawGuildSeal::Messages via ClearMessages 0x0064AE66.
  - 0048d490 / 00a0eaf0: the deque element C2DVector, inferred from the 8-byte stride and the
    CJoystickPositionTracker::RecentPositions PDB type.
  - 01228df0: the static tree's element type is not recoverable (descriptive name).
- 00c21750 reuses chunk 434's CSoundEvent model, whose libpk types are `struct` (U). The leaf does not encode them
  (`?AssignPhysicalPlayer@CSoundEvent@@` prefix), but check against agent434 if the full mangled name is ever needed.
- Local base models instead of shared headers: CCharString / CWideString / CVector / CArray / CList / CIntelligentPointer
  / CCountedPointer / the STLport `_STL` pieces are minimal local models in every source. Retype them with
  `rebuild/include/engine/<Class>.h` where a header exists.
- 00a6f910 and 00e7e020 declare callees only (`__unguarded_linear_insert`, CScriptThing copy ctor). Their relocations
  were checked with calls.py.
