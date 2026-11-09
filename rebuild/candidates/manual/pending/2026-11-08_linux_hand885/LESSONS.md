# Chunk 876 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- VC7.1's inline cap (`/d2inlS`) measures the caller's WHOLE IL, not the IL seen so far: in a probe with 1,358 GetLength sites,
  600 extra plain calls placed at the start or at the end cut the expanded sites from 146 to 66 (mini t22/t25, row
  0x00CBFB7D). Code after a site therefore moves that site's decision. Each S step is worth about 120 GetLength expansions
  there (S7 26, S8 146, S10 386).
- Inline decisions under the cap are not monotone in source order. Retail RunCutsceneMacro expands `find(k) == end()` at
  0x00CC61B8, calls the same `==` at 0x00CC6962, and expands it again at 0x00CC6DA9. Expansions that shrink the IL hand
  budget back. Do not read one called site as "the budget ran out here".
- An argument that contains a call makes VC7.1 preload the pDSTGame vtable into a callee-saved register before evaluating
  the arguments. Retail evaluates the call first, so the source held it in a named local (`float alpha = GF..(p0);
  CScriptThing thing = actor->GetScriptThing(); pDSTGame->EntitySetAlpha(thing, alpha, true);`, 0x00CC4FB9..0x00CC4FE7). That was 42
  preload sites in one function.
- A `cond ? a : b` bool or float argument compiles to one call with a CSE'd select. Retail's `push 1`/`push 0` (or two
  `fld` loads) branches before one shared call tail come from `if (cond) f(.., a); else f(.., b);` (UseCamera fov,
  PauseThing, MuteSounds, 0x00CBFB7D).
- `it = m.find(k); if (it != m.end())` on a block-local iterator declared earlier copies the find temporary into the local,
  evaluates find before end(), and keeps `!=` out of line. `if (m.find(k) != m.end())` does none of these (0x00CCC9C5).
- In a function whose biased frame (`lea ebp,[esp-68h]`) is larger than 0x80 bytes, frame slots look ordered by use weight
  round ebp. A slot-offset diff with mixed deltas is downstream of inline and code differences; it is not a declaration
  order problem (tools/frame.py, 0x00CBFB7D).
- Cross-jumping keeps the LAST of N identical `call dtor ; jmp join` tails in retail (0x00CC8231, six FadeThing* /
  SetThingConscious tails). Ours keeps the first when an earlier inline or register difference changes the tail set.

- Inline-budget per-site devices (session 4, 0x00CBFB7D): under `/d2inlS<N>` a `#pragma inline_depth(0)` region outlines exactly
  the call sites inside it (hoist accessors retail expands, e.g. `end()`, into a local first); around a declaration it also
  outlines the default argument's `allocator()` ctor, so construct the vector from a named allocator declared before the region.
  The reverse (retail expands past the cap) needs a `__forceinline` twin that carries the BODY: a forceinline wrapper whose body
  calls the plain inline is capped like the inline itself.
- A free-function `__forceinline` twin of `find(k) == end()` evaluates its arguments right to left, so end() is loaded before
  the find call (into a callee-saved register); `IsEnd(m.find(k), m)` with `c.end()` inside keeps retail's order (0x00CC61B8).
- Same-TU non-template callee bodies that only read through their reference arguments (CSExtractParameterList 0x00CBFACA) make
  VC7.1 treat the caller's local as not escaped: vector._M_start stays in a register across unrelated external calls and an index
  loop over it is strength-reduced to a pointer + down counter (0x00CD0DBC, 0x00CD0E4C). A body that passes the reference on to
  virtual calls (PlayCutsceneSound) changes nothing.
- `for (long i = 1; i <= n; ++i)` enters with `cmp n,1 ; jl`; retail's `test n,n ; jle` + strength-reduced `(i + 1) * 10` is
  `for (long i = 0; i < n; ++i)` (0x00CC2DDC). `size() > 0` gives `jbe`, `!= 0` gives `je` (0x00CC6A3C). An unsigned count gives
  the loop entry `je` on the size itself (0x00CD0E4C).
- A cl.exe compile of a header that hits `#error` spins under Wine until the timeout: do not use `#error` to probe overlays.

Review at landing:
- 0x00CBFB7D needed 41 grades over 4 sessions; it is parked with a complete source in differ/00cbfb7d.cpp. Its TU carries same-TU
  bodies of seven landed/retail helpers (IsTrue/IsFalse/IsNull, IsCutsceneSkipped, CSFindThing, CutsceneMacro_ReadLightScene,
  CSExtractParameterList) under `#pragma auto_inline(off)`: the lander must take only the Leaf symbol from this object.
- The row's header opt-ins (hdr/README.md) are additive `#ifdef FABLE_RETAIL_*` blocks. Fold them into the shared headers
  only if a later attempt lands the row; nothing else uses them yet.
- The top-tested CrowdKill wait loop (0x00CCF7BE) is not reproduced by any `while`, `for (;;)`, `while (true) {if..else
  break;}` or `bool more` spelling in a probe TU. VC7.1 rotates all of them, and `bool more` keeps a flag register.

