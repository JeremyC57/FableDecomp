# Chunk 650 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **A callee that is "inlined everywhere but once" is a plain non-inline function in the same TU, not a forced call** (0x00A64910).
  Define the helpers (here RotateLeft/RotateRight, whose out-of-line copies retail also has) as ordinary members before the caller,
  with no `inline` keyword and no pragma. VC7.1 /Ob2 auto-inlining then makes retail's choice (5 of 6 inlined), and that choice
  fixes the whole-function register assignment. `#pragma inline_depth(0)` around one call to an inline member reproduced the
  structure but not the registers (this edi vs ebp, 3 vs 4 pushes).
- **Keep a repeated null-or-black test as one inline helper** (0x00A64910). Open-coding `a == 0 || a->Colour == 1` at three sites
  grew the body by 4 bytes. A file-static inline helper and a static member helper both match.
- **Reuse a landed sibling's stand-in model for an STL algorithm family** (0x0097AF70). The __make_heap 0x0097B0E0 model (memberwise
  C2DVector copy ctor plus a same-TU auto_inline(off)/noinline operator() stand-in) matched __adjust_heap on the first grade.
  The by-value comparator copy then loads its words two ahead of the stores.
- Copy-propagation ties stayed put: 0x00AF56E0 (a store from ebx vs edi after `mov edi,ebx`, 11 loop/assignment forms) and
  0x007E4530 (a pointer in edx vs eax, 6 forms).

Review at landing:
- Grades this session: 0097af70 1 (+1 src regrade), 00a64910 3 probes + 1 regrade (one probe at 761 while removing the helper),
  00af56e0 11 (over the 4-attempt rule), 007e4530 6 (over the rule). The other 6 rows were not touched (owner stop).
- 0x00A64910's TU emits RotateLeft/RotateRight bodies as extra functions (retail 0x00A646E0 landed, 0x00A64680). The leaf is pinned to
  the exact mangled RemoveFromTreeFixup name.
- 0x0097AF70 carries the operator() stand-in body (retail 0x0097ABF0), the same practice as the landed 0x0097B0E0.
- differ/007e4530.cpp (copied unchanged) contains `#pragma pack(push,1)`. Strip it before any regrade or landing.
- differ/ files for the untouched rows are the brief's best attempts copied unchanged. Their line 2 is not always a `// Grade:` line.
- Tests sit at test/<addr>.cpp (the name finish.py reads), not test/<addr>_test.cpp.
