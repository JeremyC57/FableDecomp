# FableDecomp — hand-off for an outside AI or contributor

You are helping reconstruct the native PC **Fable: The Lost Chapters** `Fable.exe` as C++ source that compiles to
the **exact retail bytes**. You don't need the in-house tooling to help. If you write a translation unit per
function, the maintainers grade it with the real compiler and land it only if it matches.

Repo: `JeremyC57/FableDecomp` (branch `FullDecomp`). Read `docs/ARCHITECTURE.md` for the big picture.

## 1. What a row is and what "match" means

- A **row** is one retail function: an address, a name, a length and its bytes. The bytes come from your own copy of
  `Fable.exe`: file offset = VA − 0x400000 for the `.text` section, with ImageBase 0x400000.
- **MATCH**: your object code is byte-identical to retail.
- **RELOCATION_MATCH**: identical except for relocated operands (call targets, global/vtable/string addresses), and every
  one of those operands names the **same symbol** retail uses. This is what almost every landed row is. Your code only
  has to call the right function or reference the right global. The linker places them.
- Rows are landed only through `tools/decomp_pipeline/verify_and_land.py`. It recompiles each source, compares bytes and
  relocation identity, runs a behaviour test, and rejects inline assembly.

## 2. Compiler

- **Microsoft Visual C++ 7.1 (Visual Studio .NET 2003), `cl` 13.10.3077 (RTM).** The retail Rich header names it for
  1,369 of 1,380 C++ objects. Eleven objects used the QFE build 13.10.4035 (DDK 2003), and two catalog rows need it.
- Where the maintainers got it: archive.org item **`visual-studio-2003-unpacked`**, file `VisualStudio2003.zip`
  (SHA-1 `298398e607a8e6033abe261987b94cab8fb57c24`) — <https://archive.org/details/visual-studio-2003-unpacked>.
  - Use `Vc7/bin` (cl.exe, c1xx.dll, c2.dll, link.exe) plus `mspdb71.dll` and `msobj71.dll` from `Common7/IDE`.
  - It runs under Wine on Linux; `docs/BUILDING.md` has the lane setup.
  - You are responsible for having a licence to use it.
- **Do not use the free "Visual C++ Toolkit 2003"**: it is a pre-RTM 13.10.3052 and produces different code.
- Base flags: **`cl /nologo /c /O2 /Oy /W3`**, 32-bit.
- Extra flags that some rows need:
  - `/GS` — the prologue loads the security cookie and xors it into a frame slot. Test links then need
    `bufferoverflowu.lib`.
  - `/EHsc` — the function has an EH frame (`push -1 / push handler / mov eax,fs:[0]`).
  - `#pragma optimize("s", on)` — size-optimised translation units. Signs: `lea ebp,[esp-N]` frames, push/pop instead of
    mov, and fewer inlines.
  - `#pragma optimize("t", on)` and `#pragma inline_depth(N)` — speed blocks and limited-inline blocks. The maintainers'
    grader sweeps these automatically.
- Inspect your output with `dumpbin /disasm /relocations x.obj`, or `objdump -d -r -Mintel` on the COFF.

## 3. Hard rules (the lander rejects violations)

1. **No inline assembly at all**: no `__asm`, `naked`, `_emit` or `volatile`, including in comments and identifiers
   (e.g. `m_emitters` trips the check).
2. **Use the shared class headers** `rebuild/include/engine/<Class>.h` for `this`. They come from the PDB, with real
   member names and offsets. Don't write a local `struct T { char pad[...]; }` when a header exists. If retail provably
   differs, say so in a `// Layout:` comment with the offset evidence.
   - Retail `CThing` is 0x10 smaller than the PDB view, so derived padding starts at 0x94.
   - Retail STLport vectors are 12 bytes; some headers use 16.
   - `CTCCombat` is 0xc short before `AlwaysBlockThings`, and `CTCCreatureNavigation` is 4 short after
     `PossibleNavigators`.
   - A class with a virtual function and an `unsigned __int64` member pads its vfptr to 8. Never add an explicit
     `__vftable` member next to virtual functions.
