# Hand chunk 3010 (C3DAnimation2 .. C3DAnimation2): 0 of 1 byte-exact

The row starts at its real function start and ends at its own `ret 0Ch` (0x00A4E950, checked by chunks 2014 / 3004 and again here);
the oracle length 1619 is right, no SPLITS.tsv. Every grade / trace used `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent3010/hdr`
(prior/3004 overlay + three body changes, hdr/README.md).

## What matched (0)
None.

## Parked for Fable 5.1 (1)
- 00a4e300 C3DAnimation2::GetDeltaMovement DIFFER(1615v1619): 13 x87 commutative orders only -- product sums (stores 0x00A4E6FB..0x00A4E810) retail (T1+T3)+T2 rows 1-2 / (T2+T1)+T3 row 3, ours (T3+T2)+T1; second norm 0x00A4E578 retail X,W,Z,Y ours W,Z,Y,X | tried: everything in the logs below; session 3 found the exact c2 mechanism (linearizer hash sort on record ids, NOTES 'Continuation session (agent 3)') and a RELOCATION_MATCH configuration (candidate_g04/: SWAP Transpose + GFLimit + SetMultiplyNoTranslation computing then clearing the translation row) whose SetMultiplyNoTranslation text FableWin contradicts | next: coordinator decision on candidate_g04, else find the natural source of ~60..88 extra c2 records (see 'Ranked ideas after session 3')

## Skipped
None.

## Label corrections
None. (The overlay comment on GetNonLoopingDuration says "no out-of-line copy"; retail has it at 0x00A4BE10 -- body proven equal, LABELS.tsv.)

## Names
- Row: ?GetDeltaMovement@C3DAnimation2@@UBEXAAVCMatrix3x4@@MM@Z (Ego_r + FableWin; vtable 0x0129D560; chunks 2014 / 3004).
- Callee unchanged: C3DAnimationSequenceData::GetKeyframeInterpolate(C3DKeyframe&, float) const 0x00A4DEA0 (landed binder).
- Inline helpers proven against retail out-of-line copies this chunk (LABELS.tsv): CMatrix3x4::SetMultiplyNoTranslation 0x00A4B820
  (byte-identical), C3DAnimation2::GetNonLoopingDuration 0x00A4BE10 (relocation-equal), C3DKeyframe::ToMatrix(CMatrix3x4&, bool)
  0x00A52450 (landed; relocation-equal with the header bodies once PreScale(MMM) is forced).
- Descriptive (made-up) names: the parameter / local names (delta, start_time, end_time, duration, start_key, end_key, start_matrix,
  end_matrix, movement) and the Transpose copies e12 / e13 / e23 -- no PDB local names exist.

