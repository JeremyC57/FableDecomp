# Chunk 748 -- retry of the 8 parked hand-SSE rows (Linux Wine lane)

Work dir: $S/w22/chunk748. Mirror HEAD: see MIRROR_HEAD.txt. Every grade is in LOG.tsv (`bash g.sh`), probes in p/, diagnostic
helper-function probes (contain `__asm`; never deliverables) in diag/.

## Results
| address | name | grade | attempts | technique |
|---|---|---|---|---|
| 0x00A9D580 | CMatrix4x4::AlignedPreScaleTransposed | **RELOCATION_MATCH** (src/) | 320 | macro GF_SSE_MATRIX_PRESCALE_TRANSPOSED |
| 0x00A9D480 | CMatrix4x4::AlignedPreScale | DIFFER(243v243) dl=8 | 139 | macro GF_SSE_MATRIX_PRESCALE |
| 0x00A9D750 | C3DAnimationManager::ApplyBoneMorphScaling | DIFFER(551v551) dl=24 | 23 | both prescale macros |
| 0x00989230 | CShaderRenderManager::UpdateObjectSpaceTextureTransform | DIFFER(636v636) dl=26 | 245 | macro GF_SSE_MATRIX_MULTIPLY |
| 0x00AE1320 | CParticleProgram::ClingToMesh | DIFFER(681v681) dl=47 | 41 | macro GF_SSE_MATRIX_MULTIPLY |
| 0x00AE29D0 | CParticleProgram::SetupSimulation | DIFFER(726v726) dl=42 | 32 | macro GF_SSE_MATRIX_MULTIPLY |
| 0x00ADF960 | CParticleSimulation::SatisfyDistanceConstraints | DIFFER(764v766) dl=9 | 27 | macro GF_SSE_SATISFY_DISTANCE_CONSTRAINTS |
| 0x00988E80 | CShaderRenderManager::UpdateLocalisedSphereMapping | DIFFER(889v917) dl=93 | 12 | macro GF_SSE_MATRIX_MULTIPLY |

Every SSE region of all eight rows is byte-exact with the macros. All parked residues are outside the SSE blocks (x87 fallback
order, one frame-slot order, one induction-pointer bias, one x87 tail): no helper/asm/pragma was used for them (owner rule).
Tests: every src/ and differ/ row has a .test.cpp that passes under g++ -DFABLE_PORTABLE and VC7.1 (asm path and
/DFABLE_PORTABLE). Row sources contain no `__asm`, `naked`, `_emit` or `volatile`.

## C++ / intrinsic attempts before helper (per SSE site; grades on the row's own address, VC7.1 RTM 3077 /O2 unless noted)

### Site 1 -- 0x00A9D5C9..0x00A9D5EC (AlignedPreScaleTransposed kernel; same block expanded at 0x00A9D7C1 in ApplyBoneMorphScaling)
Retail: `mov [esp],ecx` (C++ homes `this`), `mov edx,[esp]`, `movaps xmm3,[esp+10h]` (the aligned scale local), `movaps xmm0..2,[edx+0/10h/20h]`,
`mulps xmm0..2,xmm3`, `movaps [edx..],xmm0..2`; E14/E24/E34 saved around it through dword slots.
| spelling | flags | VC7.1 emitted | grade |
|---|---|---|---|
| plain C++ (12 float multiplies, aligned local) | /O2 | x87 only, no SSE | n/a (no SSE without intrinsics) |
| i1a: _mm_load_ps scale + 3 rows, _mm_mul_ps, _mm_store_ps (named temps) | /O2 | this kept in ecx, rows read [ecx], stores interleaved, own xmm numbering | 199v221 dl=43 |
| i2a: __m128 rows[3] array | /O2 | same shape | 199v221 dl=38 |
| i3a: _mm_set_ps(0,Z,Y,X) (no aligned local) | /O2 | builds the vector with unpcklps, no frame vector | 222v221 dl=46 |
| i4a: _mm_loadu_ps/_mm_storeu_ps | /O2 | movups, ecx-based | 199v221 dl=40 |
| i5a: load-mul-store per row in one expression | /O2 | closest; still ecx-based, xmm0/xmm1 reuse | 197v221 dl=36 |
| i6a / i7a: float* rows / scale pointer locals | /O2 | pointer locals folded into registers (never re-read from the frame) | 199v221 dl=39 |
| i8a: retail register names as variables (x0..x3) | /O2 | allocator renumbers | 199v221 dl=43 |
| i1b: __declspec(align(16)) union {__m128; float[4]} scale | /O2 | same as i1a | 199v221 dl=41 |
| i1a, i5a | /arch:SSE | the x87 arm also moves to SSE scalar code | 266v221 dl=60 |
| i1a, i5a | /arch:SSE2 | same | 266v221 dl=62 |
| /G5 /G6 /G7 /Oa /Ow /Ox /O1 /Op on the best form | | x87 operand order unchanged, SSE arm unchanged | no fix |
| __forceinline helper function with the asm (diag/hpre_fn.cpp) | /O2 | homes both params (`mov [esp+0Ch],eax`), re-reads the scale through a pointer `mov eax,[esp+0Ch] ; movaps xmm3,[eax]` | 237v221 dl=24 |
| **`__asm` macro GF_SSE_MATRIX_PRESCALE_TRANSPOSED naming the locals** | /O2 | retail block exactly | **RELOCATION_MATCH** |