Session 5 (2026-11-08):
- c2prio.py's default `--func` (from the `// Leaf:` line) matches nothing for RunCutsceneMacro's very long mangled name ("0
  function(s) traced" after 125 s): pass `--func RunCutsceneMacro`.
- In the 74 KB function a `for (;;) { if (!(c) || f()) break; ... }` loop compiles byte-identically to `while (c && !f())`:
  the rotation is not a front-end artefact of `while` (0x00CCF7BE stays top-tested only in retail).
- Same `moveType = 1` gives `inc reg` when the value has a register holding 0 (FollowNavRoute) and `mov [mem],1` when it
  does not (WalkUpToThing 0x00CC2443): retail's `inc edi` is an allocation result, not a `++` spelling.
- A frame of equal-density 4-byte slots (regime-1 layout) shows a block of slots shifted by 4 with a permuted boundary when one
  tie symbol differs; measure with a slot pairing (t5/fmap.py) and grade shapes with a frame-masked share, not the raw one.

## Session 6 (GOTCHAS-ready)
- c2 13.10.3077 priority K(b) counts every candidate referenced in the block, enregisterable or not: a 74 KB function's entry
  block that initialises 7 flags which never get a register still makes the first `__fastcall` parameter's priority 2*19 = 38;
  binding the flags to local references (`bool& r = flag;`, no code) removes them from the candidates (K 12, prio 24) and
  flips name/-1 to retail's ebx/esi (RunCutsceneMacro 0x00CBFB91 / 0x00CBFC01).
- Candidate removal without code: a local REFERENCE (or pointer) bound to the variable works; `(void)&x`, a by-reference
  parameter of an inlined (FABLE_FORCEINLINE or inline) call, and `T x[1]` do not (all stay candidates).
- A split constant's region piece can only take a register once it exists: the const -1 prologue piece (prio 44) appears after
  15 splits, so a 38-priority parameter coloured earlier takes esi first. "Its priority is higher" is not enough -- check when
  the piece is born (tools/pk/chain.py walks the split chain through reused ids).
- c2 frame packer (frames > 0x80): items sorted by size, weights = memory references counted before cross-jumping, then a K&R
  quicksort (0x107ae0c4, middle pivot, unstable) by density w*1000/size descending (comparator 0x107ae9c2, flag +0x18 first),
  offsets top-down from 0x68: equal-density ties land in quicksort order, i.e. by when each item reached its final weight.
- TintScreenOut (0x00CD11E0): retail evaluates GFCharStringToFloat(params[0]) before loading pDSTGame -> a named local before
  the call, not an argument expression.
- The 4th loop family for CrowdKill's wait (goto loop) compiles byte-identically to while/for: 0x00CCF7BE's top-tested shape is
  not a spelling.

## Session 7 (GOTCHAS-ready)
- VC7.1 c2 has a function-size mode switch: in a function of more than ~1600 simple statements (between 1500 and 1750
  `if (f(k)) g(k);` lines, ~10k tuples) a local reference or pointer bound to a local (`bool& r = x;`, used or not, or bound
  inside an inline helper, or held by a local helper object) takes x out of the register candidates with no code; below it the
  same binding is resolved and x stays a candidate (micro s7/micro/runshape.sh; RunCutsceneMacro 0x00CBFB7D is far above).
- What does NOT remove a local from c2's candidates in that mode: a local POD struct / class with ctor / enum-indexed array of
  the flags (VC7.1 scalar-replaces aggregates: each member is its own candidate and frame item), inline helpers taking the
  flag by reference / pointer / const reference / returning it by reference, nested inline forwarding (parameters are
  substituted); `(void)&x`, `*(char*)&x`, `&x != 0` (folded). An escape to an out-of-line callee emits a `lea` (retail has none).
- A `GetLength() != 0 && StartsWith("$")` condition whose temporary needs a conditional destructor materialises the whole &&
  (`test; jne; mov bl,1; jmp; xor bl,bl`); the ternary `A ? B : false` materialises only B (`mov ebx,eax; neg ebx; sbb bl,bl;
  inc bl`) but still emits a false arm; nested ifs drop the conditional-destructor flag (s7/pfx, 0x00CC0418).
- Regime-1 x87: a named loop `float fraction` is duplicated (`fld st(k); fmul mem`) and consumed at its last use; writing
  `(float)i` at each use adds `fld st(0)` dups. Retail's SlideTeleport keeps fi untouched until `fstp st(0)` before the call
  (open; s7/x87).

## Session 8 (GOTCHAS-ready)
- c2prio's json `line` fields are the source file line minus 2 for this row's compile: pass base 235 (function start + 2) to
  cdiff2.py, or every verdict votes on a window two lines early (it invented `i` divergences and hid c #760's).
- VC7.1 c2 const candidates (-1/0/1, string addresses): one root per value, live through the whole loop; inline `strcmp`
  (`repe cmpsb`) and a 12-byte struct copy (`movsd` x3) pin esi+edi in their blocks, so a const root live there starts
  ebx-only. A split re-grows the pieces from the entry block, absorbing blocks while the union of used registers is not full:
  whichever coloured region the growth meets first decides whether the rest keeps the fixed blocks (ebx-only) or drops them
  (edi-only). One early edi user in the right region flips the outcome for the whole function (s8 wc120).
- In a regime-1 function, colouring-order divergences that only show late (K62, K91, K521 here) can all come from ONE
  earlier split whose outcome final registers cannot show: compare piece extents (c2prio8 --dumpk/--dumpr), not registers.
- Spelling variants at the fixed-register sites (|| v else-if, ternary, scope blocks, named v unnamed temporaries) do not
  move the const splits when the sites already match retail; one shared `long c` for two loops stays two candidates.