3. **One symbol, one address.** Some functions have byte-identical copies, and each must bind to the one retail calls.
   - `LowerBound` has copies at 0x40F020 (non-const) and 0x4365B0. Route the 0x4365B0 call through the const overload.
   - `CThing::GetPosition` has copies at 0x4C73D0 and 0x6A4D00; the second is `CThingCreatureBase::GetPosition`.
4. If retail calls the CRT `free` thunk at 0x00BFEA14, write `free()`, never `operator delete`. This includes STLport
   vector storage.
5. Don't redeclare `operator new(unsigned int)`.
6. Behaviour tests are C++03:
   - no C++11 and no NRVO;
   - `long` is 8 bytes on 64-bit g++ hosts;
   - use exactly representable float constants such as `0.125f`, because x87 and SSE round differently;
   - don't name a local `lt`, `gt` or `amp`.
7. Name everything by evidence. Check class names against `ghidra_out/struct_layouts_egor.tsv`, and treat manifest
   labels that were propagated across identical bodies with suspicion. Many are wrong (e.g. 0x49D870 is the
   world-frame getter, not "GFGetPreMainMemoryUsed").

## 4. What to send back per row

- `<address>.cpp`: one self-contained translation unit, headed `// <address> <Class::Function>` plus any special flags.
  - Declare callees only. Define a body only when retail inlines it, or when a stand-in is needed (§5.1).
- Optionally `<address>_test.cpp`: a small program that calls the function on a fake object and prints
  `<address>_TEST PASS`.
- For a miss, send your best attempt plus one line on what still differs, e.g. "frame 0x2c vs 0x28" or "esi/edi swapped
  in the loop".

## 5. Levers that turned near-misses into matches

The full log is `docs/pipeline/GOTCHAS.md`, one line per lesson; read its tail first. These are the ones that recur.

### 5.1 Out-of-line callees and stand-ins (the biggest single lever)
- Some callees receive a local's address, fill a local (out-pointer, `long&`, const-ref slot, vector to fill), or
  return a class by value. Give each one a **same-TU noinline body**, using `#pragma auto_inline(off)` or
  `__declspec(noinline)`. VC7.1 then sees the local as non-escaping and shares frame slots the way retail does.
  - Typical: `LowerBound`, `GetDef`/`GetSubDef`, `GetEffectFromString`, `_M_insert_overflow`, `__find`,
    `IS_THING_ALIVE`.
  - Stand-ins must be non-template and should write only their out-parameter.
- The `LowerBound` body helps **only** when the residue is the lookup key's frame word failing to pack with another
  local (frame bigger than retail). That fixed 5 of 6 such rows. On register-rotation residue it does nothing or makes
  things worse.

### 5.2 Register choice and load order
- **Inline accessors** such as `GetDef()`, `GetWorld()`, `GetPlayerGui()` or `GetPhysics()` load early and shift
  scratch-register rotation. Named locals load late. Try one before the first diff where eax/ecx/edx rotate
  differently.
- A **named value local** moves a read ahead of the argument pushes. `const T& x = e;` fixes SIB base/index order and
  some load orders.
- A global used as `this` of a call whose arguments are a call chain is read **first**, through an inline accessor or
  a named local.
- Split chained calls `P->F(a)->G()` into a named pointer local.
- `__int64` arguments: `unsigned __int64 uid = p->UID; f(uid);` loads the low half before the high half.
- A virtual call returning `C3DVector` by value, or taking an out-pointer, needs inline accessors to load `this` into
  ecx before the pushes.
- Scratch-register rotation is fixed at the **first** differing pair, so fix that one first.

### 5.3 Frame layout
- VC7.1 shares stack words by **lexical scope**. Put a local in its own `{}` block to free its word for a later
  sibling.
- Name a by-reference vector argument (`C3DVector position = …; f(position);`) when retail reserves an extra 12-byte
  slot.
