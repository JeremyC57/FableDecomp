## 0x00B783F0 CEngineWaterRenderer::Render (bwf2 author, session 1)
- grade ~20: DIFFER 9343v9343 (size equal), 6 bookmark-test load-order ties left. Moved by: shared headers + overlays (inline
  SetTextureBumpEnvMatrix, inline CRenderStateCaptureBlock, SetCommonRenderStates FORCEINLINE in hdr), CScopedShadersEnable FORCEINLINE
  opt-in (plain inline ran out of /Ob2 budget: dtor called out of line, +297 bytes, slots 0x20/0x24 swapped), named `ice` in the
  early test (-1 tie), `component_params` ref for params.Params (-1 tie).
- did NOT move: SetRenderState spellings (early-return, desired_state, swapped &, !(), named mask), nested ifs, const locals,
  named water/sea (changes code), settings ref (changes code), plain/auto-inline SetCommonRenderStates in TU (not inlined).
- FINAL (session 1, ~110 grades incl. sweeps): best = differ/00b783f0.cpp (= src/c1.cpp), DIFFER 9343v9343, 34 fg hunks, of which only
  6 are real (insert/delete load-order ties; the other hunks are identical-text relocation-mask artefacts of fg).
  Remaining ties: exp2 SBEM(1, f,0,0,f) enabled arm, all 4 blocks (0x00B7945D..0x00B79589) and exp3 Dst/AB (0x00B7A5D0, 0x00B7A635).
- Ties are sensitive to the number of inline instances before them (probe: 4 dead `(void)X.empty();` between exp2's
  SetFogEnable(true) and the 3rd SetCommonRenderStates() fix exp3 Dst/AB -> src/g4/b4.cpp, 4 ties; NOT legit source, probe only).
  The exp2 SBEM(1) quartet never moved: insertions before exp2 (src/g5), time/early-test spellings (20-variant sweep src/g),
  bool declaration styles (src/g2), SBEM body spellings (named values, reinterpret_cast), SCRS spellings (settings ref, bump bool,
  inverted arms, ==true, stage loop), capture-block member variants, SetRenderState spellings, SetCommonRenderStates defined in TU
  before/after Render. Begin()!=end() instead of empty() moves other ties (src/u1.cpp).
- FableWin /Od sizes: SetCommonRenderStates ours 75i/260b v FW 106i/333b (+31i): the helper TEXT differs in the original
  (or FW asserts). Best next idea: find the real SetCommonRenderStates text (31 more /Od instructions: extra statements that vanish
  in release, or extra inline helper calls), and what real code sits between exp2 and exp3 (~4 inline instances worth of IL).
- Names: all callee names from Ego_r/FableWin (dossier); no landed spelling proven wrong. Descriptive local names only (no made-up
  function names). Globals: NEngineInternal::GPRenderManager / GPShaderRenderManager (landed spelling), g_pEngineSkyRenderer 0x01436E50.

## Session 2 (attempt 2, author) -- checkpoint after ~50 grades/probes (work dir a2/, filter a2/rh.py lists REAL hunks only)
- Best unchanged: differ/00b783f0.cpp = a2/base.cpp, 13 real hunks (quartet 0x945d/0x94c1/0x9525/0x958f, exp2 disabled stage-2 reg pair
  0x00B79938, exp3 Dst/AB 0x00B7A5D0/0x00B7A635). The other ~21 fg hunks are fg normalisation artefacts (ours '00000000' -> REL).
- Mechanism read: the quartet = IL operand order of `state.BookmarkMask & CurrentBookmarkMask` (retail Current first) in ONE SBEM instance
  (exp2 enabled arm, stage 1); the else arm's identical-shape blocks come out right. Rulers do NOT move it: dead empty() 1..48 at top /
  before exp2 / inside SCRS arm / after; dead `bool=fg` 1..48; dead `const long` 6..56; dead `long = member+N` 10..60 (>=20 changes
  tail merges/size); rulers after exp3 or at the end change nothing at all. Other exp2 blocks flip with rulers, the quartet never.
