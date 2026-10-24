# Chunk 396 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **C3DVector memberwise copy ctor** (`C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}`, pack 1) changes how VC7.1 caches a struct that contains it.
  - 0x007A4260 sort: a comparator copied by value has its words re-read from the parameter slot. With a POD C3DVector they are cached in ebx/ebp.
  - 0x007D9B40 random_shuffle<KnickKnack*>: the swap temporary keeps three position words in the frame and the pointer in eax, and the loop spills `-4 - first`. With a POD C3DVector the result was 163v159.
- **Constant 1 in ebx** -- an inline `bool IsDead() const { return (flags & 1) != 0; }` accessor makes VC keep the constant 1 in ebx. An open-coded bit test does not (0x007AA4D0, 230v231 -> match).
- **Empty STLport tag on a still-live argument slot** -- `lea r,[esp+x_slot]` is pushed for an empty `__false_type`/`__true_type`/`random_access_iterator_tag` while x itself is still used.
  - VC does this only when the callee has a same-TU `__declspec(noinline)` body that reads x and leaves the tag parameter unnamed. Seen for `_M_insert_overflow` in 0x007B4F20 and 0x007CA350, and for both `find` and `_M_insert_overflow` in 0x007CB850.
  - The single-element `insert(pos, x)` bodies (0x007D78A0, 0x007DC750, 0x007DCB10) home their tags on x's slot with only a declaration.
- **Traits sharing the dead count slot** -- a same-TU noinline `CPersistTraits<T>::TransferBinaryIn` body lets the traits object share the dead count slot (8-byte frame) in GFSerialiseVectorBinaryIn 0x007B5370.
- **Inline `_M_increment`** -- put STLport's `_Rb_tree` `_M_increment` inline in the TU when the range erase (0x007AD180) and the map writer (0x007AD270) inline the iterator walk.
- **Tag reference in edx** -- under /Gr, with a hidden result pointer in ecx (the result class has a user ctor), the tag reference rides in edx when it is the first fastcall-eligible argument after the hidden pointer (0x007B51B0 `__copy_aux`).
- **Named local vs temporary for an address argument** -- a CCharString passed by const& must be a named local in its own block to get `lea eax,[esp+8] ; push eax`, destroyed before the following compare. A temporary in the call passes the ctor's returned eax instead (0x007C53F0, 72v76 -> match).
- **Shrink-wrapped prologue** -- writing the same call in both else arms instead of one shared fall-through gives VC7.1's shrink-wrapped prologue. ebx/esi are pushed only on the deep path, and each path gets its own epilogue (0x007CABD0 `thing.Kill(false)` twice, 203v217 -> match).
- **`__forceinline` interface lookups** -- the `GetInterface` / `GetCheckedInterface` templates must be `__forceinline`. Plain inline left GetCheckedInterface as a call (0x007CABD0 first grade, 131v217).
- **Inlined container copy ctor depends on the caller** -- the STLport container copy ctor (`_Vector_base(n, a)`, allocate = malloc or null) is inlined into `__uninitialized_copy` / `__uninitialized_fill_n` (0x007DC500 / 0x007DC580) but called out of line from the bigger non-POD `insert` (0x007DCB10 -> 0x007DC350). Declare it without a body in the insert TU.
- **Counted iterator advance** -- `for (i = 0; i < n; ++i) { ++a; ++b; }` compiles to `if (n > 0) { a += n; b += n; }` (`lea edx,[ecx+ecx*2] ; lea eax,[eax+edx*4]`), as in 0x007DB5C0. The retail guard is `test ecx,ecx`.
- **Halton x87 shapes** (0x007D7A90 / 0x007D7AE0):
  - `float invBase = 1.0f / base` is `fild ; fdivr [1.0f] ; fst [frame]`.
  - `(base + 1.0) * h` is `fild ; fadd qword [1.0]` (a double constant).
  - Loop-invariant `1.0 / base` and `1 - value` (value read through a reference) are hoisted onto the x87 stack.
- **CFilter_Bind3 operator()** -- `return (*F1)(t) && (*F2)(t) && (*F3)(t);` with a stateless `__forceinline` middle filter (never loaded from +4) matched first grade (0x007C2800). The same body is inlined in GetAllThingsInRoughArea 0x007C2D10, which is the way to identify an unreferenced out-of-line instance.

Review at landing:
- No row used more than 4 grades.
- **Parked rows:** 0x007AF940, 0x007B8530 and 0x007B8AA0 are register-allocation-only diffs. 0x007DB5C0 differs because retail has no destructor for the empty hand after `PlayersCards.insert`; VC7.1 kept it for every form tried.
- **Local models beside an engine header:**
  - 0x007D22F0 uses a local polymorphic `CDefClassBase` (needed for the copy ctor/vptr). engine/CDefClassBase.h is the flattened POD view.
  - 0x007CABD0 uses a local method-only `CTCInventoryItem`. engine/CTCInventoryItem.h has no methods.
- **Unresolved element type:** in 0x007CA350 the pointee of `CDefPointer<const T>` is not identified. CDefClassBase stands in; every CDefPointer instance has the same bytes, but the mangled name will not be the retail one.
- **Errors in already-landed sources:**
  - 0x007C6A40, landed as "CTCWillEnflame::CancelMove", is CTCTurncoatSpell::CancelMove. Its caller looks up interface 0x9E, which is CTCTurncoatSpell's GetInterfaceType; CTCWillEnflame's is 0x95, and its real CancelMove is 0x007A44C0 in this chunk.
  - 0x007AA830, landed as "CTCAssassinRush_FinalCleanup", is a vtable slot-2 thunk that jumps to the real FinalCleanup 0x007AA4D0 (this chunk).
- **Note on the landed CTCBurntEffect ctor (0x007CDFE0):** its same-TU `__copy` stand-in uses `long*` as the distance type. The real instance 0x007CE1F0 (this chunk) is `__copy<CThing**, back_insert_iterator<..>, int>`, so the mangled names differ; that is harmless.
- **Catalog rows covering several functions:** none.
- **Stray directory:** the agent folder contains a `D:\Documents\FableTLC/rebuild/build/landverify` directory created by the lane tooling (tall.py/q.py working path), not by the sources.
