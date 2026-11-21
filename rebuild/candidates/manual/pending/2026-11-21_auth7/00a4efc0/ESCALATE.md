# ESCALATE 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData (3491 B) -- AUTH7, 2026-11-21 (one-shot step done)
S = /tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad. Full log: S/w22/auth7/00a4efc0/LOG.md.

## Best source (new): S/w22/auth7/00a4efc0/b3_00a4efc0.cpp, /Gr, FABLE_HDR_OVERLAY=S/w22/auth7/00a4efc0/hdr (= AUTH4's overlay)
DIFFER(3491v3491), residue 16, the same hunk as before (position block: compressed_size < uncompressed_size operand order).
Its object code is byte-identical to a2's. Control: a2 regraded in the same session = 16 (overlay not stale).

## Step 1: measured against Ego_d with VC10 (all evidence, none chosen by readout)
New tool: a VC10 --stl standalone model of the row (S/w22/auth7/00a4efc0/v10/model.h: Ego_d TPI layouts and signatures,
CArray<T> : std::vector<T> 20 B, CCharString 8 B). Every function-scope local's frame offset equals Ego_d's BPREL offset.
The model of the corrected text (v10/m2.cpp) gives 3872 B = Ego_d 3872, and all 158 lines equal Ego_d's line for line.
Corrections, each with its own Ego_d evidence:
- (A) First loop: `keyframe` declared before `matrix`. Ego_d 1078 / 1079 are 24 / 21 B; ours were 21 / 24.
- (B) Six one-object CArray declarations, not two multi-declarator lines. Ego_d 1100..1105 are 15 15 15 12 12 15 B: 15 is a
  disp32 lea and 12 a disp8 one, matching each object's own Ego_d offset.
- (C) `rotation_track[c_frame] = keyframe.Rotation;`, not `= keyframe.GetRotation();`. Ego_d 1114 is 46 B, ours 60. With (C) the
  model's `this` moves from -552 to -536, which is Ego_d's own offset: the 16-B return temporary is gone.
- (D) C3DVector::GetMagnitude through the existing arm FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF (`GFRoot(X*X+Y*Y+Z*Z)`).
  The default arm `(float)sqrt(GetSquaredMagnitude())` is 31 B; Ego_d is 66 (9/53/4). sqrtf(sum), GFRoot(sum) and a squared
  local + sqrtf all give 9/53/4; `(float)sqrt(sum)` gives 69. Spelling is Inferred inside that family.
- (E) The VC6 for-scope idiom on all 8 loops: `if (false) {} else for (...)` (GOTCHAS 442; landed TU-mate 0x00A9D750).
  Ego_d's for lines are +9 where the exit jump is near (the five c_frame loops) and +6 where it is short (the three c_unique
  loops), exactly the idiom's xor/je/jmp. Known-text VC10 loops calibrate exactly (Ego_d std::_Fill_n<float*,ulong,float>
  12/26/12/6; _Copy_impl<C3DVector*> 12/28/24), so the loop codegen itself is right. /ZI was ruled out: those env blocks are
  VC7-era LUA / ImeUI libs. (E) also puts the six +-1/2 B TrackMode / AnimatedFlag stores on Ego_d: those were register
  rotation state, not text.
- Checked against Ego_d, after grading (advisor): the named locals of every expanded helper (egod_helper_locals.txt). None
  has a named local, GetMagnitude included, so the two 66-B arms are separated: SQRTF (no local) fits and SQUARED_LOCAL does
  not. The graded set was therefore complete for helper locals.
  - Parameter names differ only in spelling, with no code effect: Ego_d C3DVectorWord::operator==(rhs) v the overlay's `other`.
    That is a header name fix, not a set member.
  - Callee spellings: Ego_d has std::max<float> and std::copy<_Vector_iterator<..>, T*> for CQuaternion, C3DVectorWord and
    unsigned char, consistent with the text.
  - Sizes equal to Ego_d: the C3DVectorWord ctors / operator==, C3DVector(f,f,f) / operator- / operator*, IsNonZero (ZYX arm),
    GetPosition / GetScaling, GetByteSize, GetMatrix(Count), GetBoneType, GetSamplesPerSecond, GetBoneName, SetScaling,
    GetMaximumAbsoluteComponent, SetupIdentityFlag.
  - Equals, IsIdentity, InitialiseFromMatrix and Allocate are out-of-line calls in retail, so only their declarations matter.
  - The headline "line for line" was measured on v10/m2.cpp; m2's function and helper bodies are textually identical to b3's.
