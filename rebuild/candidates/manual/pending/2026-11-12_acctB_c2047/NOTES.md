# Hand chunk 2047 (C3DAnimationManager .. C3DAnimationManager): 1 of 1 byte-exact

## What matched (1)
- 00a9e1e0 C3DAnimationManager::ApplyParentTransformations -- RELOCATION_MATCH (2472 bytes) at the THIRD full grade (5 grades in all,
  pass 1 only; the 4th is the documented final source). Per bone: parent index through GetBone(i)->GetParent(); a root converts its
  keyframe straight into matrices[i]; other bones convert into a 16-byte-aligned local and concatenate with the parent's matrix through the
  inline CMatrix4x4::AlignedSetMultiply (SSE kernel GF_SSE_MATRIX_MULTIPLY when g_SSEAvailable, else the x87 row loop). `transpose` picks
  ToMatrixTransposed + AlignedSetMultiplyTransposed(local, parent) (= parent * local) or ToMatrix + AlignedSetMultiply(local, parent).
  SHAPE lessons: (1) the inline bodies are proven against retail's uncalled out-of-line copies 0x00AA39A0 / 0x00AA3AF0 (free q.py);
  (2) the CQuaternion 4x4 conversions, the C3DKeyframe wrappers and AlignedSetMultiplyTransposed are FABLE_FORCEINLINE (Inline-budget);
  (3) AlignedSetMultiply's x87 arm reads through the CMatrix4x4 element accessors (FableWin's COMDAT order) -- this alone made the
  transposed x87 loop retail's; (4) `unsigned long i;` at function scope (one counter for both loops) fixed the row-vector loop's
  induction-pointer base and three of its four column orders; (5) a named `const C3DBone2* bone = mesh.GetBone(i);` (two more named
  symbols, the r3004 mod-8 lever measured with a dead-local diagnostic: k = 2, 6, 7 mod 8 all fix it) fixed the last column.

## Grades (full-row compiles diffed against retail) -- 5 used, pass 1
1. draft (plain inline everything, old `v +=` x87 arm, per-loop `for (unsigned long i ...)`): DIFFER(845v2472): the four quaternion
   conversions and AlignedSetMultiplyTransposed stayed calls (c2 scores 283 / 286 / 565, threshold 74).
2. + `/d2inlT566`: DIFFER(2473v2472), 699/742 instructions equal: everything (both ToMatrix expansions, the SSE kernels, hoisting, frame)
   equal except the two x87 AlignedSetMultiply loops' column term orders and the row-vector loop's induction base (`lea eax,[esp+54h]` v
   retail `[esp+58h]`).
3. accessor x87 arm + forced inlines (no flag) + `unsigned long i` at function scope + named `bone`: RELOCATION_MATCH.
4. final documented source (comments only): RELOCATION_MATCH (gall.sh), vland read-only: RELOCATION_MATCH, behaviour PASS, no reloc issue.
5. one comment wording fix in the final file: RELOCATION_MATCH (gall.sh); tall.sh + tall.py PASS; finish.py rerun.

