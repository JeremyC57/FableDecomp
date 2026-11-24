# AUTH14 (2026-11-22): 0x008E18D0 UpdateLightningBeams, two grades and a per-site decision table, then STOP

Owner orders: do not park; stalls go to the advisor. Locks read AUTH14. Outputs ONLY under `$S/w22/auth14/008e18d0/`.
`S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad`. Never write into /home/user/FableDecomp.

## Read first (only these)
- Rulings: the last two 2026-11-22 entries of `docs/journal/2026-11/2026-11-20_ADVISOR_DECISIONS_B.md` (the 0x008E18D0 items). Binding.
- `rebuild/candidates/manual/pending/2026-11-22_auth10/008e18d0/` (ESCALATE.md, e4.cpp, hdr.diff, proof/) and
  `pending/2026-11-22_auth11/008e18d0/` (MEASURE.md, e5.cpp, LOG.md); AUTH10's tools `$S/w22/auth10/tools/` (README first; READOUT only);
  AUTH11's files `$S/w22/auth11/008e18d0/`. AUTH12's always-inline finding: `pending/2026-11-22_auth12/00cf4ec0/ESCALATE.md` "Inline readout".
- `$S/w22/auth1/BRIEF.md` "Paths", "Tools", "Hard rules".

## Steps
1. Grade (stock VC7.1, e4's configuration `/Gr /d2inlT65`, the same overlay): e5 with C5, and e5 without C5 (C5 = the GetTC wrapper opt-in,
   which replaces an arm documented as fitted, so it must stand alone). Report both against e4's 2044v2073 in the same configuration.
2. Decision table: for every callee at the four forced sites (~CParticleUpdateParameters, CVectorMap<..>::LowerBound,
   __uninitialized_copy<pair<ulong,C3DVector>>, and any other callee at those sites), list retail's decision (call / expand) at EVERY
   occurrence in the row, with our readout's cost and score per occurrence. Apply the rule "inlined iff cost - 4 <= S" where it applies.
   Classify each callee: UNIFORM (retail treats it the same everywhere: a cost question; say which cost would fit) or MIXED (needs the
   frequency hunt).
3. Write DECISIONS.md (the table, the classification, the two grades). Do NOT start a cost or frequency hunt. STOP and report.
