# Chunk 189: 5 verified candidates, 21 parked, 14 unstarted

Regenerated from the current catalog on 2026-10-17; Claude's original chunk reservations were unavailable.
All matched candidates have actual-implementation VC7.1 behavior fixtures. Final landing must use --qfe.
rows.tsv preserves the complete 40-row reservation without retail bytes.

## Verified candidates
- 00a8b5f0 RELOCATION_MATCH 194v194: 3 attempts; pair-value methods and iterator objects; bone collector behavior PASS
- 00a8eda0 RELOCATION_MATCH 165v165: 3 attempts; derived ctor body assignments and early buffer local; behavior PASS
- 0096d2d0 RELOCATION_MATCH 175v175: 3 attempts including one missing forward declaration; staffing behavior PASS
- 0093dcd0 RELOCATION_MATCH 219: Four attempts; vector subtraction and GetLengthSquared2D reproduce retail x87 sequence. Fixture covers combat gating, valid sequence, strict 1.5-metre boundary, and ignored height.
- 00958c00 RELOCATION_MATCH 163v163: 2 attempts; guard-list behavior PASS

## Parked for retry
- 00a57960 DIFFER(226v210): Two attempts; determinant algorithm recovered, but x87 coordinate loading/spilling differs. Shared C2DLineF layout reused.
- 00a4e250 DIFFER(175v171): Four attempts; clamping helper and keyframe/identity selection recovered, but quaternion conditional return spills through an extra temporary.
- 00a3c1a0 DIFFER(182v189): 4 bounded attempts; see NOTES
- 00ac09a0 DIFFER(188v209): Two attempts; conversion and scoped previous-pointer lifetimes recovered. Retail retains two zeroing stores and a register-based add-ref that VC7.1 eliminates or folds in this model.
- 00a361a0 DIFFER(189v193): 4 attempts; correct CRC/helper lookup and transform; vector temporary and x87/register scheduling differs
- 00a2e760 DIFFER(183v183): 3 attempts; exact operations/registers, PackedNamesSize test and HelperPoints.Size store scheduled early; parked scheduling-only difference
- 00a44cd0 DIFFER(178v175): 4 bounded attempts; see NOTES
- 0097f690 DIFFER(224v226): One attempt; route-start and destination retry/filter control flow recovered; register allocation keeps result in EDI rather than retail stack slot.
- 0097f780 DIFFER(231v233): Two attempts including prototype correction; same result-register versus stack allocation mismatch as the non-proximity route helper.
- 009577e0 DIFFER(181v181): Four attempts; all instructions align except the target-address LEA is scheduled before temporary construction instead of after GetPItem.
- 0092bcb0 DIFFER(205v207): Two attempts; all predicates and calls align. Vector assignment uses three displaced stores rather than retail add-to-this plus stores.
- 0094f090 DIFFER(223v223): One attempt; geometry, normalization, scaling, height query, and full length align. Only virtual-call argument scheduling and return-copy register coloring differ.
- 009612f0 DIFFER(174v174): 1 attempt; all operations/control flow match; player and saved-position ESI/EDI coloring swapped
- 0091ea70 DIFFER(240v240): 3 attempts; same control flow, calls and length; EDI/EBX and initial creature-load register coloring differs
- 00922600 DIFFER(166v166): Two attempts; exact length and all branches/calls; bound-filter operand initialization and scratch register scheduling differ.
- 0094c850 DIFFER(168v169): Two attempts; exact branch flow and placement cleanup, with only the first work-building LEA versus ADD instruction differing.
- 0092d6f0 DIFFER(197v198): Four attempts; exact state flow and one-byte-near structure. World-time/member accessor scheduling and final epilogue differ.
- 0095b5a0 DIFFER(224v222): Two attempts; target selection, pointer lifetime, combat registration and arena calls align; component and target-address registers differ.
- 0095a7d0 DIFFER(188v188): One attempt; all event predicates, branches, fields, and calls match. Only the adjacent FrameOfLastHit/component loads are swapped.
- 0093dc10 DIFFER(192v192): Two attempts; every branch, call and length matches; remaining difference is the X/Y squared-load order in the x87 distance comparison.
- 0096f180 DIFFER(188v188): Three attempts; calls and rectangle arithmetic align, but filter/temporary stack reuse differs (32-byte versus 28-byte frame).

## Continue
Work UNSTARTED rows in STATUS.tsv. Source work/codex-lane/wine/env.sh for every compiler shell.
Full local disassembly/oracle: work/codex-lane/w22/chunk189-dis.txt and work/codex-lane/batch/chunk189_oracle.tsv.
Original attempts for 00a3c1a0 and 00a44cd0: four each; do not retry without new source-model evidence.
