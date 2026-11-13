# Hand chunk 2044 (C3DAnimationManager .. C3DAnimationManager): 1 of 2 byte-exact

## What matched (1)
- 00a9f540 C3DAnimationManager::CalcMeshTransformations (CMatrix4x4 overload) -- RELOCATION_MATCH at its FIRST full grade, written
  from the blended twin's source (the second row was done after the first because it is the simpler twin; see below). Evaluates a
  blend state: pool scratch of bone_count * 5 keyframes behind a 64-bit range check that returns at once, CombineBaseAnimations,
  then per combination blend (count re-read every pass) the bank animation sampled with ConvertTimeToFrames and applied per bone by
  four file-static helpers, then ApplyBoneMorphDisplacement / ApplyParentTransformations / ApplyBoneMorphScaling and FreeAll.
  SHAPE lessons: (1) FableWin.pdb's COMDAT run right after CalcMeshTransformations names the TU's inline helpers AND four undecorated
  file-static functions ApplyRelativeAnimation / ApplyRelativeAnimationAlpha / ApplyPartialAnimation / ApplyPartialAnimationAlpha;
  retail keeps uncalled LTCG custom-convention copies of them (0x00A9D710, 0x00A9D980, 0x00A9D9E0, 0x00A9DC20) and expands all four
  in both rows. Written as statics, VC7.1 keeps them as calls (927 bytes); FABLE_FORCEINLINE gives retail's exact size
  (Inline-budget line). (2) ConvertTimeToFrames' body must be VISIBLE in the TU (it holds GFFloatToLongFloor asm so it stays a call):
  with a declaration only, frame / fraction escape and every bone loop reloads them (2249 v 2288 bytes). (3) The 64-bit range check
  is `__int64 keyframe_count = bone_count * 5; if (keyframe_count < 0 || keyframe_count > 0x7fffffff) return;` then
  `AllocateType<C3DKeyframe>((unsigned long)keyframe_count)`; a checked copy of an unsigned count instead makes the inline
  AllocateType multiply 64x64 (`__allmul`). (4) `alpha == 1.0f` (fucompp) in this row; the blended row uses `fabs(alpha - 1.0f) < 0.0001f`.

## Parked for Fable 5.1 (1)
- 00aa0090 C3DAnimationManager::CalcBlendedMeshTransformations DIFFER(2390v2390): 604/622 instructions equal unmasked (frame
  offsets all equal); only the q.Y / q.Z / q.W product order of the expanded BlendWith (partial-alpha loop): 0x00AA07E8 / 0x00AA0808 /
  0x00AA0828 retail `fld [esp+10h] ; fmul [esi-14h|10h|0Ch] ; fld [kf] ; fmul st(2)` (scale0 * Rotation first), ours
  `fld [esp+0A0h|0A4h|0A8h] ; fmul st(1)` (scale1 * other first) | tried: 10 grades (pass 1: 7, pass 2: 3) + ~70 free feature
  probes, see measurements | next: a natural, code-neutral source of ~16..32 dead-float-locals' worth of IL in this function only
  (PROVEN lever: probe/diag_fa1_dd20.cpp = differ source + 20 dead `float ddN = blend * N.0f;` = RELOCATION_MATCH).

## 0x00AA0090 measurements (tools in tools/: cc.py compile+disassemble, feat.py x87 features, rv.sh / sweep*.sh variants, lt.py /
## ltv.sh LTCG out-of-line copies, c2/ = agent3010's c2h6/c2h7 key-trace hooks repathed, sums2.py / ids.py analysers)
Grades (full-row diffs against retail), pass 1:
1. probe/p1.cpp (inline loops, no helpers): DIFFER 2339v2390, 481/609 masked.
2. probe/p2.cpp (+GFGetInterpolatedValue, IsRelative/IsPartial, V2 range check): 2368v2390, 529/618.
3. probe/p4.cpp (+static FABLE_FORCEINLINE Apply* helpers, GetAlphaWeight, visible ConvertTimeToFrames): 2390v2390, 600/622;
   dot order already retail, q order not; GetAnimation args pushed before PeekAnimation (retail: after).
4. probe/p5.cpp (+named `info` local): 2390v2390, 600/622: call order fixed, dot order flipped to ours (Y,Z,W,X).
5. LTCG build of p5 (tools/decomp_pipeline/linux_lane/lane_tools/ltcg_grade.py): DIFFER, ebp frame, SAME x87 orders -> retail's
   row is NOT an LTCG codegen artefact; the .obj compile is right.
6. probe/diag63.cpp (63 dead global stores, DIAGNOSTIC only): orders retail but real code added (2771B) -- proves the lever only.
7. differ/00aa0090.cpp (= p5 + documentation): DIFFER(2390v2390) as 4.
Pass 2 (after re-analysis: the dot order follows the fabs spelling, the q order the IL amount):
8. probe/fa1.cpp (`fabsf(alpha - 1.0f)` in both tests): DIFFER 2390v2390, 604/622 unmasked -- the dot product is retail's.
9. probe/diag_fa1_dd20.cpp (fa1 + 20 dead float locals at the top, DIAGNOSTIC, not deliverable): RELOCATION_MATCH.
10. differ/00aa0090.cpp (= fa1 + documentation): DIFFER(2390v2390), 604/622.
Pass 3 not started: no candidate passed the free feature screen (tools/rv.sh + feat.py); grades were kept for one.
Free measurements (no retail diff; features read from our own object with tools/feat.py):
- Mechanism (agent3010 tools, key hook at c2 0x1075CD1D): commutative operands are sorted by DESCENDING key, hash16 = sum of
  operand hashes, leaf hash = record id * 32. Our ids: partial+0 569, partial+4/8/c 823/822/821, scale0/scale1 557/558; this-side
  fields are indirection nodes with fixed keys 0x10008 / 0x18008. Retail's orders need partial+0 <= 511 and (scale0 >= ~612 or the
  late fields >= ~933) under that model -- i.e. a non-uniform record shift.
