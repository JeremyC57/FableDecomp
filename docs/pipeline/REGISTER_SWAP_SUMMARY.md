# Register-swap and operand-order misses: what we see and what we've tried

This is a brief for an outside helper. Context: byte-exact C++ reconstruction of `Fable.exe` with MSVC 7.1
(cl 13.10.3077, `/O2 /Oy`, x87 floating point, STLport). Most parked rows have the right length and the right
instructions. They differ only in **which register** or **which operand order** the compiler picked.

The real question: **which C++ source shapes make VC7.1 choose a given register or a given x87 operand order?**

## 1. Scratch-register rotation (eax / ecx / edx swapped)
**Looks like:** retail loads a pointer into `eax` and ours into `ecx`, or `mov edx,[esi]; call [edx+0x78]` against
`mov eax,[esi]; call [eax+0x78]`. The swap persists for a few instructions, then the code converges.

**What decides it:** the rotation is fixed at the **first** differing pair, by what the compiler materialises first
and what is still live.

**Worked:**
- An inline accessor at **one** call site (`GetDef()`, `GetOwner()`, `GetGraphicAppearance()`). This changes when the
  load happens.
- A named value local, which loads early. Contrast with a `const T&` local.
- `*(begin() + i)` instead of `vec[i]`.
- Naming the reference a call returns before reading a member: `const CWorld& w = GetWorld(); X* b = w.PBank;`.
- Keeping a value genuinely live across a branch so the compiler avoids its register.

**Didn't work, or only sometimes:**
- reordering declarations;
- `register` hints, which VC7.1 ignores;
- splitting expressions into more statements;
- QFE versus RTM compiler.

## 2. Callee-saved register roles (esi / edi / ebx / ebp swapped)
**Looks like:** retail keeps `this` in `edi` and a loop pointer in `esi`; ours has them reversed. Or retail keeps a
zero constant in `ebx` for the whole function where ours uses immediates, or the other way round.

**Worked, occasionally:**
- Moving where a value is first used.
- A `const T&` alias for a container.
- Writing every exit out in full, so blocks tail-merge the way retail's do.

**Mostly unsolved:**
- "Retail caches pointer X in a callee-saved register across several calls; ours reloads it."
- "Ours keeps 0 in `ebx`; retail uses immediates."

We have tried more than 10 spellings per row: named locals, references, inline wrappers, block scopes and
`__forceinline` helpers. None of them moved these. We suspect retail's register allocator saw a different
use-count or weight for the value, but we don't know which source shapes set that weight.

## 3. Stack-frame slot sharing (frame bigger or smaller by 4 to 16 bytes)
**Looks like:** retail's frame is `0x28` where ours is `0x2c`. Two locals share a stack word in retail but not in ours,
or retail reuses a dead argument slot.

**Worked:**
- A same-file `__declspec(noinline)` / `#pragma auto_inline(off)` **body** for any callee that receives a local's
  address. Typical examples are `LowerBound`, `GetDef` and `GetSubDef`. This makes the local non-escaping, so it can
  share a slot. It fixed 5 of the 6 rows where the lookup key's slot was the issue.
- `{}` block scopes, because VC7.1 shares slots by lexical scope.
- A 4-byte `long` dummy instead of an empty struct.
- Packing scalar locals into one `C3DVector`.
- Naming a by-reference argument when retail has an extra 12-byte slot.
- Naming a default-argument temporary, e.g. `SAllocator allocator; CLongList worn(allocator);`. On
  `AddClothingToThing` this put every other local into retail's slot (147 to 27 diff lines).

**Unsolved:**
- "Retail puts a temp inside a dead parameter's slot (`[ebp+0Fh]`)."
- "Retail places a local in a dead argument slot with no frame at all."

## 4. x87 operand order (`fld A; fmul B` against `fld B; fmul A`)
**Looks like:** the same arithmetic, but the two operands of a multiply or add are loaded in the opposite order.

**What we learned today:** the order depends on **how each operand is spelled as an address expression**, and on
which nodes the compiler created first. A choice inside an inlined helper can flip a pair several statements later.

**Worked:**
- Indexing through raw float pointers: `const float* pa = &a.X; pa[1] * pb[2]`.
- Mixing pointer and member access per component.
- Another equivalent address form when one form fixes one pair and breaks another. `*(&b.Y - 1)` for `b.X` matched
  `CTCWillLightning::MakeRandomBeam` after more than 60 grades.
- Parenthesising constant products.
- `extern const float` constants, to stop folding.
- A `C3DVector` user copy constructor.
- `(float)(double)(...)` wrappers, which add no bytes but move instruction scheduling.

**Didn't work:**
- swapping the order operands are written in, for example `a*b` to `b*a`; the compiler canonicalises it;
- scalar float locals, which made things worse;
- rewriting `+=` as `x = x + y` or `x = y + x`.

## 5. Instruction scheduling (one independent instruction one slot early or late)
**Looks like:** `mov ecx,ebx` placed before or after a store, or a `push` placed before or after an `fsubp`.

**Worked:** adding 2 or 3 folded x87 nodes earlier in the block, for example with `(float)(double)` wrappers or
`-(a*b) + c*d`. This shifts the scheduling window.

**Unsolved:** under `/Os`, the float-argument slot reservation (`push ecx`) is placed before the last `fsubp` in
`obj->SetPosition(C2DVector(a - b, c - d), 0.0f, 0)`. It affects five rows, and about 30 spellings have been tried.

## Things we'd most like ideas on
1. Which source shapes change VC7.1's register-allocation weights, so that it keeps a pointer in `esi` or `edi`
   across calls, or stops caching 0 in `ebx`.
