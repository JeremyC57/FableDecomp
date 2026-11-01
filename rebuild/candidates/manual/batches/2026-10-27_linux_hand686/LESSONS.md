# Chunk 686 -- lessons (GOTCHAS one-liners)

- A libpk method that calls 31 sibling setters in a row needs every setter's landed body in the same TU as
  `__declspec(noinline)`: VC7.1 uses the callees' ecx-preservation facts and the preset walk only matches with them (00c68a50).
- The shared `engine/CCountedPointer.h` (not a local counted-pointer model) is what gives retail's register map at a
  `CCountedPointer<CBankStateBlock> state(new ...)` + AddEntry site: the copy's Info pointer stays in esi through the calls
  into the block-end dtor (00a3a410, 376 -> 206 diff lines).
- VC7.1 declines to expand STLport `vector::resize` at two sites in a function that also inlines the CCountedPointer header
  members; `__forceinline` on resize is the only lever found (00a3a410).
- A `size != GetX()` compare where retail loads the member into a register before the call is not an operand-order
  question: both orders compile to `cmp eax, [mem]` (00a3a410).
- Vector normalisation written as `v /= len` with `operator/=(float s) { return *this *= 1.0f / s; }` (and
  `scale = newLen / origLen`) reproduces retail's `fdivr 1.0 ; fst spill ; fmul st(1) ; fstp st(0)` sequence; the
  reciprocal-local form (`inv = 1/len; v *= inv`) spills differently (00b202d0, 135 -> 85 diff lines).
- VC7.1 hoists every ready identity store of a local CMatrix3x4 into the prologue whatever the declaration position; when
  retail keeps 9 of 12 after other code, something makes them unready (unsolved, 00b202d0).
- The `anyRemapped` flag form (scan for any remapped key, then rebuild the map) matched RemapGroups; writing the scan as a
  find-and-break without the flag does not (00ac97d0).
- STLport `erase(first,last)` with the real `copy -> __copy_trivial -> memmove` text folds to `finish = first + n` exactly
  like the hand-shortened form (00a3a410); it is not an inlining lever.
- A 12-byte element `fill` loop that loads X, Y, Z in retail order is not reachable through fill/operator=/finish-expression
  variants (00b801f0, 12 probes) -- suspect register-allocation facts from the TU's other members.
- A 5-arg ctor retail inlines at one site and calls at another is not reproduced by out-of-class / after-function /
  temporary forms (007808b0): VC inlines both.

## Review at landing
- 0x00CBDAD0: callers bind `IntrosortLoop_00cbdad0`; respell to the instantiation in the Leaf line.
- 0x00C68A50: 00/c6 setters landed twice with different class tails (12 files shorter); SetReverbFlags carries noinline
  copies of the 8 scale setters -- dedupe by function name when merging TUs.
- 0x00A5BBA0 retail_asm: `// Retail-asm:` evidence cites 0x00A5BBD6..0x00A5BC12 with the SSE instructions.