- A 4-byte dummy (`long Reserved;`) can reuse a dead key slot, whereas an empty struct grows the frame.
- A named result then `operator=` (`{ CCharString name = Get(...); defName = name; }`) fixes slot order and a `lea`
  re-take.

### 5.4 Control flow and code shape
- Write every exit out in full; don't `goto` a shared label. VC7.1 tail-merges identical tails the way retail does.
- Split a disjunction as `if (a) f = true; else if (b) f = true;` to get the flag register and merged stores.
  Otherwise use a named bool for `a == 1 || a == 3`.
- Retry loops: `if (r == 4) continue; break;` keeps the loop unrotated.
- A return chain's last test is written `if (!f()) return false; return true;`.
- `if (x >= 0)` after an `x == 0` test gives retail's `jl`.
- Re-read members (not local copies) when retail keeps a redundant re-test.
- Put a virtual call after an if/else inside each arm.
- A same-class callee that retail inlines at one constant call site only should be defined in the same TU; `/Ob2`
  reproduces the selective inline.

### 5.5 Components, STL and strings
- Interface/component lookup: `*out = (T*)((it == last || k < it->type) ? last : it)->component;`. The component
  loads straight into ecx.
- `*(begin() + i)` instead of `Start[i]` keeps `i` in a register and hoists the base loads.
  - STLport loops use `_M_start`, with size from `_M_finish - _M_start`.
  - A vector modelled as a nested `{_M_start; _M_finish; empty()}` struct stops CSE across a flag store.
- STLport list iterators need a declared copy constructor. Use `insert(end(), x)` for push_back.
- POD vector `push_back` needs `__forceinline` on `push_back` and `_M_insert_overflow`, plus a separate
  `uninitialized_copy` helper.
- Passing `const char*` to a `const CCharString&` parameter builds all temporaries before the first push. Explicit
  `CCharString(...)` temporaries interleave with the pushes.
- Bind a literal CCharString to a block-scoped `const CCharString&`.
- Write CCharString `== ""` as `if (strcmp(text, "") == 0) return true; return false;`.
- Ref-counted defs: `if (--RefCount == 0) DeleteThis();`. Conditionally destroyed `CCountedPointer` temps get
  `__forceinline` copy ctor and dtor.
- Filters whose dtor stores the vptr need an empty user dtor (`~CAnd(){}`). A user `CPred() {}` ctor stops the zeroing
  of an empty predicate passed by value.

### 5.6 Floating point (x87)
- Operand order follows load-node creation order, i.e. the first source reference. Write `Y*Y + X*X + Z*Z` versus
  `X*X + (Y*Y + Z*Z)` to match.
- Parenthesise constant products: `((R - 1.5f) * 0.01f) * x`.
- An `extern const float` epsilon stops `eps*eps` folding.
- A `C3DVector` user copy ctor is needed when retail keeps vectors in the frame.
- A `__forceinline` member taking a position **by value**, called with `GetPosition()`, gives the
  load-ahead-of-store copy; a named local never does.
- `#pragma intrinsic(sqrt)` gives inline `fsqrt`.
- Normalise via `operator*=` only when the vector is homed in memory.
- In big functions VC7.1 inlines `C3DVector`, `CrossProduct`, `CRightHandedSet`, `GFRandom`, `WrapAngle`, `HasTC` and
  `IsDead` only when they are `__forceinline`.

### 5.7 Bitfields and flags
- `unsigned char f; f |= 0x10;` gives `or byte ptr [m],imm`.
- An `unsigned short` bitfield gives `shr cl,5; test cl,1`.
- `HasTC` as `if (bits & mask) return true; return false;` gives `test dword,imm`.
- Clearing through a reference or setter gives load/and/store. A direct `x.f = 0` gives `and [mem],imm`.

## 6. Known gaps (don't sink time here)