2. How to make VC7.1 reuse a dead parameter's stack slot for a temporary.
3. ~~String-literal ternary~~ solved 2026-10-23: write the call in each branch so VC7.1 cross-jumps them (see GOTCHAS).
4. The `/Os` `push ecx` before `fsubp` case in section 5.

## Outside suggestions tried (round 2, 2026-10-23); all no change unless noted
- **Typed zeros** (`0L`, `0U`, `'\0'`) for the `ebx`-zero row: bytes identical. Constants are canonicalised before
  register allocation, so their spelling cannot change it.
- **Pre-initialising a pointer and then overwriting it, or `(T*&)p = ...`**: identical bytes, or worse. On
  `AssignAbilityPC`, retail gives `old` the same treatment as an earlier `icon` local (`ebx` plus a shared slot). Ours
  keeps it in memory. Block scope, declaration order, a whole-call ternary and hoisting `manager` don't move it.
- **The comma operator `C2DVector((0, a - b), c - d)` and a named vector local** for the `/Os` `push ecx`/`fsubp`
  case: the comma forms are byte-identical, and the named local is worse.
- **A dead parameter slot for a temporary**: no clean spelling found. Only aliasing the parameter's own storage
  moves the temp there, which isn't acceptable source.

## Round 3 (2026-10-23)
- **Solved shape:** naming nothing and writing the **whole** find-and-use block in each `if`/`else` arm fixed
  `AssignAbilityPC`'s `old` register and slot. It is now 996 v 996 bytes. Piping the result into an inline helper or a
  ternary made it worse.
- **Still open on that row:** the first `use` parameter's `new` temp is stored in container's dead slot instead of
  ability's dead slot.
- **No change:** allocator spellings on `AddClothingToThing`, and identity wrappers (inline or `__forceinline`) on
  either axis for the `/Os` push case.
- **New open question:** retail stores a `new T(param)` temp in the dead slot of the **same** parameter the
  constructor reads (`mov ecx,[ebp+8]` then `mov [ebp+8],eax`). Ours picks another dead parameter's slot or a frame
  slot. Changing the constructor (by value, `const long&`, assigning in the body), naming the pointer or adding block
  scopes did not change it. This blocks `AssignAbilityPC` (996 v 996) and its sibling `AssignThingPC`.

## Round 4 (2026-10-23)
- `CActionParamLong(ability++)`, `ability--` and `(ability = ability)` give identical bytes: the dead store is removed,
  so it can't free the slot. `CActionParamLong p = { ability };` won't compile because the class has a constructor and
  a vtable, so it isn't an aggregate.
- `CLongList worn((const SAllocator&)SAllocator());` is the same as the unnamed temp: 147 diff lines. Only the named
  local gets to 27.
- **New family question, about 17 rows (`CThingSearchTools` rough-area templates).** Retail encodes the per-row `Width`
  load as `[hoisted + LevelInfo + 4]`, with `LevelInfo` as the SIB index; ours makes `LevelInfo` the base. Every
  spelling of the indexing tried canonicalises to the same tree. Which source shapes make VC7.1 swap base and index?
- **Round 5 on the SIB family (007fcef0, 15 grades):**
  - `offset[(const char*)LevelInfo]`, `(DWORD)offset + (DWORD)LevelInfo + 4`, and a `pBase` char pointer all produce
    the same bytes as each other.
  - When `offset` is a **user loop variable** (`long offset = level*20;` in the level loop), the load *does* come out
    as retail's `[offset + LevelInfo + 4]`. But the extra variable changes register allocation for the whole loop:
    142 diff lines, or 553 v 542 bytes when it's declared just before the y loop.
  - When the same arithmetic sits inside the inline `GetCellPtr`, VC7.1 hoists it itself and the order goes back to
    `[LevelInfo + offset + 4]`.
  - So base and index follow **who created the invariant**: a user variable becomes the base, a compiler-hoisted temp
    becomes the index. Still needed: a spelling where the compiler does the hoisting but the offset ends up as the
    base.
- **Round 6 on the SIB family:** all of these were written as byte offsets through `(const char*)LevelInfo` to keep
  the arithmetic right, and all are byte-identical to the base version, so the front end folds them before
  hoisting: a comma operator `(level*20, 0) + ...`, `level * sizeof(CMapwhoLevelInfo)`, `0L + level*20`,
  `&base[4] + level*20`, and `level*20 + (base + 4)`.
- **Round 7 on the SIB family (structural):**
  - `do { long offset = level*20; <y loop> } while (false);` gives the same bytes as declaring `offset` just before
    the y loop: 553 v 542. Scoping it tightly doesn't stop it changing register allocation for the whole loop.
  - `if (map) { ... }` around the y loop is worse: 129 diff lines.
  - Wrapping `LevelInfo` in an anonymous struct gives base bytes.
  - Declaring `offset` inside the y-loop body gives base bytes. The compiler hoists it again, so it's the index
    again. Retail's order only appears when a *variable* holds the offset at loop entry, and that variable always
    costs a register and moves the rest.
- **Round 8 on the SIB family:**
  - Inside the per-row inline, so `LevelInfo` still reloads each row: `(&LevelInfo)[0]` bound to a const pointer, a
    `(char*)LevelInfo + 4` shifted base plus `level*20`, and a static `LevelWidth(LevelInfo, level)` helper taking
    the pointer explicitly. All give base bytes, still `[LevelInfo + offset + 4]`.
  - Binding the shifted base once before the y loop: 546 v 542, 101 diff lines. Retail reloads `LevelInfo` each
    row, so a loop-invariant copy is the wrong model anyway.
