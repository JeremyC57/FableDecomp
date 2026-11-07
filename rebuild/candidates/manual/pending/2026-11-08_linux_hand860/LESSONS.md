# Chunk 860 lessons

- **Callee visibility decides slot sharing:**
  - VC7.1 shares an address-taken local's or dead parameter's frame slot with later locals only when every callee that
    receives the address is visible in the TU and does not capture it.
  - Micro proof: an out-of-line declared `Ext(C* const&)` blocks reuse of item's slot. A visible `auto_inline(off)` body
    allows it. A visible body that forwards the reference to a declared-only function blocks it again.
  - Retail gets visibility from STLport templates and earlier-defined same-.cpp functions. Use same-TU bodies (landed ones
    where they exist) under `#pragma auto_inline(off)`. 0x005AECC0 went from md 349 to 76.
- **Hidden return pointer:** a local std::pair model without constructors returns `pair<K,V>` in edx:eax. STLport's pair has
  ctors, so retail passes a hidden return pointer. Give the model its ctors.
- **Bookmark lane gap:** in 0x009D8CF0, a function-wide knob (indices filled by statements plus a manager-reference local) fixed
  all the stencil-block load-order flips. The edx/ecx colouring of three texture-stage blocks never moved in about 370
  combinations: it is a separate effect.
- **Resize folding:** a track vector declared right at its `resize` folds retail's `insert(end(), n, x)` to `push 0` (size known
  0). Declared at the top, ours keeps the size computation, yet retail folds anyway (see the slot-sharing lever).
- **Random combination search:** comb.py (random combinations of per-site alternatives, graded about 0.3 s each on cached
  compiles) finds count-effect fixes that one-at-a-time edits miss. 0x009D8CF0 went from md 32 to 12 on a two-site
  combination.
- **C2DExtentsI store order:** retail stores Y before X in 0x0049A0B0's Initialise blocks; a `{ Y = y; X = x; }` ctor gives it.