- Dead-code sweeps (DIAGNOSTIC): `float ddN = blend * N.0f;` at the function top (code-neutral, 2390B): k = 15..31 flip the q sums
  to retail's (k=17 not), the dot never moves. Dead global stores: k >= 63 flip q; the dot is retail exactly when k = 3 mod 4
  (3, 7, 63, 67, 71..); 20 dead floats + 3 dead global stores give BOTH retail orders. Location in the function does not matter.
- fabs spellings (free): fabsf(x) and fabs((double)x) give the retail dot order, (float)fabs(x) / fabs(x) do not; fabsf + 16..32
  dead floats give both orders (k=14 not, k=15 three of four sums).
- BlendWith written through an inline CQuaternion::SetSlerp (landed 0x00A22590 body) + temp copy: twin byte-identical, blended
  dot retail, q unchanged (another body-neutral layer; does not add enough IL).
- Natural spellings measured, no order change (all 2390B): Dot(this/other), scratch keyframe outside the if, param orders of the
  static helpers (3 orders), unsigned frame, count in the for-init, named time / alpha components, CCountedPointer direct-init,
  frame / fraction at function scope, q.W term order, FABLE_RETAIL_CARRAY, FABLE_RETAIL_C3DKEYFRAME_CTOR, the twin in the same TU.
  Code-changing: a bool full_weight local (2376B), an interpolated C3DAnimationState local (2406B).
- LTCG out-of-line copy of ApplyPartialAnimationAlpha (tools/lt.py, compared with retail 0x00A9DC20): 174/187 equal; same custom
  register convention (al = enabled, ecx = frame, esi = keyframe); differs in callee-clean `ret 0Ch` v `ret`, one dead-parameter
  frame word and the same two x87 orders (retail dot Y,X,Z,W) -- a small, fast (1 s) target for helper-text experiments.
Ranked next ideas: (1) natural blended-only IL worth ~15..31 dead floats: candidates not yet measured -- an inline interpolate member
on the state (e.g. a C3DAnimationStateAlpha / C3DAnimationState temporary built from both states and read back), the time / alpha
interpolation through the PDB's GFDebugDynamicCast<..., C3DAnimationStateAlpha> (Ego_r/FableWin list that instantiation; its use
site is unknown), two-state PeekBaseAnimation through CIVCountedPointer copies; (2) disassemble 0x00AA2110 CalcBlendedBoneTransformation
(same blended loop, 4736 bytes) and check its expanded BlendWith orders against the same source shape; (3) make the LTCG copy
0x00A9DC20 match first (helper text, then rerun the row).

## Names and evidence
- Rows: Ego_r.pdb + FableWin.pdb exact spellings (QAE, const refs / const C3DVector*, CMatrix4x4*, unsigned long, bool); arg counts
  = `ret 20h` / `ret 18h`; callers 0x00AA0A4A (the landed CMatrix3x4 overload 0x00AA09F0) and 0x00A9FE80 / 0x00AE136B / 0x00AE2A28.
- Callees: bound spellings reused (Allocate, FreeAll, CombineBaseAnimations, CombineBlendedBaseAnimations, PeekAnimation,
  GetAnimation, operator delete); ApplyBoneMorphDisplacement / ApplyParentTransformations / ApplyBoneMorphScaling (private const ABE,
  Ego_r) and CQuaternion::operator*= `??XCQuaternion@@QAEABV0@ABV0@@Z` (unbound before), ConvertTimeToFrames (Ego_r QBEXMAAJAAM).
  GetKeyframeInterpolate 0x00A4C1F0 uses the PDB spelling (NAME_FIXES.tsv).
- Inline helpers: GFGetInterpolatedValue (shared header, FableWin), IsRelative / IsPartial (C3DAnimationStats), IsBoneEnabled /
  PeekSequence (C3DAnimation2), IsIdentity (sequence + data), GetKeyframeInterpolate (sequence), GetAlphaWeight, GetBoneCount,
  PeekBaseAnimation / GetCombinationBlendCount / PeekCombinationBlend, SetSlerpFromIdentity / PostApplyRelativeAnimationAlpha /
  BlendWith / Dot: all PDB names. The four Apply* statics: undecorated FableWin / Ego_r static names (Ego_r 0x00BE1380..0x00BE16D0).
- Descriptive (made up): parameter names only (`transpose` for the final bool, `matrix_count` for the unused unsigned long).
- Globals: g_ArcCosTable / g_CosTable (the landed majority spelling; reloc_bindings also has g_ACosTable / g_cosTable for the same
  addresses -- one address, two names: coordinator to pick; no PDB data symbol on this machine).

## Header debt
- engine/C3DKeyframe.h POSTAPPLY defines MULASSIGN / ADDASSIGN after CQuaternion.h / C4DVector.h are included (rows define them).
- engine/C3DAnimationStateAlpha.h default branch is the flattened generated layout (no base); the retail opt-in is new here.
- engine/C3DAnimationBlendState.h had no CSharedData definition anywhere in the repo (landed rows use local models).
- Landed 0x00A9F2F0 / 0x00AA0DC0 use a local `InterpolateFloat` helper: FableWin's name is GFGetInterpolatedValue (shared header).

