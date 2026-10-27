# Chunk 993 -- Fable 5.1 (high effort) on 40 parked rows that Opus 5.5 had failed (2026-10-27)

A model comparison run at the owner's request: could Fable 5.1 recover parked rows that Opus 5.5 agents gave up on?
The 40 rows were all still open on 2026-10-27, and each row's prior best source was re-graded under the current lane first
(PRIOR_BASELINE.tsv: all 40 DIFFER), so every match here is new.

| Group | Prior attempts | Rows | Matched |
|---|---|---|---|
| 1 | Opus 5.5 high chunk agent, then the Opus 5.5 extra-high retry agent (retry 10, segment 7) | 13 | 6 |
| 2 | Opus 5.5 high chunk agent (chunks 231-242), random sample | 27 | 1 |
| All | | 40 | 7 (79 grades) |

All 7 landed with `vland.py --land` after the land5 conversions (verify.log: 7 RELOCATION_MATCH PASS, no RELOC_IDENTITY).

**The lever:** the six CTCSpecialAbilities::Setup* rows and SetupAssassinRush shared one gap the retry pass had called a
lane gap ("AddTC this-load before the pushes"). It is a formulation: CThing::HasTC must be the real STLport 4.6
`std::bitset<274>::test` with its range check (`if (pos >= 274) { puts("bitset"); abort(); }` then `!= 0`). With it VC7.1
keeps the thing pointer as a value and loads it into ecx right after the CCharString ctor; a hand-folded mask sinks the
load below the pushes. 0072caa0 also needed the actions bound as temporaries to `SetCurrentAction(const
CCreatureActionBase&)` (the PDB signature) so the two tails cross-jump. The agent expects the same lever to close catalog
parks 007eac70, 0072b260 and 00729f60. Lessons in LESSONS.md, per-row parked notes in NOTES.md, every grade in GRADES.tsv.

Parked (33, in `pending/2026-10-27_linux_hand993_fable_test/differ/`): the agent judges about 20 as formulation with a next
idea and about 13 as lane gaps (register rotation, merged-tail placement, /Os receiver placement, x87 spills), plus one
dead-compare (volatile) candidate, 00909a00.
