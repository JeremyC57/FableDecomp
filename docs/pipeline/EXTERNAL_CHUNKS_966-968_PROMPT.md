# External chunks 966–968: prompt for the outside AI

Paste everything below the line into the outside AI's session. It is self-contained. The general reference is
`docs/pipeline/EXTERNAL_AI_HANDOFF.md`; the previous round's prompt is `EXTERNAL_CHUNKS_963-965_PROMPT.md`. This prompt
repeats every fix from that round and adds the new ones (marked **NEW**).

---

You are reconstructing functions of the native PC **Fable: The Lost Chapters** `Fable.exe` as C++ that compiles under
**MSVC 7.1 RTM (cl 13.10.3077)** to the retail bytes. Last round (963–965) landed **51 of 120**. Every row was regraded
and its test re-run on our side.

48 landed as delivered. **3 were wrong as delivered**: 00870370, 008709f0 and 00c3bcb0 called scalar `operator
new`/`delete` where retail calls the array thunks. We fixed them (F10 below). Your review annotations were not needed:
our lander treats a name-vs-label disagreement as a warning, so give the evidence in `ROW.json` and move on.

## 1. Scope: these 120 rows and nothing else

- The rows are in `docs/pipeline/external_chunks_966-968.tsv` (columns `chunk address_va file_offset file_offset_end
  length namespace name`). Bytes are `Fable.exe[file_offset:file_offset_end]`. Exe SHA-1 is
  `39fd416ba23a91a89713ec80c789f57ddb22e2c8`.
- Your `AUTHORIZED_SCOPE.json` for this round is `docs/pipeline/external_966-968_AUTHORIZED_SCOPE.json`: chunks 966,
  967 and 968, with exactly those 120 addresses.
- Do **not** work on 960–965 rows, including your parked ones. We retry those in-house.

| Chunk | Rows | Content |
|---|---|---|
| 966 | 40 | STL and serialisation templates, 295–334 bytes: `TransferIn`/`TransferBinaryIn`/`TransferOut`, vector insert/assign/realloc/resize, `_Adjust_heap`/`_Push_heap`/`_Pop_heap`, introsort, `GFSerialiseMapBinaryIn`. |
| 967 | 40 | Free-function rows "Com…"–"Cre" by manifest name: combat helpers, engine primitives, copy constructors, vector assigns. 241–639 bytes. |
| 968 | 40 | Free-function rows "Cre…"–"CTC" by manifest name: def readers and writers, destructors, camera, navigation, persist. 241–639 bytes. |

**Names are guesses.** For 967–968, `CCombatManager_ComputeAttackZone`-style names are manifest labels, and most of
these rows are really class members. Name each one by evidence: callers, the vtable slot, RTTI, and
`ghidra_out/struct_layouts_egor.tsv`. Put the result in `ROW.json` as `assigned_name` plus `evidence`. A descriptive
placeholder is fine if you mark it as one.

## 2. Hard rules (any violation and the row is rejected)

1. No `__asm`, `naked`, `_emit` or `volatile`: not in code, comments or identifier substrings (`m_emitters` fails).
   **Never use `volatile` "to force a reload". If only `volatile` would match, park the row.**
2. `this` uses `rebuild/include/engine/<Class>.h` when a header exists. If you write a local layout instead, put the
   offset evidence in a `// Layout:` comment.
3. **One symbol, one retail address.**
   - **CRT thunks:** call the CRT thunk retail calls. That is `free()` for `0x00BFEA14`, and never `operator delete`
     there. The array thunks are covered in F10.
   - **Duplicate callees:** retail has byte-identical duplicate functions. Each must get its own symbol.
     - `LowerBound`: 0x40F020 is the non-const overload; reach 0x4365B0 through the const overload.
     - `GetPosition`: 0x4C73D0 is `CThing::GetPosition`; 0x6A4D00 is `CThingCreatureBase::GetPosition`.
   - **NEW: Porting a sibling's source:** if you reuse one row's source verbatim for a byte-twin row whose callees sit
     at different addresses, wrap the copied model in its own namespace. Otherwise one mangled callee binds two retail
     addresses.
