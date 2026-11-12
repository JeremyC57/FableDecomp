# Chunk Q20 notes (2026-11-12)

Result: 2 RELOCATION_MATCH, 4 ported but still DIFFER, 14 not ported (0 grades). Grade with `export FABLE_HDR_OVERLAY=$S/w22/chunkQ20/hdr`.

## Matched (payload.json / oracle.tsv / src/ / tests/)
- 0x007B0360 CTCFishingSpot::FrameUpdate -- matches on repo headers alone (no overlay needed). CTCFishingSpot has no shared header;
  the class is modelled locally on the shared CTCBase / CThing / CIntelligentPointer / CTCDParticleEmitter headers (members from Ego_r.pdb).
  A shared `engine/CTCFishingSpot.h` would be the natural follow-up (Alloc, OnInitialActivate, RemoveParticles IAE, OnSerialise, FrameUpdate).
- 0x008B7040 CCreatureAction_JumpOutFromCover::Initialise -- needs overlay `C3DVector.h` (NORMALISE_LOCALS_XYZ) and the new
  `CCreatureAction_JumpOutFromCover.h`. Land those two headers with the row.

## DIFFER (best source in differ/<addr>.cpp, residue on line 2)
- 0x009DDC60 DIFFER(899v899): frame-slot swap of top/right at esp+14h/18h only.
- 0x007BCF7F DIFFER(808v808): commutative FP operand order (magnitude X*X first, one cross pair, velocity fld/fmul) + fall/shortfall slot packing.
- 0x007BD845 DIFFER(642v642): the same FP operand order + RandomiseShotDirection temp slot + two scheduling slots.
- 0x009FB750 DIFFER(789v789): width load hoisted above the first GetByteColourDepth call; row-copy src/dst eax/edx swapped.
All four are now on the shared headers (with overlay opt-ins), so they are landing-shaped once the residue goes.

## Not ported (prior drafts kept in differ/, Q20 note on line 2)
00a78b20, 00540320, 00649e8b, 006c1250, 0096cd70 (Q19 leftovers, local models; budget), 0063f7c0 (needs a CThingSearchTools.h
ForEachInRoughArea body opt-in; Q19's sibling had the same reload-order residue), 00489d40 (many CPlayer opt-ins), 00a1d290, 00584b86,
00622a00, 00b4a700 (budget), 00814260 (needs CTCBoastingPosition, GetAllDummyObjectNames opt-in, CStringParser), 00b39080 (mesh / stats
model gaps), 007daef0 (no CTCTavernGameCardBase / Blackjack retail headers). Prior drafts of the six ported rows are in work/prior_drafts/.

## Names
- No landed spelling proven wrong (NAME_FIXES.tsv is header-only). Landed decls checked: GFVXToAX 0x009E1E10 / GFVYToAY 0x009E1E40 and
  GFAXToVX 0x009E1DA0 / GFAYToVY 0x009E1DD0 are right everywhere; RotateXY 0x00A14040 right.
- Draft-only spellings corrected in Q20 sources (none landed): 0x00A14040 RotateAboutZ -> C3DVector::RotateXY; 0x009E1E10/0x009E1E40
  GFAXToVX/GFAYToVY -> GFVXToAX/GFVYToAY (the 009ddc60 prior draft); 0x007BEDD0 GetTargetPosition -> GetShootAtPos; CTCPhysicsBase slot 43
  Stop -> SetInteractionFlags, slot 38 Update -> EnableGravity, slot 32 SetRHSet -> ResetRHSet (shot prior drafts).
- Made-up / descriptive names (not found in either PDB, strings, FSE or EgoCore):
  - `CTCFishingSpot::CreateParticle` (0x007B0360) -- private FABLE_FORCEINLINE helper; retail expanded it inline 3x, so no symbol exists.
  - `g_ScreenRHW` 0x0139652C -- kept as the existing landed spelling (6 landed uses), not new.
  - Local variable names (toTarget, side, direction, fraction, fall, shortfall...) are descriptive.

## Coordinator
- Overlay table: hdr/README.md. Three new headers (CCreatureAction_JumpOutFromCover.h, NThingShot_CTCShotDefinitionMultiArrow.h,
  NThingShot_CTCShotDefinitionDrainLife.h); the rest are additive `#ifdef FABLE_RETAIL_*` opt-ins. C3DVector.h turns the existing
  `#if defined(..._NORMALISE_LOCALS)` into `#elif` behind the new XYZ branch (default path unchanged). All overlays were re-diffed against
  the repo headers at hand-back time: not stale.
- Only C3DVector.h + CCreatureAction_JumpOutFromCover.h are needed for the matched rows.
- Tests compile and PASS under g++ -std=c++03 and are cl /Zs clean.
