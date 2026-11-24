# AUTH10 (Account A authoring agent, 2026-11-22): 0x008E18D0 escalated, 0x00CF4EC0 handed over

Brief: `BRIEF.md`; state: `STATUS.tsv`. Objects and the forced build's object are left out. AUTH10 returned early (context).
- **0x008E18D0 UpdateLightningBeams** (`008e18d0/ESCALATE.md`). The plain best is `proof/e4.cpp`, DIFFER(2044v2073) at /Gr /d2inlT65.
  - With four forced c2 inline decisions (`proof/forced_sites.txt`, the p25N hook extended by `tools/inl_force.py`), e4 grades
    RELOCATION_MATCH. All four sites are loop-versus-sky; no single inline threshold fits them.
  - Text fixes found: the sky's `end_pos` as one statement (Ego_d line 3358), and Ego_d's dead locals at 3283 / 3285. Their VC10 line
    sizes are NOT yet measured.
- **0x00CF4EC0 CFanMarker::Main** (`00cf4ec0/HANDOVER.md`): c0.cpp DIFFER(6244v6164) on row-local models.
- Not started: 0x00B4D370, 0x00BD3070, 0x00502E90, 0x0093D270.

Advisor (2026-11-22, owner delegated):
- **0x008E18D0 does not land.** A forced c2 is an oracle, not a grade, and no "bracket" grade is recorded. Next, bounded:
  1. measure e4 with odall10 against Ego_d line by line, plus the wrappers on the path to the four sites (calibrate on Ego_d's
     out-of-line GetTC<CTCDParticleEmitter>, 74 B; the unexplained 27 B at line 3297 is the lead);
  2. check the for-lines against the VC6 for-scope idiom (GOTCHAS 442) and whether it moves inl.py's f;
  3. only if residue remains, read c2's decision routine at the hook for any input beyond size, c1, f and T, within a set budget;
     past the budget, an owner packet on hypothesis (a).
- **0x00CF4EC0:** port c0.cpp onto the shared headers before any more forcing; inline scores depend on helper body sizes.
