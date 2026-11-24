# 0x00A4EFC0 -- AUTH2 log (continues S/w22/auth1/00a4efc0/LOG.md; overlay S/w22/auth2/00a4efc0/hdr = copy of AUTH1's, add-only v HEAD 8e771c0ec0)
1. AllocateType opt-in test (advisor): regrade landed AllocateType users with FABLE_RETAIL_CSIMPLEMEMORYPOOL_ALLOCATETYPE_EGOD (+ COND false),
   (a) row config as landed, (b) + ASSERT_DEAD_ARM. Rows: 00a4c5e0 (5 uses), 00a50510 (3), 00aa0fa0 (2), 00a4fe60, 00bd2d90.
   RESULT (AUTH1 text, local total_size): (a) 00a4c5e0 MATCH, 00a50510 DIFFER 35, 00aa0fa0 DIFFER 15 (reloc addends), 00a4fe60 MATCH,
   00bd2d90 DIFFER 9 (FP operand order); (b) 00a4c5e0 M, 00a50510 1757v1748, 00aa0fa0 M, 00a4fe60 M, 00bd2d90 9. -> FITTED, rejected.
   (Overlay fix: the opt-in now includes fable_assert.h itself; before, every user without its own include failed C3861.)
2. V1 = same shape without the named local (`if ((u64)count*sizeof(T) > 0xffffffffUL)`): row 3491v3491 residue 16 (fix of (3) kept);
   users (a): all MATCH except 00a50510 residue 10 (FP operand order, 2 hunks); (b): 00a50510 1757v1748 (others MATCH).
   Note: rows without DEAD_ARM are judged in config (a); (b) for 00a50510 grows 9 B under any if-shaped body.
3. V3 (V1 without the assert line) row 26 (fix of (3) LOST: the dead arm inside AllocateType is what fixes it), 00a50510(a) 10;
   V5 (`if (count) return <ternary>; return 0;`) row 26, 00a50510 10; V7 (`if (!count) return 0; if (...) {ASSERT; return 0;} return ...`)
   row 3507v3491 53, 00a50510 1763v1748. => 00a50510 (config a) holds only for the default `count != 0 ? ... : 0` shape; the row needs the
   dead arm. Next: default ternary + a separate dead assert statement (config (a) users are token-equal to the default + `((void)0);`).
4. V10 (default ternary preceded by a separate dead assert statement): row 26, 00a50510 MATCH. V11 (= default text via the opt-in) row 26.
5. V1 on the other class-branch users (config a): 00aa0090, 00aa2110, 00a9eb90, 00a9f540 all MATCH. CParticleProgram 00ae04e0/0540/05a0 use
   the flattened default (no FABLE_RETAIL_CSIMPLEMEMORYPOOL): not users of this text. => V1 holds for 8 of 9; the one break, 00a50510,
   records in its own header that its x87 operand order is c2 candidate-order FITTED (named loop bone, separate ++data_index, ...),
   and it breaks under EVERY if-shaped body (V1/V3/V5/V7/AUTH1), i.e. it is fitted to the one-line ternary, which Ego_d's 7-line table
   (21..34) contradicts. V1 itself lacks Ego_d's separate 11/16-B total_size line.
6. Next: Ego_d-consistent local spellings that c1 may fold (const local), on row + 00a50510/00aa0fa0/00bd2d90.
   RESULT: V12 (`const` local) and V13 (`count * (u64)sizeof(T)` local, `> 0xffffffff`) both: row 16, 00a50510 35, 00aa0fa0 15, 00bd2d90 9 --
   any named local breaks 00bd2d90, whose header says its x87 order is a count of live symbols that Ego_d's own local set fixes (not fitted).
   => the users exclude a named local; V1 is the text that holds for every non-fitted user.
7. Dead-arm condition under V1 (row only): cond false / u64 size test / !"..." to see if the condition moves bytes.
   RESULT: both 3491v3491 residue 16 = the condition is byte-neutral for the row (false / u64 test / !"...").
8. /Od calibration of every expanded helper (S/w22/auth2/odall.py, mini od/h1.cpp, VC7.1 /Od): SetupIdentityFlag 7/54/15/2/15/4 = Ego_d
   EXACTLY (so Ego_d's compiler/flags give VC7.1 /Od sizes: same 7-B prolog, 4-B epilog). One-line header bodies lump on one line in
   VC7.1, so their check is the total: ours GetBoneName 31 (Ego_d 16+27+6=49), GetMatrix 28 (Ego_d 28 = 7+15+6 OK), GetMatrixCount 22
   (Ego_d 22 OK), GetBoneType/GetSamplesPerSecond 17, GetPosition/GetScaling 31 (Ego_d 31 OK), GetRotation 44 (OK), SetScaling 30 (Ego_d 34),
   GetMaximumAbsoluteComponent 9/212/6 (Ego_d 9/244/6), C3DVectorWord() 14, (const C3DVector&) 68, == 82, GetByteSize 22.
   AllocateType V1: if 6 (Ego_d 10: near je over its 261-B arm), test-with-multiply 19/28 (Ego_d local 11/16 + test 16), arm 56, ret 4/14/17/2 OK.
   FAILS: GetBoneName (-18), SetScaling (-4), GetMaximumAbsoluteComponent (-32), AllocateType local/test lines.
9. Ego_d's compiler: S_COMPILE3 records give cl 16.00.40219 (VS2010) for 1,292 modules (13.10.3077 for 14, 13.00.9466 for 22) -> Ego_d is
   a VC10 debug build of a later revision; its line sizes are VC10 /Od, not VC7.1 /Od. There is no VC10 in the lane, so the four helper
   deltas (GetBoneName 49 v 47 with /EHsc, 31 without; SetScaling 34 v 30; C3DVectorWord::operator== 86 v 82; GetMaximumAbsoluteComponent
   body 244 v 212) cannot be shown to be text failures: each is a few bytes per statement, the size VC10 /Od codegen changes (float
   constant args via fldz, ternary temps). Variants measured at VC7.1 /Od (od/h2.cpp): GetBoneName x5 (31/31/31/53/38; /EHsc 47/47/47/144/54),
   SetScaling x6 (30/30/30/57/56/39), operator== x4 (82/71/82/80), GetMaximumAbsoluteComponent x4 (all 227): none reaches Ego_d's size,
   so no helper text change is indicated by calibration; no helper was changed (a change only to move c2's count would be a device).
10. Final text: overlay opt-in = V1 (no local, FABLE_RETAIL_ASSERT(false)); a2.cpp = e1 without the COND macro: row DIFFER(3491v3491) 16;
    00bd2d90 MATCH, 00aa0fa0 MATCH, 00a50510 DIFFER 10 (its fitted x87 order). ESCALATE.md written.
