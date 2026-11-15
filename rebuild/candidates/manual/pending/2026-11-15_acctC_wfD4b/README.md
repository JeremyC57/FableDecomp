# acctC hand-back wfD4b (parked rows, 2026-11-15)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x0049A0B0 | CProgressDisplay::InitialiseTextures | 2,920 | RELOCATION_MATCH | PASS |

Coordinator-verified (`coordverify.log`) and rule-audited by an independent agent: no violations, no flags or pragmas.
REVIEW ITEMS: FITTED (marked): the two `{ RandFunc random(&seed); ... }` blocks; the bare block at lines 100-141 is unmarked
(scope fitted to the frame slot packing). Local `struct RandFunc` has no engine header (landed 0x0049B1E0 does the same).
New declaration opt-in FABLE_RETAIL_CSTRINGPARSER_DTOR_OUTOFLINE (retail `call 0x4047a0` at 0x0049A9F5).
