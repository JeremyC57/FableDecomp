# agentC3 NOTES (w22 acctC lane; continues agentC2)

## 0x00BB3E80 CEngineClothRenderer::RenderClothPrimitive -- IN PROGRESS
Start: C2 best (ref/00bb3e80/best_agentC2.cpp, copied to src/), DIFFER(4540v4540), 20 fg hunks, 3 real sites.
Measured model of the 3 sites (tools/srs2.py): the 16 SetRenderState expansions from 0x00BB46E0 (pass==1 arm 5, else arm 11) each
have (L) the order of the two loads CurrentBookmarkMask (C) / state.BookmarkMask (M) and (O) the operand order of test/or (a = CBM,mask;
b = mask,CBM). Retail: L = CCCCCCCCCCCCCCCM, O = aaaaaaaabbbbbbbb. C2 best: L = CCCCCCCCCCCCCMMM, O = aaaaaaaaabbbbbbb.
- Scheduler DAG (tools/sched/tall.py): the two loads have NO edge and equal priority; their order is the IL tuple order (idx tie).
- Dead-local ruler sweeps (diagnostic only, tools/sweep.sh; position in the function does not matter = per-function symbol count):
  L-step cycles with period ~32 named symbols (best L-step 13 at k=0 for C2's text: it can never reach 15 with this text);
  O-step moves one expansion per ~32 symbols (k=52 -> 8 = retail, k=84 -> 7).
- SetRenderState text variants (DESIRED / BOOKMARK / DIRTY opt-ins) change the per-expansion record count: DESIRED only + k=46..47 gives
  L right and O-step 9 (only 0x00BB49E9 wrong). Next: find a text / helper set where both steps line up, then natural names.
- KEY RESULT (diagnostic): the repo DEFAULT SetRenderState / AddStateToUpdateList text (none of DESIRED / BOOKMARK / DIRTY) plus
  124..128 dead-local rulers anywhere in the outer body grades RELOCATION_MATCH (probe/v/none_k124.cpp; window k=124..128, both ends
  checked; position independent: start of body or before RenderPolygons give the same). So the whole residue is "the function has ~124
  fewer named-symbol records than the original, created before the alpha-state expansions", with the default setter text.
- Per-expansion names (DESIRED/BOOKMARK/DIRTY) change the SLOPE (records between the 16 expansions), not only the offset: with 1..3 of them
  no k in 0..130 lines up L and O (bigsweep1.txt). Only the default text keeps the L-peak at 15 every ~31 units.
- Unit costs measured (none config, tools/units.py): HasOcclusionInfo accessor 0, named `offset` vector in GetAppropriateMipmap +1..3,
  C4DVector(colour) / conversion-operator texts of RenderUtil::SetTextureFactor NEGATIVE, __assume changes code, /Zi /Z7 /Zd no effect,
  a preceding function in the TU no effect (records are per function).
- L mechanism (traced, tools/sched/traw2.py + tools/ldids.py): the two loads' tuple order flips when the CBM load's destination temp id
  crosses a value (0x377 -> 0x378 in the none config); temp ids shift by 1 per outer named local.
- DECOUPLED LEVERS (position probes, tools/posprobe.py: a forced-inline probe with N dead locals called at a chosen point):
  records created AFTER the alpha-state expansions (probe at A = before SetSelfIlluminationAmbient, or Z = end) move ONLY the O-step;
  O depends on the function's TOTAL record count: O8 (retail) needs total extra ~110..140 (k_early + N_late), e.g. k0+A112, k30+A80,
  k62+A48, k62+A64 all RELOCATION_MATCH; k62+A80 overshoots to O7. L depends only on records created BEFORE the expansions and needs
  early k in a window {0, 30, 62, 94, 124} (+0..4) with the default SetRenderState text.
  So the natural fix needs ~110+ more records than C2's source, early ones in multiples of ~31. Searching for the natural source.
- WHAT COUNTS (late/early probes, all diagnostic): named scalar locals = 1 unit (early -> L and O, late -> O only); named class
  objects whose fields are touched weigh more (8 outer `C3DVector v = cam.PeekCameraPos();` copies = RELOCATION_MATCH,
  differ/00bb3e80.DIAG_relocmatch_8vec.cpp; 7 or 9 do not); references, unused named params, empty inline calls, `(void)expr`,
  `sizeof`, `__noop`, `if (!(c)) {}` count 0; `__assume` changes code; TU flags (/Ox /Oi- /GF /Gy /Ob1 /GR /G5 /G6 /Gr /Zi /Z7 /EHsc)
  change nothing; a preceding function or a visible stub RenderPolygons body changes nothing.
- BEST LEAD: DEAD CODE COUNTS. `if (0) { C3DVector v = ...; }` blocks and assert-shaped dead blocks
  `if (0) { if (!(cond)) Fail("cond", __FILE__, __LINE__); }` create records that c2 later deletes: they move O ONLY (L unchanged,
  wherever they are, early or late). Default SetRenderState text + 27..31 such dead assert blocks = RELOCATION_MATCH
  (differ/00bb3e80.DIAG_relocmatch_deadasserts.cpp; 25 -> O9, 32 -> O7). FableWin's own row body is +126 /Od instructions over ours
  (FW_EXTRA=/EHsc: 440i v 566i) = its asserts. Hypothesis for the coordinator/owner: Lionhead's release ASSERT expands to dead code
  (records, no bytes) rather than to nothing; the DESIRED/BOOKMARK/DIRTY "FITTED" named locals of earlier render rows may be the same
  effect measured with a different ruler. Needs a project decision (a shared ASSERT macro with a dead-code release form + where the
  original asserts were) -- not something a row may invent. Not handed in.
- Natural candidates already measured in the default-text config (none moves O by the ~3 steps needed): HasOcclusionInfo accessor,
  named `offset`/`centre` vectors in GetAppropriateMipmap (centre copy changes code), C3DBoundingSphere COPY / CENTER_VALUE opt-ins,
  GFLimit<long,long,long>/<long,int,long>, CRGBColour CTOR4_INITIALISE, CRGBFLOATCOLOUR_INITIALISE_CTOR (+4..6 L), named tint
  CRGBColour / CRGBFloatColour / C4DVector constant, conversion-operator SetTextureFactor texts (negative), effect refs in all outline arms,
  retail CEngineShadowRenderer class + params(4,5,0) ctor (L -1), FABLE_RETAIL_CARRAY (+1..3 L), CVERTEXSHADER / CPARTICLEPROGRAM /
  CPARTICLESIMULATION / CCOUNTEDPOINTER models (0), SetRenderState probe texts (current_mask local, swapped operands, element ref (code
  change), BOOKMARK-only, post-increment AddState (0)), setters without `?:` (0).
- Behaviour test: test/00bb3e80.cpp PASS under g++ and VC7.1 (vt.py) -- a host model, ready for when the row matches.
- NEXT (for whoever continues): (1) project decision on a release ASSERT form that leaves dead IL (see LESSONS); if adopted, place the
  FableWin-evidenced asserts (row body ~1-2, helpers per the fwsize "assert?" gaps) and regrade -- the row only needs O to move from 11 to 8
  with the default setter text (L already right at k=0). (2) Otherwise look for natural dead-after-folding code in helpers expanded in the
  row (constant-argument branches), which also moves O only. (3) Keep the default SetRenderState text: any per-expansion named local
  breaks L. Diagnostic matches: differ/00bb3e80.DIAG_*.cpp (dead locals k=124..128 / 8 C3DVector copies / 27..31 dead asserts).