## Free measurements (probe compiles and c2 traces, no retail diff of the row; ~70 probe compiles in all)
- tools/feat.py: extracts the two x87 loops of an object (from `mov edi,4` to the loop's `jne`) and compares them with retail's loops
  (relocations masked) -- the screen used between grades 2 and 3. tools/scr.sh compiles + screens a list of probes.
- probe/v/b.cpp (accessor body, /d2inlT593) and probe/v/c.cpp (same, forced opt-ins, no flag): byte-identical objects; loop0 (transposed)
  EQUAL, loop1 23/51.
- probe/s1 (11 natural variants on c.cpp): direct member reads, local at loop scope, named keyframe / matrix / parent-matrix references,
  `parent >= 0` arms swapped, named bone, count in the for test, `long i`: loop1 never equal; `iout` (`unsigned long i;` at function scope):
  size 2472 exact, loop1 45/51 (columns 1..3 retail, induction base retail).
- probe/diag (DIAGNOSTIC dead `unsigned long ddN = bone_count * N;` locals, never delivered): on c.cpp no k in 1..32 fixes loop1; on iout
  k = 2, 6, 7, 10, 14, 15 give both loops EQUAL (mod-8 pattern {2, 6, 7}) -> two more natural named symbols needed.
- probe/s2 (natural two-name variants on iout): `bone` (named const C3DBone2*) and `pref` (named const CMatrix4x4& parent_matrix in each
  else arm) both EQUAL / 2472 bytes; `kfref`, `mref`, function-scope `parent` / `local` not. `bone` was graded (grade 3).
- Inline trace (tools/inl.sh, agent2003's c2 hook): plain CQuaternion::ToMatrix[Transposed] 283 / 286; forced, the C3DKeyframe wrappers
  297..313; AlignedSetMultiplyTransposed 565 (old body) / 592 (accessor body), c1 431 / 432 (under the 450 cap).
- Out-of-line body checks (probe/ool): C3DKeyframe::ToMatrixTransposed v 0x00AA3AF0 and ::ToMatrix v 0x00AA39A0: RELOCATION_MATCH with
  forced conversions and no flag (plain: DIFFER 43 bytes, i.e. call; RELOCATION_MATCH with /d2inlT308).
- /Od instruction counts v FableWin.exe.signatures.tsv (tools/icount.py; FableWin = plain /Od, calibrated: GetParent 9/17 B, GetBoneCount
  9/20 B, C3DKeyframe::ToMatrix 16/40 B all exact): AlignedSetMultiplyTransposed 13/29 B exact; AlignedSetMultiply FableWin 179 / 567 B,
  accessor body 168 / 532 B, old `v +=` body 148, single-expression Data form 143, k-loop `+=` 145; CQuaternion::ToMatrix 4x4 FableWin 228
  v 216 (FableWin's GFSqr is 10 instr v our 6: a later revision); ApplyParentTransformations FableWin 278 v ours 111 (FableWin GetBone is
  94 instr with a debug check v 11 here: two such checks would account for the difference -- weak evidence, not used).
- Cross-check of the accessor body on the chunk-748 parked AlignedSetMultiply users (repo headers + only this CMatrix4x4 / AlignedSSE
  overlay, tools/xcmp.py v Fable.exe bytes, not grades of my row): 0x00989230 174 -> 177 equal instructions, 0x00AE1320 186 -> 195,
  0x00AE29D0 212 -> 216, 0x00988E80 206 -> 207 -- better in all four, none matched (their remaining column orders follow their own
  record counts, the lever that closed this row).
- prior/00a9f540.cpp regraded under this overlay: RELOCATION_MATCH.

## Names and evidence
- Row: Ego_r.pdb 0x00BE1D40 + FableWin.pdb 0x033825F0 (private const ABE, four arguments = `ret 10h`); callers 0x00A9FE01 (0x00A9F540)
  and 0x00AA09B7 (0x00AA0090). Declaration = the chunk-2044 overlay C3DAnimationManager.h MESHTRANSFORMS one.
- Inline helpers (PDB names, two sources each): C3DMesh2::GetBoneCount / GetBone (Ego_r + FableWin), C3DBone2::GetParent (Ego_r + FableWin,
  retail COMDAT 0x00A4BD30), C3DKeyframe::ToMatrix / ToMatrixTransposed (4x4) and CQuaternion::ToMatrix / ToMatrixTransposed (4x4)
  (FableWin COMDAT order + retail out-of-line copies), C3DKeyframe::GetPosition, CMatrix4x4::AlignedSetMultiply /
  AlignedSetMultiplyTransposed / operator() (FableWin 0x02E8E150 / 0x02E8E180 / 0x02E8E450 / 0x02E8E480), GFSqr.
- Global: g_SSEAvailable 0x013D2880 (bound spelling). Float pool 0.0f / 1.0f / 2.0f (relocation pairing checked by hand: every relocation
  of the row maps to retail's operand).
- Descriptive (made up): parameter names keyframes / matrices / mesh / transpose (the PDB sets carry none) and locals bone / parent / local.
- NAME_FIXES.tsv: 0x00AA3AF0 is ?ToMatrixTransposed@C3DKeyframe@@QBEXAAVCMatrix4x4@@_N@Z (landed as ?ToMatrix, the same name as 0x00AA39A0).

## Header debt / review
- hdr/README.md lists every opt-in. AlignedSetMultiply's accessor x87 arm is an opt-in; it improves all four chunk-748 users and is
  the candidate default body (one body for all rows) -- coordinator to decide at landing.
- CQuaternion 4x4 conversions forced (opt-in): landed 0x00AA39A0 matches with /d2inlT308 today; with the forced opt-in it matches with
  no flag (measured), so the flag could be dropped if the opt-in is folded.
- `__declspec(align(16)) CMatrix4x4 local;` follows the landed precedent (0x00ADF300 / 0x00ADF760, chunk 748 rows); CMatrix4x4 itself is
  not 16-aligned in retail (0x00B4FC40 has CMatrix4x4 locals without an aligned frame).

## Parked for Fable 5.1 (0)
None.

## Skipped
None. The row's extent is right: 0x00A9E1E0 `push ebp` .. 0x00A9EB85 `ret 10h`, next function 0x00A9EB90 (BlendAnimations).

## Label corrections
- 0x00AA3AF0 catalogued / landed as C3DKeyframe::ToMatrix: it is C3DKeyframe::ToMatrixTransposed (NAME_FIXES.tsv).
- 0x00AA39A0 manifest parameter list (CQuaternion*, CMatrix3x4*, C3DVector*) is wrong: (CMatrix4x4&, bool) const.

## C++ attempts before helper (ASM_RULES rule 4: 30+ plain C++, then 10+ intrinsics, per site)
Sites: A = 0x00A9E528..0x00A9E615 (transposed loop: dst = matrices[i], a = matrices[parent], b = the aligned local from
ToMatrixTransposed) and B = 0x00A9E9CC..0x00A9EAB9 (row-vector loop: a = the local from ToMatrix, b = matrices[parent]); both are
retail's `mov eax,[esp+N] ; mov edx,[esp+N] ; mov edi,[esp+N]` re-read of the pointers just stored, then the 71-instruction
movaps / shufps 00,55,AA,FF / mulps / addps tree / movaps block with fixed xmm0..7. Harness (free fragment probes, no grades):
probe/asm/gen.py writes spell.cpp, one fragment per spelling per site fed exactly that site's inputs (the local built by the real
inline conversion); probe/asm/run.py compiles it with VC7.1 RTM 13.10.3077 under the row's flags (/O2 /Oy /W3) and + /arch:SSE,
/arch:SSE2 (res_base / res_sse / res_sse2.tsv) and records, per fragment minus the site's memcpy baseline, the x87 / packed / scalar SSE
counts, the xmm registers used, whether the three homed pointers are re-read, and whether retail's whole block appears (frame offsets
masked); probe/asm/table.py writes table.md (below). Positive control c00 (the shared macro itself, not a spelling) is detected as
RETAIL KERNEL at both sites under the row's flags. Result: 35 plain C++ spellings x 2 sites x 3 flag sets: x87 (or scalar SSE under
/arch:SSE / SSE2) only, never a packed instruction; 13 intrinsic spellings x 2 x 3: packed SSE with the compiler's own xmm numbering and
scheduling (8..28 movaps per fragment), never re-reading the homed pointers; none gives retail's block. So the shared helper stays
(ASM_RULES 3.2). (The chunk-748 site-3 table for the same kernel, pending/2026-11-04_linux_hand748/NOTES.md, has ~10 spellings; this
table covers both of this row's sites in full.)

| # | spelling | site A 0x00A9E528, /O2 /Oy /W3 (row flags) | site B 0x00A9E9CC, row flags | site A + /arch:SSE | site A + /arch:SSE2 |
|---|---|---|---|---|---|
| c00 | CONTROL (not a spelling): the shared GF_SSE_MATRIX_MULTIPLY macro on homed pointers, as the row | +252 bytes; 12 addps, 24 movaps, 16 mulps, 16 shufps, 1 homed pointers re-read, 1 xmm regs 01234567 -> **RETAIL KERNEL** | +252 bytes; 12 addps, 24 movaps, 16 mulps, 16 shufps, 1 homed pointers re-read, 1 xmm regs 01234567 -> **RETAIL KERNEL** | +252 bytes; 12 addps, 24 movaps, 16 mulps, 16 shufps, 1 xmm regs 01234567 -> retail xmm block but no pointer re-read | +254 bytes; 12 addps, 24 movaps, 16 mulps, 16 shufps, 1 xmm regs 01234567 -> retail xmm block but no pointer re-read |
| i00 | retail kernel instruction for instruction (load b rows, copy x4 to x5..x7, shufps 00/55/AA/FF, mulps, add tree, store), unrolled | +247 bytes; 12 addps, 28 movaps, 16 mulps, 16 shufps, 1 xmm regs 01234567 -> not retail | +249 bytes; 12 addps, 28 movaps, 16 mulps, 16 shufps, 1 xmm regs 01234567 -> not retail | +247 bytes; 12 addps, 28 movaps, 16 mulps, 16 shufps, 1 xmm regs 01234567 -> not retail | +249 bytes; 12 addps, 28 movaps, 16 mulps, 16 shufps, 1 xmm regs 01234567 -> not retail |
| i01 | same kernel in a row loop | +91 bytes; 3 addps, 10 movaps, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail | +89 bytes; 3 addps, 10 movaps, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail | +90 bytes; 3 addps, 10 movaps, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail | +94 bytes; 3 addps, 10 movaps, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail |
| i02 | _mm_set1_ps broadcasts of a's elements | +142 bytes; 3 addps, 8 movaps, 4 movss, 4 mulps, 4 shufps, 1 xmm regs 0123456 -> not retail | +135 bytes; 3 addps, 8 movaps, 4 movss, 4 mulps, 4 shufps, 1 xmm regs 0123456 -> not retail | +117 bytes; 3 addps, 8 movaps, 4 movss, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail | +121 bytes; 3 addps, 8 movaps, 4 movss, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail |
| i03 | _mm_load_ps1 broadcasts | +108 bytes; 3 addps, 8 movaps, 4 movss, 4 mulps, 4 shufps, 1 xmm regs 0123456 -> not retail | +102 bytes; 3 addps, 8 movaps, 4 movss, 4 mulps, 4 shufps, 1 xmm regs 0123456 -> not retail | +117 bytes; 3 addps, 8 movaps, 4 movss, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail | +121 bytes; 3 addps, 8 movaps, 4 movss, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail |
| i04 | unaligned _mm_loadu_ps / _mm_storeu_ps kernel | +247 bytes; 12 addps, 16 movaps, 12 movups, 16 mulps, 16 shufps, 1 xmm regs 01234567 -> not retail | +249 bytes; 12 addps, 16 movaps, 12 movups, 16 mulps, 16 shufps, 1 xmm regs 01234567 -> not retail | +247 bytes; 12 addps, 16 movaps, 12 movups, 16 mulps, 16 shufps, 1 xmm regs 01234567 -> not retail | +249 bytes; 12 addps, 16 movaps, 12 movups, 16 mulps, 16 shufps, 1 xmm regs 01234567 -> not retail |
| i05 | operand pointers homed in locals (pa / pb / pd), kernel through them | +247 bytes; 12 addps, 28 movaps, 16 mulps, 16 shufps, 1 xmm regs 01234567 -> not retail | +249 bytes; 12 addps, 28 movaps, 16 mulps, 16 shufps, 1 xmm regs 01234567 -> not retail | +247 bytes; 12 addps, 28 movaps, 16 mulps, 16 shufps, 1 xmm regs 01234567 -> not retail | +249 bytes; 12 addps, 28 movaps, 16 mulps, 16 shufps, 1 xmm regs 01234567 -> not retail |
| i06 | union { __m128; float[4] } rows | +187 bytes; 3 addps, 10 movaps, 4 movss, 4 mulps, 4 shufps, 1 xmm regs 0123456 -> not retail | +174 bytes; 3 addps, 10 movaps, 4 movss, 4 mulps, 4 shufps, 1 xmm regs 0123456 -> not retail | +134 bytes; 3 addps, 10 movaps, 4 movss, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail | +138 bytes; 3 addps, 10 movaps, 4 movss, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail |
| i07 | linear add chain ((m0 + m1) + m2) + m3 | +93 bytes; 3 addps, 11 movaps, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail | +92 bytes; 3 addps, 11 movaps, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail | +104 bytes; 3 addps, 11 movaps, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail | +108 bytes; 3 addps, 11 movaps, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail |
| i08 | _MM_SHUFFLE macro selectors, one expression per row | +91 bytes; 3 addps, 10 movaps, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail | +89 bytes; 3 addps, 10 movaps, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail | +90 bytes; 3 addps, 10 movaps, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail | +94 bytes; 3 addps, 10 movaps, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail |
| i09 | chained copies x4 = x5 = x6 = x7 = load | +91 bytes; 3 addps, 10 movaps, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail | +89 bytes; 3 addps, 10 movaps, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail | +90 bytes; 3 addps, 10 movaps, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail | +94 bytes; 3 addps, 10 movaps, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail |
| i10 | unpack-based broadcasts (_mm_unpacklo_ps / _mm_movelh_ps / _mm_movehl_ps) | +96 bytes; 3 addps, 11 movaps, 2 movhlps, 2 movlhps, 4 mulps, 1 unpckhps, 1 unpcklps, 1 xmm regs 01234567 -> not retail | +94 bytes; 3 addps, 11 movaps, 2 movhlps, 2 movlhps, 4 mulps, 1 unpckhps, 1 unpcklps, 1 xmm regs 01234567 -> not retail | +95 bytes; 3 addps, 11 movaps, 2 movhlps, 2 movlhps, 4 mulps, 1 unpckhps, 1 unpcklps, 1 xmm regs 01234567 -> not retail | +99 bytes; 3 addps, 11 movaps, 2 movhlps, 2 movlhps, 4 mulps, 1 unpckhps, 1 unpcklps, 1 xmm regs 01234567 -> not retail |
| i11 | non-temporal stores _mm_stream_ps | +247 bytes; 12 addps, 24 movaps, 4 movntps, 16 mulps, 16 shufps, 1 xmm regs 01234567 -> not retail | +249 bytes; 12 addps, 24 movaps, 4 movntps, 16 mulps, 16 shufps, 1 xmm regs 01234567 -> not retail | +247 bytes; 12 addps, 24 movaps, 4 movntps, 16 mulps, 16 shufps, 1 xmm regs 01234567 -> not retail | +249 bytes; 12 addps, 24 movaps, 4 movntps, 16 mulps, 16 shufps, 1 xmm regs 01234567 -> not retail |
| i12 | b rows reloaded per row (no hoisted x0..x3) | +91 bytes; 3 addps, 10 movaps, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail | +89 bytes; 3 addps, 10 movaps, 4 mulps, 4 shufps, 1 xmm regs 01234 -> not retail | +90 bytes; 3 addps, 10 movaps, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail | +94 bytes; 3 addps, 10 movaps, 4 mulps, 4 shufps, 1 xmm regs 01234567 -> not retail |
| p00 | baseline: the local copied into D with memcpy (keeps the conversion alive; subtracted from every count) | +0 bytes; nothing beyond the baseline -> not retail | +0 bytes; nothing beyond the baseline -> not retail | +0 bytes; 1 xmm regs 01234567 -> not retail | +0 bytes; 1 xmm regs 01234567 -> not retail |
| p01 | nested r/c loops, one 4-term sum via Data | +94 bytes; 1 x87 fadd, 11 x87 faddp, 13 x87 fld, 15 x87 fmul, 2 x87 fstp -> not retail | +156 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +187 bytes; 12 addss, 1 movaps, 10 movss, 15 mulss, 1 xmm regs 01234567 -> not retail | +191 bytes; 12 addss, 1 movaps, 10 movss, 15 mulss, 1 xmm regs 01234567 -> not retail |
| p02 | nested loops through the element accessors (the row's x87 arm) | +94 bytes; 1 x87 fadd, 11 x87 faddp, 13 x87 fld, 15 x87 fmul, 2 x87 fstp -> not retail | +156 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +181 bytes; 12 addss, 5 movaps, 5 movss, 15 mulss, 1 xmm regs 01234567 -> not retail | +185 bytes; 12 addss, 5 movaps, 5 movss, 15 mulss, 1 xmm regs 01234567 -> not retail |
| p03 | v += chain per element | +94 bytes; 1 x87 fadd, 11 x87 faddp, 13 x87 fld, 15 x87 fmul, 2 x87 fstp -> not retail | +156 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +187 bytes; 12 addss, 1 movaps, 10 movss, 15 mulss, 1 xmm regs 01234567 -> not retail | +191 bytes; 12 addss, 1 movaps, 10 movss, 15 mulss, 1 xmm regs 01234567 -> not retail |
| p04 | pairwise sums (p0+p1)+(p2+p3) as the SSE add tree | +94 bytes; 1 x87 fadd, 11 x87 faddp, 13 x87 fld, 15 x87 fmul, 2 x87 fstp -> not retail | +156 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +187 bytes; 12 addss, 1 movaps, 10 movss, 15 mulss, 1 xmm regs 01234567 -> not retail | +191 bytes; 12 addss, 1 movaps, 10 movss, 15 mulss, 1 xmm regs 01234567 -> not retail |
| p05 | k-outer broadcast into a row buffer (the kernel's algorithm) | +121 bytes; 4 x87 faddp, 8 x87 fld, 4 x87 fmul, 4 x87 fstp -> not retail | +119 bytes; 4 x87 faddp, 8 x87 fld, 4 x87 fmul, 4 x87 fstp -> not retail | +147 bytes; 4 addss, 8 movaps, 9 movss, 4 mulss, 1 xmm regs 01234567 -> not retail | +153 bytes; 4 addss, 8 movaps, 9 movss, 4 mulss, 1 xmm regs 01234567 -> not retail |
| p06 | broadcast form, row buffer seeded by k = 0 | +114 bytes; 4 x87 faddp, 7 x87 fld, 8 x87 fmul, 3 x87 fstp -> not retail | +132 bytes; 4 x87 faddp, 8 x87 fld, 8 x87 fmul, 4 x87 fstp -> not retail | +195 bytes; 4 addss, 8 movaps, 14 movss, 8 mulss, 1 xmm regs 01234567 -> not retail | +201 bytes; 4 addss, 8 movaps, 14 movss, 8 mulss, 1 xmm regs 01234567 -> not retail |
| p07 | fully unrolled 16 statements over E11..E44 | +482 bytes; 4 x87 fadd, 44 x87 faddp, 55 x87 fld, 60 x87 fmul, 11 x87 fstp -> not retail | +476 bytes; 4 x87 fadd, 44 x87 faddp, 55 x87 fld, 60 x87 fmul, 11 x87 fstp -> not retail | +835 bytes; 48 addss, 4 movaps, 68 movss, 60 mulss, 1 xmm regs 01234567 -> not retail | +837 bytes; 48 addss, 4 movaps, 68 movss, 60 mulss, 1 xmm regs 01234567 -> not retail |
| p08 | float* pointer walk | +103 bytes; 1 x87 fadd, 11 x87 faddp, 13 x87 fld, 15 x87 fmul, 2 x87 fstp -> not retail | +165 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +196 bytes; 12 addss, 11 movss, 15 mulss, 1 xmm regs 01234567 -> not retail | +200 bytes; 12 addss, 11 movss, 15 mulss, 1 xmm regs 01234567 -> not retail |
| p09 | temporary result matrix, then *this = result | +107 bytes; 4 x87 fadd, 8 x87 faddp, 11 x87 fld, 13 x87 fmul, 3 x87 fstp -> not retail | +168 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +188 bytes; 12 addss, 9 movss, 13 mulss, 1 xmm regs 01234567 -> not retail | +192 bytes; 12 addss, 9 movss, 13 mulss, 1 xmm regs 01234567 -> not retail |
| p10 | per-row float out[4] then copy | +97 bytes; 4 x87 fadd, 8 x87 faddp, 11 x87 fld, 13 x87 fmul, 3 x87 fstp -> not retail | +175 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +194 bytes; 12 addss, 12 movss, 13 mulss, 1 xmm regs 01234567 -> not retail | +198 bytes; 12 addss, 12 movss, 13 mulss, 1 xmm regs 01234567 -> not retail |
| p11 | reversed loops (r, c from 3 down) | +94 bytes; 1 x87 fadd, 11 x87 faddp, 13 x87 fld, 15 x87 fmul, 2 x87 fstp -> not retail | +156 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +188 bytes; 12 addss, 11 movss, 15 mulss, 1 xmm regs 01234567 -> not retail | +192 bytes; 12 addss, 11 movss, 15 mulss, 1 xmm regs 01234567 -> not retail |
| p12 | column-outer loop order | +158 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +114 bytes; 1 x87 fadd, 11 x87 faddp, 14 x87 fld, 15 x87 fmul, 3 x87 fstp -> not retail | +244 bytes; 12 addss, 20 movss, 16 mulss, 1 xmm regs 01234567 -> not retail | +248 bytes; 12 addss, 20 movss, 16 mulss, 1 xmm regs 01234567 -> not retail |
| p13 | double accumulation | +180 bytes; 1 x87 fadd, 11 x87 faddp, 23 x87 fld, 15 x87 fmul, 13 x87 fstp -> not retail | +160 bytes; 12 x87 faddp, 20 x87 fld, 16 x87 fmul, 8 x87 fstp -> not retail | +219 bytes; -2 movss, 1 x87 fadd, 11 x87 faddp, 27 x87 fld, 15 x87 fmul, 16 x87 fstp, 1 xmm regs 01234567 -> not retail | +223 bytes; -2 movss, 1 x87 fadd, 11 x87 faddp, 27 x87 fld, 15 x87 fmul, 16 x87 fstp, 1 xmm regs 01234567 -> not retail |
| p14 | C4DVector rows: row(b,k) * a + ... (inline operator* / operator+) | +1104 bytes; 12 x87 fadd, 32 x87 fld, 16 x87 fmul, 33 x87 fstp -> not retail | +1314 bytes; 12 x87 fadd, 33 x87 fld, 16 x87 fmul, 33 x87 fstp -> not retail | +1279 bytes; 12 addss, 6 movaps, 77 movss, 16 mulss, 1 xmm regs 01234567 -> not retail | +1283 bytes; 12 addss, 6 movaps, 77 movss, 16 mulss, 1 xmm regs 01234567 -> not retail |
| p15 | result built in a local, memcpy into D | +107 bytes; 4 x87 fadd, 8 x87 faddp, 11 x87 fld, 13 x87 fmul, 3 x87 fstp -> not retail | +168 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +188 bytes; 12 addss, 9 movss, 13 mulss, 1 xmm regs 01234567 -> not retail | +192 bytes; 12 addss, 9 movss, 13 mulss, 1 xmm regs 01234567 -> not retail |
| p16 | row reference const float (&ar)[4] | +94 bytes; 1 x87 fadd, 11 x87 faddp, 13 x87 fld, 15 x87 fmul, 2 x87 fstp -> not retail | +155 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +183 bytes; 12 addss, 3 movaps, 7 movss, 15 mulss, 1 xmm regs 01234567 -> not retail | +187 bytes; 12 addss, 3 movaps, 7 movss, 15 mulss, 1 xmm regs 01234567 -> not retail |
| p17 | down-counting do/while | +80 bytes; 3 x87 faddp, 4 x87 fld, 4 x87 fmul, 1 x87 fstp -> not retail | +85 bytes; 3 x87 faddp, 4 x87 fld, 4 x87 fmul, 1 x87 fstp -> not retail | +97 bytes; 3 addss, 5 movss, 4 mulss, 1 xmm regs 01234567 -> not retail | +103 bytes; 3 addss, 5 movss, 4 mulss, 1 xmm regs 01234567 -> not retail |
| p18 | b's 16 floats loaded into named locals first | +533 bytes; 16 x87 fadd, 32 x87 faddp, 59 x87 fld, 52 x87 fmul, 28 x87 fstp -> not retail | +618 bytes; 16 x87 fadd, 32 x87 faddp, 46 x87 fld, 52 x87 fmul, 15 x87 fstp -> not retail | +940 bytes; 48 addss, 12 movaps, 78 movss, 52 mulss, 1 xmm regs 01234567 -> not retail | +942 bytes; 48 addss, 12 movaps, 78 movss, 52 mulss, 1 xmm regs 01234567 -> not retail |
| p19 | b transposed into a local, then row dot rows | +256 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +242 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +356 bytes; 12 addss, 6 movaps, 20 movss, 16 mulss, 1 xmm regs 01234567 -> not retail | +360 bytes; 12 addss, 6 movaps, 20 movss, 16 mulss, 1 xmm regs 01234567 -> not retail |
| p20 | a's row copied into four locals | +84 bytes; 4 x87 fadd, 8 x87 faddp, 12 x87 fld, 13 x87 fmul, 5 x87 fstp -> not retail | +150 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +203 bytes; 12 addss, 2 movaps, 15 movss, 13 mulss, 1 xmm regs 01234567 -> not retail | +207 bytes; 12 addss, 2 movaps, 15 movss, 13 mulss, 1 xmm regs 01234567 -> not retail |
| p21 | D zeroed then += over k | +95 bytes; 1 x87 fadd, 2 x87 faddp, 4 x87 fld, 4 x87 fmul, 2 x87 fstp -> not retail | +94 bytes; 1 x87 fadd, 2 x87 faddp, 4 x87 fld, 4 x87 fmul, 2 x87 fstp -> not retail | +111 bytes; 3 addss, 9 movss, 4 mulss, 1 xmm regs 01234567 -> not retail | +117 bytes; 3 addss, 9 movss, 4 mulss, 1 xmm regs 01234567 -> not retail |
| p22 | unsigned loop counters | +94 bytes; 1 x87 fadd, 11 x87 faddp, 13 x87 fld, 15 x87 fmul, 2 x87 fstp -> not retail | +156 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +187 bytes; 12 addss, 1 movaps, 10 movss, 15 mulss, 1 xmm regs 01234567 -> not retail | +191 bytes; 12 addss, 1 movaps, 10 movss, 15 mulss, 1 xmm regs 01234567 -> not retail |
| p23 | flattened index i = r*4 + c | +537 bytes; 24 x87 faddp, 32 x87 fld, 32 x87 fmul, 8 x87 fstp -> not retail | +525 bytes; 24 x87 faddp, 32 x87 fld, 32 x87 fmul, 8 x87 fstp -> not retail | +731 bytes; 24 addss, 40 movss, 32 mulss, 1 xmm regs 01234567 -> not retail | +733 bytes; 24 addss, 40 movss, 32 mulss, 1 xmm regs 01234567 -> not retail |
| p24 | inline Dot4(row, column, stride) helper | +94 bytes; 1 x87 fadd, 11 x87 faddp, 13 x87 fld, 15 x87 fmul, 2 x87 fstp -> not retail | +156 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +188 bytes; 12 addss, 11 movss, 15 mulss, 1 xmm regs 01234567 -> not retail | +192 bytes; 12 addss, 11 movss, 15 mulss, 1 xmm regs 01234567 -> not retail |
| p25 | inline RowTimesMatrix(out, row, m) helper | +40 bytes; 1 call -> not retail | +43 bytes; 1 call -> not retail | +183 bytes; 12 addss, 3 movaps, 7 movss, 15 mulss, 1 xmm regs 01234567 -> not retail | +187 bytes; 12 addss, 3 movaps, 7 movss, 15 mulss, 1 xmm regs 01234567 -> not retail |
| p26 | out-of-line (noinline) helper call | +8 bytes; 1 call -> not retail | +8 bytes; 1 call -> not retail | +8 bytes; 1 call, 1 xmm regs 01234567 -> not retail | +10 bytes; 1 call, 1 xmm regs 01234567 -> not retail |
| p27 | operands swapped in each product (b * a) | +94 bytes; 1 x87 fadd, 11 x87 faddp, 13 x87 fld, 15 x87 fmul, 2 x87 fstp -> not retail | +156 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +187 bytes; 12 addss, 1 movaps, 10 movss, 15 mulss, 1 xmm regs 01234567 -> not retail | +191 bytes; 12 addss, 1 movaps, 10 movss, 15 mulss, 1 xmm regs 01234567 -> not retail |
| p28 | while loops | +94 bytes; 1 x87 fadd, 11 x87 faddp, 13 x87 fld, 15 x87 fmul, 2 x87 fstp -> not retail | +156 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +187 bytes; 12 addss, 1 movaps, 10 movss, 15 mulss, 1 xmm regs 01234567 -> not retail | +191 bytes; 12 addss, 1 movaps, 10 movss, 15 mulss, 1 xmm regs 01234567 -> not retail |
| p29 | row loop, four column statements via Data | +98 bytes; 1 x87 fadd, 11 x87 faddp, 13 x87 fld, 15 x87 fmul, 2 x87 fstp -> not retail | +156 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +189 bytes; 12 addss, 1 movaps, 10 movss, 15 mulss, 1 xmm regs 01234567 -> not retail | +193 bytes; 12 addss, 1 movaps, 10 movss, 15 mulss, 1 xmm regs 01234567 -> not retail |
| p30 | row loop, four column statements via accessors | +98 bytes; 1 x87 fadd, 11 x87 faddp, 13 x87 fld, 15 x87 fmul, 2 x87 fstp -> not retail | +156 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +189 bytes; 12 addss, 1 movaps, 10 movss, 15 mulss, 1 xmm regs 01234567 -> not retail | +193 bytes; 12 addss, 1 movaps, 10 movss, 15 mulss, 1 xmm regs 01234567 -> not retail |
| p31 | terms in reverse order p3 + p2 + p1 + p0 | +94 bytes; 1 x87 fadd, 11 x87 faddp, 13 x87 fld, 15 x87 fmul, 2 x87 fstp -> not retail | +156 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +187 bytes; 12 addss, 1 movaps, 10 movss, 15 mulss, 1 xmm regs 01234567 -> not retail | +191 bytes; 12 addss, 1 movaps, 10 movss, 15 mulss, 1 xmm regs 01234567 -> not retail |
| p32 | explicit (float) casts on every product | +94 bytes; 1 x87 fadd, 11 x87 faddp, 13 x87 fld, 15 x87 fmul, 2 x87 fstp -> not retail | +156 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +183 bytes; 12 addss, 3 movaps, 7 movss, 15 mulss, 1 xmm regs 01234567 -> not retail | +187 bytes; 12 addss, 3 movaps, 7 movss, 15 mulss, 1 xmm regs 01234567 -> not retail |
| p33 | SIMD-like F4 struct emulating load / broadcast / mul / add tree / store | +1556 bytes; 12 x87 fadd, 30 x87 fld, 16 x87 fmul, 30 x87 fstp -> not retail | +1602 bytes; 12 x87 fadd, 29 x87 fld, 16 x87 fmul, 29 x87 fstp -> not retail | +1860 bytes; 12 addss, 1 movaps, 106 movss, 16 mulss, 1 xmm regs 01234567 -> not retail | +1864 bytes; 12 addss, 1 movaps, 106 movss, 16 mulss, 1 xmm regs 01234567 -> not retail |
| p34 | pointers homed in locals pa / pb / pd (retail's homing), then the loop | +94 bytes; 1 x87 fadd, 11 x87 faddp, 13 x87 fld, 15 x87 fmul, 2 x87 fstp -> not retail | +156 bytes; 12 x87 faddp, 16 x87 fld, 16 x87 fmul, 4 x87 fstp -> not retail | +187 bytes; 12 addss, 1 movaps, 10 movss, 15 mulss, 1 xmm regs 01234567 -> not retail | +191 bytes; 12 addss, 1 movaps, 10 movss, 15 mulss, 1 xmm regs 01234567 -> not retail |
| p35 | homed pointers + F4 kernel emulation | +1500 bytes; 12 x87 fadd, 30 x87 fld, 16 x87 fmul, 30 x87 fstp -> not retail | +1526 bytes; 12 x87 fadd, 29 x87 fld, 16 x87 fmul, 29 x87 fstp -> not retail | +1782 bytes; 12 addss, 1 movaps, 106 movss, 16 mulss, 1 xmm regs 01234567 -> not retail | +1786 bytes; 12 addss, 1 movaps, 106 movss, 16 mulss, 1 xmm regs 01234567 -> not retail |
