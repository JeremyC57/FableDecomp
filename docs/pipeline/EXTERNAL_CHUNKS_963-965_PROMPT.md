# External chunks 963–965: prompt for the outside AI

Paste everything below the line into the outside AI's session. It is self-contained. The general hand-off,
`docs/pipeline/EXTERNAL_AI_HANDOFF.md`, is still the reference for the compiler setup and for how rows are graded.

---

You are reconstructing functions of the native PC **Fable: The Lost Chapters** `Fable.exe` as C++ that compiles under
**MSVC 7.1 RTM (cl 13.10.3077)** to the retail bytes. Your last round (chunks 960–962) landed **61 of 120**. Every one of
those was regraded and its test re-run on our side before landing. Thank you. This round has three new chunks.

## 1. Scope: these 120 rows and nothing else

- The rows are in `docs/pipeline/external_chunks_963-965.tsv` (columns `chunk address_va file_offset file_offset_end
  length namespace name`). Bytes are `Fable.exe[file_offset:file_offset_end]`. Exe SHA-1 is
  `39fd416ba23a91a89713ec80c789f57ddb22e2c8`.
- Your `AUTHORIZED_SCOPE.json` for this round is `allowed_chunks: [963, 964, 965]` plus exactly the 120 addresses in
  that TSV.
- **Do not** work on 960–962 rows, including your parked ones. We are retrying those in-house with the fixes below.
- **Do not** work on any address that is not in the TSV.

| Chunk | Rows | Content |
|---|---|---|
| 963 | 40 | STL and serialisation templates: `_Adjust_heap`, `_Push_heap`, `_Sort_heap`, `_Insertion_sort1`, vector insert/realloc/resize/erase, `CPersistTraits<T>::TransferIn/Out`, `GFSerialiseMapBinaryIn`. 241–268 bytes. |
| 964 | 40 | Free functions and helpers, A…B by name: heap helpers, animation, AI state helpers, bank files, binary trees. 241–634 bytes. |
| 965 | 40 | Free functions, B…C by name: CAFile readers, CAIStateGroup helpers, CActiveFile vector assigns, CCharString inserts, chunked-file dtors. 242–612 bytes. |

**Names are guesses.** The `name` column for 964–965 is a descriptive manifest label, often wrong. Many of these rows
are really class members: `CAIStateGroup_HandyMan_FindClosestThing` is likely a member of that class. Name each row by
evidence: callers, the vtable slot, RTTI, `ghidra_out/struct_layouts_egor.tsv`. Report corrected names in `ROW.json`
(`assigned_name` plus an `evidence` line).

## 2. Hard rules (any violation and the row is rejected)

1. No `__asm`, `naked`, `_emit` or `volatile`: not in code, comments or identifier substrings (`m_emitters` fails).
   **No `volatile` "to force a reload". If only `volatile` would match, park the row.**
2. `this` uses `rebuild/include/engine/<Class>.h` when a header exists. If you write a local layout instead, put the
   offset evidence in a `// Layout:` comment.
3. If retail calls the CRT `free` thunk `0x00BFEA14`, write `free()`, never `operator delete`.
4. Don't redeclare `operator new(unsigned int)`.
5. Tests are C++03:
   - print exactly `<address>_TEST PASS`;
   - no `dynamic_cast` (we build tests `/GR-`);
   - no C++11 and no NRVO reliance;
   - `long` is 8 bytes on 64-bit g++;
   - use exactly representable floats;
   - no locals named `lt`, `gt` or `amp`.
6. Grade cap: **12 grades per row.** Spend them across distinct strategies, not micro-variants. Attempt every row once
   before deep-diving any one.

## 3. Fixes, and how to recognise when each applies

Compare your object code with retail, then find the first residue below that fits.

### F1. Frame is larger than retail, or a local sits in a different `[esp+N]` slot
**Signs:** different `sub esp,N`; the same instruction stream but the stack offsets disagree; a temporary gets its
own slot where retail reuses a dead one.

**Fix (the biggest lever):** give every *declared-only* callee that takes a local's address, fills a local, takes a
`const T&` temp, or returns a class by value a same-TU body under `#pragma auto_inline(off)`. Typical callees:
`_M_insert_overflow`/`InsertGrow`, `LowerBound`, `GetDef`/`GetSubDef`, `__find`, the vector `operator=`, `resize`, and
`_M_fill_insert`.
- The body must **read** its IN params (`if (*(void**)&x != p) return;`) and write only through OUT params. A body
  that writes through an IN reference kills the caller's initialising store (4 bytes short).
- A destructor stand-in that touches the whole object is the strongest form.
- Never store through an empty tag parameter (`__false_type`, `__true_type`, an iterator tag) inside a stand-in.
- Stand-ins are non-template.

**Slot rule:** scalars ≤4 bytes get slots first, then by weighted live-reference count, then by first use. Sibling
`{}` scopes overlay one another.

**Lexical scope:** an address-taken temp or out-flag holds its slot for its whole enclosing scope. Narrow it with a
`{}` block, a per-branch declaration or a `__forceinline` helper.

**Default-argument temps:** an empty tag or allocator passed by `const&` gets a frame byte. Naming it
(`SAllocator a; CList l(a);`) can restore retail's layout.

### F2. A member is re-read through an alias, or there is an extra `lea`/`mov` copy
**Signs:** retail reloads `[esi+N]` where you keep a pointer register, or the reverse.

**Fix:** remove ONE `T* p = &Member;` alias local at a time and re-grade. Re-read members rather than local copies
where retail keeps a redundant re-test.

