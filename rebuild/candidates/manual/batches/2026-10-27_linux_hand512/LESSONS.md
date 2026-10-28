# Chunk 512 -- lessons

- STLport fill/insert/overflow bodies are template-stable: one matched `_M_fill_insert` (0xa750e0) became 0xa84690, 0xa85cc0,
  0xa96f00 and 0xa97040 at 1-2 grades each by swapping the element type and the tag-taking helpers.
- Tag homing: a tag-taking callee given a same-TU `__declspec(noinline)` body under `#pragma auto_inline(off)` that never
  reads its tag makes VC7.1 home the empty tag on retail's (live) argument slot; with only a declaration the tag slot differs.
- Access matters for the mangled name: STLport `_M_insert_overflow` is protected (IAE); `_M_fill_insert`, `_M_range_insert`
  and `operator=` are public (QAE). Existing landed bindings often have the opposite.
- `__median` with u64 compares and `erase` with a loop needed `__forceinline` to inline as retail does.
- A same-TU noinline body for an out-of-line ctor (vector(n)) tells VC the ctor does not capture the object, so the local's
  start stays in a register across later calls.
- `(float)fabs(x) <= 0.0001f` gives a dword fcomp; without the cast the compare goes to double.
- `__copy_trivial` must be the `(last == first) ? result : (char*)memmove(...) + n` ternary to get retail's compare order.
- C3DKeyframe::ToMatrix(CMatrix4x4&) is the TRANSPOSE of the 3x4 version's rotation block (translation in E14/E24/E34).
  Its residue (integer stores hoisted to after E11) did not move with the stores in the outer function or with a 16-float ctor.
- Shrink-wrapping (VC deferring callee-saved pushes past an early return) did not yield to source restructuring or the QFE
  compiler in 0xa77780; park such rows early.
- Retail called one __uninitialized_copy address from both const- and non-const-source call sites (0xa74ac0): expect
  RELOC_CONFLICT where an earlier landing bound the const spelling to a byte-identical twin.
