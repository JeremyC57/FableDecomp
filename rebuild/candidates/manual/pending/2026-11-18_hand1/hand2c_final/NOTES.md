# hand2c NOTES (2026-11-19)

Tools: tools/g.py <addr> <src> "<idea>" (grade + relocation-masked diff, logs ATTEMPTS.log), tools/vland.sh <addr...> (strict read-only
vland on src/+test/), tools/mkpayload.py. Baseline of every draft: tmp/base/summary.tsv (grade, hunks, difflines) + tmp/base/<addr>.txt.

## Matched
- 0x006C06D0 CTCBuyableHouse::GetRentMoneyPos -- RELOCATION_MATCH + test PASS (vland strict). Closed by porting the local model onto
  the shared headers (CTCBuyableHouse / CRightHandedSet / C3DVector / C2DVector) with Ego_d's locals (forward copy normalised, offset
  C2DVector from rhs.GetForward() * 0.7f) and the existing C3DVector opt-in NORMALISE_SQUARED_ZXY. New opt-in:
  FABLE_RETAIL_CTCBUYABLEHOUSE_GETRENTMONEYPOS (declaration). NAME_FIX: the landed caller binds a public pointer spelling.

## Open rows
(see STATUS.tsv)
- 0x00F36540 NFractal::CPerlinNoise::GetGaussian2DNoise -- RELOCATION_MATCH + test PASS (vland strict; test also passes on g++ via the
  FABLE_PORTABLE twin). Closed by: shared engine/CPerlinNoise.h, the existing GFFloatToLongFloor helper (no pragma y-off needed: the
  helper's asm already gives the ebp frame), Ego_d's locals, and the PDB inline GFGetInterpolatedValue with its FableWin /Od-proven
  NAMED body: the extra delta tuples move the scheduling-region cut so the epilogue pops precede the two x87 cleanup fstp
  (0x00F36675). Measured with a 40-variant sweep (probe/v00f36540.py): NAMED + one-expression result matches with fade spellings
  F2/F3/F5; picked `(3.0f - (2.0f * v)) * v * v`. Ego_d's v1_x / v1_y are dead (byte-neutral) and left out (no Fidelity line needed).
- 0x00F36010 NFractal::CPerlinNoise::Get2DNoise (catalog: "CPerlinNoise::GetGaussian2DNoise" -- wrong, NAME_FIXES) -- RELOCATION_MATCH
  + test PASS. Ego_d.pdb's CPerlinNoise class record (types.txt 0x232DF) lists private inline const helpers SCurve, Interp(a,b,t),
  LookupIndex(i), LookupGradient(i) (C3DVector BY VALUE: that is retail's 12-byte frame temporaries), LookupScalar(i); added to the
  overlay header under FABLE_RETAIL_CPERLINNOISE_GETNOISE with bodies sized from their Ego_d /Od lengths. Hash order b00, b10, b01,
  b11; gradients C2DVector from LookupGradient; values via C2DVector::Dot(C2DVector(v_x, v_y)).
  Header grouping chosen jointly over the three CPerlinNoise rows: SCurve `t*t*(3.0f-(2.0f*t))` (left-assoc required by the /Od
  length), Interp `a+((b-a)*t)`.
- 0x00F36540 re-done on those helpers (LookupScalar / Interp / SCurve); still RELOCATION_MATCH.

## Open / escalated
- 0x00F361D0 NFractal::CPerlinNoise::Get3DNoise -- DIFFER(873v873), 6 diff lines, all scheduling: retail 0x00F36303/0x00F3630D
  `fmul st(2)` (SCurve(v0_y)'s two products) sit after `mov eax,esi` / `and eax,0xff`, ours before; retail 0x00F3640F `fsub st(1)`
  (result_z0's outer Interp) precedes gradient001's address arithmetic, ours after. Source differ/00f361d0.cpp (statement order v0,
  v1, b, t; gradients/values in 000,100,010,110 order; C3DVector::Dot default). Tried (sweeps, ~300 grades): Dot opt-ins (all worse),
  statement-block orders (8), gradient/value interleavings (18), parenthesised lines (35), helper groupings (SCurve 9 x Interp 6 x
  Lookup 12). The right-assoc SCurve `t*(t*(3-2t))` gets it to 2 lines but contradicts SCurve's /Od length. ESCALATED.
- 0x007E2DF0 CTCWobble::OnInitialActivate -- RELOCATION_MATCH + test PASS. Ported onto engine/CTCWobble.h (PASS22) with a new
  OnInitialActivate declaration opt-in; ParentRHSet assigned straight from `const CRightHandedSet& GetRHSet()` closed the edi/edx copy
  residue; the second virtual is SetRHSet (+0x12C, slot 75) -- the old draft and the landed FrameUpdate call it SetFacingAnglesFromRHSet.
  GFFloatToLongCeil helper; no pragma.
- Coordinator's double-literal candidate for the CPerlinNoise rows: refuted by one grade (VC7.1 keeps qword constants; retail is dword).

## Open, worked
- 0x00AC7EB0 ComputeSelfContraction -- DIFFER 2 lines: `tex_diff *= tex_diff` (in place, Ego_d has no squared local) fixes the tex term;
  left: normal term retail `fld st(1) ; fmul st(2) ; fmul [ecx+78h]` (n*n then *w), ours n*w*n. Tried: 9 spellings of the term, GFSqr,
  cost split, both squared in place, GetMagnitude opt-ins (5). differ/00ac7eb0.cpp.
- 0x008A8260 GetHeroSummonedCreaturesList -- Ego_d locals applied; residue unchanged (private early-exit epilogue). differ/.
- 0x00B947E0 CEngineSceneMap::Update -- RELOCATION_MATCH + test PASS. New header engine/CEngineSceneMap.h from the Ego_d class record,
  CEngineScene accessor opt-in, GFFloatToLongNear helper, GFGetTopBit via the existing FABLE_RETAIL_GFGETTOPBIT_FORCEINLINE opt-in,
  GFLimit, and `GFGetTopBit(new_size > 1 ? new_size : 1)` (a statement clamp splits new_size and the TopBit copy into two registers).
  Ego_r.exe disassembly (0x00A477E0) has the same shape (useful cross-check: ego_r.exe is VC7.1-like code).
- 0x00A62AE0 CPackedUIntArray::CalculateBias -- RELOCATION_MATCH + test PASS (vland strict). Ego_d local names; declaration order
  src_ptr, src_bytes, min_value, src_val, input_bit_mask is the only one of the 24 (after src_ptr) that homes src_val / c1 /
  input_bit_mask in the dead src_array / src_bits / count slots like retail (no `push ecx` frame). Header opt-in
  FABLE_RETAIL_CPACKEDUINTARRAY_CALCULATEBIAS (ReadNBytes inline + BitMasks). NAME_FIXES: BitMasks v landed g_PackedUIntMasks / PackedBitMasks.
- 0x00F361D0 round 2 (coordinator): right-assoc SCurve breaks siblings; scheduler measurement = region cut (81-tuple regions), retail
  has one fewer no-code PAREN tuple in R1 (sim: SCurve(tx)'s inner (2.0f*t) PAREN); include-order permutations no effect. ESCALATE file updated.
- 0x00F361D0 Get3DNoise -- RELOCATION_MATCH + test PASS (vland strict). Closed by the round-3 DAG read: R1 needs SCurve's PARENs after
  (3-2t) and on the whole product (sim enumeration of PAREN placements; uniform across fades), R2 needs one PAREN fewer per Interp.
  Header bodies `(t*t*(3.0f-2.0f*t))` / `(a+(b-a)*t)` match all three users. 0x00F36010 test helper spelling updated to match.
  Landed rows moved to landed/ (commit 039a0c8c02).
- 0x0063F4B0 ForEachInRoughArea<Bind2 nav-layer> -- PORTED onto shared headers (handQ23's 0x0063F7C0 Bind3 port re-instantiated;
  repo FOREACHINROUGHAREA_BODY). Residue: the loop-exit reload order shared with Q19/Q23 (0x008780D0 / 0x0063F7C0); include-order
  and class-definition-order permutations, plus a guarded y loop with hoisted byte offsets (the landed 0x00518130 recipe) as an
  overlay body: identical code. differ/0063f4b0.cpp.
- 0x00AC7EB0: FableWin (later revision) /Od shows `cost = 0; cost += GFSqr(tex)*W1; cost += GFSqr(normal)*W2` (GFSqr = ?GFSqr@@YIMM@Z,
  shared engine/GFWorldDistance.h); those spellings fold or keep the same n*W*n residue. Include permutations: no effect.
- 0x00C40590 (audio library, no PDB): pointer-local declaration orders and an inline member cross product make it worse; not pursued.
- 0x00BE2120 CEngineInternalPrimitive2DViewport::ClearChildPrimitive -- RELOCATION_MATCH + test PASS (vland strict). The hand2c oracle
  row was cut to 97 bytes (the group twin's length); oracle_override.tsv carries the full 106 bytes read from Fable.exe. Port = the
  landed CEngineInternalPrimitiveGroup::ClearChildPrimitive 0x00BC62D0 body over a new 2DViewport opt-in.
- 0x00A5B650 CCachedFile::Copy -- RELOCATION_MATCH + test PASS (vland strict) on first grade of the shared-header port: the prior
  "result temp slot" residue was the local model; the repo's STLport opt-ins (BACK_INSERTER + COPY_GENERIC_INLINE +
  ORIGINAL_COPY_DISPATCH_PASS22 + COPY_NOINLINE, precedent 0x007CDFE0) give the out-of-line __copy call and the tag slot.
- 0x0083DB60 CThingPhysicalSwitch::GetCurrentGraphic -- RELOCATION_MATCH + test PASS (vland strict), first grade. The PDB signature
  (IBEABVCEngineGraphic: protected, returns a reference) and Ego_d's 53-byte /Od body = `return PeekDefinition().XGraphic;` per arm with
  the repo's PEEKDEFINITION_SAMETU body; the draft's pointer return / explicit def pointer was the residue.