## Header debt
- hdr/engine/CMatrix3x4.h: Transpose body (upper-triangle copies; the old LOWER / swap variants removed -- only this row used the
  opt-in), PreScale(float,float,float) FABLE_FORCEINLINE (only this row's MULTIPLYNOTRANSLATION opt-in uses it), TransformVector(C3DVector&)
  X row grouped `xx*E11+(yy*E21+zz*E31)` (landed users 0x00A11AE0 / 0x00A11C20 / 0x00A22F80 regraded: RELOCATION_MATCH).
- Everything else as prior/3004 (chunk 2014's opt-ins; FABLE_RETAIL_C3DANIMATIONSTATS_PROTECTED still unproven).

## Ranked untried ideas (predecessor list; see "Ranked ideas after session 2" at the end for the current list)
1. Find WHICH records the reassociation compares (c2 records are pool slots, chunks of 32; the decision runs after all inlining). A
   gdb hook that logs the operand records of each x87 FADD reassociation would turn the search into arithmetic. Static scan of c2.dll
   for low-byte id reads found nothing useful (tmp/notes_log.md).
2. Natural constructs that add ~85 early IL records with no code: candidates not yet measured -- the else branch written through the
   FableWin ctor ??0CMatrix3x4@@QAE@W4EMatrix34Init@@@Z (check its store order first), GetDeltaMovement written with the keyframes'
   GetRotation() / CQuaternion copies (FableWin ?GetRotation@C3DKeyframe in this TU), the movement through the landed sibling's
   statement shape (GetMovementAtTime 0x00A4E150: `C3DVector vector = MovementVector; float fraction = ...; movement += vector * fraction;`).
3. The window is narrow (q24 6 issues, q32 0, q40 1, q48 2 over g03): any candidate must be checked with tools/m.sh (t2 issues) and
   tools/rc.sh before grading.

## Measurements (chronological log)

Overlay: hdr/ = prior/3004/hdr + my changes (hdr/README.md). Tools: tools/ (copied from prior/3004/tools, repathed) +
tools/m.sh (free check: t2 issues + frame-masked / unmasked md.py), tools/rc.sh (c2 symbol-record ids: where the
TransformVector `xx` record sits), tools/prod.sh (condensed product sums from a disassembly), tools/odis.py,
tools/c2h5/ (agent3005 record hooks, repathed). All probes in probe/tr/ (diagnostic), probe/ool/ (out-of-line checks).

## Measurements (free unless marked gNN)
- g01 = prior differ source regraded with my overlay: DIFFER(1609v1619), t2: 13 issues, frame 0x100.
- Line (1) out-of-line proofs: header SetMultiplyNoTranslation == retail COMDAT 0x00A4B820 (240 B, SAME, probe/ool/smn.cpp).
  C3DKeyframe::ToMatrix (overlay GFSqr quaternion body AND the landed 0x00A52450 x,y,z,w-copies body) == retail 0x00A52450
  (447 B, only relocations differ) once PreScale(MMM) is forced inline (diag macro FABLE_DIAG_PRESCALE_FORCE); out of line the
  two quaternion bodies are indistinguishable, and in the row they give identical bytes too (p01 / u7).
- Line (3) THE START-MATRIX SHAPE: retail keeps S11,S12,S13 on the x87 stack, spills S12/S13 at 0x00A4E567 and RELOADS S23
  (`fld [esp+84h]`, 0x00A4E571) before the second ToMatrix = the upper-triangle temps of the transpose
  (`float e12 = E12, e13 = E13, e23 = E23; E12 = E21; E13 = E31; E23 = E32; E21 = e12; E31 = e13; E32 = e23;`) with
  Transpose() called BETWEEN the two ToMatrix calls. probe/tr/u5.cpp: frame 0x104 (= retail), unmasked equal 349/399 (was 160);
  the whole start ToMatrix + spill sequence is retail's. Lower-triangle temps (prior), swap forms u2/u3, copy form u4: worse.
- Remaining after u5: product sums ours (T3+T2)+T1 every row, retail rows 1-2 (T1+T3)+T2, row 3 (T2+T1)+T3 (Tj = m1.Eij * m2.Ejk);
  second norm ours W,Z,Y,X retail X,W,Z,Y (0x00A4E578); TransformVector X row (0x00A4E8A5) ours (xx*E11+zz*E31)+yy*E21,
  retail (zz*E31+yy*E21)+xx*E11.
- These are c2 RECORD-ID ties (r3005): diagnostic `dead = (long)start_time * N;` statements (N distinct, 1 name):
  * outer dead names k=1..48 at block top: no effect on the orders.
  * 46+ dead statements between Transpose and the 2nd ToMatrix (s46/s48) or 48 just before the 1st ToMatrix (pD48): ALL nine
    product sums AND the second norm become retail's; pD48 leaves ONE store issue (TransformVector X row, 0x00A4E8C8).
  * the deciding quantity is the record chunk holding TransformVector's records (rc.sh `xx`): u5 545 bad, 578/610 bad,
    642..802 good, 834+ partly bad. The first/second ToMatrix locals sit at 292/416 for both s44 (bad) and s48 (good).
  * dead code declared BEFORE start_key (pA/pB) shifts the named symbols' ids (start_key 6->7 ...) and breaks everything
    (TI 12) at the same xx=674: the named-symbol ids of start_key / end_key / start_matrix / end_matrix (6/8/10/12) matter too.
  => retail's source allocates ~100..250 more c2 records than u5 between the named locals and TransformVector, through code
     that emits no different instruction (extra inline layers / temporaries / forwarded names).
- g02 (graded): prior source + upper-temp Transpose between the ToMatrix calls + PreScale(MMM) FABLE_FORCEINLINE (header) =
  DIFFER(1613v1619); frame 0x104 = retail; t2 14 issues (all commutative orders).
- Records created AFTER TransformVector count as well: a dead extra `end_key.ToMatrix(diag,false)` after SetTranslateOnly (z1)
  gives every product / second-norm order (only the TransformVector X row left) -- the ordering decision is made after all
  inlining. But z1-type late records flip the FIRST norm (X first) while early IL temps (pD48) keep it.
- TransformVector X row: retail (zz*E31 + yy*E21) + xx*E11 = the header body with the X row grouped `xx*E11+(yy*E21+zz*E31)`
  (exactly TransformPoint's X-row grouping in the same header). Landed users 0x00A11AE0, 0x00A11C20, 0x00A22F80 regraded with it:
  all RELOCATION_MATCH (probe/ool/u_*.cpp / up_*.cpp). Now the overlay default (hdr/engine/CMatrix3x4.h).
- q32 = g02 + grouped X row + 32 diagnostic dead statements before the first ToMatrix: ALL store orders retail's (t2 0 issues,
  frame-masked 391/399 -- the rest is the dead code itself). q24 6 issues, q40 1, q48 2: a window of about +70..+100 early c2
  records over g02. THE ONLY REMAINING LEVER: a natural spelling that allocates ~85 more c2 IL records (c1 temporaries of the
  outer function, allocated before inlining) without changing instructions.
- Natural spellings measured for records (rc.sh xx = TransformVector's record; g02/v1 = 583): COPIES quaternion body 553 (-30),
  GetMovementVector() getter (code changes), named start/end positions 584, `movement` assigned after declaration (code changes),
  per-pair transpose temps 583, swap template 589, upper-temp declaration orders 583, C3DKeyframe user ctor 583, CMatrix3x4 data
  union 583, GetNonLoopingDuration through GetFrameCount/GetSamplesPerSecond 585, /EHsc /GS no change, a preceding function in
  the TU no change (records reset per function), visible GetKeyframeInterpolate stub (code changes), inverted identity test
  (layout changes), plain-inline C3DKeyframe::ToMatrix with /d2inlT1000 (same as forced).

## Continuation session (agent 2) -- measurements (free unless marked gNN)
Tools added: tools/c2h6/ (recs2.py = per-run record dump deduped by chunk; tup.py / tup2.py = IL tuple dump with operand records;
watch.py), tools/rf.sh <srcs> (parallel hooked compile -> retail-labelled term orders "N1 N2 0 4 8 c 10 14 18 1c 20" + md.py
summary; target = all 1234 / 123), tools/tord2.py (term orders from an obj), tools/fsum.py <tags> (field-record ids of
start/end matrix and keyframe rotation fields), tools/fld.py, tools/mkv.py (variant by exact replacement), tools/hv.py +
tools/rfh.sh (header-overlay variants under tmp/hv/<name>), tools/gengrid.py (DIAGNOSTIC dead-statement grids, probe/grid/).
- Field records (parent +8, offset +0x24, size +0x20, flag byte 6 bit 1) of start_matrix / end_matrix / keyframes are created
  LATE (ids ~590..810, after TransformVector's locals) in field+remainder pairs; only the offset-0 fields get early ids.
- x87 lowering happens BEFORE c2's per-function pipe (0x1077C08E): at pipe entry the tuples are already fld/fmul/faddp
  (hooks at 0x1077dfd0 / 0x1077c367 / 0x10780353 / 0x107821a7 all show lowered IL). A generic add node (opcode 0x278) becomes
  fadd at c2 0x107101D4 (x87 stack model, 0x108A7CA4) called from the tree walker 0x10772B96 with callback 0x1075AC21.
  The comparator itself was NOT found (node memory is reused many times; watch logs in tmp/w1.json, w2.json).
- Dead statements AFTER SetTranslateOnly (g_end_0_k, probe/grid) work exactly like early ones: k=0..24 partial
  (k>=8: N2 + row 3 right), k=28..40 ALL retail orders, k>=48 N1 flips (monotonic up to 112, no periodicity). Late dead code
  even shifts the EARLY records (start_matrix E11 209 -> 242 -> 274): c2 allocates records for the whole function's IL before
  inlining, so only the TOTAL matters: about +2 record chunks (64 records; start_matrix E12 field id 596 -> 661..692).
- No simple id relation fits (end-field mod 256, start-field mod 256, max/min/sum: all fail on g_end_0_28 or q24).
- Natural code-neutral spellings measured (no order change, record shift 0..+4): GFLimit(start_time, 0.0f, duration) x2
  (FableWin/Ego_r ??$GFLimit@MMM@@YIXAAMABM1@Z; +4), named start/end positions, `float fraction` / `C3DVector offset` locals,
  direct-init movement, declaration orders (matrices first, all at function top, end_matrix at its use, end_key after the first
  lookup, end before start), `else delta.SetIdentity();` without braces, keyframe array + references, quaternion body variants
  (if/else s, x/y/z/w copies with GFSqr, copies used in the products, SetTranslateOnly for the translation), /Zi, /Z7.
  Code-changing: GetNonLoopingDuration called in the clamps (b5: 1618B, N2 + row 3 fixed), the sibling's movement block (b4),
  matrix copies for the inverse (x1..x3), `rotation` temp (x4), matrix array (k2), `const C3DAnimationSequence* track` (n5).
- SetMultiplyNoTranslation computed into 9 locals first: out of line 294 v retail 240 bytes -> rejected (retail = direct stores).
- FableWin debug sizes (VC7.1 /Od /RTC1 of our texts in brackets): GetDeltaMovement 0x240 [0x263], CQuaternion::ToMatrix 0x220
  [0x1c9], SetMultiplyNoTranslation 0x230 [0x1c9], PreScale(MMM) 0xc0 [0x99], C3DKeyframe::ToMatrix 0x70 [0x68], GetPosition 0x30
  [0x30], GetKeyframeInterpolate(float) 0x30 [0x30]. FableWin's GetDeltaMovement is SMALLER than ours although VC10 debug code is
  usually larger, and its GetNonLoopingDuration COMDAT appears long after GetDeltaMovement (0x03229480): the FableWin version
  probably does not call GetNonLoopingDuration (a later source revision, or the TLC duration was spelled differently).
- KEY NEW FACT: the deciding quantity is NOT the record count. 32 late stores of constants to a global array (no temporaries,
  probe/grid/h_gst_32.cpp) or 32 dead integer statements (h_iadd_32) give ALL retail orders; 16 give N2 + row 3 only; 32 dead float
  multiplies (real code, h_fdead_32) only partial. A header body with very different record ids (CQuaternion::ToMatrix through
  CMatrix3x4::Set(12 floats), overlay tmp/hv/qs: code-neutral, 1615B/353 equal) changes NOTHING in the orders, and 33 combinations
  of the natural code-neutral spellings above with the qs / qc / qe quaternion bodies all keep the baseline orders (tools/combo.sh).
  => the orders follow the IL statement / tuple count of the whole function (cf. GOTCHAS 0x00B6FA40 "flip point moves with the IL
  tuple count, about every 280 units"), not symbol records; about +32 simple statements anywhere are needed, +48 flips N1.
- Dead stores that c2 removes only late DO count: `start_matrix.SetIdentity(); end_matrix.SetIdentity();` right after the
  declarations (probe/n/i1.cpp) stays 1615B/397i (all dead) and moves the orders (N2 right; products a NEW pattern 321 / 231, N1
  flips). Variants i2 (start only: N1 + N2 right, products 321), i4 (end only, code changes), i5, and i1/i2/i5 x qs/qc/qe bodies
  (qs__i2: rows 3 right, N1 wrong, 1619B) -- none all-retail yet. This is the most promising natural lever found: identity-
  initialised matrices (FableWin ??0CMatrix3x4@@QAE@W4EMatrix34Init@@@Z exists; its body / enum not yet in the repo).
- Else-branch statements count too (32 constant stores after `delta.SetIdentity()`, probe/grid/h_else_32.cpp: all retail orders;
  16: N2 + row 3 only). Position in the function does not matter; only the amount of IL the function carries at x87 lowering
  (dead stores and dead statements included) does.

## Ranked ideas after session 2 (for the next continuation)
1. Find the comparator in c2 now that the lever is known to be the IL statement / tuple count, not symbol records: hook the x87
   lowering walk (tree walker 0x10772B96, callback 0x1075AC21, fadd creation 0x107101D4) for the first product sum (store
   0x00A4E6FB) and log what it compares for the two add operands (tuple numbers? node addresses? SU numbers?) in g03 v
   probe/grid/h_gst_32.cpp. Heap addresses are deterministic per source, so tuple addresses from one run (tools/c2h6/tup2.py with
   "b0":"bb5b64" at the pipe hook) can be watched in the next (tools/c2h6/watch.py). With the exact key, derive how many tuples are
   needed and where.
2. Natural sources of ~32 extra IL statements that leave no code: identity-initialised matrices (FableWin
   ??0CMatrix3x4@@QAE@W4EMatrix34Init@@@Z; probe/n/i1, i2: code-neutral, N2 right, products move to a new pattern) combined with a
   second natural IL source; dead translation stores already exist in both ToMatrix expansions (E41..E43).
3. FableWin's GetDeltaMovement is ~20% smaller in debug than our text would be (VC10 debug is usually ~20% larger than VC7.1 debug
   for these headers: SetMultiplyNoTranslation +22%, PreScale +25%): FableWin's revision probably lacks the clamps or the
   GetNonLoopingDuration call -- do not use FableWin sizes as evidence for the TLC statement list.
4. Do NOT spend more probes on small spellings: 60+ natural code-neutral variants (single and combined, four quaternion bodies)
   never moved an order; only changes worth ~16+ simple statements do.

## Continuation session (agent 3) -- THE COMPARATOR FOUND (free hooked probes, no grade)
Tools: tools/c2h7/ (run.sh <script> <abs src> <abs out.json> '<cfg>'; target must be "?Get@?$CCountedPointer" = the function c2
compiles just before GetDeltaMovement, because every phase below runs BEFORE GetDeltaMovement's own pipe event): bt.py (stack at
a hook), dl.py (tuple-list dump at hooks), raw.py, wt.py (write watchpoints + marks), cmp.py (merge-sort comparator calls),
key.py (every tuple key computed at 0x1075CD1D) + ana.py (per-sum product leaf ids / hashes / order) + kk.sh <overlay> <srcs..>.
- Constructor line: FableWin.exe.signatures.tsv sizes prove ??0C3DKeyframe@@QAE@XZ (44 B / 16 instr = three member-ctor calls),
  ??0CQuaternion / ??0C4DVector / ??0CMatrix3x4@@QAE@XZ / ??0C3DVector@@QAE@XZ (14 B / 8 instr each) are EMPTY: no dead ctor
  stores exist. Only ??0CMatrix3x4@@QAE@W4EMatrix34Init@@@Z (214 B / 83 instr: two 12-store blocks behind an enum test) and
  SetIdentity (106 B / 43 instr = 12 separate stores) store; enum enumerator names unknown, no retail out-of-line copy.
  Probes probe/c/m{1..4}{b,s,e}.cpp (overlay tmp/hv/ci, DIAG ctor bodies): m2s/m3s/m4s (12 separate stores, start only) give
  N1 + N2 retail, products unchanged (231/312); none all-retail. b = both matrices flips N1; e = code changes (1621B).
- MECHANISM (c2 per-function driver 0x107AFFxx: 0x107722E8, 0x10760539, 0x1075F923, 0x10770532 (x87 lowering 0x10773407), ...):
  inside 0x10760539 the tree linearizer 0x1075FC1C (call at 0x107606ED) canonicalises every commutative tuple (ops 0x278..0x28A):
  0x1075CD04 merge-sorts the operand list (0x1076A2F3, comparator 0x1076A394 = DESCENDING unsigned key at operand+0x10) after an
  n-ary flatten of add chains, then stores the tuple's own key (0x1075D388) at tuple+0x10:
    key(tuple) = (SU need << 24) | (operand count << 16) | hash16;  hash16(tuple) = sum_i(hash16(op_i) << (i & 7)) + (opcode - 0x24A)
    (operands in sorted order); leaf symbol operand: hash16 = rotl16(record id, 5) (= id*32 for id < 2048), temps (record kind 3)
    id << 6, a temp with a single def hashes as its def tuple; flag 1<<16 for these field-record leaves.
  So a product S*E of two field records has hash (32*max_id + 64*min_id + 48) mod 65536 and every 3-term sum is evaluated in
  DESCENDING hash order; a square X*X hashes 96*id + 48. The 16-bit WRAP is why the orders look chaotic.
- Base ids (key_base): start_key X,Y,Z,W = 208,590,592,594; end_key 367,661,663,665; S11=209 S12=600 S13=606 S21=22(e12)
  S22=602 S23=640 S31=24(e13) S32=26(e23) S33=642; E11=368 E12=667 E13=669 E21=671 E22=673 E23=675 E31=677 E32=679 E33=681.
  The late field records (590..681) are created during this linearizer; their ids depend on the record allocator's free-list
  state; gst_32 = every id >= 100 shifted +64 (e12/e13/e23 = 22/24/26 fixed).
- MODEL (tools/c2h7/model.py, reproduces base exactly): shifting all late field records (ids >= 500) or all ids >= 100 by
  +59..+88 gives ALL 13 retail orders; +89.. flips N1 only; < +58 keeps ours. So retail = our source with ~60..88 more c2 records
  allocated (high-water) before the linearizer creates the field records, keeping their relative order.
- PROOF probe (diagnostic, NOT deliverable): 8 or 16 dead locals `float ddN = X*N.0f;` at the top of CQuaternion::ToMatrix
  (overlay tmp/hv/qd8, qd16) -> 1619B/399i, ALL 399 instructions equal unmasked (the 4-byte size gap closes too); 24 flips N1.
- FableWin debug instruction counts (signatures.tsv col 5) v VC7.1 /Od of our header bodies (tools/dbgsize.py, tmp/dbg):
  equal: C3DVector(MMM) 17, += 26, SetTranslateOnly 19, TransformVector 55, GetPosition 13, PreScale(MMM) 52, all ctors;
  VC10 artefacts: float returns +4 (GFSqr, GetNonLoopingDuration), by-value C3DVector returns +3/+6;
  REAL differences: Transpose FW 22 v ours 37 = three calls of FableWin ??$SWAP@M@@YIXAAM0@Z (template<class T> void __fastcall
  SWAP(T&,T&), 18 instr = `T temp = a; a = b; b = temp;`); SetIdentity FW 43 (12 separate stores) v ours 31 (chained);
  CQuaternion::ToMatrix FW 150 v 142; SetMultiplyNoTranslation FW 160 v 157; GetDeltaMovement FW 119 v ours 129 (GFLimit
  clamps would give ~111+artefacts). FableWin's GetDeltaMovement does NOT reference GetNonLoopingDuration (its COMDAT is emitted
  far later, 0x03229480; debug COMDATs follow their first user in LIFO order, cf. GetMovementOffsetAtTime -> GetPosition,
  GetKeyframeInterpolate).
- Natural record shifts measured (tools/rfi.sh: orders + end_key/start_key W ids; base 665/594; target ~+60..88 uniform):
  SWAP Transpose (tmp/hv/sw1) +6/+6, code-identical to base (1615B, 353 equal, spill shape kept); sw1+GFLimit +10/-22;
  sw1 + Dot(*this) norm +0/+0; 12 separate SetIdentity stores (else branch) +0; identity-ctor bodies on start/both matrices
  (probe/c2/*, overlay tmp/hv/swci, with/without GFLimit): non-uniform (start fields jump), none all-retail.
- WHERE the extra records may go (tools/c2h7/model3.py, single insertion of n records before base id t): before any late id
  (anywhere before the linearizer, or in the outer IL): n = 59..88; between the start_key fields (590..594) and the S fields
  (600): n = 59..167; later: impossible. DIAGNOSTIC proofs (all 1619B/399i, every instruction equal unmasked, NOT deliverable):
  8/16 dead locals in CQuaternion::ToMatrix (qd8/qd16), 8/16 in SetMultiplyNoTranslation (dm8/dm16), 8 in Transpose (dt8).
- NATURAL, FableWin-consistent spellings found (dbgsize /Od instruction counts v FableWin signatures):
  * Transpose = `SWAP(E12, E21); SWAP(E13, E31); SWAP(E23, E32);` with `template<class T> inline void FABLE_FASTCALL
    SWAP(T& a, T& b) { T temp = a; a = b; b = temp; }`: /Od Transpose 22 + SWAP<float> 18 = FableWin 22 / 18 EXACTLY (FableWin
    ??$SWAP@M@@YIXAAM0@Z + ?Transpose@CMatrix3x4@@QAEXXZ). In the row: code-identical to the e12/e13/e23 body (1615B, spill shape
    kept), records +6 (overlay tmp/hv/sw1).
  * clamps as `GFLimit(start_time, 0.0f, duration); GFLimit(end_time, 0.0f, duration);` (engine/GFLimit.h, Ego_r + FableWin
    ??$GFLimit@MMM@@YIXAAMABM1@Z): /Od GetDeltaMovement 113 (+6 by-value GetPosition artefacts = FableWin 119; the if-chains give
    129); code-identical in the row.
  * NOT FableWin-consistent but natural-looking: SetMultiplyNoTranslation = the full 12-sum SetMultiply body followed by
    `E41 = E42 = E43 = 0.0f;` (tmp/hv/smt: /Od 211 v FableWin 160). With SWAP + GFLimit (probe/c3/gfl.cpp under tmp/hv/smt):
    ALL 13 orders retail, 1619B/399i, 399/399 equal (hooked compile, not graded) -- the first complete match with only natural
    statements, but the smt body contradicts FableWin's debug size and is unproven against 0x00A4B820 out of line.
- g04 (graded): probe/g04.cpp (= probe/c3/gfl.cpp, GFLimit clamps) under overlay tmp/hv/smt (SWAP Transpose + SetMultiplyNoTranslation
  = the 12-sum SetMultiply text then `E41 = E42 = E43 = 0.0f;`) = RELOCATION_MATCH. Also all 399 equal (hooked, not graded): tmp/hv/smc
  (SetMultiplyNoTranslation = `SetMultiply(m1, m2); E41 = E42 = E43 = 0.0f;`, needs FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY) + GFLimit.
  Both bodies are byte-identical out of line with retail 0x00A4B820 (tools/oolcmp.py probe/ool/smn.cpp: 240 SAME) BUT FableWin
  contradicts them: FW SetMultiplyNoTranslation = 160 /Od instr (our 9-sum body 157, smt 211, smc ~15) and FW SetMultiply = 205
  (standalone 12 sums) -- in FableWin both are independent bodies. Without GFLimit both leave rows 1-2 wrong (312).

## Ranked ideas after session 3 (for the next continuation / the coordinator)
1. DECIDE candidate_g04 (g06 RELOCATION_MATCH, test PASS g++ + VC7.1): its only questionable piece is the SetMultiplyNoTranslation text
   (translation row computed like SetMultiply, then `E41 = E42 = E43 = 0.0f;`). For: byte-identical with retail's out-of-line COMDAT
   0x00A4B820; `SetMultiply(m1, m2); E41 = E42 = E43 = 0.0f;` (tmp/hv/smc, needs FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY) gives the same
   399/399. Against: FableWin's debug SetMultiplyNoTranslation is 160 /Od instructions (9-sum body 157 + float artefact; smt 211; smc ~15)
   and its SetMultiply (205) is a separate 12-sum body. FableWin is a later (VC10) revision, so this is evidence, not proof.
2. The needed effect, exactly: ~+60..88 c2 records allocated before the linearizer (or +59..167 between the start_key quaternion field
   records and the start_matrix field records), keeping the late field records' relative order; check any idea with tools/rfi.sh
   (end_key W id: base 665, target ~725..753) and confirm with tools/c2h7/kk.sh + model.py. Natural spellings seen so far move it by
   -30..+13 only (tmp/grid5.txt: 72 combos of GFLimit/if, named positions, GetTranslation, GetFrameCount/GetSamplesPerSecond duration,
   three quaternion bodies, chained/separate SetIdentity -- all keep base orders).
3. Untried natural record sources: a TLC-era CCountedPointer / ASSERT macro that leaves IL in release (FableWin's CCountedPointer Get /
   operator-> are 22 / 26 debug instructions v ours 9: asserts in that revision); the quaternion conversion written through more inline
   layers (FableWin CQuaternion::ToMatrix 150 v ours 142 /Od, partly float-argument artefacts); the C4DVector -> C3DVector conversions.
4. Do NOT spend compiles on small spellings: they move ids by a few records; only constructs worth ~20+ statements of IL matter.
