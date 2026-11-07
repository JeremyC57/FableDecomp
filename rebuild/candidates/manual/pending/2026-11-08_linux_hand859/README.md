# Chunk 859 (2026-11-08): Fable retry of twenty parked near misses — 4 of 20 matched (round 1: 3, `batches/2026-11-08_landing_h/`; round 2: 1, `batches/2026-11-08_landing_i/`)

Matched (src/, test/): 0x00A852B0 CVectorMap<...>::Insert (9 grades), 0x0058301A CTCInventory::CreateEditBoxActions (5),
0x006A9690 CThingPlayerCreature::ResolveRelativeMovementAcceleration (8). The working lever: a same-TU `__declspec(noinline)` body for every
callee that receives a local's address (what retail's STLport / engine headers gave the TU) makes the local unaliased and restores retail's
store / reload order -- only where retail's TU really had the body.

Round 1 stopped after 89 grades with 17 rows parked (most at 1-4 grades); the agent was sent back to continue (goal 10 of 20, 100+ grades per
unmatched row). This directory is the round-1 snapshot; round 2 results are added when it reports. Per-row residues in NOTES.md and line 2
of each differ/ source; lessons in LESSONS.md; name fixes queued as `land0811h`.

## Round 2
Resumed for the user's goal (10 of 20). 2,283 grades in all (`ATTEMPTS.log`; cross-product grids of behaviour-identical spellings,
`tools/grid.py` + `tools/spec/`). One more match: **0x00C75F80 CCombFilter::Allocate, plain MATCH** (391 grades): store the three ring
members through `CRingBuffer* ring = &m_ring;` and call `m_ring.Clear()` on the member -- VC7.1 forwards a member store into a later
load (and CSEs the scale it enables) only when both use the same base expression. Every other row got 100-254 grades with no movement
(0x00D18DF0 kept its structural blocker: both MovieBase dtor sites expand only under the diagnostic /d2inlS15). The 16 parked rows'
differ/ line 2 lists the grades and grid axes; NOTES.md has the refuted hypotheses.