No source shape found so far reproduces these:
- whole-function register caching across calls;
- the inlined `lower_bound` extra `mov` copy;
- locals placed in dead argument slots with no frame;
- the inline-asm `fistp` rounding helpers, which need real asm and are skipped;
- block placement of a shared failure/Terminate block after `ret`;
- under `/Os`, the float-argument slot reservation (`push ecx`) placed before the final `fsubp` in
  `SetPosition(C2DVector(...), 0.0f, 0)` calls;
- a string-literal ternary argument compiled to a register select where retail pushes in each arm;
- the order of a call's `this` load versus its argument pushes in some rows (both directions seen).

A clean idea for any of these would unblock dozens of parked rows.

## 7. Rows set aside for outside work

`docs/pipeline/external_chunks_960-962.tsv` lists 120 rows (3 chunks of 40). They are camera modes, creature modes, the
particle engine, the UI system, scripted control, and the world "rough area" search templates. The in-house chunk cuts
take only top-level `C…` class rows, so they don't reach these rows.

**Where to find each row in the retail exe.** The exe is the Steam `Fable The Lost Chapters/Fable.exe`: 16,666,624 bytes,
SHA-1 `39fd416ba23a91a89713ec80c789f57ddb22e2c8`. Check yours matches first. The TSV gives, per row:

| Column | Meaning |
|---|---|
| `address_va` | Virtual address in the loaded image (ImageBase 0x400000). This is what a disassembler or debugger shows. |
| `file_offset` | Byte offset in `Fable.exe` on disk where the function starts. |
| `file_offset_end` | One past the last byte; trailing `int3`/`nop` padding is already excluded. |
| `length` | Byte count (`file_offset_end - file_offset`). |
| `namespace`, `name` | Best known class and function name. `vfunc_N` means only the vtable slot is known. |

The `.text` section starts at VA `0x00401000` and file offset `0x1000`, so for every row
**file offset = VA − 0x400000**.

To get a row's bytes:

```
python -c "d=open('Fable.exe','rb').read(); print(d[0x44FDB0:0x450182].hex())"
```

To disassemble it, run `objdump -D -b binary -mi386 -Mintel --adjust-vma=0x84FDB0` on that slice, or open the exe in
Ghidra/IDA at the VA.

**Outcome of 960–962 (2026-10-23).** The outside AI returned 61 matches. Every row was regraded with
`verify_and_land.py`, its test was re-run, and all 61 landed (batch `2026-10-23_external_960_962`). The 59 rows it
parked or blocked are being retried in-house.

**Next round: 963–965.** `docs/pipeline/external_chunks_963-965.tsv` lists 120 rows:
- 963: STL and serialisation templates;
- 964–965: free-function rows, whose manifest names are guesses.

The scope file is `external_963-965_AUTHORIZED_SCOPE.json`. The prompt to paste, with every fix and how to recognise
it, is `EXTERNAL_CHUNKS_963-965_PROMPT.md`. Sparse library-looking regions were left out: zlib/winsock `__` names and
the sound library at 0xC40000–0xCB0000. The rows are recorded in `chunk_history.tsv`, so in-house cuts never take
them.

**In-house retry of the 960–962 parks (2026-10-23):** 7 of 59 recovered (batches `2026-10-23_linux_adv_b99` and `_b103`).
- Still parked: 52. The residues are in `$S/w22/gptretry/STATUS.tsv`, and most are the rough-area loop-exit reload gap or register rotations.
- All 59 have now been attempted.

**Outcome of 963–965 (2026-10-23):** the outside AI returned 51 matches, 66 parks and 3 blocked rows.
- **Landing:** all 51 were regraded and their tests re-run here. 48 landed as-is (batch `2026-10-23_external_963_965`). Three used scalar new/delete where retail calls the MSVCR71 array thunks; they were fixed by declaring `operator new[]` / `operator delete[]` and landed in `_963_965b`.
- **Annotations:** its "review annotations" (APPROVED_REVIEW_ANNOTATIONS.json) are a review-only allowlist for label disagreements (RELOC_NAME). Our lander only warns on those, so no action was needed. Recovered helper names with evidence went to `ghidra_out/labels_external963_corrections_2026-10-23.tsv`.
- **Blocked rows:** 00c400d0 and 0099c7e0 have bad extents, because each span holds more than one function. 00c174a0 takes extra register arguments (ESI/EBX).