4. Don't redeclare `operator new(unsigned int)`.
5. Tests are C++03:
   - print exactly `<address>_TEST PASS`;
   - no `dynamic_cast` (we build `/GR-`);
   - no C++11 and no NRVO reliance;
   - `long` is 8 bytes on 64-bit g++;
   - use exactly representable floats;
   - no locals named `lt`, `gt` or `amp`.
   - **NEW:** make every test self-contained. Paste the source in; never `#include` it by a relative or absolute
     path (an absolute `/workspace/...` include hangs the compiler under Wine).
   - **NEW:** `T t = T();` is not zero-initialised under VC7.1, so use `memset`.
6. **Grade cap: 12 grades per row.** Spread them across distinct strategies. Attempt every row once before
   deep-diving any one.
7. **Report bad extents.** If an assigned span holds more than one function, or code plus a jump table and data, say
   so and park the row as BLOCKED with the split offsets, as you did for 00c400d0 and 0099c7e0. Never pad or invent.

## 3. Fixes, and how to recognise when each applies

Compare your object code with retail, then find the first residue below that fits.

### F1. Frame is larger than retail, or a local is in a different `[esp+N]` slot
**Signs:** `sub esp,N` differs; offsets disagree; a temporary has its own slot where retail reuses a dead one.

**Fix: same-TU stand-in bodies.** Give every *declared-only* callee that takes a local's address, fills a local,
takes a `const T&` temp, or returns a class by value a same-TU body under `#pragma auto_inline(off)` or
`__declspec(noinline)`.
- **NEW, important:** the stand-in body is VISIBLE to its callers. It must **READ every by-reference input**, for example
  `if (*(void**)&x != p) return;`. If it never reads one, VC7.1 deletes the caller's stores to that local. A body
  that writes through an IN reference kills the caller's initialising store. Write only through OUT parameters.
- Destructor stand-ins that touch the whole object are the strongest form.
- **NEW: Empty tags.** A callee that takes an empty tag (`__false_type`, `__true_type`, an iterator tag) gets a
  noinline stand-in written as `template <class Tag> __declspec(noinline) void _M_insert_overflow(T* pos, const T& x,
  const Tag&, ...)`, **with the tag parameter unnamed**. The tag temporaries then share a frame dword with retail;
  `auto_inline(off)` alone is not enough. This also works for `find` and other tag-taking callees. Never store through
  a tag.
- **NEW:** a user constructor on an empty tag (`CEmptyTag(){}`) removes a stray tag byte store.

**Slot rule.** Scalars of ≤4 bytes get slots first, then by weighted live-reference count, then by first use. Sibling
`{}` scopes overlay one another.

**Lexical scope.** An address-taken temp or out-flag holds its slot for its whole enclosing scope. Narrow it with a
`{}` block, a per-branch declaration or a `__forceinline` helper.
- **NEW:** a local declared after an if-block does not share that block's slots until you wrap it in `{}`.

**Default-argument temps.** Name an empty allocator or tag passed by `const&` (`SAllocator a; CList l(a);`).

### F2. A member is re-read through an alias, or there is an extra `lea`/`mov`
**Signs:** retail reloads `[esi+N]` where you keep a pointer register, or the reverse.

**Fix:** remove ONE `T* p = &Member;` alias at a time. Re-read members, not local copies, where retail re-tests.
- **NEW:** re-index (`PDef->Meshes.Start[index]` at each use) instead of binding a reference.

### F3. Callee-saved register swap (esi/edi/ebx/ebp)
**Signs:** identical instruction stream, but two callee-saved registers are exchanged.

**Measured rule.** Priority is the weighted count of real references. Loop uses weigh more; uses after an early
return or in one arm weigh less; ties go to the earlier definition. Span, dead references and spelling do not matter.
Add or remove real uses to swap.
- **NEW: Receiver spelling** changes register assignment across the whole function. `typed->HasDriverType(n)` (a
  second pointer to the same object) versus `thing->HasDriverType(n)` fixed two rows.