## C++ attempts before helper (ASM_RULES rule 4: 30+ plain C++, then 10+ intrinsics, per site)
Harness (free fragment probes, no grades): probe/asm/tbl/gen.py writes spell.cpp, one fragment per spelling fed exactly this row's
site input; probe/asm/tbl/run.py compiles it with VC7.1 RTM 13.10.3077 and the row's flags (/O2 /Oy /W3) plus a flag variant,
disassembles, and counts the conversion instructions in each fragment (res_<flags>.tsv). Adapted from agent2037 / agent2038's
probe/asm harness (c2037 / c2038 rows).
- Near sites (GFFloatToLongNear in the forced-inline GFFastArcCos / GFFastSin, retail `fld [esp+1Ch] ; fistp dword ptr [esp+N]` with
  y - 0.5f homed in [esp+1Ch]): fragment `near_NN(cosom, t, out)` = the SetSlerpFromIdentity / BlendWith block with forced-inline
  GFFastArcCos(cosom) and GFFastSin((1 - t) * omega) / GFFastSin(omega * t), tables and `(i - 256) & 0x3ff` as in the row, so each
  fragment holds all three site kinds: arccos y = (cosom + 1) * 512.5f -> 0x00A9F7CF (SetSlerpFromIdentity, relative-alpha loop) and
  0x00A9FAF0 (BlendWith, partial-alpha loop); sin y = (1 - t) * omega * 1024 -> 0x00A9F83D / 0x00A9FB5E; sin y = omega * t * 1024 ->
  0x00A9F8A7 / 0x00A9FBC8. Retail's six blocks are the same expression on the same inputs (checked in dis_all.txt: `fadd [1.0] ;
  fmul [512.5] ; fst ; fsub [0.5] ; fstp [esp+1Ch] ; fld [esp+1Ch] ; fistp` twice, `fmul [1024.0] ; fst ; fsub [0.5] ; fstp ; fld ;
  fistp ; fild ; mov eax ; add eax,-100h ; and eax,3FFh` four times); BlendWith differs only in where cosom comes from (Dot), not in the
  conversion. In every fragment below the three conversions emitted the same thing (counts are 3x), so each row of the table holds
  for all six addresses.
- Floor site (GFFloatToLongFloor in the TU's out-of-line C3DAnimation2::ConvertTimeToFrames copy, retail 0x00A52677 `fistp dword ptr
  [ebp+8]` inside 0x00A52671..0x00A5268B): the landed 0x00A52650 row's comment proves the helper but carries no 30+10 table, so it is
  measured here: fragment `Anim::floor_NN(time, frame, fraction) const` = the ConvertTimeToFrames body (frames = time *
  SamplesPerSecond, wrap by FrameCount) with the spelling for floor(frames).
Outcome classes (row flags): `call __ftol2` (truncating CRT helper, the result in eax; library spellings add `call _floor` / `_ceil` /
`_modf` / `__CIfmod` / `__copysign`); SSE converts (cvtss2si, cvttss2si, cvtsd2si, cvttsd2si, cvtps2pi, cvttps2pi, cvtps2dq,
cvttps2dq, cvtpd2dq); magic-number bit tricks (fadd 12582912.0f / 6755399441055744.0, fstp, integer mov: no conversion instruction).
/QIfist turns every x87 cast into `fistp qword` into an 8-byte slot; /arch:SSE2 turns float casts into cvttss2si (+ cvtsi2ss for the
fild) and double casts into `fnstcw ; or ah,0Ch ; fldcw ; fistp dword ; fldcw` (a truncating control-word wrapper, the whole TU on
SSE scalar code) -- the only dword fistp seen, never bare; /arch:SSE = base conversions + cvtsi2ss; /Op = the same classes (fmod via
`_fmod`, sqrt out of line); /fp:fast is not a VC7.1 option (D4002 ignored; result identical to base). No spelling, under any flag set,
gives retail's bare `fld ; fistp dword ptr [esp+N]` (near) or `fld ; fsub ; fistp dword ptr [ebp+8] ; fild ; fadd ; fstp ; mov ; cmp ;
jne ; inc dword ptr [ebp+8]` (floor), so the shared helpers stay (ASM_RULES 3.2). Counts: near 38 plain C++ + 15 intrinsic spellings
(12 compiler intrinsics); floor 32 plain C++ + 14 intrinsic spellings (11 compiler intrinsics). nintr_07 / 08 (MMX) also push sqrt out
of line (C4730). Earlier, row-level measurements (whole 0x00A9F540 TU, tools/cc.py): `(long)x` family 2125 B with six __ftol2 calls,
`(long)(x + 0.5f)` 2159 B, `(long)floor(x)` 2256 B, /QIfist 2141 B, `_mm_cvtss_si32` 2175 B, the helper 2288 B = retail; and the
older 15-spelling table for the same GFFastArcCos / GFFastSin sites, pending/2026-11-04_linux_hand747/NOTES.md.

Near sites, plain C++ (X = y: `(cosom + 1.0f) * 512.5f` in GFFastArcCos, `angle * 1024.0f` in GFFastSin; retail = GFFloatToLongNear(y - 0.5f)); counts = the fragment's three expanded conversions:
| # | spelling | /O2 /Oy /W3 (row flags) | + /QIfist | + /arch:SSE2 |
|---|---|---|---|---|
| near_00 | `(long)(X - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_01 | `(int)(X - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_02 | `static_cast<long>(X - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_03 | `long(X - 0.5f) functional cast` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_04 | `(long)X (truncation, no bias)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_05 | `(long)(double)(X - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_06 | `(long)((double)X - 0.5)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x `fistp qword`, 6x fldcw, 3x fnstcw |
| near_07 | `(long)(long double)(X - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_08 | `(long)(__int64)(X - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_09 | `(long)(unsigned long)(X - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_10 | `(long)(short)(X - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_11 | `(long)floor(X)` | 3x `__ftol2`, 3x _floor | 3x _floor, 3x `fistp qword` | 3x _floor, 3x cvtsi2ss, 3x `fistp dword`, 6x fldcw, 3x fnstcw |
| near_12 | `(long)floorf(X)` | 3x `__ftol2`, 3x _floor | 3x _floor, 3x `fistp qword` | 3x _floor, 3x cvtsi2ss, 3x `fistp dword`, 6x fldcw, 3x fnstcw |
| near_13 | `(long)floor((double)X)` | 3x `__ftol2`, 3x _floor | 3x _floor, 3x `fistp qword` | 3x _floor, 3x cvtsi2ss, 3x `fistp dword`, 6x fldcw, 3x fnstcw |
| near_14 | `(long)std::floor(X) (<cmath> float overload)` | 3x `__ftol2`, 3x _floor | 3x _floor, 3x `fistp qword` | 3x _floor, 3x cvtsi2ss, 3x `fistp dword`, 6x fldcw, 3x fnstcw |
| near_15 | `(long)ceil(X - 1.0f)` | 3x `__ftol2`, 3x _ceil | 3x _ceil, 3x `fistp qword` | 3x _ceil, 3x cvtsi2ss, 3x `fistp dword`, 6x fldcw, 3x fnstcw |
| near_16 | `(long)(X - fmod(X, 1.0f))` | 3x __CIfmod, 3x `__ftol2` | 3x __CIfmod, 3x `fistp qword` | 3x __CIfmod, 3x cvtsi2ss, 3x `fistp dword`, 6x fldcw, 3x fnstcw |
| near_17 | `(long)(X - fmodf(X, 1.0f))` | 3x __CIfmod, 3x `__ftol2` | 3x __CIfmod, 3x `fistp qword` | 3x __CIfmod, 3x cvtsi2ss, 3x `fistp dword`, 6x fldcw, 3x fnstcw |
| near_18 | `modf(X, &ip) then (long)ip` | 3x `__ftol2`, 3x _modf | 3x _modf, 3x `fistp qword` | 3x _modf, 3x cvtsi2ss, 3x cvttsd2si |
| near_19 | `long R = X - 0.5f; (implicit conversion)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_20 | `named float v = X - 0.5f; R = (long)v` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_21 | `through inline long ToLong(float)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_22 | `through inline long ToLongRef(const float&)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_23 | `sign-split round: v >= 0 ? (long)(v + 0.5f) : (long)(v - 0.5f), v = X - 0.5f` | 3x `__ftol2` | 6x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_24 | `(long)(X + 0.5f) - 1` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_25 | `(long)(X * 1.0f - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_26 | `union double magic: d = (X - 0.5f) + 6755399441055744.0, R = low word` | no conversion insn (magic add + store + integer mov) | no conversion insn (magic add + store + integer mov) | 3x cvtsi2ss |
| near_27 | `union float magic: f = (X - 0.5f) + 12582912.0f, R = bits - 0x4B400000` | no conversion insn (magic add + store + integer mov) | no conversion insn (magic add + store + integer mov) | 3x cvtsi2ss |
| near_28 | `pointer pun: f = (X - 0.5f) + 12582912.0f; *(long*)&f - 0x4B400000` | no conversion insn (magic add + store + integer mov) | no conversion insn (magic add + store + integer mov) | 3x cvtsi2ss |
| near_29 | `memcpy pun of the 12582912.0f magic sum` | no conversion insn (magic add + store + integer mov) | no conversion insn (magic add + store + integer mov) | 3x cvtsi2ss |
| near_30 | `(long)(unsigned short)(X - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x `__ftol2`, 3x cvtsi2ss |
| near_31 | `trunc then floor fix-up: R = (long)X; if ((float)R > X) --R` | 3x `__ftol2` | 3x `fistp qword` | 6x cvtsi2ss, 3x cvttss2si |
| near_32 | `(long)_copysign(floor(fabs(v)), v), v = X - 0.5f` | 3x __copysign, 3x `__ftol2`, 3x _floor | 3x __copysign, 3x _floor, 3x `fistp qword` | 3x __copysign, 3x _floor, 3x cvtsi2ss, 3x `fistp dword`, 6x fldcw, 3x fnstcw |
| near_33 | `volatile float v = X - 0.5f; R = (long)v (homed like retail)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |
| near_34 | `(long)(double)floor(X)` | 3x `__ftol2`, 3x _floor | 3x _floor, 3x `fistp qword` | 3x _floor, 3x cvtsi2ss, 3x `fistp dword`, 6x fldcw, 3x fnstcw |
| near_35 | `(long)(X - 0.5) (double constant)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x `fistp qword`, 6x fldcw, 3x fnstcw |
| near_36 | `(long)(__int64)floor(X)` | 3x `__ftol2`, 3x _floor | 3x _floor, 3x `fistp qword` | 3x _floor, 3x cvtsi2ss, 3x `fistp dword`, 6x fldcw, 3x fnstcw |
| near_37 | `direct-init long r(X - 0.5f)` | 3x `__ftol2` | 3x `fistp qword` | 3x cvtsi2ss, 3x cvttss2si |

Near sites, intrinsics (12 compiler intrinsics + 2 CRT x87 control-word calls + 1 MXCSR macro):
| # | spelling | /O2 /Oy /W3 (row flags) | + /QIfist | + /arch:SSE2 |
|---|---|---|---|---|
| nintr_00 | `_mm_cvtss_si32(_mm_set_ss(X - 0.5f))` | 3x cvtss2si | 3x cvtss2si | 3x cvtsi2ss, 3x cvtss2si |
| nintr_01 | `_mm_cvt_ss2si(_mm_set_ss(X - 0.5f))` | 3x cvtss2si | 3x cvtss2si | 3x cvtsi2ss, 3x cvtss2si |
| nintr_02 | `_mm_cvttss_si32(_mm_set_ss(X))` | 3x cvttss2si | 3x cvttss2si | 3x cvtsi2ss, 3x cvttss2si |
| nintr_03 | `_mm_cvtt_ss2si(_mm_load_ss(&v)), v = X` | 3x cvttss2si | 3x cvttss2si | 3x cvtsi2ss, 3x cvttss2si |
| nintr_04 | `_mm_cvtss_si32(_mm_sub_ss(_mm_set_ss(X), _mm_set_ss(0.5f)))` | 3x cvtss2si | 3x cvtss2si | 3x cvtsi2ss, 3x cvtss2si |
| nintr_05 | `_mm_cvtsd_si32(_mm_set_sd(X - 0.5f))` | 3x cvtsd2si | 3x cvtsd2si | 3x cvtsd2si, 3x cvtsi2ss |
| nintr_06 | `_mm_cvttsd_si32(_mm_set_sd(X))` | 3x cvttsd2si | 3x cvttsd2si | 3x cvtsi2ss, 3x cvttsd2si |
| nintr_07 | `_mm_cvtsi64_si32(_mm_cvtps_pi32(_mm_set_ss(X - 0.5f)))` | 1x ?sqrt@@YAMM@Z, 3x cvtps2pi | 1x ?sqrt@@YAMM@Z, 3x cvtps2pi | 1x ?sqrt@@YAMM@Z, 3x cvtps2pi, 3x cvtsi2ss |
| nintr_08 | `_mm_cvtsi64_si32(_mm_cvttps_pi32(_mm_set_ss(X)))` | 1x ?sqrt@@YAMM@Z, 3x cvttps2pi | 1x ?sqrt@@YAMM@Z, 3x cvttps2pi | 1x ?sqrt@@YAMM@Z, 3x cvtsi2ss, 3x cvttps2pi |
| nintr_09 | `_mm_cvtsi128_si32(_mm_cvtps_epi32(_mm_set_ss(X - 0.5f)))` | 3x cvtps2dq | 3x cvtps2dq | 3x cvtps2dq, 3x cvtsi2ss |
| nintr_10 | `_mm_cvtsi128_si32(_mm_cvttps_epi32(_mm_set_ss(X)))` | 3x cvttps2dq | 3x cvttps2dq | 3x cvtsi2ss, 3x cvttps2dq |
| nintr_11 | `_mm_cvtsi128_si32(_mm_cvtpd_epi32(_mm_set_sd(X - 0.5f)))` | 3x cvtpd2dq | 3x cvtpd2dq | 3x cvtpd2dq, 3x cvtsi2ss |
| nintr_12 | `_controlfp(_RC_NEAR, _MCW_RC) then (long)(X - 0.5f)` | 3x __controlfp, 3x `__ftol2` | 3x __controlfp, 3x `fistp qword` | 3x __controlfp, 3x cvtsi2ss, 3x cvttss2si |
| nintr_13 | `_control87(_RC_DOWN, _MCW_RC) then (long)X` | 3x __control87, 3x `__ftol2` | 3x __control87, 3x `fistp qword` | 3x __control87, 3x cvtsi2ss, 3x cvttss2si |
| nintr_14 | `_MM_SET_ROUNDING_MODE(_MM_ROUND_DOWN) then _mm_cvtss_si32(_mm_set_ss(X))` | 3x cvtss2si, 3x ldmxcsr, 3x stmxcsr | 3x cvtss2si, 3x ldmxcsr, 3x stmxcsr | 3x cvtsi2ss, 3x cvtss2si, 3x ldmxcsr, 3x stmxcsr |

Floor site 0x00A52677 (ConvertTimeToFrames), plain C++ (X = frames = `time * SamplesPerSecond`; retail = GFFloatToLongFloor(frames)):
| # | spelling | /O2 /Oy /W3 (row flags) | + /QIfist | + /arch:SSE2 |
|---|---|---|---|---|
| floor_00 | `(long)floor(X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_01 | `(long)floorf(X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_02 | `(long)floor((double)X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_03 | `(long)std::floor(X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_04 | `(int)floor(X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_05 | `static_cast<long>(floor(X))` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_06 | `(long)(double)floor(X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_07 | `(long)(float)floor(X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_08 | `(long)(__int64)floor(X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_09 | `(long)floor((long double)X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_10 | `(long)(X - 0.5f) (biased truncation)` | 1x `__ftol2` | 1x `fistp qword` | 1x cvtsi2ss, 1x cvttss2si |
| floor_11 | `(long)X - (X < (float)(long)X ? 1 : 0)` | 1x `__ftol2` | 1x `fistp qword` | 2x cvtsi2ss, 1x cvttss2si |
| floor_12 | `trunc + fix-up: R = (long)X; if ((float)R > X) --R` | 1x `__ftol2` | 1x `fistp qword` | 2x cvtsi2ss, 1x cvttss2si |
| floor_13 | `X >= 0 ? (long)X : (long)X - ((float)(long)X != X)` | 2x `__ftol2` | 2x `fistp qword` | 2x cvtsi2ss, 2x cvttss2si |
| floor_14 | `retail's algorithm in C++: r = (long)(X - 0.5f); if (X == (float)r + 1.0f) ++r` | 1x `__ftol2` | 1x `fistp qword` | 2x cvtsi2ss, 1x cvttss2si |
| floor_15 | `retail's algorithm with the integer bit compare (memcpy)` | 1x `__ftol2` | 1x `fistp qword` | 2x cvtsi2ss, 1x cvttss2si |
| floor_16 | `retail's algorithm with *(long*)& compares` | 1x `__ftol2` | 1x `fistp qword` | 2x cvtsi2ss, 1x cvttss2si |
| floor_17 | `(long)(X + 0.5f) - 1` | 1x `__ftol2` | 1x `fistp qword` | 1x cvtsi2ss, 1x cvttss2si |
| floor_18 | `(long)(X + 1.0f) - 1` | 1x `__ftol2` | 1x `fistp qword` | 1x cvtsi2ss, 1x cvttss2si |
| floor_19 | `(long)(X < 0 ? X - 0.99999994f : X)` | 1x `__ftol2` | 1x `fistp qword` | 1x cvtsi2ss, 1x cvttss2si |
| floor_20 | `(long)(X - fmod(X, 1.0f)) - (fmod < 0)` | 1x __CIfmod, 1x `__ftol2` | 1x __CIfmod, 1x `fistp qword` | 1x __CIfmod, 1x cvtsi2ss, 1x `fistp qword`, 2x fldcw, 1x fnstcw |
| floor_21 | `modf(X, &ip); (long)ip - (X < ip)` | 1x `__ftol2`, 1x _modf | 1x _modf, 1x `fistp qword` | 1x _modf, 1x cvtsi2ss, 1x cvttsd2si |
| floor_22 | `-(long)ceil(-X)` | 1x `__ftol2`, 1x _ceil | 1x _ceil, 1x `fistp qword` | 1x _ceil, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_23 | `(long)ceil(X) - ((float)ceil(X) != X)` | 1x `__ftol2`, 1x _ceil | 1x _ceil, 1x `fistp qword` | 1x _ceil, 1x cvtsi2ss, 1x cvttsd2si |
| floor_24 | `through inline long FloorToLong(float)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_25 | `union double magic of (X - 0.5f)` | no conversion insn (magic add + store + integer mov) | no conversion insn (magic add + store + integer mov) | 1x cvtsi2ss |
| floor_26 | `union float magic of (X - 0.5f)` | no conversion insn (magic add + store + integer mov) | no conversion insn (magic add + store + integer mov) | 1x cvtsi2ss |
| floor_27 | `named float f = (float)floor(X); (long)f` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_28 | `implicit long R = floor(X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |
| floor_29 | `volatile float v = X - 0.5f; (long)v + fix-up` | 1x `__ftol2` | 1x `fistp qword` | 2x cvtsi2ss, 1x cvttss2si |
| floor_30 | `(long)(double)(X - 0.5f)` | 1x `__ftol2` | 1x `fistp qword` | 1x cvtsi2ss, 1x cvttss2si |
| floor_31 | `(unsigned long)floor(X)` | 1x `__ftol2`, 1x _floor | 1x _floor, 1x `fistp qword` | 1x _floor, 1x cvtsi2ss, 1x `fistp dword`, 2x fldcw, 1x fnstcw |

Floor site, intrinsics (11 compiler intrinsics + 1 MXCSR macro + 2 CRT x87 control-word calls):
| # | spelling | /O2 /Oy /W3 (row flags) | + /QIfist | + /arch:SSE2 |
|---|---|---|---|---|
| fintr_00 | `_mm_cvtss_si32(_mm_set_ss(X - 0.5f)) (retail's biased round, SSE)` | 1x cvtss2si | 1x cvtss2si | 1x cvtsi2ss, 1x cvtss2si |
| fintr_01 | `_mm_cvt_ss2si(_mm_set_ss(X - 0.5f))` | 1x cvtss2si | 1x cvtss2si | 1x cvtsi2ss, 1x cvtss2si |
| fintr_02 | `_mm_cvttss_si32(_mm_set_ss(X))` | 1x cvttss2si | 1x cvttss2si | 1x cvtsi2ss, 1x cvttss2si |
| fintr_03 | `_mm_cvtt_ss2si(_mm_load_ss(&X))` | 1x cvttss2si | 1x cvttss2si | 1x cvtsi2ss, 1x cvttss2si |
| fintr_04 | `_mm_cvtsd_si32(_mm_set_sd(X - 0.5f))` | 1x cvtsd2si | 1x cvtsd2si | 1x cvtsd2si, 1x cvtsi2ss |
| fintr_05 | `_mm_cvttsd_si32(_mm_set_sd(X))` | 1x cvttsd2si | 1x cvttsd2si | 1x cvtsi2ss, 1x cvttsd2si |
| fintr_06 | `_mm_cvtsi64_si32(_mm_cvtps_pi32(_mm_set_ss(X - 0.5f)))` | 1x cvtps2pi | 1x cvtps2pi | 1x cvtps2pi, 1x cvtsi2ss |
| fintr_07 | `_mm_cvtsi64_si32(_mm_cvttps_pi32(_mm_set_ss(X)))` | 1x cvttps2pi | 1x cvttps2pi | 1x cvtsi2ss, 1x cvttps2pi |
| fintr_08 | `_mm_cvtsi128_si32(_mm_cvtps_epi32(_mm_set_ss(X - 0.5f)))` | 1x cvtps2dq | 1x cvtps2dq | 1x cvtps2dq, 1x cvtsi2ss |
| fintr_09 | `_mm_cvtsi128_si32(_mm_cvttps_epi32(_mm_set_ss(X)))` | 1x cvttps2dq | 1x cvttps2dq | 1x cvtsi2ss, 1x cvttps2dq |
| fintr_10 | `_mm_cvtsi128_si32(_mm_cvtpd_epi32(_mm_set_sd(X - 0.5f)))` | 1x cvtpd2dq | 1x cvtpd2dq | 1x cvtpd2dq, 1x cvtsi2ss |
| fintr_11 | `_MM_SET_ROUNDING_MODE(_MM_ROUND_DOWN) then _mm_cvtss_si32(_mm_set_ss(X))` | 1x cvtss2si, 1x ldmxcsr, 1x stmxcsr | 1x cvtss2si, 1x ldmxcsr, 1x stmxcsr | 1x cvtsi2ss, 1x cvtss2si, 1x ldmxcsr, 1x stmxcsr |
| fintr_12 | `_controlfp(_RC_DOWN, _MCW_RC) then (long)X` | 1x __controlfp, 1x `__ftol2` | 1x __controlfp, 1x `fistp qword` | 1x __controlfp, 1x cvtsi2ss, 1x cvttss2si |
| fintr_13 | `_control87(_RC_NEAR, _MCW_RC) then the biased (long)(X - 0.5f) + fix-up` | 1x __control87, 1x `__ftol2` | 1x __control87, 1x `fistp qword` | 1x __control87, 2x cvtsi2ss, 1x cvttss2si |

## Skipped
None.

## Label corrections / uncatalogued functions found
- 0x00A9D700 `fld [ecx+10h] ; ret` = C3DAnimationStateAlpha::GetAlphaWeight() const (out-of-line COMDAT, uncatalogued).
- 0x00A9D710 = static ApplyPartialAnimation (LTCG custom convention: al enabled, ecx frame; `ret 0Ch`), uncatalogued.
- 0x00A9D730 / 0x00A9D740 catalogued "SetMember_dword0x4" / "SetMemberConst" = C3DAnimationManager::Initialise(CMeshDataBank*)
  (MeshBank = arg) / Uninitialise() (MeshBank = 0) by layout + PDB names + TU order (FableWin: Initialise, Uninitialise follow the
  ctor) -- descriptive-level evidence, not graded.
- 0x00A9D980 = static ApplyRelativeAnimation, 0x00A9D9E0 = static ApplyRelativeAnimationAlpha (uncatalogued, custom convention).
- 0x00A9DC20 catalogued "_global ApplyPartialAnimationAlpha" -- correct (static, custom convention: al, ecx, esi + 3 stack args).

## Resume session (2026-11-12, agent resumed in place) -- 0x00AA0090
Log: grades.log (one line per full-row grade; features = BlendWith dot order + q-sum orders). Candidates in cand/.
KEY RESULT: cand/c32_MATCH.cpp (= cand/c32.cpp) and cand/c35.cpp grade RELOCATION_MATCH 2390v2390 for 0x00AA0090, but with
those shared helper bodies the twin 0x00A9F540 (cand/t35.cpp, built by mktwin.py) is DIFFER (dot + q.X). Shared-body changes found
with the coordinator's /opt/fable_lane/tools/fwsize.py (FableWin /Od sizes):
- C3DKeyframe::BlendWith: FableWin 48i/129b = `CQuaternion rotation; rotation.SetSlerp(<from>, <to>, t); Rotation = rotation;
  Position = Position.Interpolate(other.Position, t);` plus ONE named reference (+2i/+6b); SetSlerp (forced inline, FableWin bytes
  323 SAME) with `float cosom = from.Dot(to);`. Which reference is named decides the rows:
  w1 `const CQuaternion& other_rotation = other.Rotation;` -> blended RELOCATION_MATCH, twin DIFFER (dot);
  w7 `const CQuaternion& from = Rotation;` / w13 `const C3DKeyframe& from = *this;` -> twin RELOCATION_MATCH (robust), blended DIFFER
  (q.Y/Z/W, needs ~8 more dead-float units of blended-only IL after GA+ix+tm, ~14 without; diagnostics probe/diag_w7p2*_dd*.cpp).
- PostApplyRelativeAnimationAlpha: FableWin 32i/88b = one named reference too (p1 other_rotation or p2 other_position, both SAME).
- GFGetInterpolatedValue: FableWin 12i/28b = `float delta=(new-old)*interp; return old+delta;` (opt-in
  FABLE_RETAIL_GFGETINTERPOLATEDVALUE_NAMED in hdr/engine/GFGetInterpolatedValue.h; code-neutral, moves only orders).
- C4DVector operator*/-/+ FableWin 38/42/42i = named result, BUT it changes TLC release code (frame +0x20): not retail (opt-ins
  NAMED_SCALE / NAMED_ADDSUB / NAMED_RESULT exist in hdr, unused).
- Helper parameter order (keyframe, frame, fraction, sequence[, enabled][, alpha]) = the LTCG copies' stack order; neutral in both rows.
Resume-session result for 0x00AA0090 (final):
- differ/00aa0090.cpp = cand/x_w7p2.cpp: BlendWith `const CQuaternion& from = Rotation; CQuaternion rotation; rotation.SetSlerp(from,
  other.Rotation, t); Rotation = rotation; Position = Position.Interpolate(other.Position, t);`, SetSlerp forced inline with `cosom =
  from.Dot(to)`, PostApplyRelativeAnimationAlpha with `const C4DVector& other_position = other.Position;`. Twin with these bodies:
  differ/00a9f540.twin_with_new_bodies.cpp RELOCATION_MATCH (robust: also with p0/p1 PostApply variants, Dot d2, LTCG param order,
  fastcall statics, +0/+6/+14 dead floats). This row: DIFFER 2390v2390, every instruction equal except q.Y/q.Z/q.W product order.
- differ/00aa0090.alt_match.cpp = cand/c35.cpp: BlendWith reference on the OTHER side (`const CQuaternion& other_rotation = other.Rotation;`)
  + PostApply `other_rotation` reference: RELOCATION_MATCH for this row, but its twin (cand/t35.cpp) is DIFFER (dot X-first, retail Y-first;
  q.X); the twin's dot stays X-first under every w1-family shared variant tried (Dot d1/d2, cosom to.Dot(from)/expanded, p0/p1/p2, order).
  Twin-only GetAnimationIndex() fixes its q.X; its dot needs ~10 dead-float units more (probe/diag_tw_dd10.cpp) -- twin is frozen, not done.
- Distance to a both-rows match under the consistent bodies (diagnostic dead floats at the row top, never deliverable): x_w7p2 needs 10..14,
  + GFGetInterpolatedValue named + accessors + named index/time (x_w7p2GAixtm) 8, + named start/end time and alpha (x_k3) 5,
  + bool relative + early-return relative helpers (x_st1) 4. Dead floats inside ApplyRelativeAnimationAlpha count the same (diag_k3_rel6).
  Code-neutral natural knobs measured (each < 1 unit): state accessors, PeekMeshBank(), named index/time/start/end values, bool relative,
  function-scope counters, helper param order, __fastcall statics, relative/partial helper early-return / hoisted-local shapes.
- FableWin /Od evidence explained: the four statics are __fastcall (sizes 27/29/19/27 v FableWin 28/31/20/29; the residual +1i/+3b per
  forwarded float argument also shows on GetKeyframeInterpolate / Interpolate = later-compiler noise); Dot +2i/+8b unexplained (named
  this-ref or double result reach 28i/73b but move the dot wrong in both rows); GFFastSin +8i unexplained; SetSlerp / SetSlerpFromIdentity
  +10i with equal bytes = compiler noise; C4DVector named results reproduce FableWin sizes but change TLC release code (not retail).
- Fitted v proven: BlendWith / PostApply named-reference choice is FITTED (FableWin size consistent, retail grade decides);
  GFGetInterpolatedValue named form PROVEN by FableWin size only (used in x_w7p2GA*, not in differ/); everything else as before.

## Session 3 (2026-11-12, rapid-fire) -- PAIR MATCHED: 0x00AA0090 + 0x00A9F540 RELOCATION_MATCH with one set of helper bodies
Closing change: the shared inline GFFastSin written with named table values and a two-statement index step:
    angle = angle * 1024.0f; long i = GFFloatToLongNear(angle - 0.5f); float frac = angle - (float)i;
    i -= 256; i &= 0x3ff; float a = g_CosTable[i]; float b = g_CosTable[i + 1]; return (1.0f - frac) * a + frac * b;
on top of the Family A blended text cand/x_st1.cpp (it needed 4 dead-float units; this body supplies them, ruler +1..+6 all still
RELOCATION_MATCH in both rows). Family A won; Family B (c35 / twin) was NOT needed. src/ = cand/Ac1.cpp (+ comments) and cand/TAc1k.cpp.
- Evidence for the GFFastSin text (PROVEN-by-size relative to its sibling): VC7.1 /Od 35i/120b v FableWin ?GFFastSin@@YIMM@Z 37i/121b;
  the same naming on GFFastCos (landed 0x00A14840) 32i/108b v FableWin 34i/109b: identical +2i/+1b residual (later-compiler noise),
  and Sin - Cos = 3i/12b in both builds, which only the `i -= 256; i &= 0x3ff;` step reproduces (`i = (i - 256) & 0x3ff` gives 1i/5b).
  Plain text 29i/101b (+8i/+20b). Release bytes unchanged: with this body every landed GFFastSin user still grades RELOCATION_MATCH
  (0x00A147E0 GFFastSin, 0x00A13EB0, 0x00A145C0, 0x00AA3C50, 0x00AA3560, 0x00AA3390, 0x00A88D10, 0x00A22590, 0x00B05620) and the named
  GFFastCos text keeps 0x00A14840 RELOCATION_MATCH (probe/landed_*.cpp, grades.log "CHECK landed").
- Other variants graded on the Family A pair: named table values alone (s3), + named result (c2), + inv_frac (c8) also match both rows;
  named result alone (s4), split index alone (s5), inv_frac alone (s6), named rounded value (c4) do not (blended q.Y/Z/W). Named a/b in
  GFFastArcCos too also matches both (cand/Ac1ac.cpp) but has no size evidence (FableWin ArcCos carries assert code): left as landed.
- Twin row text: GetAnimationIndex() / named index, GetAnimationTime() / named time and `bool relative` (like the blended row) are
  neutral there (cand/TAc1k.cpp, cand/TAc1k1.cpp both RELOCATION_MATCH) and were adopted for one style across the pair.
- Dot body: both `const CQuaternion& p = q;` and `p = *this` reach FableWin's 28i/73b exactly but move the dot rotation in both rows
  (ade / adt / bde / bdt in grades.log); the plain one-expression Dot stays (FITTED: the retail grade decides; FableWin says +2i/+8b).
- Ruler facts (diagnostic only): the BlendWith dot order ROTATES (18,14,10,c -> 14,10,c,18 -> 10,c,18,14 -> c,18,14,10) with the amount of
  IL; Family A twin is flat-matched over 0..12 dead floats (10 = a SIB-order flake), Family A blended needs >= 4; Family B twin matches only
  in a narrow window (10, or 9..10 with the p=*this Dot).
Fitted v proven (whole pair): GFFastSin PROVEN-by-size (sibling-relative); GFGetInterpolatedValue named delta PROVEN-by-size (12i/28b);
BlendWith / PostApplyRelativeAnimationAlpha named-reference choice FITTED (size-equal alternatives exist; retail grade decides);
state accessors, PeekCombinationBlend / GetBoneCount / IsRelative etc. = PDB names; named index / time / start / end / alpha values,
`bool relative`, early-return relative helpers = FITTED natural spellings (code-neutral; together with GFFastSin they set c2's record count).
Retail-asm sites unchanged (the six GFFloatToLongNear sites + ConvertTimeToFrames' floor site, same tables as before); purity classify =
retail_asm for both sources. vland (read-only): both RELOCATION_MATCH, behaviour PASS, no RELOC_IDENTITY; work dir deleted.
