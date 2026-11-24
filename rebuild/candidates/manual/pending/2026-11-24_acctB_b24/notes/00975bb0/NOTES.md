# 0x00975BB0 CAIStateGroup_JackOfBladesStage1::Update -- agent B24A5 notes (2026-11-24)
Lessons that apply (read first): Codex pass 21 (pending/2026-11-11_codex_pass21/return/parked/00975bb0/PROBES.md, 48 probes A01..A48,
EXTENT.md); c2 commutative-operand order is symbol-number driven (pending/2026-11-20_c2_symbol_order/README.md); C3DVector.h carries a
row-specific IsNonZero text under FABLE_RETAIL_JACK_STAGE1_UPDATE (line ~813) and GFSqr/GFFastOneOverSqrt union opt-ins.
- ORACLE EXTENT BLOCKER (coordinator action): the registered oracle is 4707 bytes (rows.tsv 4720) and runs past the function into
  0x00976DC0.. (next functions). Codex EXTENT.md and the listing agree: the function is 4624 bytes = 4606 code + 2 alignment
  (`mov edi,edi` 0x00976DAE) + 16-byte switch table 0x00976DB0..0x00976DBF. Even a byte-exact source grades DIFFER(4624v4707) until the
  oracle is trimmed to 4624 bytes.
- Baseline regrade (v0 = differ/ = Codex draft): DIFFER(4612v4707) 17 hunks. Ours = 4596 code + 16 table (no 2-byte pad): 10 code bytes
  short. Real residue (x87 only, all integer code equal):
  1. 0x00976153..0x00976189 (IsNonZero + Normalise of vector_to_target): retail squares z, then y ([esp+34h]) then x ([esp+50h]); ours
     z, x, y; then retail `fxch st(1); fadd st(2); fadd st(1)` for Normalise's sum v ours `fadd st(2); fadd st(1)`.
  2. 0x009761EA: retail stores the Normalise factor with fstp and reloads it for each product (`fstp [14h]; fld [50h]; fmul [14h];
     fld [34h]; fmul [14h]`), ours keeps it (`fst [14h]; fmul [50h]; fld [14h]; fmul [34h]`): 4 bytes.
  3. 0x00976491..0x009764AD (combat-loop Normalise of dir_to_target): store slots 0x78/0x20/0x48 order and the same fstp-reload v fst.
  4. 0x00975C45 / 0x00976421 pairs print equal bytes (relocation-masked noise).
- B24A5 grades: IsNonZero existing opt-in texts ISNONZERO_ORIGINAL_YXZ / ISNONZERO_ZYX / NONZERO_SQUARED_XYZ on top of v0: all
  DIFFER(4608v4707) 19 hunks (worse; the JACK row text stays).
- Devices still in the draft (must go before hand-back): FABLE_FORCEINLINE GET_ROUNDED_ANGLE (Inline-budget line 3), the row-specific
  IsNonZero header text (one text per class rule). Next idea: read the retail factor-reload as Normalise writing `X*=f; Y*=f; Z*=f` through
  a by-value copy (Normalise returns C3DVector& and the caller assigns `v = v.Normalise()`): the reload of f per product suggests f lives
  in memory across the member updates (aliasing through `this`), i.e. a Normalise that is NOT inlined into a local copy -- test
  `vector_to_target.Normalise();` on a heap/pointer-held vector or GetAngleXY of `Normalise()`'s returned reference directly.
- B24A5: + existing opt-in FABLE_RETAIL_C3DVECTOR_NORMALISE_GROUPED_SUM -> DIFFER(4612v4707) 15 hunks (the shr/fstp interleave hunk
  goes; best, differ/ updated). FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23 (GFSqr parenthesised): 17 (no change).
  Remaining: square order z,y,x (retail) v z,x,y (ours) feeding both sums, the factor fstp-reload, the combat-loop store slots.
- Constraint for the next idea: BOTH Normalise sites (vector_to_target 0x009761EA and dir_to_target 0x00976491..) show retail's
  fstp+reload against our fst: the fix must be one Normalise text that fixes both sites; a variant fixing one site only is a caller fit.