**Outcome of 966–968 (2026-10-23):** the outside AI returned 54 matches, 58 parks and 8 blocked rows (00924520 among them).
- **Landing:** all 54 were regraded here with retail bytes read from Fable.exe at our offsets and their tests re-run under VC7.1. All landed as delivered (batch `2026-10-23_linux_external966`), with no relocation-identity errors.
- **Names:** its `assigned_name` values are placeholders (`Row00406e30::Create`, `RetailVector::FillInsert`, `NFactory…`), so the catalog keeps the manifest names from `external_chunks_966-968.tsv`. The source's `// Leaf:` line still selects the symbol. **Rename pass (same day):** the placeholder identifiers in all 58 landed sources (54 plus the 4 recoveries) were renamed in-house with bytes unchanged. The lander rechecked all 58 (WINS 58, no RELOC_IDENTITY or RELOC_CONFLICT) and 285 bindings were renamed. 6 rows are fully named; 50 still keep a file-local namespace or a generic `Element`; 2 are unchanged. Catalog-label corrections found on the way are in `ghidra_out/labels_external966_corrections_2026-10-23.tsv`.
- **Parks:** an in-house quick pass worked 14 near misses and recovered 4 (006a23e0, 006d41f0, 006ac910, 005dcf70; batch `2026-10-23_linux_external966_nm`). The other 10 residues are in `$S/gpt966/nm/STATUS.tsv`. The remaining 54 parks go back to Codex as retries in 969–971.

**Next round: 969–971 (ChatGPT Codex).** `docs/pipeline/external_chunks_969-971.tsv` lists 120 rows:
- 969: STL and serialisation templates, 245–259 bytes;
- 970–971: free-function rows "Ap"–"CDi", including the `CActiveEntityScriptBase_Create*` script factories.

The sound library at 0xC40000–0xCB0000 and `__` names stay excluded. The scope file `external_969-971_AUTHORIZED_SCOPE.json` also lists 54 of its own 966–968 parks as `retry_addresses`. The prompt to paste is `EXTERNAL_CHUNKS_969-971_PROMPT.md`. Section 0 runs five Codex agents (one per chunk plus two on retries) and keeps them all active until every row is landed, capped or blocked. Section 5 requires real names instead of placeholders. The rows are recorded in `chunk_history.tsv`.

**Outcome of 969–971 (Codex, 2026-10-23):** Codex returned 107 accepted rows (93 of the 120 new rows and 14 of the 54 retries), 32 parks and 35 blocked rows.
- **Landing:** all 107 were regraded here and their tests re-run under VC7.1, and all landed (batches `2026-10-23_linux_external969` and `_969b`).
- **Names:** the catalog uses Codex's `assigned_name`. This round it gave real names, e.g. `CEntityScriptBinding<NScript::CQ_ArenaScript>::CreateActiveEntityScript` for the script factories.
- **Recuts:** 41 rows had spans that ran into trailing 0xCC alignment. Codex's reviewed recuts (RECUT_APPROVED.json) were applied only after checking here that every dropped byte is 0xCC. The 31 landed recut rows are in `boundary-overrides-pending.tsv`.
- **Headers:** 009ceae0, 00a99840 and 00a9ba70 need Codex's opt-in retail layouts in `engine/CBankFile.h` (`FABLE_RETAIL_BANK_FILE`) and `engine/CMemoryDataOutputStream.h` (`FABLE_RETAIL_MEMORY_OUTPUT_STREAM`, a 12-byte CArray). Both additions leave the default layout unchanged, so they were adopted.
- **Jump table:** 00c3f110 owns a jump table; the lander reports RELOC_UNCHECKED for it. Codex audited its 7 local relocations.
- **Follow-up:** an in-house pass over its parks and blocked rows is in `$S/cdx969/{U,N}/`.
