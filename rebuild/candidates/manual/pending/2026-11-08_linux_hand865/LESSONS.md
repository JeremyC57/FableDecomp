# Chunk 865 lessons (inline-budget parks under the 2026-11-08 owner decision)

- The ten rows proven under a diagnostic knob all regrade RELOCATION_MATCH with the same knob as a per-row `flags` entry, source untouched
  (device 2). Bisected bands: the MovieBase-dtor caller-size rows match for S15..S20 and never below (S14 = default shape, S9 is the
  default): /d2inlS is a cliff at 15 for every 2.6-4.5 KB Main here, so one value, S15, serves the whole family (three TUs).
- /d2inlT bands are wide but row-specific: 0x00DAC760 T63..T72, 0x00E8EF80 T55..T72, 0x00EF4B40 T28..T36 (T37+ = default T74 shape).
  Picked the value nearest the default (T72 / T72 / T36) so the deviation is minimal; any value in the band is byte-identical.
- A nested predicate dtor (~CIsThingConscious holding a CScriptThing) that retail expands one level only cannot be reached by /d2inlT: the
  knob moves the inner inlining too (T30..T100 all differ). `#pragma inline_depth(0)` round the predicate dtor's own definition compiles it
  with the nested ~CScriptThing as a call, and Main then expands it exactly as retail (device 3, 0x00E799F0).
- A compiler-generated operator= whose body is two calls (CBaseObject::operator= + CCountedPointer::operator=) is never expanded in a
  3 KB caller under any S or T value, alone or combined (S15..S30, T60..T150, S15+T60..T150); the only device is a row-local
  `__forceinline` definition under `#ifdef _MSC_VER` (0x00D19BD0, with S15 for its MovieBase dtor sites).
- In-body `#pragma inline_depth(1)` / `(2)` regions are NO-OPS in VC7.1 RTM for nested template inlines (push_back -> _Construct,
  ~CDeletion -> ~list -> ~_List_base): only depth 0 changes anything, and it outlines the outer inline too. Do not spend grades on them.
- 0x00DAADD0's "22 of 42 push_back sites expanded" is not a budget any -inl* knob controls: fcsw75+ outlines every site, flcsw/fcsa/nlw
  change nothing, and (chunk 862) site count, callee cost and fillers never move the cut. It behaves like a per-callee expansion count.
- Visible same-TU callee bodies fix frame-slot sharing for empty dispatch-tag temporaries: 0x0083F650's forward_iterator_tag
  (`return line`) and random_access_iterator_tag (find_first_not_of) share the result-pointer argument slot in retail; with __find_if
  0x00841900 declared only, c2 gave the second tag its own home (the `name` slot, one lea off); with the landed __find_if body in the
  TU (auto_inline(off), never writes through the tag reference) the slots share (research861 2.4 applied to a 1-byte tag). The
  `return line` out-of-line range initialise is still the caller-size cap (/d2inlS); give every out-of-line callee a same-TU body
  BEFORE measuring the flag band, or the band hides a second residue.
- gr.py / G.sh apply the source's `// Flags:` line on top of `--extra`: a "default" probe of a file that carries a Flags line is not a
  default build (cost one wrong conclusion here); probe single flags from a copy without the Flags line.
- Ledger mis-cut pattern: a function whose last instruction is a call to a `__declspec(noreturn)` function owns the `int3` VC7.1 emits
  after it; the catalog row stops before it (0x0083F650: 442 listed, 443 real). Grade against the extended extent and report it.
- A residue of ONE jne displacement between two byte-identical return tails (0x00D18DF0) is a c2 tail-merge pick, not an inline decision:
  the lane compiler always sends a conditional return to the LAST identical tail; retail sent one of nine to an earlier copy. do-while(0),
  brace scopes, `if (!T) { ... }`, dropping or duplicating the neighbouring returns do not move it. Park such rows; do not count them
  as inline-budget.
- q.py (and g.sh / gall.sh) share one work dir per address: never grade the same address twice at once through them (the two builds
  overwrite each other and both report the first object); gr.py's tag-named dirs are safe.
- Header overlay copies: a copied header that includes a sibling by bare name (`#include "fable_containers.h"`,
  `#include "LHNetworkTypes.h"`) needs that sibling copied UNCHANGED into the overlay too, because MSVC resolves a quoted include
  relative to the including file's directory before the /I paths (C1083 otherwise). List such copies as unchanged in hdr/README.md.
- "Shared header or local text" is checkable by preprocessing: `cl /EP` of `#include "<hdr>"` with and without FABLE_HDR_OVERLAY must
  differ only by the blank lines the opt-in blocks leave when their macro is undefined.