- No effect (identical 13 hunks): SCRS defined in the TU instead of the header; hand-inlined exp3; an extra FORCEINLINE wrapper around
  exp2's Fog/SCRS/Fog; empty inline check calls inside SBEM; SBEM state address `*(arr + stage)`, `(unsigned long&)`, `const unsigned long*`;
  /G5, /G6; other functions before/after Render in the TU; plain-inline CScopedShadersEnable + /d2inlT300 (= FORCEINLINE result).
- Changes code: local float factor in SCRS (+259 b), SBEM param order 00,01,10,11 (slot stores), named state refs in SBEM, desired_state
  SetRenderState (9247 b, tail merges), hand-inlined exp1 / exp2, inverted bump arms, per-stage if/else, /G7, /Oa, /Ow, /Op, /d2inlS*.
- FableWin sizes explained: FW = VC10 (STL _Iterator012, $$Q rvalue refs). SCRS FW +31i/+73b = 16 float args pushed via x87 at VC10 /Od
  (+28i/+60b: 12 zeros `push ecx;fldz;fstp` +2i/+4b, 4 members fld/fstp +1i/+3b) + ONE extra setter call (3i/13b, later revision). So the
  SCRS text is confirmed apart from that later setter; SBEM FW 375i = ours + 4 asserts (~83i each, cf. SetPointScaleConstantFactor 98i v
  SetFogDensity 14i); Render FW +90i ~= one assert (~84i) + 7 float `time` pushes. No hidden helper layers indicated.
- (session 2, later grades) Also no effect on the quartet: swapped `&` operands in SetRenderState (NOTHING changes at all -> the front
  end canonicalises the `&` operands; source order is irrelevant), dead branches (removed early), sea-section nesting variants (nested in
  `if (EnableSea)` with re-test; split ifs), QFE 4035 toolchain, float-constant rulers (2..8 dead `time * K.5f`), /G5 /G6.
  Dead-code probes that fix OTHER residues (not usable): 2 dead empty() inside SCRS's enabled arm after SBEM(1) -> 9 real hunks
  (exp3 pair fixed); a dead `if (iceFg) {bool d = !X.empty();}` at top or before exp2 -> 11 (exp3 AB fixed); dropping the named `ice`
  fixes 0x00B79938 but breaks 0x00B79281.
- Reading of the quartet: in OUR build the two identical-shape blocks (exp2 enabled arm stage 1 v exp2 disabled arm stage 1) come out
  differently, and nothing that adds records (named symbols, constants, IL) moves it, so the deciding key is local to that SBEM
  instance (tuple numbering is per block, last tuple first). Best next idea: a c2 hook on the commutative-operand canonicalisation
  (agent2048 notes: hook point unknown; the c2prio tool covers RA only), or a source difference INSIDE the enabled arm's first
  SetTextureBumpEnvMatrix call that keeps the code (the arm reads Settings.BumpMatFactor once into eax and copies it to both slots;
  try forms that change how that value reaches mat00 / mat11 without a named local, which changed size by +259 b).
- FINAL session 2: best source unchanged = differ/00b783f0.cpp (a2/base.cpp), DIFFER 9343v9343, 13 real hunks (6 ties + reg pair).

## Session 3 (attempt 3) -- checkpoint (work dir a3/; probe hooks in a3/hdr = $W/hdr + PROBE_* / PRB_* macros, default text unchanged)
- New diagnostic: a3/pat.py / a3/roles.py print the per-SetRenderState-block load order (C = CurrentBookmarkMask loaded first, S = state
  mask first) for retail and ours; a3/sw.sh compiles variants (~3 s each) and prints the pattern diff. Retail 75 blocks; ours differs ONLY in
  the quartet (exp2 enabled M00..M11 stage 1 = S, retail C) and exp3 DstB/ABE (ours C, retail S). The 0x00B79938 reg swap is separate.
