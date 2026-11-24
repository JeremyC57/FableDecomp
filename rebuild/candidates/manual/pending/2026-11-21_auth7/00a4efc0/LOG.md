# 0x00A4EFC0 -- AUTH7 log (one-shot step; base = S/w22/auth4/00a4efc0/a2.cpp + hdr, copied to ./a2.cpp, ./hdr)
1. Ego_d locals/blocks (egod_locals.py -> egod_locals_typed.txt): every named local, type and block scope equals a2's. No local/block correction.
2. VC10 lane-header compile of a2 (od/a2.sizes): frame differs (STLport CArray 12 B v Ego_d 20, CCharString 4 v 8) -> sizes not comparable.
3. VC10 --stl standalone model (v10/model.h: Ego_d TPI layouts + signatures, CArray<T> : std::vector<T> 20 B; v10/m0.cpp = a2 body):
   frame offsets of all 17 function-scope locals = Ego_d's exactly; total 3823 v 3872. tools/al.py diff:
   (A) Ego_d 1078/1079 = 24/21 B, ours matrix 21 / keyframe 24 -> Ego_d declares keyframe BEFORE matrix.
   (B) Ego_d 1100..1105 = 15 15 15 12 12 15 = six one-object declarations (rotation, position, scaling, paletted rot/pos/scal; 15 = disp32
       lea, 12 = disp8, matching each object's Ego_d offset) v ours two multi-declarator lines (30 / 39).
   (C) Ego_d 1114 = 46 B v ours 60: `rotation_track[c_frame] = keyframe.Rotation;` (no CQuaternion return temp). With C the model's `this`
       moves -552 -> -536 = Ego_d's own this offset (the 16-B GetRotation temp is gone): second, independent confirmation.
   (the loop and store notes first written here were WRONG; corrected in 6-8 below.)
4. m1 = m0 + A + B + C: all lines equal Ego_d except the loop heads (uncalibrated) and 6 register-state stores; this = -536.
5. Advisor (before grading): set not shown complete; check helpers, calibrate loops on a known-text loop, treat stores as block state.
6. Loops: /ZI is NOT it (the -ZI env blocks are the VC7-era LUA / ImeUI libs; the game libraries are -Zi; VC7.1 /ZI changes only
   prolog/epilog anyway). Known-text VC10 loops calibrate exactly (Ego_d std::_Fill_n<float*,unsigned long,float> 12/26/12/6 =
   VC10 12/26/12/6; _Copy_impl<C3DVector*> 12/28/24 = 12/28/24). So +6/+9 is text: (E) the VC6 for-scope idiom
   `if (false) {} else for (...)` (GOTCHAS 442; landed TU-mate 0x00A9D750, 0x007C4430): xor/je/jmp = +6 with a short exit jump,
   +9 with a near one (the five c_frame loops are longer than 127 B, the three c_unique loops shorter). Same +6 on the landed
   0x00A4BEB0's four loops (not my row).
7. m2 = m1 + E on all 8 loops: total 3872 = Ego_d 3872, EVERY line equal (158/158), the six register-state stores included
   (they followed the loop code). The row body is now Ego_d's line for line.
8. Helpers (bodies the VC7.1 row expands; Equals / IsIdentity / InitialiseFromMatrix / Allocate are out-of-line calls in retail, so
   only declarations matter): VC10 (lane headers, overlay) v Ego_d (egod_helpers.txt): equal: C3DVectorWord ctors 68 / 14,
   operator== 86, C3DVector(float,float,float) 42, operator- 90, operator* 82, IsNonZero 91 (9/78/4), GetPosition / GetScaling 31,
   GetByteSize 22/22/19, GetMatrix 28, GetMatrixCount 22, GetBoneType 17, GetSamplesPerSecond 17, GetBoneName 49, SetScaling 34,
   GetMaximumAbsoluteComponent 259, SetupIdentityFlag 97.
   DIFFERENT: (D) C3DVector::GetMagnitude: default arm `(float)sqrt(GetSquaredMagnitude())` = 31 v Ego_d 66 (9/53/4); the family
   `sqrtf(sum)` / `GFRoot(sum)` / `squared` local + sqrtf all give 9/53/4 (`(float)sqrt(sum)` 69): use the existing arm
   FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF (`GFRoot(X*X+Y*Y+Z*Z)`), spelling Inferred inside the family.
   Named exceptions (not in the set): AllocateType<T> (Ego_d has the named 64-bit local and a 261-B debug assert; IFBODY is the
   no-local shape, pending ruling: the local breaks landed users, AUTH2); CCharString::GetLength 35 / GetAsCharArray 38 v ours 46
   (Ego_d's CCharString is 8 B PStringData + CharStringData, Ego_r's 4 B: other revision); CArray / std::copy / std::max bodies
   (Ego_d Dinkumware v retail STLport).
9. PRE-REGISTERED (before any grade): graded text = b2_00a4efc0.cpp = a2 + A + B + C + D + E, header set otherwise unchanged
   (FABLE_RETAIL_C3DKEYFRAME_GETROTATION kept although GetRotation is no longer called). The with/without-GETROTATION pair is the
   step-3 header-set probe: a match that holds only with the unused define is the dependence finding, not land-ready.
   Grades: a2 control (overlay staleness check) + b2, one each; then the order readout once.
10. b2 graded: DIFFER(3491v3491) residue 16, object code byte-identical to a2 (control a2 = 16: overlay current). Order readout: position L=7a5b R=3cea (a2 7a9b/3d0a), still L>R; finishes moved by one symbol (pf 37a1->3781, qf 3681->3661), scaling pal finish 4801->37e1. Not a match.
11. Step-3 probe b3 = b2 minus the unused FABLE_RETAIL_C3DKEYFRAME_GETROTATION define (no code: GetRotation no longer called). Readout + grade once.
    b3 result: readout identical to b2 (pf 3781 qf 3661, L=7a5b R=3cea), DIFFER 16. A removed unused inline definition does not move the fields.
12. Probe b4 = b2 + FABLE_RETAIL_C3DVECTOR_GETMAGNITUDEANDNORMALISE (an unrelated pure declaration, float GetMagnitudeAndNormalise(); in C3DVector). Readout once.
    b4 result: readout identical to b2 (pf 3781 qf 3661, L=7a5b R=3cea). Neither an unused-definition removal nor an unrelated pure declaration moves #2484 / #2493: the tie is NOT header-set dependent (on these probes); only row/helper text moved the fields (b2's set: -1 symbol each).
13. Result: Ego_d-exact row body + Ego_d-consistent helpers = byte-identical object to a2, DIFFER 16. ESCALATE.md written. Grades 3 (a2 control, b2, b3), readouts 4 (a2, b2, b3, b4).
14. After the advisor's check: helper locals (egod_helper_locals.txt) show no named local anywhere, GetMagnitude included (SQRTF fits, SQUARED_LOCAL does not). Callee spellings std::max<float> / std::copy<_Vector_iterator,T*> are in Ego_d. m2 bodies == b3 bodies. The set was complete; no new grade. AllocateType local history cited (AUTH2 LOG 1/21: row 16 with locals; AUTH4 LOG 11). ESCALATE updated.