### Site 2 -- 0x00A9D4CD..0x00A9D502 (AlignedPreScale kernel; expanded again at 0x00A9D8C5 in ApplyBoneMorphScaling)
Retail: as site 1 plus `movaps xmm4,xmm3 ; movaps xmm5,xmm3 ; shufps xmm3,xmm3,00h ; shufps xmm4,xmm4,55h (0x00A9D4EA) ; shufps xmm5,xmm5,0AAh`.
| spelling | flags | VC7.1 emitted | grade |
|---|---|---|---|
| int_i1: _mm_load_ps + _mm_shuffle_ps(v,v,0x00/0x55/0xAA) named temps | /O2 | ecx-based, shuffles interleaved with the multiplies | 226v243 dl=58 |
| int_i2: _mm_set1_ps(scale.X/Y/Z) broadcasts | /O2 | movss+shufps from the C3DVector, no aligned local | 226v243 dl=58 |
| int_i3: per-row expression | /O2 | | 221v243 dl=67 |
| int_i4: pointer locals | /O2 | | 220v243 dl=53 |
| int_i5: retail register-named temps | /O2 | closest | 245v243 dl=43 |
| int_i6: rows array | /O2 | | 220v243 dl=56 |
| int_i1 / int_i2 | /arch:SSE | x87 arm becomes scalar SSE | 297v243 dl=65 |
| **`__asm` macro GF_SSE_MATRIX_PRESCALE** | /O2 | retail block exactly (residue only in the x87 arm) | 243v243 dl=8 |
Helper-function form: same result as site 1 (parameter homing + pointer re-read) -- the macro is retail's form.

