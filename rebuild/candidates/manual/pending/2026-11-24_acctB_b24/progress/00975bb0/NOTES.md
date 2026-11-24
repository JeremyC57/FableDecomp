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