- **NEW: STLport erase** written with inline `CopyPtrs(last, _Last, first)` and `Destroy(i, _Last)` helpers fixes a
  last/out register swap.
- Fix the **first** differing pair first.
- The **ebp-only** swap family and a free ebx/ebp choice are known gaps. Park them after 3 grades.

### F4. x87 operands swapped (`fld a; fmul b` versus `fld b; fmul a`)
- Order follows the first source reference. Try `Y*Y + X*X + Z*Z` against `X*X + (Y*Y + Z*Z)`.
- The key is the base symbol of the first read: a `const T& v = obj;` alias, or an inline member, flips every product
  on that value.
- **NEW:** an incremental `c = …; c += …;` sum keeps a Y,X,Z load order.
- **NEW:** read a global into a named local first to fix an x87 load order.
- **NEW:** `float a = (float)alpha; alpha = (long)(a * fade);` gives `fild`/`fmulp` instead of `fimul`.
- **NEW:** a literal float in .rdata (e.g. `0.5f`) must be written as a literal. Declaring it `extern` flips the
  operand order, which is the opposite of the epsilon case below.
- **NEW:** `(float)fabs(x)` gives retail's dword compare; plain `fabs` compares a double.
- An `extern const float` epsilon stops `eps*eps` folding.
- `#pragma intrinsic(sqrt)` gives inline `fsqrt`.
- Last resort only: k = 0..7 unused named locals re-key the order. If you use it, say so in a `// Fidelity:` comment.

### F5. Return and exit shape
- An early-return chain keeps the **last** merged return copy; a single result-variable return keeps the **first**.
- Write every exit out in full, and never use a `goto` to one shared label.
- **NEW:** when retail falls through from a loop into the code after it, leave that branch's `return` out.
- **NEW: Flags from `&&`.** Write `bool found = a() && b(); if (found) {...}`. An if/else assigning true/false
  misplaces the false store.
- **NEW: Dtor at one exit.** When a dtor or ctor is inlined at some exits and called at one, put
  `#pragma inline_depth(0)` around that ONE return or throw. A member dtor reached through an implicit enclosing dtor is
  called out of line even when it is itself `__forceinline`, so mark the enclosing dtor `__forceinline` too.

### F6. Component and interface lookups
- Return the interface by value from a `__forceinline` helper (null default), or write
  `*out = (it == last || k < it->type ? last : it)->component;`.
- **NEW:** retail `GetTC` often inlines the STLport `lower_bound` len/half loop instead of calling LowerBound.
- **NEW:** a GetTC result left uninitialised (`T* r; if (Has) r = ...;`) matches retail when there is no zero store.

### F7. Callees that look "LTCG"
A callee taking a pointer in ecx and returning through it is usually `__fastcall` with a hidden return pointer.
- **NEW:** if a callee consumes extra registers (ESI/EBX) beyond the normal convention, as in 00c174a0, park it as
  BLOCKED with the evidence. That is a known gap.

### F8. Chunk 966 specifics: STL and serialisation templates
- **`/Gr` and SGI:** free template algorithm TUs are compiled `/Gr`, so every free template function is `__fastcall`
  (first argument in ecx, last in edx). The algorithms are SGI (`__introsort_loop` plus `__final_insertion_sort`,
  threshold 16). The containers use 12-byte vectors.
- **Element size:** `round(2^(32+shift)/magic)` from the `imul` magic. The comparator is a 4-byte functor.
- **Vector code:** loop over `_M_start` with size `_M_finish - _M_start`. `*(begin() + i)` with a signed long index
  loads the start pointer once. **NEW:** `resize` must be `__forceinline`, or VC emits a call.
- **POD push_back:** needs `__forceinline` `push_back` and `_M_insert_overflow`, plus a separate
  `uninitialized_copy` helper. **NEW:** put the vector member constructors in the init list to fix the zeroing order.