# B24A6 (2026-11-24) -- read this first
Lessons that apply (top): (1) c2 orders commutative x87 operands by symbol number (pending/2026-11-20_c2_symbol_order); the IL kit is set
up IN THIS ROW DIR: `./il.sh <variant.cpp>` (dump2 + symlog under c2dbg, ~80 s) then `./ilsum.sh <variant>` / tmp/lpool_<v>.txt /
tmp/pp_<v>.txt (FN filter patched into tools/il/pp2.py, lpool.py). (2) Inline-callee locals are numbered in FIRST-USE order and take
freed numbers (freed k5 params such as `_this`) LIFO first. (3) A TU-context probe (40-local dummy function before Update) changed
nothing: the numbering is per function -> the residue is ROW TEXT, not compile order (no ESCALATE).
- Oracle re-cut: baseline regrade DIFFER(4612v4624) 14 hunks.
- fwsize (FW_EXTRA=/EHsc, fwsize_base.txt): IsNonZero JACK refs text 39i v FableWin 31i (one expression) -> the JACK text is unevidenced.
  Normalise: no header text gives FableWin 48i/133b (LOCALS/LOCALS_YZX 47i/130b, GFSqr forms 43i/118b; float helpers in FableWin run
  +1i/+3b vs plain /Od generally -- e.g. landed AccurateNormalise text 48i/136b v FableWin 50i/137b -- so FableWin float sizes are weak).
- SITE 1 SOLVED (0x00976137..0x0097626D): the factor `fst` v retail `fstp`+reload was f = one_on_magnitude #1419 > X field #1416 / y
  #1414 (E*v loads the higher symbol first). The JACK IsNonZero refs (x,y,z k4 locals) ate the free number #1412; with a local-free
  IsNonZero, f (allocated first in the GFSqr Normalise) takes #1412 -> X, Y load first, fstp+reload as retail. Then the products must be
  CSE'd but the sums not: flat IsNonZero CSEs the whole sum; the bracket `Y*Y+(Z*Z)+X*X` (new opt-in ISNONZERO_YZX_BRACKET, hdr/README)
  gives retail's z,y,x products, both (YY+ZZ)+XX sums and pops. Normalise must be the plain SQUARE_HELPER text
  (GFSqr(X)+GFSqr(Y)+GFSqr(Z), NOT GROUPED_SUM): best.cpp = differ/ = DIFFER(4624v4624) 9 hunks, all at site 2 + noise.