- The exp3 pair is an ordinary run-boundary (moves with any IL / named-symbol count: PRB_D2/PRB_E2 = 2 dead empty() in SCRS's else arm or
  before SetAlphaTestEnable fix it with size unchanged). The QUARTET IS PINNED: rulers anywhere in Render (dead longs 1..40, empty() 1..64,
  at top / before exp2 / mid / end), named locals per SBEM (1..4) / per SCRS (1..4), dead inline calls in SRS/ASUL/SBEM (dropped entirely,
  no change at all), FtoDW helper by reference, attach-text variants, local rm / this-> / Settings ref in the enabled arm: never C.
  It flips only when the 4 AttachTextureToStage calls before exp2 are removed (ctx_4; 1..5 attaches stay S), or partly (M11 only) with 2
  named locals per SetRenderState expansion (code size changes).
- Session 3 grid (a3/gs/results.txt, ~480 compiles: s named locals per SetRenderState 0..2, b per SBEM 0..4, c per SCRS 0..4, a dead
  longs in Render 0..40) + follow-ups (b, c up to 20): the quartet turns C ONLY in a second "regime" that sets in at a total named-symbol
  threshold (s>=1 with a few more, b>=12, c>=16, 2 per SRS): it ALWAYS comes with exp1 bump-section changes (positions 10/14/15/18/24
  of the pattern, tail merges, size 9252..9314) -- retail has exp1 in regime 1 and the quartet in regime 2, which no count reaches.
  Best regime-1 rows: (s,b,c,a)=(0,1,2,16)/(0,1,3,16): quartet only (exp3 pair fixed) -- probes, not source.
- Session 3 no-effect (quartet stays S, all 9343 unless noted): real SetRenderState texts with named locals A1..A6 (desired_state + mask
  copy 9180, + bookmark 9657, + bool bookmarked 9321 (quartet partly C, exp1 broken), + element ref 8952, mask copy only 9180,
  desired + new_state 9295); attach loop (8690: VC7.1 does not unroll), NULL literals; GPRenderManager typed CRenderManagerCore* /
  CRenderManager2D*; envmap spellings (sky local, texture local 9339, ==true 9342, split ifs 9359, this->); flags /Ob1 /Ox /GS /Gy;
  __assume asserts in SBEM (code changes); (void)(expr) assert forms (folded by the front end); dead inline calls in SRS/ASUL/SBEM (no
  records at all).
- Best next idea: the quartet's S depends on code that sits between Restore() and exp2 (0 attaches -> C, 1..5 -> S; envmap removal
  changes it) and on symbol-count regimes, not on SCRS/SBEM/SRS spellings. Look for a REAL difference in that region's helpers'
  internal text that changes symbols/records without code (AttachD3DTextureToStage FW 321i v ours 36i, AttachTextureToStage 20i SAME,
  CRenderStateCaptureBlock::Restore FW 169i v 11i, GetEnvironmentMap/GenerateMipmapsWithGPU declarations), or hook c2 for the scheduler
  tie-break (the residue is the ORDER of two independent loads with identical registers, i.e. a list-scheduler tie, not RA).
- Also no effect at all (identical pattern): RestoreCaptureBlock(void* block) inline wrapper (FableWin has only the void* overload, 437i)
  used by CRenderStateCaptureBlock::Restore and the scoped block's dtor.
- FINAL session 3: best source unchanged = differ/00b783f0.cpp, DIFFER 9343v9343, 13 real hunks (quartet 0x00B7945D..0x00B79589,
  exp3 DstB/ABE 0x00B7A5D0/0x00B7A635, reg pair 0x00B79938). ~140 compiles (probes logged in ATTEMPTS.log as `probe`, grid in
  a3/gs/results.txt). Tools for the next agent: a3/sw.sh <abs src...> (compile + C/S pattern diff), a3/roles.py (block roles),
  a3/gen2.py (named-symbol ruler generator), a3/hdr (probe hooks PROBE_SRS_BODY / PROBE_SBEM_BODY / PRB_A..E / PRB_ATT / PRB_RMTYPE ...).
