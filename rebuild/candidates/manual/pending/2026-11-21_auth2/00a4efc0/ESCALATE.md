# ESCALATE 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData (3491 B) -- AUTH2, 2026-11-21
S = /tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad. Full log: S/w22/auth2/00a4efc0/LOG.md.

## Best source
S/w22/auth2/00a4efc0/a2.cpp, /Gr, FABLE_HDR_OVERLAY=S/w22/auth2/00a4efc0/hdr: DIFFER(3491v3491), residue 16 lines = the one
position-size residue AUTH1 described (unchanged; see S/w22/auth1/00a4efc0/ESCALATE.md for the hunk). This uses the V1 AllocateType
opt-in, which by the advisor's test is still REJECTED until ruled on (00A50510 breaks). Rule-clean fallback without the opt-in:
S/w22/auth1/00a4efc0/a1.cpp, DIFFER(3491v3491), 26 lines. NOTE: the opt-in name ..._ALLOCATETYPE_EGOD now carries a different text
from AUTH1's overlay under the same name. Final header regraded once: row 16; 00A4C5E0, 00AA0FA0, 00A4FE60, 00BD2D90, 00AA0090,
00AA2110, 00A9EB90, 00A9F540 MATCH; 00A50510 DIFFER 10.

## What the advisor's three instructions found
1. AllocateType opt-in test (12 landed users grepped; 9 use the class branch FABLE_RETAIL_CSIMPLEMEMORYPOOL, the 3 CParticleProgram rows
   use the flattened default and are not users). AUTH1's text (named `unsigned __int64 total_size` local) BREAKS 3 of 5 in the rows' own
   config: 00A50510 (35 lines), 00AA0FA0 (15), 00BD2D90 (9) -> fitted, rejected. Any named local breaks 00BD2D90 and 00AA0FA0 (const local,
   `count * (u64)sizeof(T)` local: same). 00BD2D90's header says its x87 order is a live-symbol count fixed by Ego_d's own local set (not
   fitted), so it is a real calibration of the symbol TOTAL; that retail's AllocateType had no named local is INFERRED (a count-sensitive
   row fixes the total, not which helper supplies it). With the local, 00A4C5E0 / 00A4FE60 still matched; the other four were not graded
   with it.
   The text that holds: V1 = Ego_d's statement shape WITHOUT the local (`if (count) { if ((u64)count*sizeof(T) > 0xffffffffUL)
   { FABLE_RETAIL_ASSERT(false); return 0; } return (T*)Allocate(count*sizeof(T)); } return 0;`). 8 of 9 users MATCH (00A4C5E0,
   00A4FE60, 00AA0FA0, 00AA0090, 00AA2110, 00A9EB90, 00A9F540, 00BD2D90), and the row keeps AUTH1's gain (16 lines; residue (3) fixed).
   The one break, 00A50510 (DIFFER 1748v1748, 10 lines, two x87 operand-order hunks), breaks under EVERY if-shaped body (V1, V3 no-assert,
   V5 `if(count) return ternary`, V7 early-return, AUTH1's) and matches only the default one-line ternary, which Ego_d's 7-statement table
   contradicts; its own header records its x87 order as c2 candidate-order FITTED (named loop bone, separate ++data_index, ...).
   The row needs the dead arm INSIDE the size branch: the ternary with a separate dead assert statement (V10) keeps 00A50510 but the row is
   back at 26; V3 (no assert) also 26.
2. Dead assert condition: byte-neutral in the row (false / `(u64)count*sizeof(T) <= 0xffffffffUL` / `!"..."` all 16). Not sizeable:
   Ego_d is a VC10 build (already recorded: EGOD_FINDINGS.md line 9, GOTCHAS 2026-11-19) and the lane has no VC10. Text carries `false`, labelled unconstrained.
3. Helper calibration at VC7.1 /Od (S/w22/auth2/odall.py, od/h1.cpp, od/h2.cpp): exact for SetupIdentityFlag (7/54/15/2/15/4), GetMatrix,
   GetMatrixCount, GetPosition, GetScaling, GetRotation, GetBoneType, GetSamplesPerSecond, C3DVectorWord ctors (14 / 68), GetByteSize,
   C4DVector::Initialise. Small deltas: GetBoneName 49 v 47 (/EHsc) or 31, SetScaling 34 v 30, C3DVectorWord::operator== 86 v 82,
   GetMaximumAbsoluteComponent body 244 v 212. 19 alternative spellings measured: none reaches Ego_d's size. With Ego_d = VC10 these deltas
   are not shown to be text failures (a few bytes per statement, VC10 /Od float-constant / temp codegen), so no helper text was changed.

## Hypotheses
(a) Residue (2) is still a colouring split decided by c2 numbering from text outside the position block: the rotation and scaling blocks
have the same shape and match. (b) The true AllocateType text is V1 and 00A50510 must be re-fitted (its x87 spellings were chosen under
the ternary); then the CSimpleMemoryPool default should become V1, not an opt-in. (c) Retail's GetMaximumAbsoluteComponent / SetScaling /
operator== texts may differ from ours; Ego_d and FableWin are both VC10, so only a VC10 compile of candidate texts can show it.

## Questions for the advisor
1. Is V1 acceptable as the AllocateType opt-in (8 of 9 users, the break being a row whose x87 order is self-declared fitted to the old
   text), or must 00A50510 be re-fitted under V1 first (I can do that: it is a landed row, needs its lock), or is V1 rejected?
2. FableWin cannot replace Ego_d here: ghidra_out/fablewin_obj_functions.tsv gives the SAME VC10 sizes (SetScaling 34, GetBoneName 49,
   C3DVectorWord::operator== 86, GetMaximumAbsoluteComponent 259, AllocateType<bool> 371). So helper calibration at byte level needs a
   VC10 in the lane or stays undecidable: does the row wait, or is "undecidable, no text change" the accepted end of that lever?
3. Residue (2) has no admissible lever left here (AUTH1's operand / condition spellings and every legit helper text measured): escalate
   to a c2 hook probe (dump the position block's symbol numbers v the scaling block's) as for 0x00C73050?
