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
