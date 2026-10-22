# Chunk 276 -- lessons from the authoring agent (fold into GOTCHAS at landing)

## Codegen lessons (source form -> retail shape)

### Locals and frame layout
- **Don't name a vector the compiler CSEs into a switch.** If a switch case's vector subexpression was already computed for an earlier squared distance, write it inline (`case 2: v = other.Point1 - c2`). VC7.1 then CSEs it and stores it to the frame with `fst`. A named local adds a frame word (0x3c vs 0x38). (0x00A57430)
- **sqrt before the epilogue.** Use `float d = (float)sqrt(best); ... return d;` with a named local at EACH exit. This schedules fsqrt before the epilogue pops, which a plain `return (float)sqrt(best)` does not. (0x00A57430)
- **Lexical blocks share frame words.** Split temporaries into `{}` blocks to make them share frame words. Edge C2DLineF temporaries share one slot, and the second edge's out-params land in the dead box-argument slot. (0x00A57CC0)
- **Declaration order picks registers.** Declare the destination reference before the source (`T& dst = ...; const T& from = ...; const T& to = ...;`). This fixes the esi/edi assignment in copy loops. (0x00AA09F0, 0x00A3C3B0)

### Comparisons and branch layout
- **Range checks with double temporaries.** Write `const double& lo = std::min<double>(a, b); const double& hi = std::max<double>(c, d); if ((float)(lo - hi) > tol) return false;`. This gives retail's sequential double-temporary min/max, then `fcomp dword [tol]`. (0x00A56C80)
- **`fcomp tol` from fabs.** `(float)fabs(x) < tol` gives `fcomp tol`. A plain `fabs(x) < tol` gives `fld tol ; fcompp`. (0x00A56C80)
- **Negated compares pick the flag test.**
  - `!(x > limit)` gives `test ah,41h ; jne`.
  - `!(a < b)` gives `test ah,5 ; jnp`.
  - `!(a > b)` gives `test ah,41h ; je`.
  (0x00A56C80, 0x00ADD690)
- **False exit at the end.** Nest the success path as `if (denominator != 0.0f) { ...; return true; } return false;`. This puts the false exit last, as retail does. (0x00A56C80)
- **je/jne layout for an empty array.** `a.Data == 0 ? 0 : size()` gives retail's layout where a plain size() does not. (0x00A3C3B0)

### Loops
- **Stop VC unrolling a loop.** Guard it with `if (count > 0)` and write `for (i = 0; i != count; ++i)`. A `<` loop gets unrolled 4x. (0x00ADD690)
- **jae for an unsigned bound.** An unsigned index compared against a `(unsigned long)count` bound gives `jae`. (0x00ADD690)

### Reference-counted releases
- **Pointee release.** `T* old = Object; ... if (--old->RefCount == 0)` gives `dec [ecx+4] ; jne`. (0x00A3C3B0)
- **Destructor form.** An inline `long DecRef() { return --RefCount; }` with `T* p = Object;` gives the dtor's dec-into-register form. (0x00A3C3B0)
- **Conditional temporaries.** A pointee-zeroing dtor (`Object = 0; Info = 0;`) on CCountedPointer temporaries reproduces retail's trailing zero stores. (0x00A4EBB0)

### Structures and calls
- **Member-assignment declarations.** `C2DVector dirA, dirB, point;` declared up front and filled by member assignment changes the x87 order against initialised declarations. It is not enough on its own. (0x00A56C80)
- **Wrapper over the first member.** CalcLocalBoneTransformation 0x00A53860 is `SequenceData->GetKeyframeInterpolate(...)` fully inlined (`mov ecx,[ecx]` first). It is byte-near the two GetKeyframeInterpolate copies 0x00A4C1F0 / 0x00A52760.
- **Arguments evaluate right to left.** An inline `ScaleRows(1/sx, 1/sy, 1/sz)` puts 1/sz deepest on the x87 stack (retail fdivr on sz first), and Scaling.Z is loaded first for the rebuild scale. (0x00AAF1E0)

### x87 operand order
- **Not driven by the source in matrix multiplies.** VC7.1 canonicalises the term order of x87 sums of products. A 96-spelling in-context search per element (probe/mx/ctx.py) did not move either 3x4 multiply in 0x00A89A70. Treat these one-instruction-order misses as a lane gap after one or two tries.
- **Sum order runs from the last term.** In 0x00AAF1E0 a plain `a*a + b*b + c*c` computes the last term first. To get retail's X-first order, write the terms mirrored.

### Lane gaps
- **Inline-asm fistp rounding in the slerp.** The slerp's table acos and sin round with the bare `fsub 0.5 ; fstp ; fld ; fistp` helper. Every GetKeyframeInterpolate copy hits this: 0x00A4C1F0, 0x00A52760, 0x00A53860.
- **Inline-asm SSE.** 0x00A9D750 is the original inline-assembly SSE block: a fixed xmm3, and the row pointer re-read through the frame.

## Review at landing
- **00a57430 jump table.** It has an embedded 4-entry jump table. q.py masks relocations, so it grades RELOCATION_MATCH, but landing may need verify_land_jumptable.py.
- **Rows that needed more than 4 grades.**
  - 00a57430 (10 probes)
  - 00a3c3b0 (11)
  - 00add690 (19)
  - 00a56c80 (25)
  - 00a89a70 (spelling search)
  These counts follow the wave-25 five-per-strategy budget; each distinct strategy stayed within five.
- **Local models next to engine headers.** Each has a `// Layout:` line with the evidence:
  - The retail C3DMeshFileChunk base is 0x2c (PDB 0x34).
  - Retail STLport vectors are 12 bytes (PDB 16).
  - C3DMesh2 is 8 bytes below the PDB: BoneCount +0x98, Bones +0x9c.
  - C3DAnimationStats uses pack 2: MovementVector +0x12.
- **Wrong catalog labels.** 00c5acb0 is skipped: it is library code, not C3DMeshFileXBoundingBoxChunk::LoadData.
- **Twin GetKeyframeInterpolate rows.** 00a4c1f0 and 00a52760 are the same source in two TUs and differ only in x87 scheduling. When one lands, the other needs its own TU flags or source order.
