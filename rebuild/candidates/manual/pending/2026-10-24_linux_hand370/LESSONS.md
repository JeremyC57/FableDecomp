# Chunk 370 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Same-TU callees give VC7.1 escape and no-touch knowledge.** Define the callee in the TU and keep it out of line
  (it is usually a COMDAT that sits beside the row in retail).
  - VC7.1 then propagates a temporary's constant members across calls that received its address. 0x0047CB50
    `map::operator[]`: `T()`'s null start/finish become `xor edx,edx ; xor ecx,ecx`, with no free.
  - It also keeps loaded fields in registers across the calls (the pair's `_M_start` in esi).
  - With the callees only declared, the build reloads after every call (218v207).
- **Empty tag or allocator temporaries homed over a LIVE argument slot** need the callee's body in the TU.
  - 0x0047C350 `??_G`: `__destroy_aux`'s `__false_type` is homed over the flags slot, `lea eax,[esp+8]`.
  - 0x004792CF: the list ctor's `allocator_type()` is homed at `[ebp+0Bh]`.
  - 0x0048D5C0: `__true_type` is homed over the element slot.
  - Without the body, VC gives the temporary a fresh slot: a `push ecx` frame word.
- **`#pragma auto_inline(off)` does not stop a speed TU from inlining a template member** called with constant
  arguments (0x0048D5C0's `_M_insert_overflow(finish, x, tag, 1, true)` was inlined, 235v122). Use
  `__declspec(noinline)` on the in-class declaration. In 0x0047CB50 and 0x0047C350, `auto_inline(off)` was enough.
- **An STLport `_Rb_tree_iterator` needs a user constructor** (`iterator(node)`); otherwise `_M_insert` returns the
  4-byte iterator in eax instead of through the hidden pointer (`ret 10h` instead of `ret 14h`, 0x0047C500).
- **A /Os TU calls the compiler-generated `operator=` of a def class out of line.** Retail inlined it (the base
  `CParentDefClassBase::operator=` 0x431ED0, then the member words), so spell it out as a `__forceinline` memberwise
  operator= (0x004761AA / 0x00476497, 55v104 before).
- **`ret 8` + tag for a def-element copy is `__copy_ptrs` / `__copy_backward_ptrs`** with the SGI `__copy` /
  `__copy_backward` inlined as `__forceinline`, and the count runs down a frame word (0x004761AA, 0x00476497).
- **A `??_G` with no vtable store and no reference anywhere in the image** is the non-virtual scalar deleting destructor
  that `delete p` emits for a plain class. Model a non-polymorphic holder with an implicit destructor
  (0x0047C350, `pair<const ECategory, vector<...>>`).
- **STLport `erase(const key&)` calls `distance(const It&, const It&)` as `__cdecl`** (`_STLP_CALL`). The caller pops
  one word (`add esp,4`) and reuses the other as the first by-value iterator slot of `erase(first, last)` (0x00475BF6).
- **STLport `insert_unique(v)` passes `__y` twice on the begin() path:** `_M_insert(/*__x*/ __y, __y, __v)`, with the
  zero `__w` pushed from the exhausted `__x` register. Elsewhere it is `_M_insert(__x, __y, __v)` with `push 0`
  (0x00485E5C).
- **The STLport deque buffer is `128 / sizeof(T)` elements.** `_M_initialize_map` writes `(max)(size_t(8),
  num_nodes + 2)` through two by-reference temporaries: a frame word, and the dead `n` slot (0x00493875).
- **The STLport `resize(n)` one-argument overload must be `__forceinline` in a /Os TU.** Index the reloaded start
  with `*(begin() + n)` / `begin()[i]`: a `*(_M_start + n)` reference materialises `lea esi,[eax+edi*8]` (0x00475DFA).
- **COMDATs of one container come from different TUs.** 0x00475BF6 erase(key) is frame-pointer /Os, while its
  sibling `_M_lower_bound` / `_M_upper_bound` 0x00475D07 / 0x00475D50 are speed with no frame. Pick the pragmas per
  row, not per address range.
- **A CCharString-keyed lower/upper bound with an inlined `Compare_00411570(a.data, b.data) < 0`** hoists k's data
  pointer only when 0x00411570's body is in the TU, but then allocates y=ebx / key=edi, while retail has edi / ebx
  (parked 0x00475D07 / 0x00475D50). With the body only declared, y gets edi but the key is re-read per iteration.

Review at landing:
- 0x0048D940 GFFloatToLongFloor is a `retail_asm` candidate: a bare dword `fistp` sequence on frame-word constants
  (0.5f, 1.0f). Under the owner's 2026-10-24 exception it can land as C++ with a minimal `__asm` block and a
  `// Retail-asm:` evidence line. This brief forbids `__asm`, so it is parked as a C++ reference in
  differ/0048d940.cpp.
- Rows that needed more than 4 grades: 0x0048608C (5 grades, the same-TU edx-clobbering stand-ins). It was recorded
  before this session resumed; all its later grades MATCH.
- Local models next to an existing engine header:
  - 0x004761AA / 0x00476497 model CParentDefClassBase / CQuickAccessItemButtonGuiDef locally, because
    `engine/CParentDefClassBase.h` is the packed, flattened PDB layout (0x3d bytes, no vptr base, no operator=).
  - 0x00475DFA (parked) uses `engine/CDataInputStream.h`.
- Stand-in bodies that are not the retail body:
  - 0x0047CB50's `insert_unique(iterator, v)` (the real 0x0047C8C0 is a hinted insert outside this chunk).
  - 0x0048D5C0's `_M_insert_overflow` (written with memmove, a simplification of the POD path).
  - 0x004792CF's (parked) In/Out traits stand-ins.
- Errors in ALREADY-LANDED sources:
  - 0x0047BAF0 (`vectorstdpairCCharString_Destroy_0047baf0.cpp`) describes the element as three CCharStrings and the
    function as Dinkumware `_Destroy(_First,_Last,_Al)`. The callee 0x0099B510 is ~CWideString, and the function is
    STLport `__destroy_aux<CHeroLogBook::CHeroLogBookEntry*>(first, last, const __false_type&)`.
  - 0x0048F50C declares GetCharWidthW as `long __cdecl WordWrapCharWidth(wchar)`; it is a static `__fastcall`
    (only its address is taken there).
  - 0x0040DED0 / 0x0040DF00 carry Dinkumware-style names; they are STLport `__uninitialized_copy` /
    `__uninitialized_fill_n`.
  - The landed 0x004720C3's comment says mode 2 = save, but its code hands mode 2 the load stream (+0x24).
- Inferred names to double-check:
  - 0x0047C350's holder type (unreferenced in the image).
  - 0x00493875's element (CSubtitleRenderer::CSetTextInfo is the only 0x1C-byte deque element in Ego_r.pdb).
  - 0x0048D5C0's behavioural name: no STLport algorithm has this 3-argument filtering signature; the real template
    name and the +0xD4 flag (on a CThing-derived holy site) are unknown.
  - 0x0048E072 IsTextReadyForClickPast (inferred from the Ego_r name set).