- SITE 2 OPEN (0x00976483..0x009764E1, GetMagnitude of line_to_target + Normalise of dir_to_target): retail products X->[78], Z->[20],
  Y->[48], both sums XX+YY+ZZ; ours (MAGNITUDE_SQRTF GFRoot) X->[20], Z->[48], Y kept (fst [78]), sums YY+ZZ+XX. Tried on best:
  GetMagnitude 26i texts XY_THEN_Z (8 hunks, right size, sums YY+XX+ZZ / ZZ+YY+XX), SQUARED_LOCAL, NAMED_SQUARED (9), X_THEN_YZ (187),
  named-squared + brackets B1..B6 and term orders B7..B9 (flat orders identical: sorted). IL (m2): product temps XX #922 (key e680),
  YY #926 (e780), ZZ #779 (free-list) -> sum IL (YY+XX)+ZZ. Next idea: measure best with ./il.sh, then shift the product-temp / field
  numbers with an EVIDENCED helper text before 0x00976466 (fwsize DIFFs: GetTC<T> 13i v FableWin 27i/74b = GETTC_ORIGINAL_CHAIN /
  PEEKTC_ORIGINAL_CHAIN texts; IsStillRunnable 28i v 27i; CIntelligentPointer(T*) 25i v 23i; Release / GetSubDef +1i; vector
  operators -/+/* 28i v 34i = *_NAMED_RESULT opt-ins (SUB alone / ADD+SCALE alone neutral on the old base; all three moved the frame
  +4 with the JACK IsNonZero, but with the bracket IsNonZero c2 = i4+all-named kept the frame)), as 0x00B075D0 was closed.
- Devices still in the source: GET_ROUNDED_ANGLE FABLE_FORCEINLINE via FABLE_RETAIL_JACK_STAGE1_UPDATE (GetAngleXY + GFSqr formal `n`),
  must be evidenced or go (generic FABLE_RETAIL_GFANGLES_GET_ROUNDED_ANGLE_FORCEINLINE exists for wfA2 row 0x0068CB90).
- B24A6 last grades: + named-result vector operators on best: all three 13 / SUB alone 13 (break site 1) / SCALE alone 9 (neutral). Session ended here for Gate 0 (coordinator order).

# B24A7 (2026-11-24)
- SITE 2 MECHANISM FOUND: the sum order is c2's sort of the three CSE product TEMPS (k3 pool, key = temp# << 6, low 16 bits = temp#
  mod 1024). Measured with ./il2.sh (dump2 + pp2 only, ~2 min; prints product temps per line). best (b0): site 1 temps 606/819/823,
  site 2 XX 779 (free-list) / ZZ 924 / YY 928 -> sorted YY,ZZ,XX. Retail XX+YY+ZZ needs fresh ZZ,YY >= 1024 (wrap) with XX < 1024.
- PROBE pr2 (24 dead float statements after `shot_counter`, NOT landable) shifts every temp +128 (site 1 too: the shift is not
  positional) and grades RELOCATION_MATCH. pr1 (20 statements, +96) leaves ZZ=1020: slots still wrong (8 hunks). The shift comes in
  32-steps; window: +128/+160/+192 over b0 (site 2 ZZ >= 1024, site 1 YY < 1024).
- t1 (drop GETTC_EXPANDED, add existing GETTC_ORIGINAL_CHAIN = FableWin GetTC<T> 27i SAME): +32, no other code change.
- Next: stack only EVIDENCED texts (PeekTC chain, bitset::test / SetDynamicCast / IsDead / IsStillRunnable / CIntelligentPointer
  / float helpers as FableWin sizes + ghash allow) and see where the count lands; a hand-picked subset = fit -> ESCALATE.md.
- B24A7 MATCH: n9.cpp = differ/ = RELOCATION_MATCH (fg). Base B = best + FableWin-SAME helper texts (GETTC_ORIGINAL_CHAIN without
  GETTC_EXPANDED, PEEKTC_ORIGINAL_CHAIN, new CThing opt-in ISDEAD_DIRECT `return Dead;` 10i SAME, IsStillRunnable as nested ifs 27i/68b
  SAME) -- all code-neutral, temps B: site1 606/818/822, site2 779/923/927. + 9 dead release ASSERTs (fable_assert.h
  FABLE_RETAIL_ASSERT_DEAD_ARM) on the unchecked pointer results (ptc_boss, ptc_combat, p_TC_hit_locations, p_hit_location_final,
  ptc_targeted, ptc_perceive_thing, ptcd_obstruction, p_jack_def.Get(), ptc_solid_to_thing). Counts: 7 asserts +96 (ZZ 1019: no), 8: +96
  (no), 9: MATCH, 10 (8 + 2 repeats): MATCH. FableWin row 1952i/8620b v ours 1218i at /Od (+734i, fwsize flags "assert?"); one debug
  ASSERT costs ~42i at /Od /EHsc (fw/acal.cpp) -> ~10-17 asserts in the debug build. Inferred device -> ESCALATE.md.
- Other measured temp deltas: named-result / Initialise ADD+SCALE +43, SUB (either form) breaks site 1, GetMagnitude SQUARED_LOCAL -2,
  XY_THEN_Z changes the product order (8 hunks), IsDead direct -32, GetTC chain +32. ghash.py (B24B3) matched nothing here (uncalibrated).
- B24A7 HAND-BACK (final = differ/00975bb0.cpp = payload.json): RELOCATION_MATCH, vt.py PASS, strict vland PASS. Warnings: RELOC_NAME
  DELTA_NOTIONAL_ZERO and the CIntelligentPointer<const CThing> converting ctor (labels only; see NAME_FIXES.tsv), RELOC_ALIAS __CIfmod thunk.
  Pick-any (pickany.log): 12 EQ, 1 NEQ. Fixes kept, both EXISTING opt-ins with evidence:
  - CINTELLIGENTPOINTER_CTOR_OUTOFLINE: retail calls the (T*) ctor 0x0065E133 out of line at 0x00975F08, so the copy is no longer emitted.
  - CJACKOFBLADESBATTLEDEF_TYPENAME_PASS26: StatGetTypeName is inline, which the retail GetSubDef copy 0x008C33E7 expands; landed sibling
    0x00974AA0 defines the same. The GetSubDef copy is now EQ.
  NOT kept: the GetTCDriver size-pragma / inline_depth / explicit-instantiation scaffolds. None changed the 27-byte copy, and the
  explicit instantiation would be an unevidenced device anyway. GetTCDriver<CTCDPhysicalObstruction> stays NEQ against size-optimised
  owner 0x007890EA (ESCALATE.md section 2); declaration-only gives 71 hunks.
- GET_ROUNDED_ANGLE FABLE_FORCEINLINE evidence (kept, source line 3): there is no out-of-line copy in Fable.exe, reloc_bindings,
  functions.tsv or the Ego_r names. Retail expands it at 0x00976208.. (fmod 0x00BFEB28 at 0x00976236). The same rule is used by the landed
  GET_ROUNDED_ANGLE_FORCEINLINE rows 0x0068CB90 and 0x00974AA0, and plain inline is rejected at c2 cost 75 > 74.
- Lane slip (B24A7): running fwsize.py on landed rows under rebuild/src/compiled wrote _fwsize/ into the repo (the coordinator moved it).
  Always export FWSIZE_WORK=$W/_fwsize first.
- B24A7 FINAL (supersedes the pick-any lines above): GetTCDriver is fixed with the size-pragma specialisation opt-in (hdr/README.md);
  pickany.log shows 13 EQ and 0 NEQ. Strict vland PASS, vt PASS, and a mutated test (% 7) FAILS. Assert window 9..14 (ESCALATE.md section 3).
  Not landable by an author: the dead ASSERTs are an Inferred device (ESCALATE section 1), for the coordinator / owner.