### Site 3 -- 0x0098929D..0x0098938A (CMatrix4x4::AlignedSetMultiply kernel inlined in 0x00989230; same block in 0x00988E80, 0x00AE1320, 0x00AE29D0)
Retail: the C++ homes the three operands (`mov [esp+2Ch],eax ; mov [esp+28h],ecx ; mov [esp+24h],edx`), then `mov eax,[esp+2Ch] ; mov edx,[esp+28h] ;
mov edi,[esp+24h]`, b's rows in xmm0..3 (0x009892A9 `movaps xmm0,[edx]`), per row of a: xmm4 copied to xmm5..7, shufps 00/55/AA/FF in that
order, four mulps, `addps xmm4,xmm5 ; addps xmm6,xmm7 ; addps xmm4,xmm6`, `movaps [edi+k],xmm4`.
| spelling | flags | VC7.1 emitted | grade (0x00989230) |
|---|---|---|---|
| plain C++ (16 dot products) | /O2 | x87 only | (that is the fallback arm) |
| int_i1: b rows in 4 __m128, per row _mm_shuffle_ps(x,x,00/55/AA/FF) + mul + pairwise add, unrolled | /O2 | reads members straight off `this` (`[esi+210h]`), no homing, shuffles in reverse order interleaved with loads | 684v636 dl=217 |
| int_i2: _mm_set1_ps(a.Data[r][k]) broadcasts | /O2 | movss+shufps per element | 863v636 dl=244 |
| int_i3: row loop (for r) | /O2 | loop kept, registers allocated | 535v636 dl=217 |
| int_i4: float* pointer locals + loop, retail statement order | /O2 | pointers in registers, never re-read | 535v636 dl=217 |
| int_i1 | /arch:SSE | | 775v636 dl=268 |
| int_i1 | /arch:SSE2 | | 773v636 dl=268 |
| int_i4 | /arch:SSE | | 626v636 dl=249 |
| __forceinline helper function with the asm (diag/hmul_fn.cpp) | /O2 | parameter stores hoisted above the g_SSEAvailable test | 636v636 dl=41 |
| **`__asm` macro GF_SSE_MATRIX_MULTIPLY in the inline member** | /O2 | retail block exactly | 636v636 dl=26 (x87 residue only) |

### Site 4 -- 0x00ADF9DD..0x00ADFAAE (`jne`) (SatisfyDistanceConstraints kernel)
Retail: `mov esi,[ebx+8]` (constraints argument through the realigned ebx frame), `mov edi,[ebp-..]`, `sqrtps xmm4,xmm4` (0x00ADFA3F),
`maxss xmm6,xmm5` (0x00ADFA4C), `push esi ; mov esi,[ebp-0Ch] ; mov ecx,[esi+eax] ; ... ; pop esi` (0x00ADFA57), `add esi,[ebp-10h]` (stride
from memory), `dec dword ptr [ebp-4]` (0x00ADFAAB, counter in a frame word).
| spelling | flags | VC7.1 emitted | grade |
|---|---|---|---|
| plain C++ | /Zi /O2 | x87 only (the fallback arm) | |
| int_i1: per-constraint _mm_set1_ps / _mm_load_ps / _mm_sqrt_ps / _mm_max_ss / _mm_max_ps / _mm_div_ps, pointer increment | /Zi /O2 | counter and stride in registers, alphas broadcast via movss+shufps | 819v766 dl=223 |
| int_i2: retail shape (aligned temp local filled with two floats then _mm_load_ps + shuffle 00/55, stride local, do/while --remaining) | /Zi /O2 | alpha copies go through x87 `fld/fstp`, ebx/esi allocated, no push/pop, counter in a register | 831v766 dl=194 |
| int_i1 | /Zi /arch:SSE | | 765v766 dl=307 |
| int_i2 | /Zi /arch:SSE | | 868v766 dl=313 |
| int_i2 | /Zi /arch:SSE2 | | 862v766 dl=325 |
| __forceinline helper function with the asm (diag/hsat_fn.cpp) | /Zi /O2 | own aligned locals and parameter slots | 764v766 dl=22 |
| **`__asm` macro GF_SSE_SATISFY_DISTANCE_CONSTRAINTS naming the locals** | /Zi /O2 | retail block exactly | 764v766 dl=9 (x87 loop residue only) |