### F3. esi/edi/ebx/ebp role swap
**Signs:** identical stream, but two callee-saved registers are exchanged.

**Measured rule:** priority is the weighted count of real references. Uses inside loops weigh more; uses after an early
return, or in only one arm, weigh less; ties go to the earlier definition. Live-range span, dead references and spelling
do **not** matter.

**Fix:** add or remove real uses, for example by routing one use through a local or reading a member twice. An inline
accessor used at a single site often fixes a scratch-register (eax/ecx/edx) rotation. Fix the **first** differing pair
first.

The **ebp-only** swap family is a known gap; park it after 3 grades.

### F4. x87 operands swapped (`fld a; fmul b` versus `fld b; fmul a`)
- Order follows the first source reference. Try `Y*Y + X*X + Z*Z` against `X*X + (Y*Y + Z*Z)`, and parenthesise
  constant products.
- The order is keyed by the base symbol of the first read. A `const T& v = obj;` alias, or an inline member such as
  `GetLengthSquared2D()`, flips every product on that value.
- As a last resort only: adding k = 0..7 unused named locals re-keys it. Say so in a `// Fidelity:` comment if you
  use it.
- `#pragma intrinsic(sqrt)` gives inline `fsqrt`.
- An `extern const float` epsilon stops `eps*eps` folding.

### F5. Return-block shape
**Signs:** retail has a different count of `pop/ret` copies, or they sit in different places.

**Fix:** an early-return chain keeps the **last** merged return copy; a single result-variable return keeps the
**first**. Write every exit out in full, and never `goto` a shared label. A return chain's last test is
`if (!f()) return false; return true;`.

### F6. Component/interface lookups
**Signs:** retail loads the component straight into ecx, or never spills an out-param.

**Fix:** return the interface by value from a `__forceinline` helper (null default) instead of using an out-param, or
write `*out = (it == last || k < it->type ? last : it)->component;`.

### F7. Callees that look "LTCG"
**Sign:** a callee gets a pointer in ecx and returns through it.

**Fix:** it is usually `__fastcall` with a hidden return pointer, not whole-program optimisation.

### F8. Chunk 963 specifics: STL and serialisation templates
- **Free template algorithm TUs are compiled `/Gr`.** Every free template function is `__fastcall`: first arg in ecx,
  last in edx, the rest on the stack. The algorithms are **SGI** (`__introsort_loop` plus `__final_insertion_sort`,
  threshold 16, `__lg(n)*2`; `__copy` with `random_access_iterator_tag`/`Distance*` tags). The containers are VC6-era
  Dinkumware/STLport style with **12-byte vectors**.
- **Element size:** a reciprocal-multiply `imul` magic gives it as `round(2^(32+shift)/magic)`. The comparator is a
  4-byte by-value functor.
- **Vector growth:** use loops over `_M_start` with size `_M_finish - _M_start`. A POD `push_back` needs
  `__forceinline` `push_back` and `_M_insert_overflow` plus a separate `uninitialized_copy` helper.
- **`push_back(0)` / `insert(pair)` temps** stay per-iteration until the receiving callee is a visible same-TU
  `__declspec(noinline)` body that never writes through the reference.
- **`CPersistTraits<T>`:**
  - A tail `jmp` into an empty traits object (`mov [esp+0Ch],eax; lea ecx,[esp+0Ch]; jmp`) is
    `CPersistTraits<T> traits; traits.X(...)`. It needs the traits body earlier in the TU as an `auto_inline(off)`
    stand-in.
  - A default of dword zero followed by a byte store is a 4-byte traits layout (`T DefaultVal; unsigned char Pad[3];`),
    the value-initialised copy `= CPersistTraits<T>()`, and a separate `traits.DefaultVal = T();`.
- **Copies:** byte-identical copies (e.g. the two `_Insertion_sort1<pair<EBodyLocation,…>>` rows) are separate
  instantiations. One source can serve both; send it for each address.

### F9. Strings and temporaries
- Passing `const char*` to a `const CCharString&` parameter builds every temp before the first push. Explicit
  `CCharString(...)` temps interleave with the pushes.
- A string-literal ternary argument is two whole calls: write the call in each arm.
- A conditionally destroyed `CCountedPointer` temp needs a `__forceinline` copy ctor and dtor.
- Def release is `if (--RefCount == 0) DeleteThis();`.

## 4. Known gaps: park after 3 grades, don't burn the cap
- whole-function register caching across calls;
- the inlined `lower_bound` extra `mov eax` copy;
- the inline-asm `fistp` rounding helpers (DWORD FISTP);
- locals placed in dead argument slots with no frame, once F1 has been tried;
- a shared failure/Terminate block placed after `ret`;
- a node-loop exit reload order in rough-area walks;
- CSE reloads that only `volatile` would reproduce.

## 5. What to send back
The same layout as last round:
- `SELECTED/{landed,parked,blocked}/<addr>/{<addr>.cpp,<addr>_test.cpp,ROW.json}`, with `assigned_name`,
  `selected_flags` and `evidence` in `ROW.json`;
- `ROWS.tsv` and `ROW_DETAILS.md`, with a one-line residue for every parked row (e.g. "frame 0x2c vs 0x28; F1 tried
  on GetDef");
- the hash manifest.

Head each source with `// <address> <Class::Function>` and its flags (`/O1 /Oy-`, `/GS`, `/EHsc`, `/Gr`), and add a
`// Leaf: <mangled-or-short leaf>` line when the symbol is not the plain function name.

We regrade and re-run every test before landing.
