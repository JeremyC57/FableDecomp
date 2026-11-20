# NF118 lessons (one line each)
- A missing `#include` (engine/CCharString.h does not exist; CCharString is fable_string.h) made VC7.1 spin under Wine at 99% CPU instead of failing: run `cpp` on every new include set before the first grade.
- The split gate counts only rows that REFERENCE a target, never the target's own self row: a callee self row with a stand-in spelling is a NAME_FIXES finding, not a created split.
- Ego_d's carved per-object function list (ghidra_out/egod_carved_functions.tsv) separates .cpp members from header inlines (the object that contributed the COMDAT): use it when a claim names a header-inline getter for an out-of-line retail body.
- Porting a stand-in thing lookup onto the shared CThing: use PeekTC (const LowerBound 0x004365B0) where retail calls the const copy; GetTC binds the non-const LowerBound spelling to the const address (RELOC_CONFLICT warning, still RELOCATION_MATCH).
- An out-of-line `bool` getter whose body is `mov al,[m] ; test al,al ; setne al` (no xor) came from `if (m) return true; return false;`; `return m;` gives `mov al,[m]` only and `m != false` adds `xor eax,eax`.
- A functor holding a reference member reloads it after each call when read through `this`; retail's single load came from a named local copy of the reference.
- Probe every REFS row of a batch of small entries in one background run (tools/probe.py) before opening any row: four of eight entries here needed only the self row.