- Named exceptions, not in the set:
  - AllocateType<T>: Ego_d has a parameter `elements`, a block-scope `unsigned __int64 overflow_check` (line 24, 11 B) and a
    261-B debug assert. IFBODY is the no-local shape, pending ruling, because the local breaks landed users (AUTH2).
  - CCharString::GetLength 35 / GetAsCharArray 38 v ours 46: Ego_d's CCharString is 8 B (PStringData + CharStringData), Ego_r's
    is 4 B, so it is another revision.
  - CArray / std::copy / std::max bodies: Ego_d uses Dinkumware, retail STLport.

## Step 2: one set, graded once
b2 = a2 + A + B + C + D + E, pre-registered in LOG before grading: DIFFER(3491v3491) 16, object byte-identical to a2.
Order readout (dump2 + order.py): position L=7a5b R=3cea (a2 7a9b / 3d0a), still L>R, so compressed is evaluated first. The
finish fields moved by one symbol (pf 37a1->3781, qf 3681->3661) and scaling's pal finish from 4801 to 37e1. No flip.

## Step 3: header-set dependence
- b3 = b2 minus the now-unused FABLE_RETAIL_C3DKEYFRAME_GETROTATION define: readout identical, DIFFER 16.
- b4 = b2 + FABLE_RETAIL_C3DVECTOR_GETMAGNITUDEANDNORMALISE, an unrelated pure declaration: readout identical.
- These probes change only what the row never references, so they show no more than that unreferenced declarations are
  not numbered. The brief's condition for the fitted-dead-temporary route (the fields move with the header set) was NOT met.
- What follows: the only freedom left that Ego_d cannot calibrate is in the expanded bodies: the STLport CArray / vector members
  and std::copy (retail STLport v Ego_d Dinkumware), the CCharString getters (another revision), and AllocateType (below).

## State and questions for the advisor
The row text is now Ego_d's line for line in every measurable place, and retail still evaluates the position compare the
other way. The only Ego_d-evidenced difference left in the expanded code is AllocateType's named block local `overflow_check`
(and its parameter name `elements`). It was excluded before grading because AUTH2 measured that a named local breaks the
landed users 00A50510 / 00AA0FA0 / 00BD2D90.
1. AllocateType history: the local is NOT untested. AUTH1's named-local text (`total_size`) and AUTH2's V12 (`const` local)
   / V13 (`count * (u64)sizeof(T)` local) each graded the row at 16 on the old body (S/w22/auth2/00a4efc0/LOG.md items 1 and
   21), and they break 00A50510 (35), 00AA0FA0 (15) and 00BD2D90 (9). AUTH4 logged that the AllocateType shape is what re-draws
   the count word: the default ternary moves it from 0x0380 to 0xf380 (S/w22/auth4/00a4efc0/LOG.md item 11). Is Ego_d's exact
   shape (parameter `elements`, block local `overflow_check`) admissible as a member of the new set (one readout, and a re-fit
   of the three users if it flips), or is it out until those rows are re-fitted?
2. If not, the header-set condition for the fitted-dead-temporary ruling is unmet. Does the row go to the owner, as a proven c2
   order tie with an Ego_d-exact source, or is a fitted text in the uncalibratable bodies (STLport / CArray) admissible?
3. Should b3 (Ego_d-exact, same bytes) replace a2 as the row's recorded best text in any case? I recommend yes.
Budget: grades 3 (a2 control, b2, b3), readouts 4, about 30 VC10 measurements.
Environment: `apt-get install winbind` (ntlm_auth, for VC10 /Zi under wine) was refused by the permission system. It turned
out not to be needed, because /ZI is not in the game libraries' flags. VC10 /Zi stays unavailable on the lane.