Why intrinsics cannot reach any site: VC7.1 never re-reads a pointer it just stored (retail's `mov [esp],ecx ; mov edx,[esp]`), never keeps a
loop counter in memory with `dec dword ptr`, never pushes a live register around a load, and numbers xmm registers itself.
Those are the inline-asm frame contract (asm operands come from their home slots), so retail ran hand-written blocks.

## New shared helpers / macros (hdr/, both NEW files)
- `engine/CMatrix4x4AlignedSSE.h`: GF_SSE_MATRIX_PRESCALE_TRANSPOSED(matrix, scale), GF_SSE_MATRIX_PRESCALE(matrix, scale),
  GF_SSE_MATRIX_MULTIPLY(dst, a, b). `__asm` by default, fable_portable::AlignedRowsTimesVector / AlignedRowsTimesBroadcast /
  AlignedMatrixMultiply under FABLE_PORTABLE. `// Retail-asm:` 0x00A9D5C9 / 0x00A9D5CC / 0x00A9D5DC, 0x00A9D4EA, 0x00A9D7C1 / 0x00A9D8C5,
  0x009892A9. Dual-path test tests/portable/test_CMatrix4x4AlignedSSE.cpp: VC asm PASS, VC /DFABLE_DUAL_PATH_TEST (asm + twin) PASS,
  VC /DFABLE_PORTABLE PASS, g++ -std=c++03 -DFABLE_PORTABLE -Wall -Wextra -pedantic PASS; header alone compiles under g++ portable.
- `engine/CParticleSimulationSSE.h`: GF_SSE_SATISFY_DISTANCE_CONSTRAINTS(constraints, positions, alphas, stride, count, temp, epsilon,
  maxadj) (label gf_sse_distance_constraint_next: one expansion per function), fable_portable::SatisfyDistanceConstraintsSSE.
  `// Retail-asm:` 0x00ADF9DD .. 0x00ADFAAB. Dual-path test tests/portable/test_CParticleSimulationSSE.cpp (3-4-5 / axis pairs, zero
  alphas, self pair, irrational stretch): VC asm PASS, VC dual PASS (also /Od), VC portable PASS, g++ portable -pedantic PASS.
  (32-bit indices in the twin: `unsigned long` is 8 bytes on LP64 g++.)

## Header overlay (hdr/, default=same for every CHANGED file per hdrcheck2)
- `engine/CMatrix4x4.h` (FABLE_RETAIL_CMATRIX4X4): declares AlignedPreScale / AlignedPreScaleTransposed and
  `__forceinline AlignedSetMultiply(a, b)`, and includes CMatrix4x4AlignedSSE.h after the class. The inline AlignedSetMultiply body
  (SSE arm = GF_SSE_MATRIX_MULTIPLY on homed pointer locals, x87 arm = row/column loop; `extern bool g_SSEAvailable` 0x013D2880)
  lives in CMatrix4x4AlignedSSE.h, outside its include guard, so `crawl/purity.py` sees rows that call it as calling a declared
  retail-asm helper (its `// Retail-asm-helper:` list names the three macros and AlignedSetMultiply).
- `engine/CParticleProgram.h` (FABLE_RETAIL_CPARTICLEPROGRAM): SetupSimulation (public const, 7 args), private statics __fastcall
  GetMatrixBuffer / GetVectorBuffer, TransformVertices, ApplyConstraints(CParticleSimulation&, long, float) const, ClingToMesh (5 args)
  const, PeekSimGroups(). Landed siblings 0x00AE0070 / 0x00AE0710 still RELOCATION_MATCH with the overlay (SIBLINGS.tsv).
- `engine/CParticleSimulation.h` (FABLE_RETAIL_CPARTICLESIMULATION): CopyFrom(const CParticleSimulation&, CMovableResourceMemoryPool&)
  (0x00ADF610) and private SatisfyDistanceConstraints(const CParticleDistanceConstraint*, unsigned long).

## Purity / vland
`FABLE_HDR_OVERLAY=$D/mirror purity.classify`: src/00a9d580.cpp and all seven differ rows = `retail_asm` (each row carries its own
`// Retail-asm:` line naming the retail block's addresses and instructions). Read-only vland (vland_result.txt): 00a9d580
RELOCATION_MATCH, behaviour PASS, WINS 1.

## Parked residues and next ideas
- 0x00A9D480 (dl=8): x87 fld/fmul operand order of four products (retail swaps only E12). Searched 125 + 729 row-form combinations
  (unrolled / column loop / row pointer per row) and 120 prologue orders: max 8 of 9. Next: forms that change the function-wide
  value numbering (a const C3DVector& copy of scale, reading scale before the SSE test, a do/while row loop).
- 0x00A9D750 (dl=24): same x87 family twice, plus the second inline copy homes self/t0..t2 in reverse slot order. Next: carry the
  0x00A9D480 fix once found; try declaring the AlignedPreScale copy's locals in the other order inside the inline member.
- 0x00989230 / 0x00AE1320 / 0x00AE29D0 (dl 26 / 47 / 42): only the AlignedSetMultiply x87 arm's term order (retail sums columns
  3,1,2,0 / 3,2,1,0, column 2 differently). Single-expression forms are canonicalised to one order; sequenced forms over 96 + 96 + 144
  permutations/parenthesisations and a joint search found nothing better. Next: the real FableWin inline body may be a CMatrix4x4
  operator* / SetMultiply call (non-aligned twin) -- check FableWin.pdb inline lines for 0x00989230 and reuse that member's landed body.
- 0x00ADF960 (dl=9, 764v766): the x87 loop's constraint pointer is biased +4 (Index2) where retail biases +8 (Distance). Tried index,
  pointer, count-down and macro-access loops. Next: read c.Distance first in the loop body, or walk `&constraints->Distance`.
- 0x00988E80 (dl=93, 889v917): x87 tail (two fast-inverse-sqrt normalisations, 0.5 scale/bias) and the AlignedSetMultiply slot
  order (retail b/this/a at 8/0Ch/10h). Only 12 attempts -- the most room left; the old draft's FastOneOverSqrt form is in p/e80.

## GOTCHAS one-liners
- `// Retail-asm-helper:` takes identifiers (split on spaces/commas): list the macro / inline names, never a description.
- purity.py only sees helpers whose name the row spells: an inline member that expands an asm macro must sit in the helper header and be listed.
- VC7.1 inline-asm macros: one `__asm` per line with `\` continuation; a label inside the macro is fine but only one expansion per function.
- An `__asm` operand naming a C local reads its frame slot; a `__forceinline` helper parameter gets its own slot and a pointer re-read -- use a macro when retail re-reads a homed pointer.
- `xmmintrin.h` includes `xmm_func.h` only under `__ICL`: include checkers that flag it missing are false positives.
- Portable twins of 32-bit layouts: use `unsigned int` for 32-bit fields (`unsigned long` is 8 bytes on LP64 g++).
- VC7.1 x87 `E *= s` operand order follows function-wide value numbering, not spelling; change loop structure, not expressions.
- Using the real retail class header (FABLE_RETAIL_<CLASS>) instead of a layout cast can fix ebx/edi register swaps.

## Retail evidence that the SSE arms are hand-written asm (every row)
- The matrix pointer is homed to a frame word by the C++ before the block and re-read into a fixed register inside it
  (0x00A9D5BE `mov [esp],ecx` .. 0x00A9D5C9 `mov edx,[esp]`; 0x00A9D7A1 `mov [esp+10h],edx` .. 0x00A9D7C1 `mov edx,[esp+10h]`;
  0x009892A5 `mov edi,[esp+24h]`; 0x00AE13D2 `mov edi,[esp+18h]`). VC7.1 never emits a store immediately re-loaded into
  another register for a C++ pointer local; it is the inline-asm frame contract (asm operands are read from their home slots).
- Fixed xmm numbering that no allocation would choose (scale in xmm3 loaded first, rows in xmm0..2, products in place).
- Values live across the block (the saved fourth column) travel through dword frame slots, never x87 / registers.

## x87 commutative operand order (the non-SSE residue of every row)
VC7.1 picks which operand of `E *= s` (or a product term) it `fld`s from function-wide load-node / value-number order, not
from source spelling: every per-element spelling (`*=`, `E = E * s`, `E = s * E`, E names vs Data[][] vs this->) of an
unrolled body gives identical code (search580c: 72 random mixes, all one result). What moves it is STRUCTURE: loops vs
straight-line statements per row, and C++ memory references elsewhere in the function (xmmintrin loads/stores of the rows in
the SSE arm flip it; an `__asm` block, an empty `__asm{}` and dead reads do not). /G5 /G6 /G7 /Oa /Ow /Ox /O1 /Op do not fix it.
Matched by search, not by theory: 0x00A9D580 needs its third row as a column loop (125-combination row-form search).