- **`CPersistTraits<T>`:**
  - A tail `jmp` into an empty traits object is `CPersistTraits<T> traits; traits.X(...)`, with the traits body
    earlier in the TU as an `auto_inline(off)` stand-in.
  - A dword-zero-then-byte-store default is a 4-byte traits layout (`T DefaultVal; unsigned char Pad[3];`), plus
    `= CPersistTraits<T>()`, plus a separate `traits.DefaultVal = T();`.
- **Duplicate pairs:** several rows are byte-identical pairs, e.g. `TransferBinaryIn` 0x466C8E/0x46BA4B and
  0x46B545/0x4E9E0B. They are separate instantiations, so send a source per address, and namespace them if their
  callees differ (rule 3).

### F9. Strings, temporaries and constructors
- **String temporaries:** passing `const char*` to a `const CCharString&` builds every temporary before the first
  push. Explicit `CCharString(...)` temporaries interleave with the pushes.
- **String-literal ternary argument:** this is two whole calls, so write the call in each arm.
  **NEW:** write `SetIndices`/`SetStreamSource`-style calls as whole calls in if/else too.
- **CCountedPointer temporaries:** a conditionally destroyed temporary needs a `__forceinline` copy ctor and dtor.
  Def release is `if (--RefCount == 0) DeleteThis();`.
- **NEW: Byte stores:** a colour struct whose bytes are stored in the ctor BODY (`{R=r; G=g; B=b; A=a;}`), not an
  init list, gives retail's store order. VC7.1 emits an inline ctor's byte stores in reverse source order.
- **NEW: Unrolled fills:** VC7.1 fully unrolls a 4-iteration fill loop, so write the loop, not 4 stores.
- **NEW: Compiler-generated manager destructors:** declare no dtor on the manager, and give contained wrappers a
  `__forceinline virtual` dtor.
- **NEW: Repeated lookups:** a named resource local read once before the first of several lookups gives retail's
  cached-register run, where an inline getter reloads the global each time.

### F10. NEW: Array new/delete
**Signs:** retail calls the MSVCR71 thunks `operator new[]` at `0x00BFEB22` or `operator delete[]` at `0x00BFEB1C`.
Your object calls `??2@YAPAXI@Z` / `??3@YAXPAX@Z` instead.

**Fix:** VC7.1 lowers `new T[n]` / `delete[] p` to the SCALAR operators unless the array forms are declared. Add both
lines to the TU:
```
void* __cdecl operator new[](unsigned int size);   // MSVCR71 ??_U, thunk 0x00BFEB22
void __cdecl operator delete[](void* p);           // MSVCR71 ??_V, thunk 0x00BFEB1C
```
Keep scalar `operator delete(p)` where retail calls the scalar thunk at `0x00BFE9BC`. One function can use both.

## 4. Known gaps: park after 3 grades
- whole-function register caching across calls;
- the inlined `lower_bound` extra `mov eax` copy;
- inline-asm `fistp` rounding helpers (DWORD FISTP);
- locals in dead argument slots with no frame, once F1 has been tried;
- a shared failure/Terminate block after `ret`;
- the rough-area node-loop exit reload order;
- `this`-before-arguments order for a global interface pointer;
- pure SetState `test`/`or` operand swaps;
- CSE reloads that only `volatile` would reproduce.

## 5. What to send back
The same layout as last round:
- `SELECTED/{landed,parked,blocked}/<addr>/{<addr>.cpp,<addr>_test.cpp,ROW.json}`;
- `ROWS.tsv`;
- `ROW_DETAILS.md`, with a one-line residue for every parked row;
- `SHA256SUMS`.

**`ROW.json`:**
- `assigned_name` must be a plain C++ name, with no spaces or notes. Put uncertainty in `evidence`, not the name.
- `selected_flags` lists only the EXTRA flags (`/Gr`, `/GS`, `/EHsc`, `/O1 /Oy-`), not the base `/O2 /Oy /W3`.

**Each source:**
- head it with `// <address> <Class::Function>` and its flags;
- add a `// Leaf: <symbol>` line when the symbol is not the plain function name.

We regrade and re-run every test before landing.
